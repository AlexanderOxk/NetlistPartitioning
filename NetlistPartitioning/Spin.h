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
    double energy_adjacent();
    void next_state(bool flip);
    void randomize();
};

