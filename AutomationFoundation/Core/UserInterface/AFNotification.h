#pragma once
#include "AFUserWidget.h"

#include "AFnotification.generated.h"

class UAFImage;
class UAFTextBlock;

UCLASS()
class UAFNotification : public UAFUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidgetOptional), EditAnywhere)
	UAFImage* Icon;

	UPROPERTY(meta=(BindWidget), EditAnywhere)
	UAFTextBlock* DisplayText;
};
