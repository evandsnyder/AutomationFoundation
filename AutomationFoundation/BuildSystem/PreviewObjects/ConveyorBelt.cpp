#include "ConveyorBelt.h"

#include "SupportBeam.h"
#include "Machine.h"
#include "AutomationFoundation/BuildSystem/AttachPoint.h"
#include "AutomationFoundation/BuildSystem/ConveyorItem.h"
#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"
#include "Components/SplineMeshComponent.h"

AConveyorBelt::AConveyorBelt()
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

	bIsStartLockedIn = false;
}

void AConveyorBelt::BeginPlay()
{
	Super::BeginPlay();

	SetAllMaterialOverlays(HologramMaterial);

	if (IsValid(BeltMaterialClass))
	{
		BeltMaterial = UMaterialInstanceDynamic::Create(BeltMaterialClass, this);
	}
}

void AConveyorBelt::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(IsValid(LastItemInQueue))
	{
		LOG_INFO(LogTemp, "Last Item can Tick: %hs", LastItemInQueue->CanEverTick() ? "true" : "false");
	}
}

/**
 * Configures the build mode preview of this object
 */
void AConveyorBelt::CreatePreview()
{
	Super::CreatePreview();

	check(IsValid(BeltSupportClass));

	StartSupport = GetWorld()->SpawnActor<ASupportBeam>(BeltSupportClass);
	if (StartSupport)
	{
		StartSupport->SetActorTransform(GetActorTransform());
		StartSupport->CreatePreview();
	}

	StartSupport->SetAllMaterialOverlays(HologramMaterial);
}

/**
 * Determines whether or not this conveyor belt can be placed at a given position
 * @return whether this conveyor belt is placeable
 */
bool AConveyorBelt::IsPlaceable()
{
	if (!bIsStartLockedIn)
	{
		return Super::IsPlaceable();
	}

	if (Spline->GetSplineLength() > 3000.f)
	{
		// LOG_WARNING(LogTemp, "Spline Too Long: %.4f", Spline->GetSplineLength());
		return false;
	}

	if (FMath::IsNearlyZero(GetRelativeRotation()) &&
		FMath::IsNearlyZero(Spline->GetLocationAtSplinePoint(GetLastSplinePoint(), ESplineCoordinateSpace::Local).X))
	{
		// LOG_WARNING(LogTemp, "Rotation is 0 and X Distance is 0");
		return false;
	}

	// Checks if things are too steep
	FVector Location = Spline->GetLocationAtSplinePoint(Spline->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::Local);
	const float SplineLinearDistance = Location.Length();
	Location.Normalize();
	const FVector NormalizedDistance = Location / SplineLinearDistance;
	if (FMath::Abs(NormalizedDistance.Z) > 0.0007)
	{
		// LOG_WARNING(LogTemp, "Spline is too Steep");
		return false;
	}

	// TODO: Add collision detection for the belt itself
	// for (USplineMeshComponent* SplineMesh : SplineMeshComponentsArray)
	// {
	// 	SplineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 	FVector2D* CurrentData = SplineMeshComponents.Find(SplineMesh);
	//
	// 	// Hit Box 1
	// 	FVector SplineStartPosition = SplineMesh->GetStartPosition();
	// 	FVector TransformedPosition = GetActorTransform().TransformPosition(SplineStartPosition);
	// 	FVector UpVector = Spline->GetUpVectorAtDistanceAlongSpline(CurrentData->X, ESplineCoordinateSpace::World);
	// 	FVector Direction = Spline->GetDirectionAtDistanceAlongSpline(CurrentData->X, ESplineCoordinateSpace::World);
	//
	// 	FVector StartPosition = TransformedPosition + Direction + (UpVector * 5.0f);
	// 	FVector EndPosition = StartPosition + (UpVector * 5.f);
	//
	// 	FRotator Orientation = FRotationMatrix::MakeFromXZ(UpVector, Direction).Rotator();
	// 	FVector HalfSize{0.49f, 49.f, 1.f};
	//
	// 	TArray<FHitResult> HitResults;
	//
	// 	::DrawDebugSphere(GetWorld(), StartPosition, 32, 8, FColor::Emerald); // Start
	// 	::DrawDebugSphere(GetWorld(), UpVector, 32, 8, FColor::Orange); // UpVector
	// 	::DrawDebugSphere(GetWorld(), Direction, 32, 8, FColor::Purple); // Direction
	// 	FCollisionQueryParams Params;
	// 	Params.bTraceComplex = true;
	// 	Params.AddIgnoredActor(StartSupport);
	// 	Params.AddIgnoredActor(EndSupport);
	// 	::DrawDebugBox(GetWorld(), StartPosition, EndPosition, FColor::Magenta);
	// 	if (GetWorld()->SweepMultiByChannel(HitResults,
	// 	                                    StartPosition,
	// 	                                    EndPosition,
	// 	                                    Orientation.Quaternion(),
	// 	                                    ECC_Visibility,
	// 	                                    FCollisionShape::MakeBox(HalfSize),
	// 	                                    Params))
	// 	{
	// 		return false;
	// 	}
	//
	// 	break;
	//
	// 	// Hit Box 2
	//
	// 	SplineMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// }

	return true;
}

/**
 * Cancels placing this conveyor belt
 */
void AConveyorBelt::CancelPreview()
{
	if (IsValid(StartSupport)) StartSupport->Destroy();
	if (IsValid(EndSupport)) EndSupport->Destroy();

	Super::CancelPreview();
}

/**
 * Refreshes the preview location while in placement mode
 * If the start support beam has not been placed yet, we will lock its position and start placing the end support beamzs
 * @param HitResult Describes the raycast hit location that should be used for the placement
 */
void AConveyorBelt::UpdateLocation(const FHitResult& HitResult)
{
	if (!bIsStartLockedIn)
	{
		UpdateStartLocation(HitResult);
	}
	else
	{
		UpdateEndLocation(HitResult);
	}
}

/**
 * Updates the location of the start support beam
 * @param HitResult 
 */
void AConveyorBelt::UpdateStartLocation(const FHitResult& HitResult)
{
	USceneComponent* SupportAttachPoint;

	if (const ASupportBeam* SupportPreview = Cast<ASupportBeam>(HitResult.GetActor()))
	{
		// Checking Against Empty
		LOG_INFO(LogTemp, "Should Be empty Support Preview: %s", *HitResult.GetActor()->GetName())
		SupportAttachPoint = SupportPreview->GetAttachPoint();
		const FTransform Transform = SupportPreview->GetActorTransform();

		FRotator NewRotation = SupportPreview->GetActorRotation();
		if (Transform.InverseTransformPosition(HitResult.ImpactPoint).X < 0.0f)
		{
			NewRotation += FRotator(0.0f, 180.0f, 0.0);
		}

		SetActorLocationAndRotation(SupportAttachPoint->GetComponentLocation(), NewRotation);

		StartSupport->SetActorHiddenInGame(true);
		OtherStartAttachPoint = nullptr;
		return;
	}

	if (AAttachPoint* AttachPoint = Cast<AAttachPoint>(HitResult.GetActor()))
	{
		if (AttachPoint->CanConnect(StartDirection))
		{
			OtherStartAttachPoint = AttachPoint;
			SetActorLocationAndRotation(OtherStartAttachPoint->GetActorLocation(), OtherStartAttachPoint->GetActorRotation());
			StartSupport->SetActorHiddenInGame(true);
		}
		else
		{
			LOG_INFO(LogTemp, "Cannot Connect to Attach Point");
			OtherStartAttachPoint = nullptr;
		}
		return;
	}

	StartSupport->SetActorHiddenInGame(false);
	StartSupport->UpdateLocation(HitResult);
	SupportAttachPoint = StartSupport->GetAttachPoint();
	if (IsValid(SupportAttachPoint))
	{
		const FTransform Transform = SupportAttachPoint->GetComponentTransform();
		SetActorLocationAndRotation(Transform.GetLocation(), Transform.GetRotation());
	}

	OtherStartAttachPoint = nullptr;
}

/**
 * Updates the location of the end support beam. This is more complicated and will handle drawing the current layout of the spline shape and visualization
 * @param HitResult 
 */
void AConveyorBelt::UpdateEndLocation(const FHitResult& HitResult)
{
	USceneComponent* SupportAttachPoint;
	const int32 LastSplinePoint = GetLastSplinePoint();
	FVector NewTangent;
	if (const ASupportBeam* SupportPreview = Cast<ASupportBeam>(HitResult.GetActor()))
	{
		SupportAttachPoint = SupportPreview->GetAttachPoint();
		NewTangent = SupportPreview->GetActorForwardVector();

		const float X = SupportPreview->GetActorTransform().InverseTransformPosition(HitResult.ImpactPoint).X;
		if (X > 0.0f)
		{
			NewTangent *= -1.0f;
		}
		NewTangent *= 100.f;

		Spline->SetLocationAtSplinePoint(LastSplinePoint, SupportAttachPoint->GetComponentLocation(), ESplineCoordinateSpace::World);
		Spline->SetTangentsAtSplinePoint(LastSplinePoint, NewTangent, NewTangent, ESplineCoordinateSpace::World);
		EndSupport->SetActorHiddenInGame(true);
		OtherEndAttachPoint = nullptr;
		return;
	}

	if (AAttachPoint* AttachPoint = Cast<AAttachPoint>(HitResult.GetActor()))
	{
		if (AttachPoint->CanConnect(EndDirection))
		{
			OtherEndAttachPoint = AttachPoint;

			NewTangent = AttachPoint->GetActorForwardVector() * -100.f;

			Spline->SetLocationAtSplinePoint(LastSplinePoint, AttachPoint->GetActorLocation(), ESplineCoordinateSpace::World);
			Spline->SetTangentsAtSplinePoint(LastSplinePoint, NewTangent, NewTangent, ESplineCoordinateSpace::World);
			EndSupport->SetActorHiddenInGame(true);
			return;
		}
	}

	EndSupport->SetActorHiddenInGame(false);
	EndSupport->UpdateLocation(HitResult);

	SupportAttachPoint = EndSupport->GetAttachPoint();
	check(IsValid(SupportAttachPoint));

	Spline->SetLocationAtSplinePoint(LastSplinePoint, SupportAttachPoint->GetComponentLocation(), ESplineCoordinateSpace::World);
	NewTangent = SupportAttachPoint->GetForwardVector() * 100.0f;
	Spline->SetTangentsAtSplinePoint(LastSplinePoint, NewTangent, NewTangent, ESplineCoordinateSpace::World);

	OtherEndAttachPoint = nullptr;

	UpdateSplineShape();
	UpdateSplineVisual();

	HologramMaterial->SetVectorParameterValue(TEXT("Color"), IsPlaceable() ? FColor::Green : FColor::Red);
}

/**
 * Rotates the start support beam or the end support beam if the start is already placed
 * @param Value Scroll value
 */
void AConveyorBelt::ScrollPreview(float Value)
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

/**
 * If the Start support beam has not been placed yet, it will lock in it's position and start updating the end support beam
 * @return Returns true if the belt was fully placed successfully. 
 */
bool AConveyorBelt::PlacePreview()
{
	if (bIsStartLockedIn)
	{
		return PlaceEndSupport();
	}

	return PlaceStartSupport();
}

/**
 * Places a given support
 * @param Support Support to placez
 */
void AConveyorBelt::PlaceSupport(ASupportBeam* Support)
{
	if (Support->IsHidden())
	{
		Support->Destroy();
		return;
	}

	Support->PlacePreview();
}

/**
 * Locks in the position of the start support. If this is connected to an attach point, then we will update the conveyor belt direction
 * @return 
 */
bool AConveyorBelt::PlaceStartSupport()
{
	EndSupport = GetWorld()->SpawnActor<ASupportBeam>(BeltSupportClass);
	if (EndSupport)
	{
		EndSupport->SetActorTransform(GetActorTransform());
		EndSupport->CreatePreview();
	}

	TempConveyor->DestroyComponent();

	const FVector Tangent = GetActorForwardVector() * 100.0f;
	Spline->SetTangentsAtSplinePoint(0, Tangent, Tangent, ESplineCoordinateSpace::World, true);

	bIsStartLockedIn = true;

	// Doing some configuration on the attach points
	if (IsValid(OtherStartAttachPoint))
	{
		LOG_INFO(LogTemp, "We are placing while connected to another Belt!");
		// OtherStartAttachPoint->SetConnectedBuild(this);
		EndDirection = OtherStartAttachPoint->GetDirection();
		StartDirection = InvertDirection(EndDirection);

		if (BeltMaterial)
		{
			const float MovementSpeed = StartDirection == EBuildDirection::Input ? 0.1 : -0.1;
			BeltMaterial->SetScalarParameterValue("Speed", MovementSpeed);
		}
	}

	return false;
}

/**
 * Finalizes the belt placement, links any machines or belts that are connect to attach points and enables collisions
 * @return 
 */
bool AConveyorBelt::PlaceEndSupport()
{
	for (auto const& Entry : SplineMeshComponents)
	{
		Entry.Key->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Entry.Key->SetCollisionResponseToAllChannels(ECR_Block);
	}

	SetAllMaterialOverlays(nullptr, true);

	PlaceSupport(StartSupport);
	PlaceSupport(EndSupport);

	CreateAttachPoints();

	// TODO: Set material flow and direction correctly here

	SetupLinkedBuilds();

	return true;
}

/**
 * Updates the shape of the spline according to a set of rules
 * This keeps the spline loooking clean and allows run-time placement of the conveyor belt
 */
void AConveyorBelt::UpdateSplineShape() const
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
		Spline->AddSplinePoint(SplineTransform.GetLocation() + (SplineForwardVector * 100.0f), ESplineCoordinateSpace::Local);
	}
	else
	{
		GetSplineInformation(LastIndex, ESplineCoordinateSpace::Local, SplineTransform, SplineTangent, SplineForwardVector);
		UpdatedTangent = SplineForwardVector * FMath::Clamp(GetDistanceBetweenTwoSplinePoints(1, PenultimateIndex, true), 0.0f, 300.0f);
		Spline->SetTangentsAtSplinePoint(PenultimateIndex, UpdatedTangent, UpdatedTangent, ESplineCoordinateSpace::Local);
		Spline->SetLocationAtSplinePoint(PenultimateIndex, SplineTransform.GetLocation() + (SplineForwardVector * -100.0f), ESplineCoordinateSpace::Local);
	}

	// If we are facing forward, we don't need any more than 4 points so remove all the excess points
	float Rotation = GetRelativeRotation();

	if (!FMath::IsNearlyEqual(FMath::Abs(Rotation), 180.f))
	{
		while (Spline->GetNumberOfSplinePoints() >= 5)
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
		Spline->AddSplinePoint(SplineTransform.GetLocation() + (SplineForwardVector * 100.0f), ESplineCoordinateSpace::Local);
	}

	float Y = 0.0f;
	float X = Spline->GetLocationAtSplinePoint(Spline->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::Local).X;
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

/**
 * Generates static mesh components that conform to the shape of the spline
 */
void AConveyorBelt::UpdateSplineVisual()
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
			CurrentSplineMesh->SetMaterial(1, BeltMaterial);
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

/**
 * Gets the index of the last spline point
 * @return The index of the last point in the spline
 */
int32 AConveyorBelt::GetLastSplinePoint() const
{
	return Spline->GetNumberOfSplinePoints() - 1;
}

/**
 * Gets all the information available for a given spline point by its index
 * @param PointIndex Index of the spline point
 * @param CoordinateSpace Whether to get the spline local or world space coordinates
 * @param OutTransform Transform of the spline point
 * @param OutTangent The tangent of the spline point
 * @param OutForwardVector The forward vector of the spline point
 */
void AConveyorBelt::GetSplineInformation(int32 PointIndex, ESplineCoordinateSpace::Type CoordinateSpace, FTransform& OutTransform, FVector& OutTangent, FVector& OutForwardVector) const
{
	OutTransform = Spline->GetTransformAtSplinePoint(PointIndex, CoordinateSpace);
	OutTangent = Spline->GetTangentAtSplinePoint(PointIndex, CoordinateSpace);

	OutForwardVector = OutTangent.GetSafeNormal();
}

/**
 * Gets the distance between two spline points
 * @param IndexA Start spline point
 * @param IndexB End spline point
 * @param bUseAbsolute Whether to get the absolute value or the raw value
 * @return Distance between the spline points
 */
float AConveyorBelt::GetDistanceBetweenTwoSplinePoints(int32 IndexA, int32 IndexB, bool bUseAbsolute) const
{
	const float Distance = Spline->GetDistanceAlongSplineAtSplinePoint(IndexA) - Spline->GetDistanceAlongSplineAtSplinePoint(IndexB);

	return bUseAbsolute ? FMath::Abs(Distance) : Distance;
}

float AConveyorBelt::GetRelativeRotation() const
{
	return Spline->GetRotationAtSplinePoint(GetLastSplinePoint(), ESplineCoordinateSpace::Local).Yaw;
}

void AConveyorBelt::GetRelativeLocation(float& OutX, float& OutY) const
{
	const FVector Location = Spline->GetLocationAtSplinePoint(GetLastSplinePoint(), ESplineCoordinateSpace::Local);
	OutX = Location.X;
	OutY = Location.Y;
}

void AConveyorBelt::GetEndQuad(float& OutX, float& OutY) const
{
	GetRelativeLocation(OutX, OutY);
	OutX = FMath::Clamp(OutX, -1.0f, 1.0f);
	OutY = FMath::Clamp(OutY, -1.0f, 1.0f);
}

float AConveyorBelt::GetLinearDistanceOfSpline() const
{
	return Spline->GetLocationAtSplinePoint(GetLastSplinePoint(), ESplineCoordinateSpace::Local).Length();
}

void AConveyorBelt::HandleShortUTurnBelt() const
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

	Spline->SetLocationAtSplinePoint(2, StartLocation, ESplineCoordinateSpace::World);


	const FVector NewTangent = GetActorRightVector() * DistanceBetweenPoints * Y;

	// Fix the mid-point tangent
	Spline->SetTangentsAtSplinePoint(2, NewTangent, NewTangent, ESplineCoordinateSpace::World);
}

void AConveyorBelt::HandleMidUTurnBelt() const
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

void AConveyorBelt::HandleLongUTurnBelt() const
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

/**
 * Generates the start and end attach points that other conveyor belts and machines can use to link to this conveyor belt
 */
void AConveyorBelt::CreateAttachPoints()
{
	// Spawn the Start Point
	check(IsValid(AttachPointClass));
	const FVector StartPointLocation = Spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
	const FRotator StartPointRotation = Spline->GetRotationAtSplinePoint(0, ESplineCoordinateSpace::World) + FRotator(0.0f, 180.0f, 0.0f);

	MyStartAttachPoint = Cast<AAttachPoint>(GetWorld()->SpawnActor(AttachPointClass, &StartPointLocation, &StartPointRotation));
	MyStartAttachPoint->SetParentBuild(this);

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
	MyStartAttachPoint->AttachToActor(this, AttachmentRules);

	if (IsValid(OtherStartAttachPoint))
	{
		OtherStartAttachPoint->TryLinkConnectedBuilds(MyStartAttachPoint);
	}

	// Spawn the End Point...
	const int32 LastSplinePoint = GetLastSplinePoint();
	const FVector EndPointLocation = Spline->GetLocationAtSplinePoint(LastSplinePoint, ESplineCoordinateSpace::World);
	const FRotator EndPointRotation = Spline->GetRotationAtSplinePoint(LastSplinePoint, ESplineCoordinateSpace::World);

	MyEndAttachPoint = Cast<AAttachPoint>(GetWorld()->SpawnActor(AttachPointClass, &EndPointLocation, &EndPointRotation));
	MyEndAttachPoint->SetParentBuild(this);
	MyEndAttachPoint->AttachToActor(this, AttachmentRules);

	// Check if this is connected to the end of another belt
	if (IsValid(OtherEndAttachPoint))
	{
		if (OtherEndAttachPoint->TryLinkConnectedBuilds(MyEndAttachPoint))
		{
			StartDirection = OtherEndAttachPoint->GetDirection();
			EndDirection = InvertDirection(StartDirection);
		}
	}
	else if (IsValid(OtherStartAttachPoint))
	{
		// Connecting to start of another belt...
		EndDirection = OtherStartAttachPoint->GetDirection();
		StartDirection = InvertDirection(EndDirection);
	}
	else
	{
		// Otherwise, this is a freestanding belt...
		StartDirection = EBuildDirection::Input;
		EndDirection = EBuildDirection::Output;
	}

	MyStartAttachPoint->SetDirection(StartDirection);
	MyEndAttachPoint->SetDirection(EndDirection);
}

/**
 * Flips a Build Direction
 * @param InDirection 
 * @return The opposite Build direction
 */
EBuildDirection AConveyorBelt::InvertDirection(EBuildDirection InDirection) const
{
	if (InDirection == EBuildDirection::Input)
	{
		return EBuildDirection::Output;
	}

	if (InDirection == EBuildDirection::Output)
	{
		return EBuildDirection::Input;
	}

	return InDirection;
}

/**
 * Gets the transform at a distance along spline from a clamped range from 0 to 1
 * @param Alpha 
 * @return 
 */
FTransform AConveyorBelt::GetTransformByAlpha(float Alpha) const
{
	float Start = 0.0f;
	float End = 0.0f;

	if (StartDirection == EBuildDirection::Output)
	{
		Start = Spline->GetSplineLength();
	}
	else
	{
		End = Spline->GetSplineLength();
	}

	return Spline->GetTransformAtDistanceAlongSpline(FMath::Lerp(Start, End, Alpha), ESplineCoordinateSpace::World);
}

float AConveyorBelt::GetLengthByAlpha(float Alpha) const
{
	return FMath::GetMappedRangeValueClamped(FVector2f(0.0f, 1.0), FVector2f(0.0f, Spline->GetSplineLength()), Alpha);
}

AConveyorItem* AConveyorBelt::GetLastItem() const
{
	return LastItemInQueue;
}

void AConveyorBelt::SetupLinkedBuilds()
{
	LOG_INFO(LogTemp, "Starting Build Link");

	SetupLinkedAttachPoint(OtherStartAttachPoint);
	SetupLinkedAttachPoint(OtherEndAttachPoint);

	LOG_INFO(LogTemp, "Build Linking Completed...");
}

void AConveyorBelt::SetupLinkedAttachPoint(AAttachPoint* AttachPoint)
{
	if (!IsValid(AttachPoint))
	{
		LOG_WARNING(LogTemp, "AttachPoint is Invalid");
		return;
	}

	if (AMachine* Machine = Cast<AMachine>(AttachPoint->GetParentBuild()))
	{
		LOG_INFO(LogTemp, "We are connected to a machine");
		switch (AttachPoint->GetDirection())
		{
		case EBuildDirection::Unspecified:
			break;
		case EBuildDirection::Input:
			LOG_INFO(LogTemp, "This AttachPoint is an Input Node and Accepts Items");
			Machine->ConnectAttachPointAsInput(AttachPoint);
			break;
		case EBuildDirection::Output:
			LOG_INFO(LogTemp, "This AttachPoint is an Output Node and Provides Items");
			Machine->ConnectAttachPointAsOutput(AttachPoint);
			break;
		}
		return;
	}

	if (AConveyorBelt* ConveyorBelt = Cast<AConveyorBelt>(AttachPoint->GetParentBuild()))
	{
		LOG_INFO(LogTemp, "We are connected to a Conveyor Belt");
		// Get the first item on the conveyor belt..
		if (ConveyorBelt->ItemsOnConveyor.IsEmpty())
		{
			return;
		}
		AConveyorItem* FirstItem = ConveyorBelt->ItemsOnConveyor[0];
		if (AttachPoint->GetDirection() == EBuildDirection::Input && FirstItem->HasReachedEnd())
		{
			MoveItemToNextConveyor(FirstItem);
		}
	}
}

/**
 * Checks whether there is space to spawn another item
 * @return true if we can spawn another item. False otherwise
 */
bool AConveyorBelt::CanSpawnNextItem()
{
	if (IsValid(LastItemInQueue))
	{
		float DistanceAlongSpline = LastItemInQueue->GetDistanceAlongSpline();
		float ItemInterval = LastItemInQueue->GetItemInterval();
		return FMath::IsNearlyZero(DistanceAlongSpline - ItemInterval, 1.0f) || DistanceAlongSpline > ItemInterval;
	}

	return true;
}

/**
 * Adds an item to the conveyor
 * @param NewItem Item added to this conveyor belt
 * @param bUpdateLast true if this should update the last item in the queue
 */
void AConveyorBelt::AddItemToConveyor(AConveyorItem* NewItem, bool bUpdateLast)
{
	ItemsOnConveyor.Add(NewItem);
	if (bUpdateLast)
	{
		if (IsValid(LastItemInQueue))
		{
			if (LastItemInQueue->OnTimelineStarted.IsBound())
			{
				LastItemInQueue->OnTimelineStarted.Clear();
			}
			NewItem->SetItemInFront(LastItemInQueue);
			NewItem->BindEvents(LastItemInQueue);
		}
		UpdateLastItemInQueue(NewItem);
	}
}

/**
 * Determines whether this belt's input source is connected to another buildable object
 * A buildable object may be a belt, drill, or crafting machine of some sort
 * @return true if the input is connected to another resource's output, false otherwise
 */
bool AConveyorBelt::IsInputConnected() const
{
	if (StartDirection == EBuildDirection::Input)
	{
		return IsValid(MyStartAttachPoint->GetConnectedBuild());
	}

	return IsValid(MyEndAttachPoint->GetConnectedBuild());
}

/**
 * Determines whether this belt's output source is connected to another buildable object
 * A buildable object may be a belt, drill, or crafting machine of some sort
 * @return true if the output is connected to another resource's input, false otherwise
 */
bool AConveyorBelt::IsOutputConnected() const
{
	if (StartDirection == EBuildDirection::Output)
	{
		return IsValid(MyStartAttachPoint->GetConnectedBuild());
	}

	return IsValid(MyEndAttachPoint->GetConnectedBuild());
}

/**
 * Moves a specific item to the next conveyor belt in a chain of linked conveyor belts
 * @param MovedItem Item to move to the next belt
 */
void AConveyorBelt::MoveItemToNextConveyor(AConveyorItem* MovedItem)
{
	ItemsOnConveyor.Remove(MovedItem);

	FBuildInformation BuildInformation;
	if (GetNextBuild(BuildInformation))
	{
		if (AConveyorBelt* Belt = Cast<AConveyorBelt>(BuildInformation.Build))
		{
			Belt->AddItemToConveyor(MovedItem);
			MovedItem->SetupTimeline(Belt);
		}
	}
}

/**
 * Remove's an item from this belt and stores it in whatever machine or container is connected to this belt
 */
void AConveyorBelt::ItemConsumed()
{
	// TODO: Implement...
}

/**
 * Handles logic related to removing an item from the middle of a belt
 * Ensures that any items following this item get updated to point to the correct "next" item to avoid stalled conveyor belts
 * @param Item Item Taken off the conveyor
 */
void AConveyorBelt::OnItemPickedUp(AConveyorItem* Item)
{
	// Get our index
	int32 ItemIndex = ItemsOnConveyor.Find(Item);
	if (ItemIndex == INDEX_NONE)
	{
		LOG_WARNING(LogTemp, "Picked up item was not on belt. this shouldn't happen");
		return;
	}

	// Get our precedingItem
	AConveyorItem* PrecedingItem = Item->GetItemInFront();
	// Get our following Item
	AConveyorItem* SucceedingItem = ItemIndex < ItemsOnConveyor.Num() - 1 ? ItemsOnConveyor[ItemIndex + 1] : nullptr;

	if (Item == LastItemInQueue)
	{
		ItemsOnConveyor.Remove(Item);
		if (!ItemsOnConveyor.IsEmpty())
		{
			LastItemInQueue = ItemsOnConveyor.Last();
		}
		else
		{
			LastItemInQueue = nullptr;
		}
		Item->OnSpawnNext.Broadcast();
		Item->Destroy();
		return;
	}

	// If this is the last item on our belt and we are in front of another belt, get the first item on that belt (if it has one)
	FBuildInformation BuildInformation;
	if (SucceedingItem == nullptr && GetPreviousBuild(BuildInformation))
	{
		if (AConveyorBelt* PreviousConveyor = Cast<AConveyorBelt>(BuildInformation.Build))
		{
			if (!PreviousConveyor->ItemsOnConveyor.IsEmpty())
			{
				SucceedingItem = PreviousConveyor->ItemsOnConveyor[0];
			}
		}
	}

	// Okay... we should have anything that matters...
	if (IsValid(SucceedingItem))
	{
		SucceedingItem->BindEvents(PrecedingItem);
		SucceedingItem->SetItemInFront(PrecedingItem);
		SucceedingItem->RestartMovement();
		ItemsOnConveyor.Remove(Item);
	}

	Item->Destroy();
}

/**
 * Recursively updates all belts to ensure they are consistently aware of what belt is the last item
 * This matters for chains of belts to keep them synchronized
 * @param NewItem 
 */
void AConveyorBelt::UpdateLastItemInQueue(AConveyorItem* NewItem)
{
	// Last Item in Queue is always synchronized across chains of belts
	LastItemInQueue = NewItem;
	FBuildInformation BuildInformation;
	if (GetNextBuild(BuildInformation))
	{
		if (AConveyorBelt* Belt = Cast<AConveyorBelt>(BuildInformation.Build))
		{
			if (Belt->LastItemInQueue != LastItemInQueue)
			{
				Belt->UpdateLastItemInQueue(LastItemInQueue);
			}
		}
	}

	if (GetPreviousBuild(BuildInformation))
	{
		if (AConveyorBelt* Belt = Cast<AConveyorBelt>(BuildInformation.Build))
		{
			if (Belt->LastItemInQueue != LastItemInQueue)
			{
				Belt->UpdateLastItemInQueue(LastItemInQueue);
			}
		}
	}
}

/**
 * Gets the connected attach point and the placeable item it is connected to if there is one
 * @param OutBuildInformation
 * @return true if there is a machine or belt connected to the output of this machine
 */
bool AConveyorBelt::GetNextBuild(FBuildInformation& OutBuildInformation)
{
	if (EndDirection == EBuildDirection::Output)
	{
		OutBuildInformation.AttachPoint = MyEndAttachPoint;
		if (APlaceable* ConnectedBuild = MyEndAttachPoint->GetConnectedBuild())
		{
			OutBuildInformation.Build = ConnectedBuild;
			return true;
		}
		return false;
	}

	OutBuildInformation.AttachPoint = MyStartAttachPoint;
	if (APlaceable* ConnectedBuild = MyStartAttachPoint->GetConnectedBuild())
	{
		OutBuildInformation.Build = ConnectedBuild;
		return true;
	}


	return false;
}

/**
 * Gets the connected attach point and the placeable item it is connected to if there is one
 * @param OutBuildInformation 
 * @return true if there is a machine or belt connected to the input of this machine
 */
bool AConveyorBelt::GetPreviousBuild(FBuildInformation& OutBuildInformation)
{
	if (EndDirection == EBuildDirection::Input)
	{
		OutBuildInformation.AttachPoint = MyEndAttachPoint;
		if (APlaceable* ConnectedBuild = MyEndAttachPoint->GetConnectedBuild())
		{
			OutBuildInformation.Build = ConnectedBuild;
			return true;
		}
		return false;
	}

	if (StartDirection == EBuildDirection::Input)
	{
		OutBuildInformation.AttachPoint = MyStartAttachPoint;
		if (APlaceable* ConnectedBuild = MyStartAttachPoint->GetConnectedBuild())
		{
			OutBuildInformation.Build = ConnectedBuild;
			return true;
		}
	}

	return false;
}

/**
 * Handles custom scrolling on the conveyor belt. In this case, it will raise or lower the start or end support
 * @param Value 
 */
void AConveyorBelt::CustomScrollPreview(float Value)
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

void AConveyorBelt::SetAllMaterialOverlays(UMaterialInterface* MaterialOverlay, bool bRemoveOverlay)
{
	Super::SetAllMaterialOverlays(MaterialOverlay, bRemoveOverlay);
}

USplineComponent* AConveyorBelt::GetSpline() const
{
	return Spline;
}

float AConveyorBelt::GetSpeed() const
{
	return Speed;
}
