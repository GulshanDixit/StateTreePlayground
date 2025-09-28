# StateTree Event Flow Playground (UE 5.6.1)

A small sandbox exploring Unreal’s StateTree with event-driven Gameplay Tags and lightweight tasks. We fire tags on a timer, relay them into the tree, and watch L1 → L2 → L3 plus L3’s A/B/C hops. One unified listener, a built-in Delay in the wait state, and a configurable debug printer.

## What’s Inside

* Linked StateTrees (nested)
  * `ST_Level1` → links to `ST_Level2`
  * `ST_Level2` → links to `ST_Level3`
* Event-driven transitions via Gameplay Tags
  * L1/L2 listen for cross-level tags
  * L3 listens under a parent namespace and hops A/B/C/Wait
* Reusable tasks (C++ BPBase)
  * `ListenAndRelayEventsTask` — bind to sequencer, filter, and **immediately** relay tags as StateTree events (works as Global or State task)
  * `DebugPrintTask` — logs Enter/Exit with color, screen/log output, `[t=…]` timestamps, optional rounding

## Why Event-Driven ?
This project is fully event-driven. Tasks bind to the sequencer on `EnterState` and unbind on `ExitState`, and transitions are triggered directly from those callbacks. Per-frame ticking isn’t used in this iteration; in this setup the BPBase tick wasn't working as expected, so the design leans into callbacks rather than a queued, tick-drained relay. The result is a simpler, deterministic flow that generally suits StateTree.

## Architecture
<pre>
L1 (ST_Level1)
  ├─ Global task: ListenAndRelayEventsTask (ListenForExactTag = Events.StateTree.Level.L1.ToL2)
  └─ State: Level 2_Link → Linked Asset = ST_Level2

L2 (ST_Level2)
  ├─ Global task: ListenAndRelayEventsTask (ListenForExactTag = Events.StateTree.Level.L2.ToL3)
  └─ State: Level 3_Link → Linked Asset = ST_Level3
      └─ On Child Succeeded → (optionally) WaitAndSucceed

L3 (ST_Level3)
  ├─ State: Wait (event routes)
  ├─ States: StateA, StateB, StateC
  ├─ Any-State transitions:
  │    ├─ Events.StateTree.Level.L3.To.StateA         → StateA
  │    ├─ Events.StateTree.Level.L3.To.StateB         → StateB
  │    ├─ Events.StateTree.Level.L3.To.StateC         → StateC
  │    └─ Events.StateTree.Level.L3.To.WaitAndSucceed → WaitAndSucceed
  └─ State: WaitAndSucceed (Delay task, default 3s)
  </pre>

### Important Behavior Note
- If **Level 2** does **not** define an **On Child Succeeded** transition for the **Level 3_Link** state, then when **Level 3** succeeds (e.g., from `WaitAndSucceed`), control bubbles up and the overall flow returns to **Level 1 Start** automatically.
- If you **do** add **On Child Succeeded** in Level 2 (e.g., to a local state `WaitAndSucceed`), you’ll see the Level 2 cleanup state run before returning to Level 1.
Both are valid, this is documented so you’re not surprised by either behavior.

## Gameplay Tags

Organized for easy filtering:

<pre>
Events.StateTree
Events.StateTree.Level
Events.StateTree.Level.L1.ToL2
Events.StateTree.Level.L2.ToL3

Events.StateTree.Level.L3
Events.StateTree.Level.L3.To
Events.StateTree.Level.L3.To.StateA
Events.StateTree.Level.L3.To.StateB
Events.StateTree.Level.L3.To.StateC
Events.StateTree.Level.L3.To.WaitAndSucceed
</pre>

L3 listeners use `AcceptUnderParents = { Events.StateTree.Level.L3 }`

## The Sequencer (Drives the Demo)

`AGameEventSequencer` is a tiny actor that fires a list of tags at a fixed interval.

- Place one in the level (`StateTreeSmoke`).
- Set `EventSequence`, e.g.:

<pre>
Events.StateTree.Level.L1.ToL2
Events.StateTree.Level.L2.ToL3
Events.StateTree.Level.L3.To.StateA
Events.StateTree.Level.L3.To.StateB
Events.StateTree.Level.L3.To.StateC
Events.StateTree.Level.L3.To.WaitAndSucceed
</pre>

- It logs when it fires, e.g.:
`LogEventSequencer: [Time=12.01] Sequencer fired tag: Events.StateTree.Level.L3.To.StateB`

## Logging & Debug
`DebugPrintTask` produces readable, timestamped entries:
- Options: `bPrintOnEnter/Exit`, `bPrintToScreen`, `bPrintToLog`, `bIncludeGameTime`, `bRoundTimeToInt`, `TextColor`, `DurationSeconds`
- Sample:
<pre>
LogEventSequencer: [Time=6.01] Sequencer fired tag: Events.StateTree.Level.L3.To.StateA
LogDebugPrintTask: [t=6.01] [Exit]  Level3 -> Wait
LogDebugPrintTask: [t=6.01] [Enter] Level3 -> StateA
...
LogDebugPrintTask: [t=12.01] [Enter] Level3 -> WaitAndSucceed
LogDebugPrintTask: [t=15.44] [Exit]  Level3 -> WaitAndSucceed
</pre>

### How to Run
1. UE 5.6.1 (C++ project).
2. Open map: `StateTreeSmoke`.
3. Ensure host actor `BP_StateTreeHost` with StateTreeComponent is placed and points to `ST_Level1` with **Start Logic Automatically = true**.
4. Ensure `BP_GameEventSequencer` is also placed in the level with filled EventSequence.
5. Run PIE, watch Output Log, or On Screen Log.

### Binding Choices
The listener task supports:
- Sequencer class (add `Sequencer : GameEventSequencer` to the StateTree and provide it on the task), or
- Auto-find (by actor tag / class).
Either way, binding happens on `EnterState` and unbinds on `ExitState`.

### Code Map

<pre>
Source/StateTreePlayground
  ├─ Tasks/
  │   ├─ ListenAndRelayEventsTask.*        // unified, event-driven listener (Global or State usage)
  │   └─ DebugPrintTask.*                  // enter/exit logs, screen/log, timestamps
  ├─ Actors/
  │   └─ GameEventSequencer.*              // fires tags on a timer, logs each fire
  └─ Utils/
      └─ STEventBindingUtils.*             // resolves the sequencer via provider/auto-find
</pre>

### Known Quirks / Decisions
- **No queued relay**: an earlier version queued tags and drained them on Tick; since Tick wasn’t reliable in this context, this build relays **immediately** in the delegate. It’s simpler and works great for event-style StateTrees.
- **Success bubbling**: If L2 doesn’t handle “Child Succeeded” from L3, control jumps straight back to L1 Start when L3 succeeds. If L2 does handle it (e.g., `WaitAndSucceed`), you’ll see that intermediate step before returning.

### Requirements
- Unreal Engine **5.6.1**
- C++ toolchain
- Gameplay Tags enabled (see `Config/DefaultGameplayTags.ini`)
- StateTree plugin enabled (`StateTree` + `Gameplay StateTree)

### Contributing / Playing

- Tweak `EventSequence`, interval, or add new leaves under `Events.StateTree.Level.L3.To.*` and watch the tree respond.
- Swap L2’s handling of L3 success to see different parent-level behaviors.
- Change the wait time in `Delay Task` to feel completion transitions.
- If you want to explore ticking again, flip `bShouldCallTick` and enable “Should Call Tick” per node in the StateTree editor (but: event-driven is the happy path).

### License
MIT — see [LICENSE](https://opensource.org/license/mit)
© 2025 StateTreePlayground Project. Created by github.com/GulshanDixit.
