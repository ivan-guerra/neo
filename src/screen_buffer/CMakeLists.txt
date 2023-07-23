cmake_minimum_required(VERSION 3.13...3.25)

project(screen_buffer DESCRIPTION "a vertically scrolling 2D screen buffer"
                      LANGUAGES   CXX
)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE screen_buffer.cc
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${NEO_INCLUDE_DIR}
)
