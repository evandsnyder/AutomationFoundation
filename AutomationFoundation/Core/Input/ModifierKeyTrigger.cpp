#include "ModifierKeyTrigger.h"

#include "EnhancedPlayerInput.h"

ETriggerType UModifierKeyTrigger::GetTriggerType_Implementation() const
{
	return ETriggerType::Implicit;
}

ETriggerState UModifierKeyTrigger::UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime)
{
	return (IsActuated(ModifiedValue) && PlayerInput->IsPressed(ModifierKey)) ? ETriggerState::Triggered : ETriggerState::None;
}

#if WITH_EDITOR
/**
 * Prevents non-modifier keys being used as modifiers
 * @param PropertyChangedEvent 
 */
void UModifierKeyTrigger::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName().IsEqual(GET_MEMBER_NAME_CHECKED(UModifierKeyTrigger, ModifierKey)))
	{
		if (!ModifierKey.IsModifierKey())
		{
			ModifierKey = EKeys::LeftControl;
		}
	}
}
#endif
