// © 2025 StateTreePlayground Project. MIT License. Created by github.com/GulshanDixit

#include "Utils/STEventBindingUtils.h"

#include "Actors/GameEventSequencer.h"
#include "Kismet/GameplayStatics.h"

namespace STEventBindingUtils
{
AGameEventSequencer* ResolveSequencer(const UObject* WorldContext, bool bAutoFind, FName Tag,
                                      TSubclassOf<AGameEventSequencer> ClassFilter)
{
    if (!bAutoFind || !IsValid(WorldContext))
    {
        return nullptr;
    }

    const UWorld* World = WorldContext->GetWorld();
    if (!World)
    {
        return nullptr;
    }

    // 1) By actor tag
    if (!Tag.IsNone())
    {
        TArray<AActor*> Tagged;
        UGameplayStatics::GetAllActorsWithTag(World, Tag, Tagged);
        for (AActor* Actor : Tagged)
        {
            if (AGameEventSequencer* Sequencer = Cast<AGameEventSequencer>(Actor))
            {
                return Sequencer;
            }
        }
    }

    // 2) First of class filter
    if (*ClassFilter != nullptr)
    {
        TArray<AActor*> Found;
        UGameplayStatics::GetAllActorsOfClass(World, ClassFilter, Found);
        if (Found.Num() > 0)
        {
            return Cast<AGameEventSequencer>(Found[0]);
        }
    }

    // 3) Any sequencer
    TArray<AActor*> Any;
    UGameplayStatics::GetAllActorsOfClass(World, AGameEventSequencer::StaticClass(), Any);
    return Any.Num() > 0 ? Cast<AGameEventSequencer>(Any[0]) : nullptr;
}
} // namespace STEventBindingUtils
