#include "Lobby/CLobbySurvivorState.h"
#include "Lobby/CLobbySurvivor.h"
#include "Net/UnrealNetwork.h"
#include "Utility/CDebug.h"

ACLobbySurvivorState::ACLobbySurvivorState()
{

}

void ACLobbySurvivorState::SaveHeadMeshName(FName InHeadName)
{
	if (HasAuthority()) // 서버에서만 실행
	{
		CDebug::Print("Saved HeadMeshName !");
		SavedHeadName = InHeadName;
	}
}

FName ACLobbySurvivorState::GetHeadMeshName() const
{
	return SavedHeadName;
}

void ACLobbySurvivorState::OnRep_SavedHeadName()
{
	//CDebug::Print("OnRep_SavedHeadName Called !");
	//ACLobbySurvivor* lobbySurvivor = Cast<ACLobbySurvivor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	//lobbySurvivor->UpdateMesh();
	// 클라이언트 측에서 헤드 메시 이름이 변경되었을 때 실행할 로직 추가
}

void ACLobbySurvivorState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACLobbySurvivorState, SavedHeadName);
}