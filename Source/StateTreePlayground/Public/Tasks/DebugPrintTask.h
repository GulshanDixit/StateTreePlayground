// © 2025 StateTreePlayground Project. MIT License. Created by github.com/GulshanDixit

#pragma once

#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "CoreMinimal.h"

#include "DebugPrintTask.generated.h"

class UStateTreeComponent;

/**
 * Prints a message on state Enter/Exit with configurable screen/log output, color, duration,
 * and optional game-time stamp (rounded or fractional).
 */
UCLASS(DisplayName = "Debug Print (C++ BPBase)")
class STATETREEPLAYGROUND_API UDebugPrintTask : public UStateTreeTaskBlueprintBase
{
    GENERATED_BODY()

protected:
    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context,
                                           const FStateTreeTransitionResult& Transition) override;

    virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

    UPROPERTY(EditAnywhere, Category = "Debug")
    bool bPrintOnEnter = true;

    UPROPERTY(EditAnywhere, Category = "Debug")
    bool bPrintOnExit = true;

    UPROPERTY(EditAnywhere, Category = "Debug")
    bool bPrintToScreen = true;

    UPROPERTY(EditAnywhere, Category = "Debug")
    bool bPrintToLog = true;

    UPROPERTY(EditAnywhere, Category = "Debug")
    bool bIncludeGameTime = true;

    UPROPERTY(EditAnywhere, Category = "Debug", meta = (EditCondition = "bIncludeGameTime"))
    bool bRoundTimeToInt = false;

    UPROPERTY(EditAnywhere, Category = "Debug")
    FString Message = TEXT("");

    UPROPERTY(EditAnywhere, Category = "Debug")
    FColor TextColor = FColor::Cyan;

    UPROPERTY(EditAnywhere, Category = "Debug", meta = (ClampMin = "0.0"))
    float DurationSeconds = 2.0f;

private:
    void PrintMessage(bool bIsEnter) const;

    FString BuildPrefix(const UWorld* World, bool bIsEnter) const;

    UPROPERTY()
    TWeakObjectPtr<UStateTreeComponent> CachedComponent = nullptr;
};