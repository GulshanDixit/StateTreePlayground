// © 2025 StateTreePlayground Project. MIT License. Created by github.com/GulshanDixit

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "StateTreePlaygroundGameState.generated.h"

class AGameEventSequencer;

UCLASS()
class STATETREEPLAYGROUND_API AStateTreePlaygroundGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AStateTreePlaygroundGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_GameEventSequencer();

	void SetGameEventSequencer(AGameEventSequencer* Sequencer);

protected:
	UPROPERTY(ReplicatedUsing = OnRep_GameEventSequencer)
	TObjectPtr<AGameEventSequencer> GameEventSequencer;

	UPROPERTY(EditDefaultsOnly, Category = "GameState")
	TSubclassOf<AActor> StateTreeHostClass;

private:
	void SpawnStateTreeHosts();

	bool bHasSpawnedHosts = false;
};
