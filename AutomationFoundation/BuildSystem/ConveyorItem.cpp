#include "ConveyorItem.h"

#include "AutomationFoundation/Core/AutomationFoundationCharacter.h"
#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"
#include "AutomationFoundation/Inventory/InventoryItemInstance.h"
#include "AutomationFoundation/Inventory/ItemSpecification.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "PreviewObjects/ConveyorBelt.h"
#include "PreviewObjects/Machine.h"

AConveyorItem::AConveyorItem()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	DisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DisplayMesh"));
	DisplayMesh->SetupAttachment(RootComponent);
	DisplayMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));

	DisplayMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DisplayMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	DisplayMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	DisplayMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	MovementTimeline = FTimeline();

	FloatCurve = CreateDefaultSubobject<UCurveFloat>(TEXT("Float"));
}

void AConveyorItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AConveyorItem::SpawnNext()
{
	OnSpawnNext.Broadcast();

	GetWorldTimerManager().ClearTimer(SpawnNextTimer);
}

void AConveyorItem::BeginPlay()
{
	Super::BeginPlay();
	
	FOnTimelineFloat TimelineTickFunction;
	TimelineTickFunction.BindDynamic(this, &AConveyorItem::TimelineTick);
	MovementTimeline.AddInterpFloat(FloatCurve, TimelineTickFunction);

	if (ConveyorRef.IsValid())
	{
		ConveyorRef->AddItemToConveyor(this, true);
		SetupTimeline(ConveyorRef.Get());
		const float TimerLength = ItemInterval / ConveyorRef->GetSpeed();
		GetWorldTimerManager().SetTimer(SpawnNextTimer, this, &AConveyorItem::SpawnNext, TimerLength);
	}
}

void AConveyorItem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	MovementTimeline.TickTimeline(DeltaSeconds);
}

void AConveyorItem::SetItemSpecification(UItemSpecification* NewItemSpecification)
{
	ItemSpecification = NewItemSpecification;
	if (IsValid(ItemSpecification.Get()))
	{
		DisplayMesh->SetStaticMesh(ItemSpecification->DisplayMesh.LoadSynchronous());
	}
}

FText AConveyorItem::GetInteractionText()
{
	return FText::FromString("Item");
}

void AConveyorItem::OnInteract(AActor* InteractInstigator)
{
	if (const AAutomationFoundationCharacter* Character = Cast<AAutomationFoundationCharacter>(InteractInstigator))
	{
		UInventoryItemInstance* NewItem = NewObject<UInventoryItemInstance>();
		NewItem->ItemSpecification = ItemSpecification.Get();
		NewItem->CurrentStackSize = 1; // A Conveyor Item will only ever be a stack of 1...

		if (Character->TryAddItemToInventory(NewItem))
		{
			ConveyorRef->OnItemPickedUp(this);
		}
	}
}

void AConveyorItem::TimelineTick(float InterpolationValue)
{
	TrackProgress = InterpolationValue;
	LOG_INFO(LogTemp, "Track Progress Alpha: %.4f", TrackProgress);

	if (ItemInFront.IsValid())
	{
		if (ShouldStop())
		{
			ForceStop();
		}
	}

	if (ConveyorRef.IsValid())
	{
		const FTransform NextPosition = ConveyorRef->GetTransformByAlpha(TrackProgress);
		const FTransform EndPosition = ConveyorRef->GetTransformByAlpha(1.0f);

		LOG_INFO(LogTemp, "Position: %s // %s", *NextPosition.GetLocation().ToString(), *EndPosition.GetLocation().ToString());

		if (ConveyorRef->GetTransformByAlpha(1.0f).GetLocation().Equals(NextPosition.GetLocation(), 1.0f))
		{
			OnReachConveyorEnd();
		}
		else
		{
			SetActorTransform(NextPosition);
		}
	}
	else
	{
		LOG_ERROR(LogTemp, "Conveyor Ref should be valid for all conveyor items")
	}
}

void AConveyorItem::RestartMovement()
{
	MovementTimeline.Play();

	FTimerManager& TimerManager = GetWorldTimerManager();

	if (TimerManager.IsTimerPaused(SpawnNextTimer))
	{
		TimerManager.UnPauseTimer(SpawnNextTimer);
	}

	OnTimelineStarted.Broadcast();
}

void AConveyorItem::StopMovement()
{
	if (ShouldStop())
	{
		ForceStop();
	}
}

void AConveyorItem::ForceStop()
{
	MovementTimeline.Stop();
	FTimerManager& TimerManager = GetWorldTimerManager();
	if (TimerManager.IsTimerActive(SpawnNextTimer))
	{
		TimerManager.PauseTimer(SpawnNextTimer);
	}
	OnTimelineStopped.Broadcast();
}

void AConveyorItem::OnReachConveyorEnd()
{
	LOG_INFO(LogTemp, "Item reached end of belt");
	ForceStop();
	bHasReachedEnd = true;

	FBuildInformation BuildInformation;
	if (ConveyorRef->GetNextBuild(BuildInformation))
	{
		if (Cast<AConveyorBelt>(BuildInformation.Build))
		{
			ConveyorRef->MoveItemToNextConveyor(this);
		}
		else if (AMachine* Machine = Cast<AMachine>(BuildInformation.Build))
		{
			if (Machine->AcceptInput(BuildInformation.AttachPoint, this))
			{
				Destroy();
			}
		}
	}
}

UItemSpecification* AConveyorItem::GetItemSpecification() const
{
	return ItemSpecification.IsValid() ? ItemSpecification.Get() : nullptr;
}

void AConveyorItem::SetItemInterval(float NewInterval)
{
	ItemInterval = NewInterval;
}

bool AConveyorItem::HasReachedEnd() const
{
	return bHasReachedEnd;
}

void AConveyorItem::SetConveyor(AConveyorBelt* NewBelt)
{
	ConveyorRef = NewBelt;
}

bool AConveyorItem::ShouldStop()
{
	if (ItemInFront.IsValid())
	{
		bool bShouldStop = false;
		if (ItemInFront->ConveyorRef == ConveyorRef)
		{
			bShouldStop = ItemInFront->GetDistanceAlongSpline() - GetDistanceAlongSpline() < ItemInterval;
		}
		else
		{
			bShouldStop = ItemInFront->GetDistanceAlongSpline() + ConveyorRef->GetSpline()->GetSplineLength() - GetDistanceAlongSpline() < ItemInterval;
		}
		return bShouldStop && ItemInFront->IsStopped();
	}

	return false;
}

bool AConveyorItem::IsStopped() const
{
	return !MovementTimeline.IsPlaying();
}

bool AConveyorItem::IsLastItem() const
{
	return ConveyorRef->GetLastItem() == this;
}

void AConveyorItem::SetupTimeline(AConveyorBelt* NewConveyorBelt)
{
	bHasReachedEnd = false;
	if (IsValid(NewConveyorBelt))
	{
		ConveyorRef = NewConveyorBelt;
	}

	MovementTimeline.SetPlaybackPosition(0.0f, false, false);

	float PlayRate = FMath::GridSnap(10.0f / (FMath::RoundToInt32(NewConveyorBelt->GetSpline()->GetSplineLength()) / NewConveyorBelt->GetSpeed()), 0.001);
	LOG_INFO(LogTemp, "Play rate set to: %f", PlayRate);

	// Calculate New Play Rate..
	MovementTimeline.SetPlayRate(PlayRate);
	RestartMovement();
}

AConveyorItem* AConveyorItem::GetItemInFront() const
{
	return ItemInFront.IsValid() ? ItemInFront.Get() : nullptr;
}

void AConveyorItem::BindEvents(AConveyorItem* NewItemInFront)
{
	if (ItemInFront.IsValid())
	{
		ItemInFront->OnTimelineStopped.RemoveDynamic(this, &AConveyorItem::StopMovement);
		ItemInFront->OnTimelineStarted.RemoveDynamic(this, &AConveyorItem::RestartMovement);
	}

	NewItemInFront->OnTimelineStopped.AddDynamic(this, &AConveyorItem::StopMovement);
	NewItemInFront->OnTimelineStarted.AddDynamic(this, &AConveyorItem::RestartMovement);
}

float AConveyorItem::GetDistanceAlongSpline()
{
	return ConveyorRef->GetLengthByAlpha(TrackProgress);
}

float AConveyorItem::GetItemInterval() const
{
	return ItemInterval;
}

void AConveyorItem::SetItemInFront(AConveyorItem* NewItem)
{
	ItemInFront = NewItem;
}
