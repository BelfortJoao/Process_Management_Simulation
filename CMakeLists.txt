cmake_minimum_required(VERSION 3.24)
project(src C)

set(CMAKE_C_STANDARD 99)

add_executable(src src/main.c
        src/manager/timer.c
        src/manager/timer.h
        src/manager/cpu.c
        src/manager/cpu.h
        src/manager/running.c
        src/manager/running.h
        src/manager/blocked.c
        src/manager/blocked.h
        src/manager/ready.c
        src/manager/ready.h
        src/processtable/processtable.c
        src/processtable/processtable.h
        src/process/process.c
        src/process/process.h
        src/control/control.c
        src/control/control.h
        src/printer/printer.c
        src/printer/printer.h
        src/manager/processmanager.c
        src/manager/processmanager.h src/input/input.c src/input/input.h src/error/error.h src/datastruct/queue.c src/datastruct/queue.h src/datastruct/queuenode.c src/datastruct/queuenode.h src/processtable/processtablecell.c src/processtable/processtablecell.h src/processtable/processtablecellqueue.c src/processtable/processtablecellqueue.h)

target_link_libraries(src m)