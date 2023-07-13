cmake_minimum_required(VERSION 3.13...3.25)

add_executable(${CMAKE_PROJECT_NAME})

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE neo.cc
)

install(TARGETS ${CMAKE_PROJECT_NAME}
    RUNTIME DESTINATION ${NEO_BIN_DIR}
)
