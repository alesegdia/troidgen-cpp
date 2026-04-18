# troidgen-cpp

A C++20 port of **troidgen** — a procedural dungeon layout generator inspired by Metroid-style games. Rooms are randomly placed, physically separated, grouped by connectivity, and linked together with doorways. A restriction/lock-and-key system controls which rooms are accessible.

---

## Table of Contents

1. [Overview](#overview)
2. [Building](#building)
3. [Running the Tests](#running-the-tests)
4. [Architecture](#architecture)
5. [Core Concepts](#core-concepts)
   - [Rooms and Rects](#rooms-and-rects)
   - [Links and Doorways](#links-and-doorways)
   - [Restrictions](#restrictions)
   - [Overlap Solving](#overlap-solving)
   - [Group Extraction](#group-extraction)
   - [Link Building](#link-building)
   - [Layout Builder](#layout-builder)
   - [Floyd-Warshall Solver](#floyd-warshall-solver)
6. [Key Class Reference](#key-class-reference)
7. [Extending the System](#extending-the-system)

---

## Overview

The generation pipeline works as follows:

```
IRoomProvider                     IRoomGroupValidator
     │                                    │
     ▼                                    │
provideList(n rooms)                      │
     │                                    │
     ▼                                    │
MiscRoomUtils::RandomPlaceInRange         │
     │                                    │
     ▼                                    │
OverlapSolver::solve ──────────────┐      │
     │   (iterative physics)       │      │
     ▼                             │      │
GroupExtractor::solve              │      │
     │   (flood-fill DFS)          │      │
     ▼                             │      │
LinkBuilder::generate              │      │
     │   (connect touching rooms)  │      │
     ▼                             │      │
pick largest group ◄───────────────┘      │
     │                                    │
     └──► notifySelected ──► validate ────┘
              (repeat until valid)
```

This loop repeats until `IRoomGroupValidator::validate()` returns `true` — for example, when the chosen group contains at least N rooms.

---

## Building

Requirements: CMake ≥ 3.16, a C++20 compiler (MSVC 2022, GCC 11+, or Clang 13+).

```sh
cmake -S src -B build
cmake --build build --config Debug
```

All build outputs land in `build/`:
- `build/Debug/troidgen.lib` — static library
- `build/tests/Debug/Test_*.exe` — test executables

---

## Running the Tests

```sh
# Geometry / math tests
./build/tests/Debug/Test_RectCollision
./build/tests/Debug/Test_RectTouching

# Restriction system
./build/tests/Debug/Test_Restrictions

# Overlap solver
./build/tests/Debug/Test_OverlapSolverManual
./build/tests/Debug/Test_OverlapSolverRandom
./build/tests/Debug/Test_OverlapSolverRandomBig
./build/tests/Debug/Test_OverlapSolverRandomProvider

# Room provider
./build/tests/Debug/Test_ManualGeometryProviderBasic

# Full layout generation pipeline
./build/tests/Debug/Test_LayoutBuilder
./build/tests/Debug/Test_LayoutBuilder2
```

---

## Architecture

```
src/
├── CMakeLists.txt
├── include/troidgen/
│   ├── util/
│   │   ├── Vec2.hpp              2D float vector
│   │   ├── Rect.hpp              Axis-aligned rectangle
│   │   ├── Matrix2D.hpp          Generic 2D matrix (template, header-only)
│   │   ├── UpperMatrix2D.hpp     Symmetric matrix using upper triangle (template, header-only)
│   │   ├── RNG.hpp               std::mt19937 singleton wrapper
│   │   ├── Logger.hpp            No-op logger
│   │   ├── Util.hpp              shuffle<T> (template, header-only)
│   │   └── MiscRoomUtils.hpp     RandomPlaceInRange
│   ├── restriction/
│   │   ├── RestrictionSet.hpp    Boolean flag array (lock-and-key)
│   │   └── RestrictionSetFactory.hpp  Preset factory methods
│   ├── room/
│   │   ├── Direction.hpp         Enum: TOP DOWN LEFT RIGHT NODIR
│   │   ├── RoomType.hpp          Enum: PASSAGE ABILITY COMMON
│   │   ├── LinkInfo.hpp          Data template for a doorway slot
│   │   ├── RoomInfo.hpp          Data template for a room type (prefab)
│   │   ├── Link.hpp              A single doorway slot on a room wall
│   │   ├── LinkPair.hpp          Two matched doorways being connected
│   │   └── Room.hpp              Core entity (inherits Rect)
│   ├── IRoomProvider.hpp         Interface: supply rooms for a pass
│   ├── IRoomGroupValidator.hpp   Interface: decide when generation is done
│   ├── IRoomGroupFilter.hpp      Interface: pick best group from candidates
│   ├── IRoomValidator.hpp        Interface: per-room filter
│   ├── IWorldComposer.hpp        Interface: stub for world composition
│   ├── BiggestGroupFilter.hpp    Picks the group with the most rooms
│   ├── MinSizeRoomGroupValidator.hpp   Passes when group.size() > min
│   ├── MinSpaceRoomGroupValidator.hpp  Passes when total floor area > min
│   ├── RoomRestrictionValidator.hpp    Filters rooms by RestrictionSet
│   ├── ManualRoomProvider.hpp    Randomly samples from a curated list
│   ├── RandomRoomProvider.hpp    Creates rooms with random dimensions
│   ├── ExactRoomProvider.hpp     Tracks quotas; converges over passes
│   ├── ExactRoomProviderValidator.hpp  Delegates to ExactRoomProvider::isOk()
│   ├── GraphBuilder.hpp          Builds weighted adjacency matrix
│   ├── GroupExtractor.hpp        Flood-fill connected-component finder
│   ├── LinkBuilder.hpp           Connects touching room pairs with doorways
│   ├── FloydWarshallSolver.hpp   All-pairs shortest path; finds spawn/goal
│   ├── OverlapSolverConfig.hpp   Config for the physics separation pass
│   ├── OverlapSolver.hpp         Iterative physics-based room separation
│   ├── LayoutBuilderConfig.hpp   Config for the main generation loop
│   └── LayoutBuilder.hpp         Top-level generation orchestrator
└── tests/
    └── Test_*.cpp                10 standalone test programs
```

---

## Core Concepts

### Rooms and Rects

`Rect` is a simple axis-aligned bounding box:

```cpp
struct Rect {
    Vec2 position;  // top-left corner
    Vec2 size;      // width and height

    bool collideWith(const Rect& other) const;  // strict overlap (no touching)
    bool isTouching(const Rect& other) const;   // edge-shared adjacency
                                                // corner-only contact returns false
};
```

`Room` inherits `Rect` and adds game-world state:

```cpp
struct Room : public Rect {
    std::vector<Link> links;          // all doorway slots on this room
    std::vector<Room*> neighboors;    // rooms touching this one (populated by GraphBuilder)
    RoomInfo* rinfo;                  // pointer to the prefab template (nullable)
    RoomType rtype;                   // COMMON, PASSAGE, or ABILITY
    std::optional<RestrictionSet> restrictionSet;  // access constraint flags
    bool isVisited;
};
```

Rooms can be constructed in three ways:
- From a `RoomInfo*` template (prefab with predefined links)
- With explicit `(width, height, restriction*)` — links are auto-computed for every border tile
- As a copy of another room (links are copied without their connections)

---

### Links and Doorways

Each room has a `std::vector<Link>` containing one `Link` per border tile (corner tiles get two links, one per wall direction). A `Link` represents a potential doorway:

```cpp
struct Link {
    Vec2      relCoord;       // tile offset within the room
    Direction direction;      // wall face: TOP / DOWN / LEFT / RIGHT
    Room*     ownerRoom;      // back-reference (non-owning)
    Room*     connectedRoom;  // null if unconnected
    Link*     connectedLink;  // the other side of the doorway
};
```

Two links can connect if:
1. They belong to different rooms.
2. The rooms are **touching** (edge-shared).
3. The directions are **opposite** (e.g., one faces RIGHT, the other LEFT).
4. Their absolute coordinates are exactly **1 tile apart** in the connection axis.

`LinkPair` holds a matched pair and calls `lA->connectTo(lB)` to establish the bidirectional connection.

---

### Restrictions

`RestrictionSet` is a fixed-size boolean flag array representing access constraints:

```cpp
RestrictionSet key(4, {true, false, true, false});  // "1010"
RestrictionSet lock(4, {true, true, true, false});   // "1110"

key.resolves(lock);   // true  — every bit set in 'key' is also set in 'lock'
lock.resolves(key);   // false — 'lock' has bit 1 set, 'key' does not
```

`resolves(target)` returns `true` when `this` is a **subset** of `target` — meaning "this key can open this lock". Rooms carry a `RestrictionSet` that indicates what access level is required to enter them. `RoomRestrictionValidator` uses this to filter rooms during group extraction.

`RestrictionSetFactory` provides four named presets: `makeRS_0000`, `makeRS_1000`, `makeRS_0100`, `makeRS_1010`.

---

### Overlap Solving

`OverlapSolver` implements iterative physics-based room separation:

1. **Scale up** — all room positions and sizes are multiplied by `resolution` (default 64) to provide sub-tile precision during the solve.
2. **Push loop** — for each overlapping pair, compute a centroid-difference push vector, normalise it, optionally add random noise, then displace the room.
3. **Clamp** — keep rooms inside `enclosingRect`.
4. **Scale back** — divide positions by `resolution` and round to the nearest integer; divide sizes by `resolution` (no rounding).

Fixed rooms from a previous `outputLayout` are included in the collision checks but never moved.

Key config options (`OverlapSolverConfig`):

| Field | Default | Effect |
|---|---|---|
| `resolution` | 64 | Sub-tile precision during solve |
| `separationParameter` | 1.0 | Push strength multiplier |
| `enableTweakNearSeparation` | true | Scales push by room perimeter |
| `enableRandomDisplacement` | true | Adds small random noise to push vector |
| `enableRandomPushIfZero` | true | Applies random push when centroid difference is zero |
| `earlyBreak` | false | Stop inner loop after first displaced room per pass |
| `enclosingRect` | (-400,-400,800,800) | World boundary |

---

### Group Extraction

`GroupExtractor` performs a flood-fill DFS to find connected components:

```
for each unassigned room R:
    start a new group G = { R }
    for each other room Q not yet assigned:
        if Q is touching R and R.getPossibleConnections(Q) is non-empty
           and Q does not collide with R:
            add Q to G, recurse from Q
    emit group G
```

An optional `IRoomValidator*` can exclude rooms from the flood-fill entirely (e.g., `RoomRestrictionValidator` to only include rooms with a specific restriction mask).

---

### Link Building

After group extraction, `LinkBuilder` connects each touching pair of rooms:

```
for each room R in the group:
    for each neighbour N of R (populated by GraphBuilder):
        if R and N are touching and not yet directly connected:
            find all valid LinkPairs (canConnect == true)
            pick one at random and connect it
```

This gives every adjacent pair of rooms exactly one doorway.

---

### Layout Builder

`LayoutBuilder::generate()` is the top-level loop:

```cpp
// Simplified pseudocode
RNG::rng = make_unique<RNG>();  // reseed on every call
do {
    rooms = provider.provideList(lbc.numRooms);
    RandomPlaceInRange(rooms, lbc.spawnRect);

    rooms = OverlapSolver.solve(lbc.osc, rooms, fixedGroup);

    groups = GroupExtractor.solve(rooms);
    for (group : groups) LinkBuilder.generate(group);

    outputLayout = largest group;
    provider.notifySelected(outputLayout);
} while (!validator.validate(outputLayout - fixedGroup));

return outputLayout;
```

A multi-pass build can be achieved by passing the result of a previous `generate()` call as `fixedGroup`. Fixed rooms are never moved by the overlap solver, so new rooms grow around them.

**Room providers:**

| Class | Behaviour |
|---|---|
| `RandomRoomProvider(min, max)` | Creates rooms with random integer dimensions in `[min, max]` |
| `ManualRoomProvider` | Randomly samples N rooms from a curated `add()`-ed list |
| `ExactRoomProvider` | Tracks per-`RoomInfo` quotas; each pass returns only the deficit |

**Validators:**

| Class | Behaviour |
|---|---|
| `MinSizeRoomGroupValidator(n)` | Passes when `group.size() > n` |
| `MinSpaceRoomGroupValidator(area)` | Passes when total floor area `> area` |
| `ExactRoomProviderValidator` | Passes when `ExactRoomProvider::isOk()` is true |

---

### Floyd-Warshall Solver

`FloydWarshallSolver` finds the **most distant pair of rooms** in a connected layout, typically used to designate a spawn point and goal room:

1. **Build graph** — `GraphBuilder::build()` returns an `UpperMatrix2D<float>` where cell `(i,j)` is the Euclidean distance between rooms `i` and `j` if they are touching, or `FLT_MAX` otherwise.
2. **Run Floyd-Warshall** — iterates over all reachable node triples to find all-pairs shortest paths.
3. **Find extremes** — the pair `(i, j)` with the greatest finite shortest-path distance becomes spawn/goal.
4. **Trace path** — reconstructs the room sequence from spawn to goal using the `next` matrix.

```cpp
GraphBuilder gb;
auto matrix = gb.build(rooms);

FloydWarshallSolver fws;
fws.Solve(matrix);

int spawnIdx = fws.GetSpawnRoom();
int goalIdx  = fws.GetGoalRoom();
const auto& path = fws.GetPath();  // indices into 'rooms'
```

---

## Key Class Reference

| Class | Header | Role |
|---|---|---|
| `Vec2` | `util/Vec2.hpp` | Mutable 2D float vector |
| `Rect` | `util/Rect.hpp` | AABB; `collideWith`, `isTouching` |
| `Matrix2D<T>` | `util/Matrix2D.hpp` | Row-major 2D matrix backed by `std::vector<T>` |
| `UpperMatrix2D<T>` | `util/UpperMatrix2D.hpp` | Symmetric matrix; always accesses upper triangle |
| `RNG` | `util/RNG.hpp` | `std::mt19937`; global `RNG::rng` singleton |
| `RestrictionSet` | `restriction/RestrictionSet.hpp` | Boolean flag array; `resolves()` = subset check |
| `Room` | `room/Room.hpp` | Core game entity; inherits `Rect` |
| `Link` | `room/Link.hpp` | Doorway slot; `canConnect`, `connectTo` |
| `OverlapSolver` | `OverlapSolver.hpp` | Physics-based room separation |
| `GroupExtractor` | `GroupExtractor.hpp` | Flood-fill connected-component finder |
| `LinkBuilder` | `LinkBuilder.hpp` | Randomly connects touching room pairs |
| `GraphBuilder` | `GraphBuilder.hpp` | Builds weighted adjacency matrix |
| `FloydWarshallSolver` | `FloydWarshallSolver.hpp` | All-pairs shortest path; spawn/goal detection |
| `LayoutBuilder` | `LayoutBuilder.hpp` | Top-level generation orchestrator |

---

## Extending the System

**Custom room provider:** Implement `IRoomProvider`.

```cpp
class MyProvider : public troidgen::IRoomProvider {
public:
    std::vector<std::shared_ptr<Room>> provideList(int numRooms) override { /* ... */ }
    void notifySelected(const std::vector<std::shared_ptr<Room>>& rooms) override { /* ... */ }
    bool isOk() override { return true; }
};
```

**Custom validator:** Implement `IRoomGroupValidator`.

```cpp
class MyValidator : public troidgen::IRoomGroupValidator {
public:
    bool validate(const std::vector<std::shared_ptr<Room>>& group) override {
        return group.size() >= 15;
    }
};
```

**Minimal generation example:**

```cpp
#include "troidgen/LayoutBuilder.hpp"
#include "troidgen/LayoutBuilderConfig.hpp"
#include "troidgen/RandomRoomProvider.hpp"
#include "troidgen/MinSizeRoomGroupValidator.hpp"
#include "troidgen/util/RNG.hpp"

using namespace troidgen;

int main() {
    LayoutBuilderConfig lbc;
    lbc.spawnRect = Rect(-10, -10, 20, 20);
    lbc.osc.enclosingRect = Rect(-30, -20, 60, 40);

    RandomRoomProvider provider(1, 4);
    MinSizeRoomGroupValidator validator(10);

    LayoutBuilder lb;
    auto rooms = lb.generate(lbc, provider, validator);

    // rooms is a vector<shared_ptr<Room>> with > 10 connected, link-joined rooms
}
```
