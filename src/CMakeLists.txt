cmake_minimum_required(VERSION 3.16)
project(troidgen VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 REQUIRED)

# -----------------------------------------------------------------------
# Library sources
# -----------------------------------------------------------------------
add_library(troidgen STATIC
    # util
    src/util/Vec2.cpp
    src/util/Rect.cpp
    src/util/RNG.cpp
    src/util/MiscRoomUtils.cpp

    # restriction
    src/restriction/RestrictionSet.cpp
    src/restriction/RestrictionSetFactory.cpp

    # room
    src/room/Direction.cpp
    src/room/RoomType.cpp
    src/room/RoomInfo.cpp
    src/room/Link.cpp
    src/room/LinkPair.cpp
    src/room/Room.cpp

    # core
    src/BiggestGroupFilter.cpp
    src/ExactRoomProvider.cpp
    src/FloydWarshallSolver.cpp
    src/GraphBuilder.cpp
    src/GroupExtractor.cpp
    src/LayoutBuilder.cpp
    src/LinkBuilder.cpp
    src/ManualRoomProvider.cpp
    src/OverlapSolver.cpp
    src/RandomRoomProvider.cpp
    src/util/Util.cpp

    # renderer
    src/renderer/RectDebugger.cpp
)

target_include_directories(troidgen PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(troidgen PUBLIC SDL2::SDL2)

# -----------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------
add_subdirectory(tests)
