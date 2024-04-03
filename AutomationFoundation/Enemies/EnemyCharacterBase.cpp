#include "EnemyCharacterBase.h"

#include "AutomationFoundation/Components/HealthComponent.h"
#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"
#include "AutomationFoundation/UserInterface/AFUWHealthBar.h"
#include "Components/WidgetComponent.h"

AEnemyCharacterBase::AEnemyCharacterBase()
{
	GameplayTags.AddTag(FGameplayTag::RequestGameplayTag(FName("AutomationFoundation.Enemy")));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

	HealthComponent->OnHealthChanged.AddDynamic(this, &AEnemyCharacterBase::OnHealthChanged);

	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar Widget"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
}

void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(HealthBarWidgetComponent))
	{
		HealthBarWidgetComponent->SetWidgetClass(HealthBarWidgetClass);

		UAFUWHealthBar* HealthBarWidget = Cast<UAFUWHealthBar>(HealthBarWidgetComponent->GetWidget());
		if (IsValid(HealthBarWidget))
		{
			HealthComponent->OnHealthChanged.AddDynamic(HealthBarWidget, &UAFUWHealthBar::OnHealthChanged);
		}
	}
}

void AEnemyCharacterBase::BeginDestroy()
{
	Super::BeginDestroy();
}


void AEnemyCharacterBase::OnHealthChanged(AActor* InInstigator, UHealthComponent* OwningComponent, float UpdatedHealth, float Delta)
{
	if (!HealthComponent->IsAlive())
	{
		Destroy();
	}
}
