#pragma once

#include "CoreMinimal.h"

/**
* 파일명, 함수명, 줄 순서대로 출력하는 매크로
* LogLine - 로그 출력
* PrintLine - 기본 메시지 출력 (기본 색상 노란색, 기본 출력 시간 10초)
* PrintLine_Detail(Color, Duration) - 커스텀 메시지 출력 (색상 FColor, 시간 float)
* 
* Log 함수 : int32, float, bool, FVector, FRotator, FQuat, UObject 값 로그 출력
* Log(Value) -> 값만 출력
* Log("Text", Value) -> 값 앞에 텍스트 붙여서 출력
* Log(Value, "Text") -> 값 뒤에 텍스트 붙여서 출력
* 
* Print 함수 : int32, float, bool, FVector, FRotator, FQuat, UObject 값 메시지 출력 (기본 색상 노란색, 기본 출력 시간 10초)
* Print(Value) -> 값만 출력 (기본 색상 노란색, 기본 출력 시간 10초)
* Print("Text", Value) -> 값 앞에 텍스트 붙여서 출력 (기본 색상 노란색, 기본 출력 시간 10초)
* Print(Value, "Text") -> 값 뒤에 텍스트 붙여서 출력 (기본 색상 노란색, 기본 출력 시간 10초)
* Print(Value, Color, Duration) -> 값만 출력 (지정 색상, 지정 출력 시간)
* Print("Text", Value, Color, Duration) -> 값 앞에 텍스트 붙여서 출력 (지정 색상, 지정 출력 시간)
* Print(Value, "Text", Color, Duration) -> 값 뒤에 텍스트 붙여서 출력 (지정 색상, 지정 출력 시간)
* 
* bool 디버그의 경우 bool이 앞에 나오는 경우 암시적 변환 때문에 구현을 하지않음. Print("Text", bIsLive) 이런식으로만 쓸 것
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