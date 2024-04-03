// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"

#include "AutomationFoundation/Core/AutomationFoundationGameMode.h"
#include "AutomationFoundation/Inventory/ItemPickup.h"
#include "Kismet/GameplayStatics.h"
// #include "Inventory/AutoPickup.h"

// Sets default values
ATestActor::ATestActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("Setting up timer!"));
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ATestActor::OnTimeExpired, 2.0f);
	UE_LOG(LogTemp, Display, TEXT("Timer Set up"));
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestActor::OnTimeExpired() const
{
	UE_LOG(LogTemp, Display, TEXT("Timer Fired!"));

	// Let's Spawn a Wooden Axe
	AAutomationFoundationGameMode* GameMode = Cast<AAutomationFoundationGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
	{
		UE_LOG(LogTemp, Display, TEXT("Invalid Game Mode"));
		return;
	}


	FItemSpecificationDataTableReference ItemSpecificationRef;

	if (!GameMode->FindItem(TEXT("WoodenAxe"), ItemSpecificationRef))
	{
		UE_LOG(LogTemp, Display, TEXT("Could not find Expected Item"));
		return;
	}

	UInventoryItemInstance* NewItem = NewObject<UInventoryItemInstance>();
	NewItem->ItemSpecification = ItemSpecificationRef.ItemSpecificationRef.LoadSynchronous();

	const FVector SpawnLocation(1550.0f, 850.0f, 60.0f);
	const FRotator SpawnRotation(0.0f, 0.0f, 0.0f);
	const FTransform SpawnTransform(SpawnLocation);
	AItemPickup* ItemPickup = GetWorld()->SpawnActorDeferred<AItemPickup>(AItemPickup::StaticClass(), SpawnTransform);
	if (ItemPickup)
	{
		ItemPickup->ConfigureItemPickup(NewItem);
		UGameplayStatics::FinishSpawningActor(ItemPickup, FTransform(SpawnRotation, SpawnLocation));
	}
	// ItemPickup->SetActorHiddenInGame(false);

	UE_LOG(LogTemp, Display, TEXT("Successfully Spawned New Pickup Actor"))
}
