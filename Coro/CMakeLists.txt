project(coro)

file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)

add_library(${PROJECT_NAME}-interface INTERFACE)
target_include_directories(${PROJECT_NAME}-interface INTERFACE Public)

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC Public)
target_include_directories(${PROJECT_NAME} PRIVATE Private)
