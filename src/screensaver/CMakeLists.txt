cmake_minimum_required(VERSION 3.13...3.25)

find_package(Curses REQUIRED)

project(screensaver
    DESCRIPTION "curses screensaver display"
    LANGUAGES   CXX
)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE screensaver.cc
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CURSES_INCLUDE_DIR}
    PUBLIC ${NEO_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${CURSES_LIBRARIES}
    PRIVATE screen_buffer
)
