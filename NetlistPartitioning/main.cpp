#include <iostream>
#include <random>
#include <chrono>
#include "Spin.h"
#include "netlistBuilder.h"

static unsigned long x = 123456789, y = 362436069, z = 521288629;

unsigned long xorshf96(void) {          //period 2^96-1
	unsigned long t;
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;

	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;

	return z;
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
	int steps_per_temp,
	int seed
) {
	int count = 0;
	unsigned long long cooling_steps = (int)(log(stop_temp/temperature)/log(1-cooling_speed));
	srand(seed);
	std::cout << "Spin Updates: " << cooling_steps*steps_per_temp*spin_count << "\n";
	std::cout << "[                              ]" << "\r" << "[";
	auto start = std::chrono::high_resolution_clock::now();
	while (temperature > stop_temp) {
		for (int i = 0; i < steps_per_temp; i++) {
			for (int j = 0; j < spin_count; j++) {
				spins[j].next_state((double)rand() / RAND_MAX < temperature);
				//spins[j].next_state(xorshf96() < LONG_MAX * temperature);
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
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "\nExcecution time: " << duration.count() << " s";
	std::cout << "\n\n";
}

int main() {
	//std::string path = "C:\\Users\\Alexander Oxklint\\Documents\\KiCAD\\EKG\\EKGIsing.net";
	//std::string path = "C:\\Users\\Alexander Oxklint\\Documents\\KiCAD\\ST_Testboard\\ST_TestboardIsing.net";
	std::string path = "C:\\Users\\Alexander Oxklint\\Desktop\\Kandidat KiCAD\\Electrical System\\ElectricalSystemIsing.net";
	int spin_count = get_component_count(path);
	Spin* spins = new Spin[spin_count];

	double temperature = 0.99;
	double cooling_speed = 0.1;
	double stop_temp = 0.001;
	int steps_per_temp = 300;
	double A = 1;
	double B = -2;
	int seed = 1;

	build_graph_from_netlist(path, spins, spin_count, A, B);
	anneal(spins, spin_count, temperature, cooling_speed, stop_temp, steps_per_temp, seed);
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

	anneal(p_spins, p_spin_count, temperature, cooling_speed, stop_temp, steps_per_temp, seed);
	print_info(p_spins, p_spin_count);
	anneal(n_spins, n_spin_count, temperature, cooling_speed, stop_temp, steps_per_temp, seed);
	print_info(n_spins, n_spin_count);

	//-------------SUBGRAPHS-P-------------
	int p_p_spin_count = get_p_count(p_spins, p_spin_count);
	int p_n_spin_count = p_spin_count - p_p_spin_count;

	Spin* p_p_spins = new Spin[p_p_spin_count];
	Spin* p_n_spins = new Spin[p_n_spin_count];
	build_subgraphs(
		path,
		p_spins, p_spin_count,
		A, B,
		p_p_spins, p_n_spins,
		p_p_spin_count, p_n_spin_count
	);

	anneal(p_p_spins, p_p_spin_count, temperature, cooling_speed, stop_temp, steps_per_temp, seed);
	print_info(p_p_spins, p_p_spin_count);
	anneal(p_n_spins, p_n_spin_count, temperature, cooling_speed, stop_temp, steps_per_temp, seed);
	print_info(p_n_spins, p_n_spin_count);

	//-------------SUBGRAPHS-N-------------
	int n_p_spin_count = get_p_count(n_spins, n_spin_count);
	int n_n_spin_count = n_spin_count - n_p_spin_count;

	Spin* n_p_spins = new Spin[n_p_spin_count];
	Spin* n_n_spins = new Spin[n_n_spin_count];
	build_subgraphs(
		path,
		n_spins, n_spin_count,
		A, B,
		n_p_spins, n_n_spins,
		n_p_spin_count, n_n_spin_count
	);

	anneal(n_p_spins, n_p_spin_count, temperature, cooling_speed, stop_temp, steps_per_temp, seed);
	print_info(n_p_spins, n_p_spin_count);
	anneal(n_n_spins, n_n_spin_count, temperature, cooling_speed, stop_temp, steps_per_temp, seed);
	print_info(n_n_spins, n_n_spin_count);

	/*//TEST CODE - PRINTS COMPONENT AND CONNECTED COMPONENTS
	for (int i = 0; i < spin_count; i++) {
		std::cout << spins[i].name + "\n";
		for (int j = 0; j < spins[i].neighbours; j++) {
			std::cout << spins[i].adjacent[j].name << " " << spins[i].weights[j] << " ";
		}
		std::cout << "\n\n";
	}*/
}

