#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ActorComponents/CEnemyAIComponent.h"
#include "CBTTaskNode_ChangeState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSURVIVAL_API UCBTTaskNode_ChangeState : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCBTTaskNode_ChangeState();
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
		EAIStateType TargetType;

};
