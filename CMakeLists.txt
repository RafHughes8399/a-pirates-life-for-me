# ------------------------------------------------------------ #
# -------------- DO NOT TOUCH BELOW THIS LINE ---------------- #
# ------------------------------------------------------------ #

# this must be the first line of a CMake script.
# sets the lowerbound on what CMake version can be used.
cmake_minimum_required(VERSION 3.25)

# the name of this CMake project and what language it uses
# we could list more languages if we were using more.
project(COMP6771_LAB_001 LANGUAGES CXX)

# we use C++23
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

# this is helpful for editors like VS Code
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# helpful compiler flags for gcc/clang
# the descriptions for these flags can be found on the GNU Compiler Collection's webpage.
set(MY_COMPILE_OPTIONS
  -Wall
  -Wextra
  #-Werror
  -pedantic-errors
  -Wconversion
  -Wcast-align
  -Wformat=2
  -Wuninitialized
  -Wnull-dereference
  -Wnon-virtual-dtor
  -Woverloaded-virtual
  -Wdeprecated-copy-dtor
  -Wold-style-cast
  -Wzero-as-null-pointer-constant
  -Wsuggest-override
  -fstack-protector-strong
  # Disable problematic warnings for external headers
  -Wno-double-promotion
  -Wno-missing-field-initializers
)

# debug builds should be compiled with sanitizers
# sanitizers are small libraries that check things like buffer overrun with minimal runtime overhead.
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-fsanitize=address,undefined")
set(CMAKE_CXX_EXE_LINKER_FLAGS_DEBUG_INIT "-fsanitize=address,undefined")
set(CMAKE_CXX_EXE_LINKER_FLAGS_RELWITHDEBINFO_INIT "-fsanitize=address,undefined")

# add the testing library Catch2
enable_testing()
add_library(catch2_main lib/catch2_main.cpp)
target_include_directories(catch2_main PUBLIC lib)

# ------------------------------------------------------------ #
# -------------- DO NOT MODIFY ABOVE THIS LINE --------------- #
# ------------------------------------------------------------ #

# Function to add a library with strict compilation options

add_subdirectory(lib/raylib)
# Create a list to store all your custom libraries

# add libarary
# link libraries 


add_library(util src/utility_functions.cpp)
link_libraries(util)

add_library(ship_components src/ship_components.cpp)
link_libraries(ship_components)

add_library(entities src/entities.cpp)
link_libraries(entities)

add_library(ship src/ship.cpp)
link_libraries(ship)

add_library(terrain src/terrain.cpp)
link_libraries(terrain)

add_library(ocean src/ocean.cpp)
link_libraries(ocean)

add_library(wind src/wind.cpp)
link_libraries(wind)

add_library(events src/events.cpp)
link_libraries(events)

add_library(managers src/managers.cpp)
link_libraries(managers) 

add_library(octree src/octree.cpp)
link_libraries(octree)

add_library(rendering src/rendering.cpp)
link_libraries(rendering)
add_library(player src/player.cpp)
link_libraries(player)

add_library(world src/world.cpp)
link_libraries(world)

add_library(game src/game.cpp)
link_libraries(game)

# Main executable
add_executable(pirate_life src/pirate-cmake.cpp)
target_link_libraries(pirate_life catch2_main raylib)


# add_executable(test_example tests/example)
#add_test(test_example test_example)

# TODO: Add tests and install targets if needed.