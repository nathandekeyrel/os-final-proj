#include <stdio.h>
#include "sim.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_csv> <output_csv>\n", argv[0]);
        return 1;
    }

    simulation_t *sim = simulation_create(argv[1], argv[2]);
    if (!sim) {
        fprintf(stderr, "Failed to create simulation\n");
        return 1;
    }

    simulation_run(sim);
    simulation_destroy(sim);

    return 0;
}