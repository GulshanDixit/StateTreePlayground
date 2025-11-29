// © 2025 StateTreePlayground Project. MIT License. Created by github.com/GulshanDixit

#include "StateTreePlayground/Public/StateTreePlaygroundGameState.h"

#include "Net/UnrealNetwork.h"
#include "StateTreePlayground/Public/Actors/GameEventSequencer.h"

AStateTreePlaygroundGameState::AStateTreePlaygroundGameState()
{
    bReplicates = true;
}

void AStateTreePlaygroundGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AStateTreePlaygroundGameState, GameEventSequencer);
}

void AStateTreePlaygroundGameState::OnRep_GameEventSequencer()
{
    SpawnStateTreeHosts();
}

void AStateTreePlaygroundGameState::SetGameEventSequencer(AGameEventSequencer* Sequencer)
{
    GameEventSequencer = Sequencer;

    if (HasAuthority())
    {
        SpawnStateTreeHosts();
    }
}

void AStateTreePlaygroundGameState::SpawnStateTreeHosts()
{
    if (bHasSpawnedHosts)
    {
        return;
    }

    if (!GameEventSequencer || !StateTreeHostClass)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    World->SpawnActor<AActor>(StateTreeHostClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

    bHasSpawnedHosts = true;
}
