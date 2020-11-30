#include <fstream>
#include <sstream>
#include <iostream>
#include "netlistBuilder.h"

int get_component_count(std::string path) {
	std::ifstream file(path);
	std::string str;
	int component_count = 0;

	while (file >> str) {
		if (str == "(comp") {
			component_count++;
		}
	}

	return component_count;
}

int get_net_count(std::string path) {
	std::ifstream file(path);
	std::string str;
	int net_count = 0;

	while (file >> str) {
		if (str == "(net") {
			net_count++;
		}
	}

	return net_count;
}

std::string get_names(std::string path) {
	std::ifstream file(path);
	std::string str;
	std::string names;

	while (file >> str) {
		if (str == "(comp") {
			file >> str;
			file >> str;
			str.pop_back();
			names += str + " ";
		}
	}

	return names;
}

std::string get_nets(std::string path) {
	std::ifstream file(path);
	std::string str;
	std::string nets;

	while (str != "(net") {		//Move to first net
		file >> str;
	}
	while (file >> str) {		//Read nets
		if (str == "(ref") {
			file >> str;
			str.pop_back();
			nets += str + " ";
		}
		if (str == "(net") {
			nets.pop_back();
			nets += "\n";
		}
	}

	return nets;
}

void set_array_const(double* array, int size, double c) {
	for (int i = 0; i < size; i++) {
		array[i] = c;
	}
}

bool line_contains_component(std::string line, std::string comp) {
	int pos = line.find(comp);
	if (pos != std::string::npos) {
		std::istringstream line_stream(line);
		std::string str;
		while (line_stream >> str) {
			if (str == comp) {
				return true;
			}
		}
	}
	return false;
}

void build_graph_from_netlist(std::string path, Spin* spins, int spin_count, double A, double B) {
	int net_count = get_net_count(path);
	std::string names = get_names(path);
	std::string nets = get_nets(path);

	//std::cout << nets;			//TEST PRINT

	std::istringstream name_stream(names);
	for (int i = 0; i < spin_count; i++) {
		name_stream >> spins[i].name;
	}

	for (int i = 0; i < spin_count; i++) {							//For every spin...
		double* weights = new double[spin_count];
		set_array_const(weights, spin_count, A);
		std::istringstream nets_stream(nets);
		std::string line;
		for (int j = 0; j < net_count; j++) {						//...check every net...
			std::getline(nets_stream, line);
			if (line_contains_component(line, spins[i].name)) {		//...if that net contains the spin...
				std::istringstream line_stream(line);
				std::string token;
				while (line_stream >> token) {						//...cycle through the net...
					for (int k = 0; k < spin_count; k++) {
						if (token == spins[k].name) {
							weights[k] = weights[k] + B;			//...and add weights for every connection.
						}
					}
				}
			}
		}
		spins[i].add_to_graph(weights, spins, spin_count);
	}
}