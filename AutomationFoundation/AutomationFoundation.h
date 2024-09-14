// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


inline ETraceTypeQuery ETT_AF_Deconstruct = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);
inline ETraceTypeQuery ETT_AF_General = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2);
inline ETraceTypeQuery ETT_AF_Interact = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel3);

inline ECollisionChannel ECC_AF_Deconstruct = ECC_GameTraceChannel1;
inline ECollisionChannel ECC_AF_General = ECC_GameTraceChannel2;
inline ECollisionChannel ECC_AF_Interact = ECC_GameTraceChannel3;
