cmake_minimum_required(VERSION 3.16)
project(ConstellationRNG CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch SFML
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(SFML)

# Fetch ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.1
)
FetchContent_MakeAvailable(imgui)
set(IMGUI_DIR ${imgui_SOURCE_DIR})

# When we fetch SFML from source, it creates targets like "sfml-graphics"
# But ImGui-SFML looks for IMPORTED targets like "SFML::Graphics" via find_package.
# We need to manually create these aliases so ImGui-SFML can link them properly.
if (NOT TARGET SFML::System)
    add_library(SFML::System ALIAS sfml-system)
endif()
if (NOT TARGET SFML::Window)
    add_library(SFML::Window ALIAS sfml-window)
endif()
if (NOT TARGET SFML::Graphics)
    add_library(SFML::Graphics ALIAS sfml-graphics)
endif()

# Fetch ImGui-SFML
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "Don't find SFML, use the fetched one" FORCE)
set(IMGUI_SFML_USE_PRECOMPILED_HEADERS OFF CACHE BOOL "Don't use PCH" FORCE)
FetchContent_Declare(
    ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG v2.6.1 # Stable release matching SFML 2.6
)
FetchContent_MakeAvailable(ImGui-SFML)

# Patch ImGui-SFML for C++20 / latest MSVC string iterator bug
if(EXISTS "${imgui-sfml_SOURCE_DIR}/imgui-SFML.cpp")
    file(READ "${imgui-sfml_SOURCE_DIR}/imgui-SFML.cpp" IMGUI_SFML_CPP)
    string(REPLACE "std::basic_string<std::uint8_t> tmp" "auto tmp" IMGUI_SFML_CPP "${IMGUI_SFML_CPP}")
    string(REPLACE "s_clipboardText.assign(tmp.begin(), tmp.end());" "s_clipboardText.assign(reinterpret_cast<const char*>(tmp.data()), tmp.size());" IMGUI_SFML_CPP "${IMGUI_SFML_CPP}")
    file(WRITE "${imgui-sfml_SOURCE_DIR}/imgui-SFML.cpp" "${IMGUI_SFML_CPP}")
endif()

add_executable(Constellation main.cpp)

# Link
target_link_libraries(Constellation PRIVATE sfml-graphics sfml-window sfml-system ImGui-SFML::ImGui-SFML)

# Copy DLLs exactly next to the executable on Windows
if(WIN32)
    add_custom_command(TARGET Constellation POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-system>
        $<TARGET_FILE:sfml-window>
        $<TARGET_FILE:sfml-graphics>
        $<TARGET_FILE:ImGui-SFML>
        $<TARGET_FILE_DIR:Constellation>
    )
endif()
