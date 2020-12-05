#include <cstdlib>
#include "Spin.h"

inline double randf() {
    return (double)rand() / RAND_MAX;
}

Spin::Spin() {
    randomize();
    next_spin = 1;
}

void Spin::add_to_graph(double* _weights, Spin* _adjacent, int _neighbours) {
    adjacent = _adjacent;
    weights = _weights;
    neighbours = _neighbours;
}

void Spin::randomize() {
    spin = randf() > 0.5 ? -1 : 1;
}

double Spin::energy_adjacent() {
    double total = 0;
    for (int i = 0; i < neighbours; i++) {
        total += spin * adjacent[i].spin * weights[i];
    }
    return total;
}

void Spin::flip() {
    spin = next_spin;
}

void Spin::next_state(bool flip) {
    next_spin = energy_adjacent() < 0 && !flip ? spin : -spin;
    /*if (next_spin == spin) {
        next_spin = flip ? -next_spin : next_spin;
    }*/
}