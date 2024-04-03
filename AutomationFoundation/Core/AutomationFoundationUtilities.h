#pragma once

#include "Engine/Engine.h"

#define LOG(LogCategory, LogLevel, Color, X, ...) \
UE_LOG(LogCategory, LogLevel, TEXT(X) __VA_OPT__(,) __VA_ARGS__); \
if (GEngine) \
{ \
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, Color, FString::Printf(TEXT(X) __VA_OPT__(,) __VA_ARGS__), true, FVector2D(1.f, 1.f)); \
}

#define LOG_INFO(LogCategory, X, ...) LOG(LogCategory, Display, FColor::White, X, __VA_ARGS__)
#define LOG_WARNING(LogCategory, X, ...) LOG(LogCategory, Warning, FColor::Yellow, X, __VA_ARGS__)
#define LOG_ERROR(LogCategory, X, ...) LOG(LogCategory, Error, FColor::Red, X, __VA_ARGS__)