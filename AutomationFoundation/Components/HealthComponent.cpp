#include "HealthComponent.h"

#include "AutomationFoundation/Core/AutomationFoundationUtilities.h"

void UHealthComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);

	// TODO: Add Instigator to this method...
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
}

float UHealthComponent::GetHealth() const
{
	return Health;
}

float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

bool UHealthComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UHealthComponent::IsFull() const
{
	return FMath::IsNearlyEqual(MaxHealth, Health);
}
