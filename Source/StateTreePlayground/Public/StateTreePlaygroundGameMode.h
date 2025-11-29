// © 2025 StateTreePlayground Project. MIT License. Created by github.com/GulshanDixit

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StateTreePlaygroundGameMode.generated.h"

class AGameEventSequencer;

UCLASS()
class STATETREEPLAYGROUND_API AStateTreePlaygroundGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AStateTreePlaygroundGameMode();
	
	virtual void StartPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game Events")
	TSubclassOf<AGameEventSequencer> GameEventSequencerClass;
};
