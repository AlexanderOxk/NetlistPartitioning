#pragma once
#include <string>

class Spin {
public:
    int spin, next_spin;
    Spin* adjacent;
    int* weights;
    int neighbours;
    std::string name;

    Spin();
    void add_to_graph(int*, Spin*, int);
    void flip();
    int energy_adjacent();
    void next_state(bool flip);
    void randomize();
};

