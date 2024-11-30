cmake_minimum_required(VERSION 3.27)
project(os_final_proj C)

set(CMAKE_C_STANDARD 11)

include_directories(src/core)
include_directories(src/metrics)
include_directories(src/schedulers/completely-fair-scheduler)
include_directories(src/utils)

add_executable(os_final_proj
        src/core/process.c
        src/core/process.h
        src/core/queue.c
        src/core/queue.h
        src/metrics/metrics.h
        src/metrics/response_time.c
        src/metrics/turnaround_time.c
        src/metrics/virtual_runtime_variance.c
        src/schedulers/completely-fair-scheduler/cfs.c
        src/schedulers/completely-fair-scheduler/cfs.h
        src/utils/rbtree.c
        src/utils/rbtree.h
        src/schedulers/first-come-first-serve/fcfs.c
        src/schedulers/first-come-first-serve/fcfs.h
        src/schedulers/scheduler.h)
