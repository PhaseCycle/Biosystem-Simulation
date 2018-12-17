/*
Authors: Andrew Yoder, Thomas Stryjski, Zachary O'Brien
Email: aby7159@rit.edu, tgs9181@rit.edu, zjo5244@rit.edu
Date: 12/1/18
Class: EEEE-346-01
Assignment: Project 3
Purpose: Provides the interface to take in data and return the results.
*/
#include <iostream>
#include <string>
#include <vector>
#include "MacroEnvironment.h"
#include "MicroEnvironment.h"
#include "Bacteria.h"
#include "Fungus.h"
#include "Animal.h"
#include "Point_3D.h"
#include "Point.h"


using namespace std;


int main() {
	int cond, ani_pop, plant_pop, bact_pop, fung_pop, t;
	double macEnv_x, macEnv_y, micEnv_x, micEnv_y;
	double micEnv_z, min_t, max_t;
	int terminate = 1;

	Environment* sim = NULL;


	while (terminate == 1){
		srand(1);
		cout << "\n\n\nWelcome to the Biosystem Simulation program \n";
		cout << "_________________________________________________________________________________\n";
		cout << "Enter 1 to simulate macroorganisms or 2 to simulate microorganisms: ";
		cin >> cond;

		while (cond != 1 && cond != 2) {
			cout << "Enter 1 to simulate macroorganisms or 2 to simulate microorganisms: ";
			cin >> cond;
		}

		if (cond == 1) {
			cout << "Enter x size of Environment: ";
			cin >> macEnv_x;
			cout << "Enter y size of Environment: ";
			cin >> macEnv_y;
			cout << "Enter maximum environment temperature: ";
			cin >> max_t;
			cout << "Enter minimum environment temperature: ";
			cin >> min_t;
			cout << "Enter animal population: ";
			cin >> ani_pop;
			cout << "Enter plant population: ";
			cin >> plant_pop;
			cout << "Enter time for simulation: ";
			cin >> t;
			sim = new MacroEnvironment(t, min_t, max_t, macEnv_x, macEnv_y, ani_pop, plant_pop);
		}

		if (cond == 2) {
			cout << "Enter x size of Environment: ";
			cin >> micEnv_x;
			cout << "Enter y size of Environment: ";
			cin >> micEnv_y;
			cout << "Enter z size of Environment: ";
			cin >> micEnv_z;
			cout << "Enter maximum environment temperature: ";
			cin >> max_t;
			cout << "Enter minimum environment temperature: ";
			cin >> min_t;
			cout << "Enter fungus population: ";
			cin >> fung_pop;
			cout << "Enter bacteria population: ";
			cin >> bact_pop;
			cout << "Enter time for simulation: ";
			cin >> t;
			sim = new MicroEnvironment(t, min_t, max_t, micEnv_x, micEnv_y, micEnv_z, bact_pop, fung_pop);
		}

		if (cond == 1) {
			cout << "\nTime\t" << "Animal\t" << "Plant\t" << "O2\t" << "CO2\t\n";
		}
		if (cond == 2) {
			cout << "\nTime\t" << "Fungus\t" << "Bacteria\t\n";
		}
		for (int i = 0; i < sim->get_run_to_time(); i++) {
			sim->event();
			sim->print();
		}

		do {
			cout << "Do you want to run another simulation? (YES ---> 1)  (NO ---> 0) : ";
			cin >> terminate;
		} while (terminate != 1 && terminate != 0);
		cond = 0;
	}
}
