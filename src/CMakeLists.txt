cmake_minimum_required(VERSION 3.24)
project(src C)

set(CMAKE_C_STANDARD 99)

add_executable(src main.c Manager/timer.c Manager/timer.h Manager/cpu.c Manager/cpu.h Manager/executingState.c Manager/executingState.h Manager/blockedState.c Manager/blockedState.h Manager/ReadyState.c Manager/ReadyState.h Manager/ProcessTable.c Manager/ProcessTable.h Process.c Process.h Control/Controler.c Control/Controler.h Printer/Printer.c Printer/Printer.h Manager/manager.c Manager/manager.h)

target_link_libraries(src m)