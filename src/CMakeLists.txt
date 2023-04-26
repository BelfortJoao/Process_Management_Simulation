cmake_minimum_required(VERSION 3.24)
project(src C)

set(CMAKE_C_STANDARD 99)

add_executable(src main.c
        manager/timer.c
        manager/timer.h
        manager/cpu.c
        manager/cpu.h
        manager/executingstate.c
        manager/executingstate.h
        manager/blockedstate.c
        manager/blockedstate.h
        manager/readystate.c
        manager/readystate.h
        manager/processtable.c
        manager/processtable.h
        process.c
        process.h
        control/controller.c
        control/controller.h
        printer/printer.c
        printer/printer.h
        manager/manager.c
        manager/manager.h)

target_link_libraries(src m)