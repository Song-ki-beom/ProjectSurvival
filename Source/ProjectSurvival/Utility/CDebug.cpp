#include "CDebug.h"

DEFINE_LOG_CATEGORY(UserDebug)

void CDebug::Log(const FString& FileName, const FString& FunctionName, int32 LineNumber)
{
	FString cleanFileName = FPaths::GetCleanFilename(FileName);

	int32 colonLocation = FunctionName.Find("::");
	FString cleanFunctionName = FunctionName.Mid(colonLocation + 2);

	UE_LOG(UserDebug, Warning, TEXT("[%s - %s - %d]"), *cleanFileName, *cleanFunctionName, LineNumber);
}

void CDebug::Log(const FString& Message)
{
	UE_LOG(UserDebug, Warning, TEXT("%s"), *Message);
}

void CDebug::Log(int Value, const FString& Message)
{
	UE_LOG(UserDebug, Warning, TEXT("%d %s"), Value, *Message);
}

void CDebug::Log(float Value, const FString& Message)
{
	UE_LOG(UserDebug, Warning, TEXT("%f %s"), Value, *Message);
}

void CDebug::Log(bool Value, const FString& Message)
{
	FString boolValue = Value ? "TRUE" : "FALSE";
	UE_LOG(UserDebug, Warning, TEXT("%s %s"), *boolValue, *Message);
}

void CDebug::Log(const FVector& Value, const FString& Message)
{
	UE_LOG(UserDebug, Warning, TEXT("%s %s"), *Value.ToString(), *Message);
}

void CDebug::Log(const FRotator& Value, const FString& Message)
{
	UE_LOG(UserDebug, Warning, TEXT("%s %s"), *Value.ToString(), *Message);
}

void CDebug::Log(const FQuat& Value, const FString& Message)
{
	UE_LOG(UserDebug, Warning, TEXT("%s %s"), *Value.ToString(), *Message);
}

void CDebug::Log(const UObject* Value, const FString& Message)
{
	UE_LOG(UserDebug, Warning, TEXT("%s %s"), *Value->GetName(), *Message);
}

void CDebug::Log(const FString& Message, int32 Value)
{
	UE_LOG(UserDebug, Warning, TEXT("%s %d"), *Message, Value);
}

void CDebug::Log(const FString& Message, float Value)
{
	UE_LOG(UserDebug, Warning, TEXT("%s %f"), *Message, Value);
}

void CDebug::Log(const FString& Message, bool Value)
{
	FString boolValue = Value ? "TRUE" : "FALSE";
	UE_LOG(UserDebug, Warning, TEXT("%s %s"), *Message, *boolValue);
}

void CDebug::Log(const FString& Message, const FVector& Value)
{
	UE_LOG(UserDebug, Warning, TEXT("%s %s"), *Message, *Value.ToString());
}

void CDebug::Log(const FString& Message, const FRotator& Value)
{
	UE_LOG(UserDebug, Warning, TEXT("%s %s"), *Message, *Value.ToString());
}

void CDebug::Log(const FString& Message, const FQuat& Value)
{
	UE_LOG(UserDebug, Warning, TEXT("%s %s"), *Message, *Value.ToString());
}

void CDebug::Log(const FString& Message, const UObject* Value)
{
	UE_LOG(UserDebug, Warning, TEXT("%s %s"), *Message, *Value->GetName());
}

void CDebug::Print(const FString& FileName, const FString& FunctionName, int32 LineNumber, FColor Color, float Duration, int32 Key)
{
	FString cleanFileName = FPaths::GetCleanFilename(FileName);

	int32 colonLocation = FunctionName.Find("::");
	FString cleanFunctionName = FunctionName.Mid(colonLocation + 2);

	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::Printf(TEXT("[%s - %s - %d]"), *cleanFileName, *cleanFunctionName, LineNumber));
}

void CDebug::Print(int32 Value, const FString& Message, FColor Color, float Duration, int32 Key)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::Printf(TEXT("%d %s"), Value, *Message));
}

void CDebug::Print(float Value, const FString& Message, FColor Color, float Duration, int32 Key)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::Printf(TEXT("%f %s"), Value, *Message));
}

void CDebug::Print(bool Value, const FString& Message, FColor Color, float Duration, int32 Key)
{
	FString boolValue = Value ? "TRUE" : "FALSE";
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::Printf(TEXT("%s %s"), *boolValue, *Message));
}

void CDebug::Print(const FVector& Value, const FString& Message, FColor Color, float Duration, int32 Key)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::Printf(TEXT("%s %s"), *Value.ToString(), *Message));
}

void CDebug::Print(const FRotator& Value, const FString& Message, FColor Color, float Duration, int32 Key)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::Printf(TEXT("%s %s"), *Value.ToString(), *Message));
}

void CDebug::Print(const FQuat& Value, const FString& Message, FColor Color, float Duration, int32 Key)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::Printf(TEXT("%s %s"), *Value.ToString(), *Message));
}

void CDebug::Print(const UObject* Value, const FString& Message, FColor Color, float Duration, int32 Key)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::Printf(TEXT("%s %s"), *Value->GetName(), *Message));
}

void CDebug::Print(const FString& Message, int32 Value, FColor Color, float Duration, int32 Key)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::Printf(TEXT("%s %d"), *Message, Value));
}

void CDebug::Print(const FString& Message, float Value, FColor Color, float Duration, int32 Key)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::Printf(TEXT("%s %f"), *Message, Value));
}

void CDebug::Print(const FString& Message, bool Value, FColor Color, float Duration, int32 Key)
{
	FString boolValue = Value ? "TRUE" : "FALSE";
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::Printf(TEXT("%s %s"), *Message, *boolValue));
}

void CDebug::Print(const FString& Message, const FVector& Value, FColor Color, float Duration, int32 Key)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::Printf(TEXT("%s %s"), *Message, *Value.ToString()));
}

void CDebug::Print(const FString& Message, const FRotator& Value, FColor Color, float Duration, int32 Key)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::Printf(TEXT("%s %s"), *Message, *Value.ToString()));
}

void CDebug::Print(const FString& Message, const FQuat& Value, FColor Color, float Duration, int32 Key)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::Printf(TEXT("%s %s"), *Message, *Value.ToString()));
}

void CDebug::Print(const FString& Message, const UObject* Value, FColor Color, float Duration, int32 Key)
{
	GEngine->AddOnScreenDebugMessage(Key, Duration, Color, FString::Printf(TEXT("%s %s"), *Message, *Value->GetName()));
}