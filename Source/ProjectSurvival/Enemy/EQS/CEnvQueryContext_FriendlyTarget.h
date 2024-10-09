// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "CEnvQueryContext_FriendlyTarget.generated.h"


UCLASS()
class PROJECTSURVIVAL_API UCEnvQueryContext_FriendlyTarget : public UEnvQueryContext
{
	GENERATED_BODY()
	
protected:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

};
