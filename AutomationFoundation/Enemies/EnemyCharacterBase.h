#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "EnemyCharacterBase.generated.h"

class UAFUWHealthBar;
class UUserWidget;
class UWidgetComponent;
class UHealthComponent;

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

	// Maybe an inventory if we want to loot?
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTags")
	FGameplayTagContainer GameplayTags;

	AEnemyCharacterBase();

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

private:
	UFUNCTION()
	void OnHealthChanged(AActor* InInstigator, UHealthComponent* OwningComponent, float
	                     UpdatedHealth, float Delta);
};
