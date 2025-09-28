// © 2025 StateTreePlayground Project. MIT License. Created by github.com/GulshanDixit

#pragma once
#include "CoreMinimal.h"

class AGameEventSequencer;

namespace STEventBindingUtils
{
/**
 * Resolve a GameEventSequencer to bind to.
 * Priority:
 *   1) By Actor Tag (if provided)
 *   2) First of Class (if provided)
 *   3) First AGameEventSequencer in world
 *
 * @param WorldContext  Usually Context.GetOwner() from StateTree.
 * @param bAutoFind     If false, returns Explicit (or null).
 * @param Tag           Optional Actor tag to match (precise).
 * @param ClassFilter   Optional subclass filter; defaults to AGameEventSequencer.
 */
STATETREEPLAYGROUND_API
AGameEventSequencer* ResolveSequencer(const UObject* WorldContext, bool bAutoFind, FName Tag,
                                      TSubclassOf<AGameEventSequencer> ClassFilter);
} // namespace STEventBindingUtils
