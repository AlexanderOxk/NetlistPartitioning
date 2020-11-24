#include <iostream>
#include "Spin.h"
#include "netlistBuilder.h"

void clean_spins(Spin* spins, int spin_count) {
	for (int i = 0; i < spin_count; i++) {
		delete[] spins[i].weights;
	}
	delete[] spins;
}

int main() {
	std::string path = "C:\\Users\\Alexander Oxklint\\Documents\\KiCAD\\EKG\\EKG.net";
	int spin_count = get_component_count(path);
	Spin* spins = new Spin[spin_count];

	build_graph_from_netlist(path, spins, spin_count);

	/* TEST CODE - PRINTS COMPONENT AND CONNECTED COMPONENTS
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

