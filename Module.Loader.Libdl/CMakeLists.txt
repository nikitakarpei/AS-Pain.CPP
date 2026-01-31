project(module-loader-libdl)

file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)

add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC Public)
target_include_directories(${PROJECT_NAME} PRIVATE Private)
target_link_libraries(${PROJECT_NAME} PUBLIC module-loader-interface)
target_link_libraries(${PROJECT_NAME} PRIVATE libdl)