/*
Authors: Andrew Yoder, Thomas Stryjski, Zachary O'Brien
Email: aby7159@rit.edu, tgs9181@rit.edu, zjo5244@rit.edu
Date: 12/5/18
Class: EEEE-346-01
Assignment: Project 3

Purpose: Executes functions declared in Bacteria.h
*/

#include "Bacteria.h"
#include "Miscellaneous.h"
#include <cmath>


//constructors
Bacteria::Bacteria(double x, double y, double z) {
	consumption_amount = 0;
	consumption_time = 0;
	reproduction_amount = 4;
	reproduction_time = 3;
	reproduction_counter = reproduction_time;
	consumption_food_counter = consumption_amount;
	consumption_time_counter = consumption_time;
	age = 0;
	movement = 2;
	spawn_distance = 4;
	visibility = 1;

	setLocation(x, y, z);
}

//getters

double Bacteria::get_fertility() {
	return fertility;
}

//setters
void Bacteria::set_sunlight(double x) {
	sunlight = x;
}
void Bacteria::set_chemical(double x) {
	chemical = x;
}
void Bacteria::set_fertility() {
	double temper = exp((-1/200) * pow((temperature - 75), 2));
	fertility = (1 - temper) + (1 + sunlight) * (1 +chemical);
}

//other


void Bacteria::reproduce(Bacteria *O) {
	double theta = fRand(0, 2 * 3.14159265);
	double phi = fRand(0, 2 * 3.14159265);

	double x = spawn_distance * cos(phi) * cos(theta);
	double y = spawn_distance * cos(phi) * sin(theta);
	double z = spawn_distance * sin(phi);

	O = new Bacteria(x, y, z);

}
void Bacteria::aged() {
	age++;
	movement = movement - (age/20); //decereases movement the older the cell is, although this is scaled by the constant 10. Accumulates
	spawn_distance = 4 - (age / 100);//decreases spawn distance as the cell gets older, scaled by the constant 100. Acculmulates
}
