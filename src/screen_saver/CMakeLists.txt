cmake_minimum_required(VERSION 3.13...3.25)

project(screen_saver DESCRIPTION "matrix screensaver core logic"
                     LANGUAGES   CXX
)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE screen_saver.cc
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${NEO_INCLUDE_DIR}
)
