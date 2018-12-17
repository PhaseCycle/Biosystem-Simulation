/*
Authors: Andrew Yoder, Thomas Stryjski, Zachary O'Brien
Email: aby7159@rit.edu, tgs9181@rit.edu, zjo5244@rit.edu
Date: 12/8/18
Class: EEEE-346-01
Assignment: Project 3

Purpose: Executes functions declared in Plant.h
*/

#include "Plant.h"
#include "Miscellaneous.h"

//constructors
Plant::Plant(double x, double y) {
	consumption_amount = 0;
	consumption_time = 0;
	reproduction_amount = 2;
	reproduction_time = 2;
	reproduction_counter = reproduction_time;
	consumption_food_counter = consumption_amount;
	consumption_time_counter = consumption_time;
	age = 0;
	movement = 0;
	spawn_distance = 3;
	visibility = 1;

	//temp initialization
	fertility = 1;
	co2 = 7;
	o2 = 7;
	sunlight = 1;


	setLocation(x, y);
}

//getters
double Plant::get_o2() {
	return o2;
}
double Plant::get_fertility() {
	return fertility;
}

//setters
void Plant::set_co2(double x) {
	co2 = x;
}
void Plant::set_o2(double x){
	o2 = x;
}
void Plant::set_fertility() {
	//after having tried upside-down parabolas to unsucessfully model organisms having optimal fertility, we discovered the amazing gaussian curve!
	//format , in this case: e^(-1/a * (x-d)^(2)), where a is the horizontal strech factor of the curve and d is the desired/optimal value where the function = 1
	double temper = exp((-1/200) * pow((temperature - 75), 2));
	double carbondioxide = exp((-1 / 10000) * pow((co2 - 300), 2));
	if (co2 <= 0) {
		fertility = 0;
	}
	else {
		fertility = 1 + temper + sunlight + carbondioxide;
	}
}
void Plant::set_sunlight(double x) {
	sunlight = x;
}


//other

void Plant::reproduce(Plant *P,double x_max, double y_max) {
	for (int i = 0; i <= int(reproduction_amount * fertility); i++) {
		double theta, x, y;
		do {
			theta = fRand(0, 2 * 3.14159265);
			x = round((l.getX() + spawn_distance * cos(theta))*10000)/10000;
			y = round((l.getY() + spawn_distance * sin(theta))*10000)/10000;
		} while ((x > x_max) || (x < -(x_max)) || (y > y_max) || (y < -(y_max)));
		P->setLocation(x, y);
	}
}
void Plant::reproduce(Organism *O) {
	for (int i = 0; i <= int(reproduction_amount * fertility); i++) {
		double theta = fRand(0, 2 * 3.14159265);

		double x = round((l.getX() + spawn_distance * cos(theta)) * 10000) / 10000;
		double y = round((l.getX() + spawn_distance * sin(theta)) * 10000) / 10000;

		O = new Plant(x, y);
	}
}
void Plant::aged() {
	age++;
	spawn_distance = 4 + (age / 100);//increases spawn distance as the cell gets older, scaled by the constant 100. Acculmulates
}