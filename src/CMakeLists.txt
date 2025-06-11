find_package(Curses REQUIRED)

add_executable(${PROJECT_NAME} 
    main.cpp
    tetris_state.cpp
    tetris_ui_ncurses.cpp
    tetris.cpp)

target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARY})