# ------------------------------------------------------------ #
# -------------- DO NOT TOUCH BELOW THIS LINE ---------------- #
# ------------------------------------------------------------ #

# this must be the first line of a CMake script.
# sets the lowerbound on what CMake version can be used.
cmake_minimum_required(VERSION 3.0)

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
function(add_lib name source)
  add_library(${name} ${source})
  target_compile_options(${name} PRIVATE ${MY_COMPILE_OPTIONS})
  # Make raylib headers system headers to avoid warnings
  target_include_directories(${name} SYSTEM PRIVATE ${raylib_INCLUDE_DIRS})
  target_link_libraries(${name} PRIVATE raylib)
endfunction()

add_subdirectory(lib/raylib)
# Create a list to store all your custom libraries
set(MY_LIBRARIES)

# Add all your libraries with strict compilation
add_lib(event_handler src/event_handler.cpp)
list(APPEND MY_LIBRARIES event_handler)

add_lib(event src/event.cpp)
list(APPEND MY_LIBRARIES event)

add_lib(game src/game.cpp)
list(APPEND MY_LIBRARIES game)

add_lib(object src/object.cpp)
list(APPEND MY_LIBRARIES object)

add_lib(ocean src/ocean.cpp)
list(APPEND MY_LIBRARIES ocean)

add_lib(player src/player.cpp)
list(APPEND MY_LIBRARIES player)

add_lib(ship_components src/ship_components.cpp)
list(APPEND MY_LIBRARIES ship_components)

add_lib(ship src/ship.cpp)
list(APPEND MY_LIBRARIES ship)

add_lib(singleton_flyweight src/singleton_flyweight.cpp)
list(APPEND MY_LIBRARIES singleton_flyweight)

add_lib(terrain src/terrain.cpp)
list(APPEND MY_LIBRARIES terrain)

add_lib(utility src/utility_functions.cpp)
list(APPEND MY_LIBRARIES utility)

add_lib(wind src/wind.cpp)
list(APPEND MY_LIBRARIES wind)

add_lib(world src/world.cpp)
list(APPEND MY_LIBRARIES world)

# Main executable
add_executable(pirate_life src/pirate-cmake.cpp)
target_link_libraries(pirate_life ${MY_LIBRARIES} catch2_main raylib)


# add_executable(test_example tests/example)
#add_test(test_example test_example)

# TODO: Add tests and install targets if needed.