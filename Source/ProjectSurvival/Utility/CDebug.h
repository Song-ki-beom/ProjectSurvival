#pragma once

#include "CoreMinimal.h"

/**
* ���ϸ�, �Լ���, �� ������� ����ϴ� ��ũ��
* LogLine - �α� ���
* PrintLine - �⺻ �޽��� ��� (�⺻ ���� �����, �⺻ ��� �ð� 10��)
* PrintLine_Detail(Color, Duration) - Ŀ���� �޽��� ��� (���� FColor, �ð� float)
* 
* Log �Լ� : int32, float, bool, FVector, FRotator, FQuat, UObject �� �α� ���
* Log(Value) -> ���� ���
* Log("Text", Value) -> �� �տ� �ؽ�Ʈ �ٿ��� ���
* Log(Value, "Text") -> �� �ڿ� �ؽ�Ʈ �ٿ��� ���
* 
* Print �Լ� : int32, float, bool, FVector, FRotator, FQuat, UObject �� �޽��� ��� (�⺻ ���� �����, �⺻ ��� �ð� 10��)
* Print(Value) -> ���� ��� (�⺻ ���� �����, �⺻ ��� �ð� 10��)
* Print("Text", Value) -> �� �տ� �ؽ�Ʈ �ٿ��� ��� (�⺻ ���� �����, �⺻ ��� �ð� 10��)
* Print(Value, "Text") -> �� �ڿ� �ؽ�Ʈ �ٿ��� ��� (�⺻ ���� �����, �⺻ ��� �ð� 10��)
* Print(Value, Color, Duration) -> ���� ��� (���� ����, ���� ��� �ð�)
* Print("Text", Value, Color, Duration) -> �� �տ� �ؽ�Ʈ �ٿ��� ��� (���� ����, ���� ��� �ð�)
* Print(Value, "Text", Color, Duration) -> �� �ڿ� �ؽ�Ʈ �ٿ��� ��� (���� ����, ���� ��� �ð�)
*/

DECLARE_LOG_CATEGORY_EXTERN(UserDebug, Log, All);

#define LogLine CDebug::Log(__FILE__,__FUNCTION__,__LINE__);

#define PrintLine CDebug::Print(__FILE__,__FUNCTION__, __LINE__);
#define PrintLine_Detail(Color, Duration) CDebug::Print(__FILE__,__FUNCTION__, __LINE__, Color, Duration);

class PROJECTSURVIVAL_API CDebug
{
public:
	static void Log(const FString& FileName, const FString& FunctionName, int32 LineNumber);
	static void Log(const FString& Message);

	static void Log(int Value, const FString& Message = "");
	static void Log(float Value, const FString& Message = "");
	static void Log(bool Value, const FString& Message = "");
	static void Log(const FVector& Value, const FString& Message = "");
	static void Log(const FRotator& Value, const FString& Message = "");
	static void Log(const FQuat& Value, const FString& Message = "");
	static void Log(const UObject* Value, const FString& Message = "");

	static void Log(const FString& Message, int32 Value);
	static void Log(const FString& Message, float Value);
	static void Log(const FString& Message, bool Value);
	static void Log(const FString& Message, const FVector& Value);
	static void Log(const FString& Message, const FRotator& Value);
	static void Log(const FString& Message, const FQuat& Value);
	static void Log(const FString& Message, const UObject* Value);

	static void Print(const FString& FileName, const FString& FunctionName, int32 LineNumber, FColor Color = FColor::Yellow, float Duration = 10.0f, int32 Key = -1);
	static void Print(const FString& Message, FColor Color = FColor::Yellow, float Duration = 10.0f, int32 Key = -1);
	
	static void Print(int32 Value, const FString& Message = "", FColor Color = FColor::Yellow, float Duration = 10.0f, int32 Key = -1);
	static void Print(float Value, const FString& Message = "", FColor Color = FColor::Yellow, float Duration = 10.0f, int32 Key = -1);
	static void Print(bool Value, const FString& Message = "", FColor Color = FColor::Yellow, float Duration = 10.0f, int32 Key = -1);
	static void Print(const FVector& Value, const FString& Message = "", FColor Color = FColor::Yellow, float Duration = 10.0f, int32 Key = -1);
	static void Print(const FRotator& Value, const FString& Message = "", FColor Color = FColor::Yellow, float Duration = 10.0f, int32 Key = -1);
	static void Print(const FQuat& Value, const FString& Message = "", FColor Color = FColor::Yellow, float Duration = 10.0f, int32 Key = -1);
	static void Print(const UObject* Value, const FString& Message = "", FColor Color = FColor::Yellow, float Duration = 10.0f, int32 Key = -1);

	static void Print(const FString& Message, int32 Value, FColor Color = FColor::Yellow, float Duration = 10.0f, int32 Key = -1);
	static void Print(const FString& Message, float Value, FColor Color = FColor::Yellow, float Duration = 10.0f, int32 Key = -1);
	static void Print(const FString& Message, bool Value, FColor Color = FColor::Yellow, float Duration = 10.0f, int32 Key = -1);
	static void Print(const FString& Message, const FVector& Value, FColor Color = FColor::Yellow, float Duration = 10.0f, int32 Key = -1);
	static void Print(const FString& Message, const FRotator& Value, FColor Color = FColor::Yellow, float Duration = 10.0f, int32 Key = -1);
	static void Print(const FString& Message, const FQuat& Value, FColor Color = FColor::Yellow, float Duration = 10.0f, int32 Key = -1);
	static void Print(const FString& Message, const UObject* Value, FColor Color = FColor::Yellow, float Duration = 10.0f, int32 Key = -1);
};