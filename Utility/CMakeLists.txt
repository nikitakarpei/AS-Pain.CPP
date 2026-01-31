project(utility)

file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)

add_library(${PROJECT_NAME} INTERFACE ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} INTERFACE Public)