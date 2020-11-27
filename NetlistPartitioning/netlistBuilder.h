#pragma once
#include <string>
#include "Spin.h"

int get_component_count(std::string path);
void build_graph_from_netlist(std::string path, Spin* spins, int spin_count, double A, double B);