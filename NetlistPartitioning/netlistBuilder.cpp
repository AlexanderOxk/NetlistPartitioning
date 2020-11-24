#include <fstream>
#include <sstream>
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

void set_array_zero(double* array, int size) {
	for (int i = 0; i < size; i++) {
		array[i] = 0;
	}
}

void build_graph_from_netlist(std::string path, Spin* spins, int spin_count) {
	int net_count = get_net_count(path);
	std::string names = get_names(path);
	std::string nets = get_nets(path);

	std::istringstream name_stream(names);
	for (int i = 0; i < spin_count; i++) {
		name_stream >> spins[i].name;
	}

	for (int i = 0; i < spin_count; i++) {							//For every spin...
		double* weights = new double[spin_count];
		set_array_zero(weights, spin_count);
		std::istringstream nets_stream(nets);
		std::string line;
		for (int j = 0; j < net_count; j++) {						//...check every net...
			std::getline(nets_stream, line);
			if (line.find(spins[i].name) != std::string::npos) {	//...if that net contains the spin...
				std::istringstream line_stream(line);
				std::string token;
				while (line_stream >> token) {						//...cycle through the net...
					for (int k = 0; k < spin_count; k++) {
						if (token == spins[k].name) {
							weights[k] = 1;							//...and add weights for every connection.
						}
					}
				}
			}
		}
		spins[i].add_to_graph(weights, spins, spin_count);
	}
}