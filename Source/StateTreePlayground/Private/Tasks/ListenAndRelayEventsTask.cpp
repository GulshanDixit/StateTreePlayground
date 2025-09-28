// © 2025 StateTreePlayground Project. MIT License. Created by github.com/GulshanDixit

#include "Tasks/ListenAndRelayEventsTask.h"

#include "Actors/GameEventSequencer.h"
#include "Components/StateTreeComponent.h"
#include "StateTreeExecutionContext.h"
#include "Utils/STEventBindingUtils.h"

EStateTreeRunStatus UListenAndRelayEventsTask::EnterState(FStateTreeExecutionContext& Context,
                                                          const FStateTreeTransitionResult& /*Transition*/)
{
    if (const AActor* Owner = Cast<AActor>(Context.GetOwner()))
    {
        CachedComponent = Owner->GetComponentByClass<UStateTreeComponent>();
    }

    if (AGameEventSequencer* Source = STEventBindingUtils::ResolveSequencer(
                Context.GetOwner(), bAutoFindSequencerInWorld, SequencerActorTag, SequencerClass))
    {
        EventSource = Source;
        Source->OnGameEvent.AddDynamic(this, &ThisClass::HandleGameEvent);
    }

    return EStateTreeRunStatus::Running;
}

void UListenAndRelayEventsTask::ExitState(FStateTreeExecutionContext& /*Context*/,
                                          const FStateTreeTransitionResult& /*Transition*/)
{
    if (EventSource.IsValid())
    {
        if (AGameEventSequencer* Source = EventSource.Get())
        {
            Source->OnGameEvent.RemoveDynamic(this, &ThisClass::HandleGameEvent);
        }
    }
}

void UListenAndRelayEventsTask::HandleGameEvent(FGameplayTag Tag)
{
    if (!AcceptUnderParents.IsEmpty())
    {
        bool bMatchesAny = false;
        for (const FGameplayTag& Parent : AcceptUnderParents)
        {
            if (Tag.MatchesTag(Parent))
            {
                bMatchesAny = true;
                break;
            }
        }
        if (!bMatchesAny)
        {
            return;
        }
    }

    if (ListenForExactTag.IsValid() && !Tag.MatchesTagExact(ListenForExactTag))
    {
        return;
    }

    const FGameplayTag OutTag = (bOverrideRelayTag && RelayAsTag.IsValid()) ? RelayAsTag : Tag;

    if (UStateTreeComponent* Component = CachedComponent.Get())
    {
        Component->SendStateTreeEvent(FStateTreeEvent(OutTag));
    }
}
