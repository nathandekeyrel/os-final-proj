cmake_minimum_required(VERSION 3.27)
project(os_final_proj C)

set(CMAKE_C_STANDARD 11)

# Include directories
include_directories(
        src/core
        src/metrics
        src/schedulers
        src/utils
        src/simulation
)

# Source files
set(SOURCE_FILES
        src/core/process.c
        src/core/process.h
        src/core/queue.c
        src/core/queue.h
        src/schedulers/completely-fair-scheduler/cfs.c
        src/schedulers/completely-fair-scheduler/cfs.h
        src/utils/rbtree.c
        src/utils/rbtree.h
        src/simulation/sim.h
        src/simulation/main.c
        /src/metrics/metrics.c
        /src/metrics/metrics.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})