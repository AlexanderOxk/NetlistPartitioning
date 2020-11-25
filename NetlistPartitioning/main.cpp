#include <iostream>
#include "Spin.h"
#include "netlistBuilder.h"

void clean_spins(Spin* spins, int spin_count) {
	for (int i = 0; i < spin_count; i++) {
		delete[] spins[i].weights;
	}
	delete[] spins;
}

void anneal(
	Spin* spins,
	int spin_count,
	double temperature,
	double cooling_speed,
	double stop_temp,
	int steps_per_temp,
	double edge_penalty,
	double ineq_penalty
) {
	std::cout << "Goal temp: " << stop_temp << "\n";

	while (temperature > stop_temp) {
		for (int i = 0; i < steps_per_temp; i++) {
			for (int j = 0; j < spin_count; j++) {
				spins[j].next_state(temperature, edge_penalty, ineq_penalty);
				spins[j].flip();
			}
		}
		temperature *= (1 - cooling_speed);
		std::cout << "Current temp: " << temperature << "\r";
	}
	std::cout << "\n";
}

int main() {
	std::string path = "C:\\Users\\Alexander Oxklint\\Documents\\KiCAD\\EKG\\EKG.net";
	int spin_count = get_component_count(path);
	Spin* spins = new Spin[spin_count];

	double temperature = 0.8;
	double cooling_speed = 0.001;
	double stop_temp = 0.00001;
	int steps_per_temp = 50;
	double edge_penalty = 1;
	double ineq_penalty = 2;

	build_graph_from_netlist(path, spins, spin_count);
	anneal(spins, spin_count, temperature, cooling_speed, stop_temp, steps_per_temp, edge_penalty, ineq_penalty);
	
	for (int i = 0; i < spin_count; i++) {
		std::cout << spins[i].name + " " << spins[i].spin << "\n";
	}

	/*//TEST CODE - PRINTS COMPONENT AND CONNECTED COMPONENTS
	for (int i = 0; i < spin_count; i++) {
		std::cout << spins[i].name + "\n";
		for (int j = 0; j < spins[i].neighbours; j++) {
			if (spins[i].weights[j]) {
				std::cout << spins[i].adjacent[j].name + " ";
			}
		}

		std::cout << "\n\n";
	}*/
}

