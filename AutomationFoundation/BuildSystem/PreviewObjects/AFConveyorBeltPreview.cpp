#include "AFConveyorBeltPreview.h"

#include "AFBeltSupportPreview.h"
#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"
#include "Components/SplineMeshComponent.h"

AAFConveyorBeltPreview::AAFConveyorBeltPreview()
{
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	SetRootComponent(SceneRoot);
	SceneRoot->SetMobility(EComponentMobility::Movable);

	TempConveyor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Temporary Conveyor"));
	TempConveyor->SetupAttachment(RootComponent);

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);
	Spline->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	SplineMeshComponentsArray = TArray<USplineMeshComponent*>();
}

void AAFConveyorBeltPreview::BeginPlay()
{
	Super::BeginPlay();

	SetAllMaterialOverlays(HologramMaterial);
}

void AAFConveyorBeltPreview::CreatePreview()
{
	Super::CreatePreview();

	check(IsValid(BeltSupportClass));

	StartSupport = GetWorld()->SpawnActor<AAFBeltSupportPreview>(BeltSupportClass);
	if (StartSupport)
	{
		StartSupport->SetActorTransform(GetActorTransform());
		StartSupport->CreatePreview();
	}

	StartSupport->SetAllMaterialOverlays(HologramMaterial);
}

bool AAFConveyorBeltPreview::PlacePreview()
{
	if (bIsStartLockedIn)
	{
		return PlaceEndSupport();
	}

	EndSupport = GetWorld()->SpawnActor<AAFBeltSupportPreview>(BeltSupportClass);
	if (EndSupport)
	{
		EndSupport->SetActorTransform(GetActorTransform());
		EndSupport->CreatePreview();
	}

	TempConveyor->DestroyComponent();

	const FVector Tangent = GetActorForwardVector() * 100.0f;
	Spline->SetTangentsAtSplinePoint(0, Tangent, Tangent, ESplineCoordinateSpace::World, true);

	bIsStartLockedIn = true;
	return false;
}

bool AAFConveyorBeltPreview::IsPlaceable()
{
	return Super::IsPlaceable();
}

void AAFConveyorBeltPreview::CancelPreview()
{
	if (IsValid(StartSupport)) StartSupport->Destroy();
	if (IsValid(EndSupport)) EndSupport->Destroy();

	Super::CancelPreview();
}

void AAFConveyorBeltPreview::UpdateLocation(const FHitResult& HitResult)
{
	if (!bIsStartLockedIn)
	{
		StartSupport->UpdateLocation(HitResult);
		if (const USceneComponent* AttachPoint = StartSupport->GetAttachPoint())
		{
			const FTransform Transform = AttachPoint->GetComponentTransform();
			SetActorLocationAndRotation(Transform.GetLocation(), Transform.GetRotation());
		}
		else
		{
			LOG_WARNING(LogTemp, "Could not get Attach Point");
		}
		return;
	}

	EndSupport->UpdateLocation(HitResult);

	const USceneComponent* AttachPoint = EndSupport->GetAttachPoint();
	check(IsValid(AttachPoint));
	const int32 LastSplineIndex = GetLastSplinePoint();

	Spline->SetLocationAtSplinePoint(LastSplineIndex, AttachPoint->GetComponentLocation(), ESplineCoordinateSpace::World);
	const FVector Tangents = AttachPoint->GetForwardVector() * 100.0f;
	Spline->SetTangentsAtSplinePoint(LastSplineIndex, Tangents, Tangents, ESplineCoordinateSpace::World);

	UpdateSplineShape();
	// UpdateSplineVisual();

	HologramMaterial->SetVectorParameterValue(TEXT("Color"), IsPlaceable() ? FColor::Green : FColor::Red);
}

void AAFConveyorBeltPreview::ScrollPreview(float Value)
{
	if (bIsStartLockedIn)
	{
		EndSupport->ScrollPreview(Value);
	}
	else
	{
		StartSupport->ScrollPreview(Value);
	}
}

void AAFConveyorBeltPreview::PlaceSupport(AAFBeltSupportPreview* Support)
{
	if (Support->IsHidden())
	{
		Support->Destroy();
	}
	else
	{
		Support->PlacePreview();
	}
}

void AAFConveyorBeltPreview::UpdateSplineShape() const
{
	int32 PointCount = Spline->GetNumberOfSplinePoints();

	FTransform SplineTransform;
	FVector SplineTangent;
	FVector SplineForwardVector;
	FVector UpdatedTangent{};
	GetSplineInformation(0, ESplineCoordinateSpace::Local, SplineTransform, SplineTangent, SplineForwardVector);

	if (PointCount == 2)
	{
		// Adds point directly in front of origin point
		Spline->AddSplinePointAtIndex(SplineTransform.GetLocation() + (SplineForwardVector * 100.0f), 1, ESplineCoordinateSpace::Local);
	}
	else
	{
		UpdatedTangent = SplineForwardVector * FMath::Clamp(GetDistanceBetweenTwoSplinePoints(1, 2, true), 0.0f, 500.0f);
		Spline->SetTangentsAtSplinePoint(1, UpdatedTangent, UpdatedTangent, ESplineCoordinateSpace::Local);
	}

	PointCount = Spline->GetNumberOfSplinePoints();
	int32 LastIndex = GetLastSplinePoint();
	int32 PenultimateIndex = LastIndex - 1;

	if (PointCount < 4)
	{
		// Adds point directly behind end point
		GetSplineInformation(LastIndex, ESplineCoordinateSpace::Local, SplineTransform, SplineTangent, SplineForwardVector);
		Spline->AddSplinePoint(SplineTransform.GetLocation() + (SplineForwardVector * -100.0f), ESplineCoordinateSpace::Local);
	}
	else
	{
		GetSplineInformation(LastIndex, ESplineCoordinateSpace::Local, SplineTransform, SplineTangent, SplineForwardVector);
		UpdatedTangent = SplineForwardVector * FMath::Clamp(GetDistanceBetweenTwoSplinePoints(1, PenultimateIndex, true), 0.0f, 500.0f);
		Spline->SetTangentsAtSplinePoint(PenultimateIndex, UpdatedTangent, UpdatedTangent, ESplineCoordinateSpace::Local);
		Spline->SetLocationAtSplinePoint(PenultimateIndex, SplineTransform.GetLocation() + (SplineForwardVector * -100.0f), ESplineCoordinateSpace::Local);
	}

	// If we are facing forward, we don't need any more than 4 points so remove all the excess points
	float Rotation = GetRelativeRotation();
	LOG_WARNING(LogTemp, "Rotation: %.4f", Rotation);

	if (!FMath::IsNearlyEqual(FMath::Abs(Rotation), 180.f))
	{
		while (PointCount >= 5)
		{
			Spline->RemoveSplinePoint(2, true);
		}
		return;
	}

	///
	/// At this point, we are dealing with all the cases where we've rotated the spline 180 degrees (U-Turns)
	///

	PointCount = Spline->GetNumberOfSplinePoints();

	// We need at least 5 points to make this look nice, so add it if we're missing them
	// At this point, we should be guaranteed to be at 4 points minimum
	if (PointCount < 5)
	{
		GetSplineInformation(0, ESplineCoordinateSpace::Local, SplineTransform, SplineTangent, SplineForwardVector);
		Spline->AddSplinePoint(SplineTransform.GetLocation() + (SplineForwardVector * -100.0f), ESplineCoordinateSpace::Local);
	}

	float X, Y;
	// If we are very close in the X direction (either in front of or behind)
	GetRelativeLocation(X, Y);
	X = FMath::Abs(X);
	if (X >= 50.0f || X <= 100.0f)
	{
		HandleShortUTurnBelt();
		return;
	}

	return;

	PointCount = Spline->GetNumberOfSplinePoints();
	if (PointCount < 6)
	{
		GetSplineInformation(0, ESplineCoordinateSpace::Local, SplineTransform, SplineTangent, SplineForwardVector);
		Spline->AddSplinePoint(SplineTransform.GetLocation() + (SplineForwardVector * -100.0f), ESplineCoordinateSpace::Local);
	}

	GetRelativeLocation(X, Y);
	if (X > 100.0f)
	{
		HandleLongUTurnBelt();
	}
	else
	{
		HandleMidUTurnBelt();
	}
}

void AAFConveyorBeltPreview::UpdateSplineVisual()
{
	const float SplineLength = Spline->GetSplineLength();

	float TempLength = FMath::Fmod(SplineLength, SectionLengthOverride) / FMath::FloorToInt((SplineLength / SectionLengthOverride));
	TempLength += SectionLengthOverride;

	const int32 LastIndex = FMath::TruncToInt(SplineLength / SectionLengthOverride) - 1;

	UStaticMesh* StaticMesh;
	USplineMeshComponent* CurrentSplineMesh = nullptr;

	int32 CurrentIndex = 0;
	while (CurrentIndex <= LastIndex)
	{
		if (CurrentIndex == 0)
		{
			StaticMesh = ConveyorStart;
		}
		else if (CurrentIndex == LastIndex)
		{
			StaticMesh = ConveyorEnd;
		}
		else
		{
			StaticMesh = ConveyorMid;
		}

		if (CurrentIndex > SplineMeshComponentsArray.Num() - 1)
		{
			CurrentSplineMesh = Cast<USplineMeshComponent>(AddComponentByClass(USplineMeshComponent::StaticClass(), false, {}, true));
			CurrentSplineMesh->SetMobility(EComponentMobility::Movable);
			CurrentSplineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			CurrentSplineMesh->SetForwardAxis(ESplineMeshAxis::X);
			CurrentSplineMesh->SetOverlayMaterial(HologramMaterial);
			FinishAddComponent(CurrentSplineMesh, false, {});
			SplineMeshComponentsArray.Add(CurrentSplineMesh);
		}
		else
		{
			CurrentSplineMesh = SplineMeshComponentsArray[CurrentIndex];
		}

		FVector2D& Location = SplineMeshComponents.FindOrAdd(CurrentSplineMesh);
		Location.X = TempLength * CurrentIndex;
		Location.Y = TempLength * (CurrentIndex + 1);

		const float StartDistance = CurrentIndex * TempLength;
		const float EndDistance = (CurrentIndex + 1) * TempLength;

		const FVector StartPosition = Spline->GetLocationAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
		FVector StartTangent = Spline->GetTangentAtDistanceAlongSpline(StartDistance, ESplineCoordinateSpace::Local);
		StartTangent = StartTangent.GetClampedToSize(0.0, TempLength);

		const FVector EndPosition = Spline->GetLocationAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);
		FVector EndTangent = Spline->GetTangentAtDistanceAlongSpline(EndDistance, ESplineCoordinateSpace::Local);
		EndTangent = EndTangent.GetClampedToSize(0.0f, TempLength);

		if (CurrentSplineMesh)
		{
			CurrentSplineMesh->SetStartAndEnd(StartPosition, StartTangent, EndPosition, EndTangent);
			CurrentSplineMesh->SetStaticMesh(StaticMesh);
		}

		CurrentIndex++;
	}

	if (SplineMeshComponents.Num() - 1 <= CurrentIndex)
	{
		return;
	}

	StaticMesh = nullptr;

	for (int i = SplineMeshComponentsArray.Num() - 1; i > CurrentIndex; i--)
	{
		CurrentSplineMesh = SplineMeshComponentsArray[i];
		SplineMeshComponentsArray.RemoveAt(i);
		SplineMeshComponents.Remove(CurrentSplineMesh);
		if (CurrentSplineMesh)
		{
			CurrentSplineMesh->DestroyComponent();
		}
	}
}

int32 AAFConveyorBeltPreview::GetLastSplinePoint() const
{
	return Spline->GetNumberOfSplinePoints() - 1;
}

void AAFConveyorBeltPreview::GetSplineInformation(int32 PointIndex, ESplineCoordinateSpace::Type CoordinateSpace, FTransform& OutTransform, FVector& OutTangent, FVector& OutForwardVector) const
{
	OutTransform = Spline->GetTransformAtSplinePoint(PointIndex, CoordinateSpace);
	OutTangent = Spline->GetTangentAtSplinePoint(PointIndex, CoordinateSpace);

	OutForwardVector = OutTangent.GetSafeNormal();
}

float AAFConveyorBeltPreview::GetDistanceBetweenTwoSplinePoints(int32 IndexA, int32 IndexB, bool bUseAbsolute) const
{
	const float Distance = Spline->GetDistanceAlongSplineAtSplinePoint(IndexA) - Spline->GetDistanceAlongSplineAtSplinePoint(IndexB);

	return bUseAbsolute ? FMath::Abs(Distance) : Distance;
}

float AAFConveyorBeltPreview::GetRelativeRotation() const
{
	return Spline->GetRotationAtSplinePoint(GetLastSplinePoint(), ESplineCoordinateSpace::Local).Yaw;
}

void AAFConveyorBeltPreview::GetRelativeLocation(float& OutX, float& OutY) const
{
	const FVector Location = Spline->GetLocationAtSplinePoint(GetLastSplinePoint(), ESplineCoordinateSpace::Local);
	OutX = Location.X;
	OutY = Location.Y;
}

void AAFConveyorBeltPreview::GetEndQuad(float& OutX, float& OutY) const
{
	GetRelativeLocation(OutX, OutY);
	OutX = FMath::Clamp(OutX, -1.0f, 1.0f);
	OutY = FMath::Clamp(OutY, -1.0f, 1.0f);
}

float AAFConveyorBeltPreview::GetLinearDistanceOfSpline() const
{
	return Spline->GetLocationAtSplinePoint(GetLastSplinePoint(), ESplineCoordinateSpace::Local).Length();
}

void AAFConveyorBeltPreview::HandleShortUTurnBelt() const
{
	// A short belt should only have 5 points (beginning, early curve smooth, mid point, late curve smooth, and end)
	if (Spline->GetNumberOfSplinePoints() > 5)
	{
		Spline->RemoveSplinePoint(2);
	}

	float Y = Spline->GetLocationAtSplinePoint(Spline->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::Local).Y;
	Y = FMath::Clamp(Y, -0.5f, 0.5f);

	const int32 PenultimateIndex = GetLastSplinePoint() - 1;
	const FVector FirstPointLocation = Spline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::World);
	const FVector PenultimatePointLocation = Spline->GetLocationAtSplinePoint(PenultimateIndex, ESplineCoordinateSpace::World);

	const float DistanceBetweenPoints = GetDistanceBetweenTwoSplinePoints(1, PenultimateIndex, true);

	FVector StartLocation = (FirstPointLocation + PenultimatePointLocation) / 2.0f;
	StartLocation += GetActorForwardVector() * (DistanceBetweenPoints / 3.0f); // this offsets the location a little bit

	LOG_INFO(LogTemp, "We have %d points", Spline->GetNumberOfSplinePoints());
	LOG_INFO(LogTemp, "Setting spline point position to: %s", *StartLocation.ToString());

	///
	/// Draw debug sphere's for each of the existing 5 spline points from beginning to end.
	/// Red is the middle and the one being modified
	///
	::DrawDebugSphere(GetWorld(), Spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World), 16, 8, FColor::Green, true);
	::DrawDebugSphere(GetWorld(), FirstPointLocation, 24, 8, FColor::Yellow, true);
	::DrawDebugSphere(GetWorld(), StartLocation, 32, 8, FColor::Red, true);
	::DrawDebugSphere(GetWorld(), PenultimatePointLocation, 24, 8, FColor::Purple, true);
	::DrawDebugSphere(GetWorld(), Spline->GetLocationAtSplinePoint(4, ESplineCoordinateSpace::World), 16, 8, FColor::Blue, true);

	GEngine->DeferredCommands.Add(TEXT("pause"));

	return;

	// Update the Mid point to use the average between the early and late curve smoothing
	Spline->SetLocationAtSplinePoint(2, StartLocation, ESplineCoordinateSpace::World);

	const FVector NewTangent = GetActorRightVector() * DistanceBetweenPoints * Y;

	LOG_INFO(LogTemp, "Updating the tangent to: %s", *NewTangent.ToString());

	// Fix the mid-point tangent
	// This causes a crash....
	// Spline->SetTangentsAtSplinePoint(2, NewTangent, NewTangent, ESplineCoordinateSpace::World);
}

void AAFConveyorBeltPreview::HandleMidUTurnBelt() const
{
	FTransform StartTransform, EndTransform;
	FVector StartTangent, EndTangent;
	FVector StartForwardVector, EndForwardVector;
	GetSplineInformation(1, ESplineCoordinateSpace::Local, StartTransform, StartTangent, StartForwardVector);
	GetSplineInformation(4, ESplineCoordinateSpace::Local, EndTransform, EndTangent, EndForwardVector);

	Spline->SetLocationAtSplinePoint(3, FVector(StartTransform.GetLocation().X, EndTransform.GetLocation().Y, StartTransform.GetLocation().Z), ESplineCoordinateSpace::Local);
	Spline->SetTangentsAtSplinePoint(3, StartTangent, StartTangent * -1.0f, ESplineCoordinateSpace::Local);

	GetSplineInformation(1, ESplineCoordinateSpace::World, StartTransform, StartTangent, StartForwardVector);
	GetSplineInformation(3, ESplineCoordinateSpace::World, EndTransform, EndTangent, EndForwardVector);

	FVector NewLocation = StartTransform.GetLocation() + EndTransform.GetLocation();
	NewLocation /= 2.0f;
	NewLocation += GetActorForwardVector() * (GetDistanceBetweenTwoSplinePoints(1, 3, true) / 3.0f);
	Spline->SetLocationAtSplinePoint(2, NewLocation, ESplineCoordinateSpace::World);

	float X, Y;
	GetEndQuad(X, Y);
	const FVector NewTangent = GetActorRightVector() * GetDistanceBetweenTwoSplinePoints(1, 3, true) * FMath::Clamp(Y, -0.5f, 0.5f);
	Spline->SetTangentsAtSplinePoint(2, NewTangent, NewTangent, ESplineCoordinateSpace::World);
}

void AAFConveyorBeltPreview::HandleLongUTurnBelt() const
{
	FTransform StartTransform, EndTransform;
	FVector StartTangent, EndTangent;
	FVector StartForwardVector, EndForwardVector;
	GetSplineInformation(1, ESplineCoordinateSpace::Local, StartTransform, StartTangent, StartForwardVector);
	GetSplineInformation(4, ESplineCoordinateSpace::Local, EndTransform, EndTangent, EndForwardVector);

	Spline->SetLocationAtSplinePoint(2, FVector(EndTransform.GetLocation().X, StartTransform.GetLocation().Y, EndTransform.GetLocation().Z), ESplineCoordinateSpace::Local);
	Spline->SetTangentsAtSplinePoint(2, StartTangent, StartTangent, ESplineCoordinateSpace::Local);

	GetSplineInformation(2, ESplineCoordinateSpace::World, StartTransform, StartTangent, StartForwardVector);
	GetSplineInformation(4, ESplineCoordinateSpace::World, EndTransform, EndTangent, EndForwardVector);

	FVector NewLocation = StartTransform.GetLocation() + EndTransform.GetLocation();
	NewLocation /= 2.0f;
	NewLocation += GetActorForwardVector() * (GetDistanceBetweenTwoSplinePoints(2, 4, true) / 3.0f);
	Spline->SetLocationAtSplinePoint(3, NewLocation, ESplineCoordinateSpace::World);

	float X, Y;
	GetEndQuad(X, Y);
	const FVector NewTangent = GetActorRightVector() * GetDistanceBetweenTwoSplinePoints(2, 4, true) * FMath::Clamp(Y, -0.5f, 0.5f);
	Spline->SetTangentsAtSplinePoint(3, NewTangent, NewTangent, ESplineCoordinateSpace::World);
}

void AAFConveyorBeltPreview::CustomScrollPreview(float Value)
{
	if (bIsStartLockedIn)
	{
		EndSupport->CustomScrollPreview(Value);
	}
	else
	{
		StartSupport->CustomScrollPreview(Value);
	}
}

void AAFConveyorBeltPreview::SetAllMaterialOverlays(UMaterialInterface* MaterialOverlay, bool bRemoveOverlay)
{
	Super::SetAllMaterialOverlays(MaterialOverlay, bRemoveOverlay);
}

bool AAFConveyorBeltPreview::PlaceStartSupport()
{
	EndSupport = GetWorld()->SpawnActor<AAFBeltSupportPreview>(BeltSupportClass);
	if (EndSupport)
	{
		EndSupport->SetActorTransform(GetActorTransform());
		EndSupport->CreatePreview();
	}

	TempConveyor->DestroyComponent();

	const FVector Tangent = GetActorForwardVector() * 100.0f;
	Spline->SetTangentsAtSplinePoint(0, Tangent, Tangent, ESplineCoordinateSpace::World, true);

	bIsStartLockedIn = true;
	return false;
}

bool AAFConveyorBeltPreview::PlaceEndSupport()
{
	for (auto const& Entry : SplineMeshComponents)
	{
		Entry.Key->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Entry.Key->SetCollisionResponseToAllChannels(ECR_Block);
		// Don't worry about materials. They will be baked into the assets appropriately
	}

	PlaceSupport(StartSupport);
	PlaceSupport(EndSupport);

	return true;
}
