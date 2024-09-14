#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "EnemyCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, class AEnemyCharacterBase*, EnemyUnit);

class UAFUWHealthBar;
class UUserWidget;
class UWidgetComponent;
class UHealthComponent;
class AEnemyBase;

UCLASS()
class AUTOMATIONFOUNDATION_API AEnemyCharacterBase : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* HealthBarWidgetComponent;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAFUWHealthBar> HealthBarWidgetClass;

	// UPROPERTY()
	// TWeakObjectPtr<AEnemyBase> HomeBase;

	// Maybe an inventory if we want to loot?
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTags")
	FGameplayTagContainer GameplayTags;

	AEnemyCharacterBase();

	FOnDeath OnDeath;

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	UFUNCTION()
	void OnHomeBaseDestroyed();

private:
	UFUNCTION()
	void OnHealthChanged(AActor* InInstigator, UHealthComponent* OwningComponent, float
	                     UpdatedHealth, float Delta);
};
