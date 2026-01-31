project(mediator-asio)

add_library(${PROJECT_NAME}-interface INTERFACE)
target_include_directories(${PROJECT_NAME}-interface INTERFACE Public)
target_link_libraries(${PROJECT_NAME}-interface INTERFACE mediator)
target_link_libraries(${PROJECT_NAME}-interface INTERFACE Boost::boost)

file(GLOB_RECURSE HEADERS ${PROJECT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/*.cpp)
add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC Public)
target_include_directories(${PROJECT_NAME} PRIVATE Private)
target_link_libraries(${PROJECT_NAME} PUBLIC mediator)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::boost)
