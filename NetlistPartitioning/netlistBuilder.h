#pragma once
#include <string>
#include "Spin.h"

int get_component_count(std::string path);
void build_graph_from_netlist(std::string path, Spin* spins, int spin_count, int A, int B);
void build_subgraphs(
	std::string path,
	Spin* spins,
	int spin_count,
	int A,
	int B,
	Spin* p_spins,
	Spin* n_spins,
	int p_spin_count,
	int n_spin_count
);