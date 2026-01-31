project(module-loader)

add_library(${PROJECT_NAME}-interface INTERFACE)
target_include_directories(${PROJECT_NAME}-interface INTERFACE Public)
target_link_libraries(${PROJECT_NAME}-interface INTERFACE module-interface)