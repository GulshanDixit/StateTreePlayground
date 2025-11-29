// © 2025 StateTreePlayground Project. MIT License. Created by github.com/GulshanDixit

#include "StateTreePlayground/Public/StateTreePlaygroundGameMode.h"

#include "StateTreePlayground/Public/Actors/GameEventSequencer.h"
#include "StateTreePlayground/Public/StateTreePlaygroundGameState.h"

AStateTreePlaygroundGameMode::AStateTreePlaygroundGameMode()
{
    GameStateClass = AStateTreePlaygroundGameState::StaticClass();
}

void AStateTreePlaygroundGameMode::StartPlay()
{
    Super::StartPlay();

    if (GameEventSequencerClass)
    {
        AGameEventSequencer* Sequencer = GetWorld()->SpawnActor<AGameEventSequencer>(GameEventSequencerClass);

        if (AStateTreePlaygroundGameState* PlaygroundGameState = GetGameState<AStateTreePlaygroundGameState>())
        {
            PlaygroundGameState->SetGameEventSequencer(Sequencer);
        }
    }
}
