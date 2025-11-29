// © 2025 StateTreePlayground Project. MIT License. Created by github.com/GulshanDixit

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"

#include "GameEventSequencer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEvent, FGameplayTag, EventTag);

UCLASS(Blueprintable)
class STATETREEPLAYGROUND_API AGameEventSequencer : public AActor
{
    GENERATED_BODY()

public:
    AGameEventSequencer();

    UPROPERTY(BlueprintAssignable, Category = "Game Events")
    FOnGameEvent OnGameEvent;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Events", meta = (ClampMin = "0.01"))
    float IntervalSeconds = 2.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Events")
    bool bLoop = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Events")
    TArray<FGameplayTag> EventSequence;

private:
    UFUNCTION()
    void FireNext();

    FTimerHandle TimerHandle;

    int32 Index = 0;
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_FireEvent(FGameplayTag Tag);
};