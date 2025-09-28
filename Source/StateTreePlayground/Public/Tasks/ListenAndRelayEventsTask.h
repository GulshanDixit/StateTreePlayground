// © 2025 StateTreePlayground Project. MIT License. Created by github.com/GulshanDixit

#pragma once

#include "Actors/GameEventSequencer.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "ListenAndRelayEventsTask.generated.h"

class AGameEventSequencer;
class UStateTreeComponent;

/**
 * Listens to a GameEventSequencer and immediately relays accepted GameplayTags as StateTree events.
 */
UCLASS(DisplayName = "Listen & Relay Game Events (C++ BPBase)")
class STATETREEPLAYGROUND_API UListenAndRelayEventsTask : public UStateTreeTaskBlueprintBase
{
    GENERATED_BODY()

protected:
    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
                                           const FStateTreeTransitionResult& Transition) override;
    virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

    UPROPERTY(EditAnywhere, Category = "Filter")
    TArray<FGameplayTag> AcceptUnderParents;

    UPROPERTY(EditAnywhere, Category = "Filter")
    FGameplayTag ListenForExactTag;

    UPROPERTY(EditAnywhere, Category = "Relay")
    bool bOverrideRelayTag = false;

    UPROPERTY(EditAnywhere, Category = "Relay", meta = (EditCondition = "bOverrideRelayTag"))
    FGameplayTag RelayAsTag;

    UPROPERTY(EditAnywhere, Category = "Binding")
    bool bAutoFindSequencerInWorld = true;

    UPROPERTY(EditAnywhere, Category = "Binding", meta = (EditCondition = "bAutoFindSequencerInWorld"))
    FName SequencerActorTag = NAME_None;

    UPROPERTY(EditAnywhere, Category = "Binding", meta = (EditCondition = "bAutoFindSequencerInWorld"))
    TSubclassOf<AGameEventSequencer> SequencerClass;

private:
    UFUNCTION()
    void HandleGameEvent(FGameplayTag Tag);

    TWeakObjectPtr<AGameEventSequencer> EventSource;

    TWeakObjectPtr<UStateTreeComponent> CachedComponent;
};