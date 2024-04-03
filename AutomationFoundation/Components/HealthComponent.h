#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.generated.h"

class UHealthComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InInstigator, UHealthComponent*, OwningComponent, float,
                                    UpdatedHealth, float, Delta);

UCLASS(BlueprintType)
class UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float Health = 100.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float MaxHealth = 100.0f;

public:
	FOnHealthChanged OnHealthChanged;
	
	void ApplyHealthChange(float Delta);

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

private:
	friend class APawn;
};
