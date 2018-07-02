// All rights reserved, Muhammad A.Moniem 2016 http://www.mamoniem.com/

#pragma once

#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class BELLZ_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();

	UPROPERTY(transient)
		UBlackboardComponent* BlackboardComp;

	UPROPERTY(transient)
		UBehaviorTreeComponent* BehaviorComp;

	virtual void Possess(class APawn* InPawn) override;

	virtual void BeginInactiveState() override;

	void Respawn();


	UFUNCTION(BlueprintCallable, Category = Behavior)
	void SetEnemy(class APawn* InPawn);
	UFUNCTION(BlueprintCallable, Category = Behavior)
	class AGladiator* GetEnemy() const;



	UFUNCTION(BlueprintCallable, Category = Behaviour)
	void UpdateControlRotation(float DeltaTime, bool bUpdatePawn);

	UFUNCTION(BlueprintCallable, Category = Behaviour)
	bool PawnCanBeSeen(APawn * target);

	/* Checks sight to all pawns in map, sets enemy if it finds a thing */

	UFUNCTION(BlueprintCallable, Category = Behaviour)
	void OnSearchForEnemy();


protected:
	int32 EnemyKeyID;
	int32 EnemyPositionKeyID;
	FTimerHandle SpawnTimer;
	
	
};
