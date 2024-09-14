// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AFUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AUTOMATIONFOUNDATION_API UAFUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void WidgetActivated();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void WidgetDeactivated();
};
