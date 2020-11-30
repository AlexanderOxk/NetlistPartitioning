#include <iostream>
#include "Spin.h"
#include "netlistBuilder.h"

inline double randf() {
	return (double)rand() / RAND_MAX;
}

void clean_spins(Spin* spins, int spin_count) {
	for (int i = 0; i < spin_count; i++) {
		delete[] spins[i].weights;
	}
	delete[] spins;
}

double get_energy(Spin* spins, int spin_count) {
	double energy = 0;
	for (int i = 0; i < spin_count; i++) {
		energy += spins[i].energy_adjacent();
	}
	return energy;
}

void anneal(
	Spin* spins,
	int spin_count,
	double temperature,
	double cooling_speed,
	double stop_temp,
	int steps_per_temp
) {
	int count = 0;
	int cooling_steps = (int)(log(stop_temp/temperature)/log(1-cooling_speed));
	std::cout << "[                              ]" << "\r" << "[";

	while (temperature > stop_temp) {
		for (int i = 0; i < steps_per_temp; i++) {
			for (int j = 0; j < spin_count; j++) {
				spins[j].next_state(randf() < temperature);
				spins[j].flip();
			}
		}
		temperature *= (1 - cooling_speed);
		count++;
		if (count > cooling_steps / 30 - 1) {
			count = 0;
			std::cout << ".";
		}
	}
	std::cout << "\n\n";
}

int main() {
	std::string path = "C:\\Users\\Alexander Oxklint\\Documents\\KiCAD\\EKG\\EKGIsing.net";
	//std::string path = "C:\\Users\\Alexander Oxklint\\Documents\\KiCAD\\ST_Testboard\\ST_TestboardIsing.net";
	int spin_count = get_component_count(path);
	Spin* spins = new Spin[spin_count];

	double temperature = 0.9;
	double cooling_speed = 0.0001;
	double stop_temp = 0.001;
	int steps_per_temp = 50;
	double A = 1;
	double B = -1;

	build_graph_from_netlist(path, spins, spin_count, A, B);
	anneal(spins, spin_count, temperature, cooling_speed, stop_temp, steps_per_temp);
	
	int p_spin_count = 0;
	for (int i = 0; i < spin_count; i++) {
		std::cout << spins[i].name + "\t" << spins[i].spin << "\n";
		if (spins[i].spin == 1) {
			p_spin_count++;
		}
	}

	std::cout << "\n" << "Energy: " << get_energy(spins, spin_count) << "\n";
	std::cout << "+ spins: " << p_spin_count << "\n" << "- spins: " << spin_count - p_spin_count << "\n";

	/*//TEST CODE - PRINTS COMPONENT AND CONNECTED COMPONENTS
	for (int i = 0; i < spin_count; i++) {
		std::cout << spins[i].name + "\n";
		for (int j = 0; j < spins[i].neighbours; j++) {
			std::cout << spins[i].adjacent[j].name << " " << spins[i].weights[j] << " ";
		}

		std::cout << "\n\n";
	}*/
}

