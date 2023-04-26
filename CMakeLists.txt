cmake_minimum_required(VERSION 3.24)
project(src C)

set(CMAKE_C_STANDARD 99)

add_executable(src src/main.c src/Manager/timer.c src/Manager/timer.h src/Manager/cpu.c src/Manager/cpu.h src/Manager/executingState.c src/Manager/executingState.h src/Manager/blockedState.c src/Manager/blockedState.h src/Manager/ReadyState.c src/Manager/ReadyState.h src/Manager/ProcessTable.c src/Manager/ProcessTable.h src/Process.c src/Process.h src/Control/Controler.c src/Control/Controler.h src/Printer/Printer.c src/Printer/Printer.h src/Manager/manager.c src/Manager/manager.h)

target_link_libraries(src m)