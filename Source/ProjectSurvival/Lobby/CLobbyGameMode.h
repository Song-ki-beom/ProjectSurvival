#pragma once

#include "CoreMinimal.h"
#include "CMainGameMode.h"
#include "CLobbyGameMode.generated.h"

/**
* StartGame() - �����ο� = Ŀ���͸���¡ �ο� ���¿��� Ŭ�� �� Main ������ �̵�
*/

UCLASS()
class PROJECTSURVIVAL_API ACLobbyGameMode : public ACMainGameMode
{
	GENERATED_BODY()

public:
	ACLobbyGameMode();

protected:
	void PostLogin(APlayerController* NewPlayer) override;
	void Tick(float DeltaSeconds) override;

public:
	void ReadyPlayer();
	bool CheckPlayer();
	void StartGame();

private:
	uint32 NumberOfSurvivors = 0;
	uint32 NumberOfReadySurvivors = 1;


};
