// © 2025 StateTreePlayground Project. MIT License. Created by github.com/GulshanDixit

#include "StateTreePlayground/Public/Actors/GameEventSequencer.h"

#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogEventSequencer, Log, All);

AGameEventSequencer::AGameEventSequencer()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AGameEventSequencer::BeginPlay()
{
    Super::BeginPlay();

    if (EventSequence.Num() > 0 && IntervalSeconds > 0.f)
    {
        GetWorldTimerManager().SetTimer(TimerHandle, this, &AGameEventSequencer::FireNext, IntervalSeconds, bLoop,
                                        IntervalSeconds);
    }
}

void AGameEventSequencer::FireNext()
{
    if (EventSequence.Num() == 0)
    {
        return;
    }

    const UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogEventSequencer, Warning, TEXT("No valid world in GameEventSequencer"));
        return;
    }

    if (Index < EventSequence.Num())
    {
        const FGameplayTag Tag = EventSequence[Index++];
        if (Tag.IsValid())
        {
            OnGameEvent.Broadcast(Tag);
            UE_LOG(LogEventSequencer, Log, TEXT("[Time=%.2f] Sequencer fired tag: %s"), World->GetTimeSeconds(),
                   *Tag.ToString());
        }
    }

    if (Index >= EventSequence.Num())
    {
        if (bLoop)
        {
            Index = 0;
        }
        else
        {
            GetWorldTimerManager().ClearTimer(TimerHandle);
        }
    }
}
