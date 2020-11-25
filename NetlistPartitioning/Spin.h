#pragma once
#include <string>

class Spin {
public:
    double spin, next_spin;
    Spin* adjacent;
    double* weights;
    int neighbours;
    std::string name;

    Spin();
    void add_to_graph(double*, Spin*, int);
    void flip();
    double energy_adjacent(int sign, double edge_penalty, double ineq_penalty);
    void next_state(double temperature, double edge_penalty, double ineq_penalty);
    void randomize();
};

