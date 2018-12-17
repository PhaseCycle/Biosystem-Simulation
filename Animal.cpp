/*
Authors: Andrew Yoder, Thomas Stryjski, Zachary O'Brien
Email: aby7159@rit.edu, tgs9181@rit.edu, zjo5244@rit.edu
Date: 12/5/18
Class: EEEE-346-01
Assignment: Project 3

Purpose: Executes functions declared in Animal.h
*/

#include "Animal.h"
#include "Miscellaneous.h"


//constructors
Animal::Animal(double x, double y) {
	consumption_amount = 1;
	consumption_time = 5;
	reproduction_amount = 2;
	reproduction_time = 3;
	reproduction_counter = reproduction_time;
	consumption_food_counter = consumption_amount;
	consumption_time_counter = consumption_time;
	age = 0;
	movement = 3;
	spawn_distance = 4;
	visibility = 2;

	//temp setter
	o2 = 5;
	co2 = 5;
	fertility = 1;

	setLocation(x, y);
}

//getters
double Animal::get_co2() {
	return co2;
}
double Animal::get_fertility() {
	return fertility;
}

//setters
void Animal::set_o2(double x) {
	o2 = x;
}
void Animal::set_co2(double x){
	co2 = x;
}
void Animal::set_fertility() {
	//after having tried upside-down parabolas to unsucessfully model organisms having optimal fertility, we discovered the amazing gaussian curve!
	//format , in this case: e^(-1/a * (x-d)^(2)), where a is the horizontal strech factor of the curve and d is the desired/optimal value where the function = 1
	double temper = exp((-1 / 200) * pow((temperature - 50), 2));
	double oxygen = exp((-1 / 10000) * pow((o2 - 300), 2));
	if (o2 <= 0) {
		fertility = 0;
	}
	else {
		fertility = 1 + temper + oxygen;
	}
}

//other
void Animal::reproduce(Organism *O) {
		double theta = fRand(0, 2 * 3.14159265);

		double x = round((l.getX() + spawn_distance * cos(theta)) * 10000) / 10000;
		double y = round((l.getY() + spawn_distance * sin(theta)) * 10000) / 10000;

		O = new Animal(x,y);	
}
void Animal::reproduce(Animal *A, double x_max, double y_max) {
	double theta, x, y;
	do {
		theta = fRand(0, 2 * 3.14159265);
		x = round((l.getX() + spawn_distance * cos(theta))*10000)/10000;
		y = round((l.getY() + spawn_distance * sin(theta))*10000)/10000;
	} while ((x > x_max) || (x < -(x_max)) || (y > y_max) || (y < -(y_max)));
	A->setLocation(x, y);
}
void Animal::aged() {
	age++;
	movement = movement - (age / 20); //decereases movement the older the cell is, although this is scaled by the constant 10. Accumulates
	spawn_distance = 4 - (age / 100);//decreases spawn distance as the cell gets older, scaled by the constant 100. Acculmulates
}
Animal Animal::operator+(Plant *p) {
	Point newLocation = p->getLocation();
	this->setLocation(newLocation);
	dec_con_food_counter();
//	delete p;
	return *this;
}
