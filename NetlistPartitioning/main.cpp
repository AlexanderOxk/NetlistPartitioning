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

int get_p_count(Spin* spins, int spin_count) {
	int count = 0;
	for (int i = 0; i < spin_count; i++) {
		if (spins[i].spin == 1) {
			count++;
		}
	}
	return count;
}

void print_info(Spin* spins, int spin_count) {
	int p_spin_count = get_p_count(spins, spin_count);
	for (int i = 0; i < spin_count; i++) {
		std::cout << spins[i].name + "\t" << spins[i].spin << "\n";
	}
	std::cout << "\n" << "Energy: " << get_energy(spins, spin_count) << "\n";
	std::cout << "+ spins: " << p_spin_count << "\n" << "- spins: " << spin_count - p_spin_count << "\n";
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
	double cooling_speed = 0.001;
	double stop_temp = 0.001;
	int steps_per_temp = 50;
	double A = 1;
	double B = -2;

	build_graph_from_netlist(path, spins, spin_count, A, B);
	anneal(spins, spin_count, temperature, cooling_speed, stop_temp, steps_per_temp);
	print_info(spins, spin_count);
	

	//-------------SUBGRAPHS-------------
	int p_spin_count = get_p_count(spins, spin_count);
	int n_spin_count = spin_count - p_spin_count;

	Spin* p_spins = new Spin[p_spin_count];
	Spin* n_spins = new Spin[n_spin_count];
	build_subgraphs(
		path,
		spins, spin_count,
		A, B,
		p_spins, n_spins,
		p_spin_count, n_spin_count
	);

	anneal(p_spins, p_spin_count, temperature, cooling_speed, stop_temp, steps_per_temp);
	print_info(p_spins, p_spin_count);
	anneal(n_spins, n_spin_count, temperature, cooling_speed, stop_temp, steps_per_temp);
	print_info(n_spins, n_spin_count);

	/*//TEST CODE - PRINTS COMPONENT AND CONNECTED COMPONENTS
	for (int i = 0; i < spin_count; i++) {
		std::cout << spins[i].name + "\n";
		for (int j = 0; j < spins[i].neighbours; j++) {
			std::cout << spins[i].adjacent[j].name << " " << spins[i].weights[j] << " ";
		}
		std::cout << "\n\n";
	}

	for (int i = 0; i < p_spin_count; i++) {
		std::cout << p_spins[i].name + "\n";
		for (int j = 0; j < p_spins[i].neighbours; j++) {
			std::cout << p_spins[i].adjacent[j].name << " " << p_spins[i].weights[j] << " ";
		}
		std::cout << "\n\n";
	}

	for (int i = 0; i < n_spin_count; i++) {
		std::cout << n_spins[i].name + "\n";
		for (int j = 0; j < n_spins[i].neighbours; j++) {
			std::cout << n_spins[i].adjacent[j].name << " " << n_spins[i].weights[j] << " ";
		}
		std::cout << "\n\n";
	}*/
}

