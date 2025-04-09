# CMakeList.txt : CMake project for pirate-cmake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("pirate-cmake")
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
  endif()
endif()
# Add source to this project's executable.
add_executable (pirate-cmake 
"src/pirate-cmake.cpp"
"src/world.h" "src/player.h" "src/player.cpp" 
"src/ship.cpp" "src/world.cpp" "src/ocean.cpp" 
"src/config.h"  "src/object.h"  "src/object.cpp" 
"src/ship_components.cpp" "src/wind.h" "src/wind.cpp" 
"src/utility_functions.h" "src/utility_functions.cpp" "src/hitbox.h" 

"src/singleton_flyweight.h" "src/singleton_flyweight.cpp" "src/object_factory.h" "src/object_factory.cpp" "src/game.h" "src/game.cpp"   "src/event.h" "src/event_handler.h" "src/event.cpp" "src/collision_events.h" "src/event_handler.cpp")
target_link_libraries(${PROJECT_NAME} raylib)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET pirate-cmake PROPERTY CXX_STANDARD 20)
endif()


# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
# TODO: Add tests and install targets if needed.
# Include all files in textures, images, and models folders
file(GLOB TEXTURE_FILES "${CMAKE_SOURCE_DIR}/textures/*")
file(GLOB IMAGE_FILES "${CMAKE_SOURCE_DIR}/images/*")
file(GLOB MODEL_FILES "${CMAKE_SOURCE_DIR}/models/*")

# Add the resource files to the target
add_custom_target(resources ALL SOURCES ${TEXTURE_FILES} ${IMAGE_FILES} ${MODEL_FILES} "src/game.h" "src/game.cpp"   "src/event.h" "src/event_handler.h" "src/event.cpp" "src/collision_events.h" "src/event_handler.cpp")

# Ensure the resource files are copied to the build directory
add_custom_command(TARGET resources POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${TEXTURE_FILES} ${IMAGE_FILES} ${MODEL_FILES} $<TARGET_FILE_DIR:pirate-cmake>
)

set_property(TARGET pirate-cmake PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
