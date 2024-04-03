#include "RoboticTransporter.h"

#include "AutomationFoundation/Inventory/IItemAcceptor.h"
#include "AutomationFoundation/Inventory/IItemProvider.h"
#include "Components/BoxComponent.h"

ARoboticTransporter::ARoboticTransporter()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SourceDetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Source Detector"));
	DestinationDetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Destination Detector"));
}

void ARoboticTransporter::BeginPlay()
{
	Super::BeginPlay();
	if (SourceDetectionBox)
	{
		SourceDetectionBox->OnComponentBeginOverlap.AddDynamic(this, &ARoboticTransporter::OnSourceDetectionBeginOverlap);
		SourceDetectionBox->OnComponentEndOverlap.AddDynamic(this, &ARoboticTransporter::OnSourceDetectionEndOverlap);
	}

	if (DestinationDetectionBox)
	{
		DestinationDetectionBox->OnComponentBeginOverlap.AddDynamic(this, &ARoboticTransporter::OnDestinationDetectionBeginOverlap);
		DestinationDetectionBox->OnComponentEndOverlap.AddDynamic(this, &ARoboticTransporter::OnDestinationDetectionEndOverlap);
	}
}

void ARoboticTransporter::OnSourceDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                                        const FHitResult& SweepResult)
{
	if (IItemProvider* ItemProvider = Cast<IItemProvider>(OtherActor))
	{
		ItemSource = ItemProvider->GetProviderInventory();
		// ItemSource.OnItem
	}
}

void ARoboticTransporter::OnSourceDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	ItemSource = nullptr;
}

void ARoboticTransporter::OnDestinationDetectionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                                             const FHitResult& SweepResult)
{
	if (IItemAcceptor* ItemAcceptor = Cast<IItemAcceptor>(OtherActor))
	{
		ItemDestination = ItemAcceptor->GetAcceptorInventory();
	}
}

void ARoboticTransporter::OnDestinationDetectionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	ItemDestination = nullptr;
}
