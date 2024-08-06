#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Lobby/CLobbySurvivor.h"
#include "CLobbySurvivorState.generated.h"

UCLASS()
class PROJECTSURVIVAL_API ACLobbySurvivorState : public APlayerState
{
	GENERATED_BODY()

public:
	ACLobbySurvivorState();

	UFUNCTION()
		void SaveHeadMeshName(FName InHeadName);

	UFUNCTION()
		FName GetHeadMeshName() const;

	UPROPERTY(ReplicatedUsing = OnRep_SavedHeadName)
		FName SavedHeadName;

	UFUNCTION()
		void OnRep_SavedHeadName();
};
