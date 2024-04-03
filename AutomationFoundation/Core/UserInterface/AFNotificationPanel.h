#pragma once
#include "AFUserWidget.h"

#include "AFNotificationPanel.generated.h"

class UAFNotification;
class USizeBox;

UCLASS()
class UAFNotificationPanel : public UAFUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	USizeBox* NotificationPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAFNotification> NotificationClass;

public:
	UFUNCTION()
	void AddNotification();
};
