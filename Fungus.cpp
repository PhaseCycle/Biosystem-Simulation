
/*
Authors: Andrew Yoder, Thomas Stryjski, Zachary O'Brien
Email: aby7159@rit.edu, tgs9181@rit.edu, zjo5244@rit.edu
Date: 12/9/18
Class: EEEE-346-01
Assignment: Project 3

Purpose: Implementation of derived class for small fungi.
*/

#include "Fungus.h"
#include "Miscellaneous.h"
#include <cmath>

Fungus::Fungus(double x, double y, double z) {
	consumption_amount = 0;
	consumption_time = 0;
	reproduction_amount = 2;
	reproduction_time = 2;
	reproduction_counter = reproduction_time;
	consumption_food_counter = consumption_amount;
	consumption_time_counter = consumption_time;
	age = 0;
	movement = 2;
	spawn_distance = 4;
	visibility = 1;
	fertility = 2;
	setLocation(x, y, z);
}

//getters
double Fungus::get_fertility() {
	return fertility;
}

//setters
void Fungus::set_fertility() {
	double temper = exp(-1 * pow((temperature - 50), 2));
	fertility = 2 * temper * (1 + temper);
}

//other



void Fungus::reproduce(Fungus *O) {
	double theta = fRand(0, 2 * 3.14159265);
	double phi = fRand(0, 2 * 3.14159265);

	double x = spawn_distance * cos(phi) * cos(theta);
	double y = spawn_distance * cos(phi) * sin(theta);
	double z = spawn_distance * sin(phi);

	O->setLocation(x, y, z);

}
void Fungus::aged() {
	age++;
	movement = movement - (age/20); //decereases movement the older the cell is, although this is scaled by the constant 10. Accumulates
	spawn_distance = 4 - (age / 100);//decreases spawn distance as the cell gets older, scaled by the constant 100. Acculmulates
}
Fungus Fungus::operator+(Bacteria *b) {
	Point_3D newLocation = b->getLocation();
	this->setLocation(newLocation);
	dec_con_food_counter();
	//	delete b;
	return *this;
}


