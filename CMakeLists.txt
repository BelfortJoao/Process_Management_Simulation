cmake_minimum_required(VERSION 3.24)
project(src C)

set(CMAKE_C_STANDARD 99)

add_executable(src src/main.c
        src/manager/timer.c
        src/manager/timer.h
        src/ready/ready.c
        src/ready/ready.h
        src/processtable/process_table.c
        src/processtable/process_table.h
        src/process/process.c
        src/process/process.h
        src/control/control.c
        src/control/control.h
        src/printer/printer.c
        src/printer/printer.h
        src/colour/colour.h
        src/manager/process_manager.c
        src/manager/process_manager.h
        src/input/input.c
        src/input/input.h
        src/error/error.h
        src/ready/queue.c
        src/ready/queue.h
        src/ready/queue_node.c
        src/ready/queue_node.h
        src/processtable/process_table_cell.c
        src/processtable/process_table_cell.h
        src/processtable/process_table_cell_queue.c
        src/processtable/process_table_cell_queue.h
        src/blocked/blocked_queue.c
        src/blocked/blocked_queue.h
        src/blocked/blocked_node.c
        src/blocked/blocked_node.h
        src/manager/art_counter.c
        src/manager/art_counter.h
        src/CPU/CPU_node.c
        src/CPU/CPU_node.h
        src/CPU/CPU_queue.c
        src/CPU/CPU_queue.h
        src/running/running_queue.h
        src/running/running_queue.c
        src/CPU/cpu.h
        src/CPU/cpu.c)

target_link_libraries(src m)