cmake_minimum_required(VERSION 2.6)

project("takuzu-10A" LANGUAGES C)

include(SDL2.cmake)
message(STATUS "SDL2 include dir: ${SDL2_ALL_INC}")
message(STATUS "SDL2 all libraries: ${SDL2_ALL_LIBS}")

## compilation rules
include_directories(${SDL2_ALL_INC})
add_executable(game_sdl game_sdl.c model.c)
target_link_libraries(game_sdl ${SDL2_ALL_LIBS} m game)

## copy useful data files in build dir
foreach(file "arial.ttf" "Walter.ttf" "bambi2.png" "panpansad.png" "panpan3.png" "bambisad.png" "betterback.png" "pancarte.png" "fleche gauche.png" "fleche droite.png" "restartbois.png")
  file(COPY ${file} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach(file)

########################################################################################
project(game C)

link_directories(${CMAKE_SOURCE_DIR})

include(CTest)
enable_testing()

set(CMAKE_C_FLAGS "-Wall -std=c99")            # options de compilation génériques
set(CMAKE_C_FLAGS_DEBUG "-g -O0 --coverage")   # options de compilation en mode Debug
set(CMAKE_C_FLAGS_RELEASE "-O3")               # options de compilation en mode Release

add_library(game game.c game_aux.c game_ext.c queue.c game_tools.c)

add_executable(game_text game_text.c)
target_link_libraries(game_text game)

add_executable(game_solve game_solve.c)
target_link_libraries(game_solve game)

add_executable(game_test game_test.c)
target_link_libraries(game_test game)

# file copy
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/solution.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/default.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Po

### TESTS V1
# game file's functions
add_test(test_game_new ./game_test game_new)
add_test(test_game_new_empty ./game_test game_new_empty)
add_test(test_game_copy ./game_test game_copy)
add_test(test_game_equal ./game_test game_equal)
add_test(test_game_delete ./game_test game_delete)
add_test(test_game_set_square ./game_test game_set_square)
add_test(test_game_get_square ./game_test game_get_square)
add_test(test_game_get_number ./game_test game_get_number)
add_test(test_game_get_next_square ./game_test game_get_next_square)
add_test(test_game_get_next_number ./game_test game_get_next_number)
add_test(test_game_is_empty ./game_test game_is_empty)
add_test(test_game_is_immutable ./game_test game_is_immutable)
add_test(test_game_has_error ./game_test game_has_error)
add_test(test_game_check_move ./game_test game_check_move)
add_test(test_game_play_move ./game_test game_play_move)
add_test(test_game_is_over ./game_test game_is_over)
add_test(test_game_restart ./game_test game_restart)
# game_aux file's functions
add_test(test_game_print ./game_test game_print)
add_test(test_game_default ./game_test game_default)
add_test(test_game_default_solution ./game_test game_default_solution)

### TESTS V2
# game_ext file's functions
add_test(test_game_new_ext ./game_test game_new_ext)
add_test(test_game_new_empty_ext ./game_test game_new_empty_ext)
add_test(test_game_undo ./game_test game_undo)
add_test(test_game_redo ./game_test game_redo)

### TESTS GAME_IO

add_test(test_game_save ./game_test game_save)
add_test(test_game_load ./game_test game_load)

### TESTS SOLVEUR

add_test(test_game_nb_solutions ./game_test game_nb_solutions)
add_test(test_game_solve ./game_test game_solve)

