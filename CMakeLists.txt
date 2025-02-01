cmake_minimum_required(VERSION 3.16)
project(Project3 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(Project3 src/main.cpp src/WelcomeScreen.cpp src/GameScreen.cpp src/LeaderboardScreen.cpp)
target_link_libraries(Project3 PRIVATE sfml-graphics)
target_compile_features(Project3 PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
            TARGET Project3
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Project3>
            VERBATIM)
endif()

install(TARGETS Project3)