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

double Spin::energy_adjacent(int sign, double edge_penalty, double ineq_penalty) {
    double edge_energy = 0;
    double ineq_energy = 0;
    for (int i = 0; i < neighbours; i++) {
        edge_energy += 1 - sign * spin * adjacent[i].spin * weights[i];
        ineq_energy += adjacent[i].spin;
    }
    if (sign == -1) {
        ineq_energy += spin == 1 ? -2 : 2;
    }
    ineq_energy = ineq_penalty * abs(ineq_energy);
    edge_energy *= edge_penalty;
    return edge_energy + ineq_energy;
}

void Spin::flip() {
    spin = next_spin;
}

void Spin::next_state(double temperature, double edge_penalty, double ineq_penalty) {
    if (energy_adjacent(-1, edge_penalty, ineq_penalty) < energy_adjacent(1, edge_penalty, ineq_penalty)) {
        next_spin = -spin;
    }
    else if (randf() < temperature) {
        next_spin = -spin;
    }
}