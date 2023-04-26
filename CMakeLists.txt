cmake_minimum_required(VERSION 3.24)
project(src C)

set(CMAKE_C_STANDARD 99)

add_executable(src src/main.c
        src/manager/timer.c
        src/manager/timer.h
        src/manager/cpu.c
        src/manager/cpu.h
        src/manager/executingstate.c
        src/manager/executingstate.h
        src/manager/blockedstate.c
        src/manager/blockedstate.h
        src/manager/readystate.c
        src/manager/readystate.h
        src/manager/processtable.c
        src/manager/processtable.h
        src/process/process.c
        src/process/process.h
        src/control/controller.c
        src/control/controller.h
        src/printer/printer.c
        src/printer/printer.h
        src/manager/manager.c
        src/manager/manager.h)

target_link_libraries(src m)