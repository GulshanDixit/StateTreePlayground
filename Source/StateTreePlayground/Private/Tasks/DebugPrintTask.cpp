// © 2025 StateTreePlayground Project. MIT License. Created by github.com/GulshanDixit

#include "Tasks/DebugPrintTask.h"

#include "Components/StateTreeComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "StateTreeExecutionContext.h"

DEFINE_LOG_CATEGORY_STATIC(LogDebugPrintTask, Log, All);

EStateTreeRunStatus UDebugPrintTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult&)
{
    if (const AActor* Owner = Cast<AActor>(Context.GetOwner()))
    {
        CachedComponent = Owner->GetComponentByClass<UStateTreeComponent>();
    }

    if (bPrintOnEnter)
    {
        PrintMessage(true);
    }

    return EStateTreeRunStatus::Running;
}

void UDebugPrintTask::ExitState(FStateTreeExecutionContext&, const FStateTreeTransitionResult&)
{
    if (bPrintOnExit)
    {
        PrintMessage(false);
    }
}

void UDebugPrintTask::PrintMessage(bool bIsEnter) const
{
    const UStateTreeComponent* Component = CachedComponent.Get();
    const UWorld* World = Component ? Component->GetWorld() : nullptr;

    if (!World)
    {
        return;
    }

    // Check if we should print based on server/client setting
    const ENetMode NetMode = World->GetNetMode();
    const bool bIsServer = (NetMode == NM_DedicatedServer || NetMode == NM_ListenServer);
    const bool bIsClient = (NetMode == NM_Client || NetMode == NM_ListenServer);

    if (bIsServer && !bPrintOnServer)
    {
        return;
    }

    if (bIsClient && !bPrintOnClient)
    {
        return;
    }

    const FString Prefix = BuildPrefix(World, bIsEnter);

    const FString FinalText =
            FString::Printf(TEXT("%s %s"), *Prefix,
                            Message.IsEmpty() ? (bIsEnter ? TEXT("[Enter]") : TEXT("[Exit]")) : *Message)
                    .TrimStartAndEnd();

    if (bPrintToLog)
    {
        UE_LOG(LogDebugPrintTask, Log, TEXT("%s"), *FinalText);
    }

    if (bPrintToScreen && GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, DurationSeconds, TextColor, FinalText);
    }
}

FString UDebugPrintTask::BuildPrefix(const UWorld* World, bool bIsEnter) const
{
    FString TimePart;
    if (bIncludeGameTime && World)
    {
        const float Time = World->GetTimeSeconds();
        TimePart = bRoundTimeToInt ? FString::Printf(TEXT("[t=%d] "), FMath::RoundToInt(Time))
                                   : FString::Printf(TEXT("[t=%.2f] "), Time);
    }

    FString RolePart;
    if (World)
    {
        const ENetMode NetMode = World->GetNetMode();
        if (NetMode == NM_DedicatedServer)
        {
            RolePart = TEXT("[Server] ");
        }
        else if (NetMode == NM_ListenServer)
        {
            RolePart = TEXT("[ListenServer] ");
        }
        else if (NetMode == NM_Client)
        {
            RolePart = TEXT("[Client] ");
        }
    }

    FString PieIdPart;
    if (bIncludePieId && World)
    {
        const int32 PIEInstance = World->GetOutermost()->GetPIEInstanceID();
        if (PIEInstance != INDEX_NONE)
        {
            PieIdPart = FString::Printf(TEXT("[PIE=%d] "), PIEInstance);
        }
    }

    return FString::Printf(TEXT("%s%s%s%s"), *TimePart, *RolePart, *PieIdPart, bIsEnter ? TEXT("[Enter]") : TEXT("[Exit]"));
}