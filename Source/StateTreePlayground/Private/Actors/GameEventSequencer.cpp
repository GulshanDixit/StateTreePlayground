// © 2025 StateTreePlayground Project. MIT License. Created by github.com/GulshanDixit

#include "StateTreePlayground/Public/Actors/GameEventSequencer.h"

#include "TimerManager.h"

#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogEventSequencer, Log, All);

AGameEventSequencer::AGameEventSequencer()
{
    PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void AGameEventSequencer::BeginPlay()
{
    Super::BeginPlay();

	if (HasAuthority())
	{
		if (EventSequence.Num() > 0 && IntervalSeconds > 0.f)
		{
			GetWorldTimerManager().SetTimer(TimerHandle, this, &AGameEventSequencer::FireNext, IntervalSeconds, bLoop,
											IntervalSeconds);
		}
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
        	Multicast_FireEvent(Tag);
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

void AGameEventSequencer::Multicast_FireEvent_Implementation(FGameplayTag Tag)
{
	OnGameEvent.Broadcast(Tag);
	
	if (const UWorld* World = GetWorld())
	{
		UE_LOG(LogEventSequencer, Log, TEXT("[Time=%.2f] Sequencer fired tag: %s"), World->GetTimeSeconds(), *Tag.ToString());
	}
}
