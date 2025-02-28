cmake_minimum_required(VERSION 3.25)

project(nsn)

file(GLOB_RECURSE SOURCE_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

find_package(nlohmann_json 3.11.3 REQUIRED)

include(FetchContent)
FetchContent_Declare(xieite GIT_REPOSITORY "https://github.com/Eczbek/xieite" GIT_TAG main)

FetchContent_MakeAvailable(xieite)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json xieite)
