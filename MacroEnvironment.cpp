/*
Authors: Andrew Yoder, Thomas Stryjski, Zachary O'Brien
Email: aby7159@rit.edu, tgs9181@rit.edu, zjo5244@rit.edu
Date: 12/4/18
Class: EEEE-346-01
Assignment: Project 3

Purpose: Class for ennvorments on macroscopic scale;
*/
#include "MacroEnvironment.h"
#include "Miscellaneous.h"
#include <cmath>
using namespace std;


//constructors
MacroEnvironment::MacroEnvironment() {
	//Environment();
	run_to_time = 10;
	min_temp = 0;
	max_temp = 100;
	time = 0;

	double amplitude = (max_temp - min_temp) / 2;
	double center = (max_temp + min_temp) / 2;
	Sinusoid a(amplitude, 10.0, 0.0, center);
	temp = a;

	co2 = 10;
	o2 = 10;
	Sinusoid binaryoscillator(0.5, 10, 0, 0.5);
	sunlight = binaryoscillator;

	setBounds(50, 50);
	//	vector<Animal> a_temp(10);
	//	vector<Plant> p_temp(10);
	//	animals = a_temp;
	//	plants = p_temp;
	spawn_animals(10);
	spawn_plants(10);
}
MacroEnvironment::MacroEnvironment(int t, double min_t, double max_t, double x, double y, int num_animals, int num_plants) {
	//Environment(temp, t, max_t, min_t);
	run_to_time = t;
	max_temp = max_t;
	min_temp = min_t;
	time = 0;

	double amplitude = (max_temp - min_temp) / 2;
	double center = (max_temp + min_temp) / 2;
	Sinusoid a(amplitude, 10.0, 0.0, center);
	temp = a;

	co2 = 10;
	o2 = 10;
	Sinusoid binaryoscillator(0.5, 10, 0, 0.5);
	sunlight = binaryoscillator;

	setBounds(x, y);
	//	vector <Animal> a_temp (num_animals);
	//	vector <Plant> p_temp (num_plants);
	//	animals = a_temp;
	//	plants = p_temp;
	spawn_animals(num_animals);
	spawn_plants(num_plants);
}

//deconstructor
MacroEnvironment::~MacroEnvironment()
{
	for (int i = 0; i < animals.size(); i++){
		delete (animals[i]);
	}
	animals.clear();
	
	for (int j = 0; j < plants.size(); j++) {
		delete (plants[j]);
	}
	plants.clear();
}

//getters
int MacroEnvironment::animal_pop() {
	return animals.size();
}
int MacroEnvironment::plant_pop() {
	return plants.size();
}

//setters
void MacroEnvironment::setBounds(double x, double y) {
	if (x > 0)
		x_max = x;
	else if (x < 0)
		x_max = -x;
	else
		x_max = 50;
	////////////////////////////////////
	if (y > 0)
		y_max = y;
	else if (y < 0)
		y_max = -y;
	else
		y_max = 50;
}

//others
void MacroEnvironment::print() {
//	cout << time << "\t" << animal_pop() << "\t" << plant_pop() << endl;
	cout << time << "\t" << animal_pop() << "\t" << plant_pop() << "\t" << o2 << "\t" << co2 << endl;
}
void MacroEnvironment::event() {
	

	o2 += plant_pop() - animal_pop();
	if (o2 < 0)
		o2 = 0;

	co2 += animal_pop() - plant_pop();
	if (co2 < 0)
		co2 = 0;

	set_animal_variables();
	set_plant_variables();

	animal_eat_move();
	animal_die();
	animal_reproduce();
	plant_reproduce();

	animal_age();
	plant_age();
	time++;
}

//Utility functions
//Independent
double MacroEnvironment::fix_x_cord(double x_old) {

	if (x_old > x_max)
		return x_max;
	else if (x_old < -(x_max))
		return -(x_max);
	else
		return x_old;

}
double MacroEnvironment::fix_y_cord(double y_old) {

	if (y_old > y_max)
		return y_max;
	else if (y_old < -(y_max))
		return -(y_max);
	else
		return y_old;
}
void MacroEnvironment::spawn_animals(int num) {
	double x, y;
	for (int i = 0; i < num; i++) {
		x = fRand(-(x_max), x_max);
		y = fRand(-(y_max), y_max);
		animals.push_back(new Animal(x, y));
	}
}
void MacroEnvironment::spawn_plants(int num) {
	double x, y;
	for (int i = 0; i < num; i++) {
		x = fRand(-(x_max), x_max);
		y = fRand(-(y_max), y_max);
		plants.push_back(new Plant(x, y));
	}
}
double MacroEnvironment::get_sunlight(double x, double y)
{
	double s = 1 - pow(x, 2) / pow(x_max, 2) - pow(y, 2) / pow(y_max, 2);
		if (s < 0)
			s = 0;
	return sunlight.func(time) * s;

}

//Summarizing Functions ->  Made to make event() more readable
//Animal Actions
void MacroEnvironment::set_animal_variables() {
	for (unsigned int i = 0; i < animals.size(); i++) {
		animals[i]->set_o2(o2);
		animals[i]->set_co2(co2);

		animals[i]->set_temp(temp.func(time));
	}
}
void MacroEnvironment::animal_eat_move() {
	Plant * closest, * temp;
	double dist_temp, dist_closest;
	int index;

	for (int i = 0; i != animals.size(); i++) {
		dist_closest = *(animals[i]) - *(plants[0]);
		closest = plants[0];
		temp = closest;
		index = 0;
		for (int j = 0; j < plants.size(); j++) {
			dist_temp = *(animals[i]) - *(plants[j]);
			if (dist_temp < dist_closest) {
				dist_closest = dist_temp;
				closest = temp;
				index = j;
			}
		}
		if (dist_closest <= animals[i]->get_movement()) { //closest plant is within movement range
			*(animals[i]) + closest;
			//delete closest; was giving me errors
			plants.erase(plants.begin()+index);			//PLANT DELETED HERE
		}
		else if (dist_closest <= animals[i]->get_visibility()) { // closest plant is within visability range
			double x = animals[i]->getLocation().getX() + animals[i]->get_movement() * animals[i]->unit_x(*closest);
			double y = animals[i]->getLocation().getY() + animals[i]->get_movement() * animals[i]->unit_y(*closest);
			animals[i]->setLocation(x,y);
		}
		else { //plant is not within visibility range (randomly moves around);
			double x, y, theta;

			do {
				theta = fRand(0, 2 * 3.14159265);
				x = animals[i]->getLocation().getX() + animals[i]->get_movement() * cos(theta);
				y = animals[i]->getLocation().getY() + animals[i]->get_movement() * sin(theta);
			} while ((x > x_max) || (x < -(x_max)) || (y > y_max) || (y < -(y_max)));
			animals[i]->setLocation(x, y);
		}
		animals[i]->dec_con_time_counter();
	}

}

//old eat move method

//void MacroEnvironment::animal_eat_move() {
//	vector<Animal*>::iterator i;
//	vector<Plant*>::iterator closest, j;
//	double dist_temp, dist_closest;
////	int index;
//	for (i = animals.begin(); i != animals.end(); i++ ) {
//		dist_closest = *(*(i)) - *(*(plants.begin()));
//		closest = plants.begin();
//		for (j = plants.begin(); j != plants.end(); j++) {
//			dist_temp = *(*(i)) - *(*(j));
//			if (dist_temp < dist_closest) {
//				dist_closest = dist_temp;
//				closest = j;
//			}
//		}
//		if (dist_closest <= (*i)->get_movement()) { //closest plant is within movement range
//			*(*(i)) + *closest;
//			delete *closest;
//			plants.erase(closest);			//PLANT DELETED HERE
//		}
//		else if (dist_closest <= (*i)->get_visibility()) { // closest plant is within visability range
//			double new_x = (*i)->getLocation().getX() + (*i)->get_movement() * (*i)->unit_x(*(*(closest)));
//			double new_y = (*i)->getLocation().getY() + (*i)->get_movement() * (*i)->unit_y(*(*(closest)));
//			(*i)->setLocation(new_x, new_y);
//		}
//		else { //plant is not within visibility range (randomly moves around);
//			double theta = fRand(0, 2 * 3.14159265);
//
//			double new_x = (*i)->getLocation().getX() + (*i)->get_movement() * cos(theta);
//			double new_y = (*i)->getLocation().getY() + (*i)->get_movement() * sin(theta);
//
//			(*i)->setLocation(new_x, new_y);
//		}
//		(*i)->dec_con_time_counter();
//	}
//}
void MacroEnvironment::animal_die() {
//	vector<Animal*>::iterator it;

	for (unsigned int i = 0; i < animals.size();) { //techincally a proper declaration, see below
		if (animals[i]->get_con_food_counter() == 0) {
			animals[i]->set_con_food_counter(animals[i]->get_con_amount());
			animals[i]->set_con_time_counter(animals[i]->get_con_time());
			i++; //manual incrementation
		}
		else if (animals[i]->get_con_time_counter() == 0) {
			delete animals[i];
			animals.erase(animals.begin() + i);
			//when animal is deleted, every element is shifted to fill the gap, so we want to reasses the same index
		}
		else{
			i++;
		}
	}
}
void MacroEnvironment::animal_reproduce() {
	int an = animals.size();
	for (int i = 0; i < an; i++) {
		if (animals[i]->get_rep_counter() == 0) {
			animals[i]->set_fertility();
			for (int j = 0; j < int(animals[i]->get_rep_amount() * animals[i]->get_fertility()); j++) {
				Animal *a = new Animal(0, 0);
				animals[i]->reproduce(a,x_max,y_max);

				animals.push_back(a);
			}
		}
		animals[i]->dec_rep_counter();
	}
}
void MacroEnvironment::animal_age(){

	for (unsigned int i = 0; i < animals.size(); i++) {
		animals[i]->aged();
	}

}

//Plant Actions
void MacroEnvironment::set_plant_variables() {
	for (unsigned int i = 0; i < plants.size(); i++) {
		plants[i]->set_co2(co2);
		plants[i]->set_o2(o2);
		double x = plants[i]->getLocation().getX();
		double y = plants[i]->getLocation().getY();
		double sun = get_sunlight(x, y);
		plants[i]->set_sunlight(sun);

		plants[i]->set_temp(temp.func(time));

	}
}
void MacroEnvironment::plant_reproduce() {
	int pl = plants.size();
	int fer;
	if (pl > 0){
		for (int i = 0; i < pl; i++) {
			if (plants[i]->get_rep_counter() == 0) {
				fer = int(plants[i]->get_rep_amount() * plants[i]->get_fertility());
				for (int j = 0; j < fer; j++) {
					Plant *p = new Plant(0,0);
					plants[i]->reproduce(p);
					plants.push_back(p);
				}
			}
		}
		plants[i]->dec_rep_counter();
	}
}
void MacroEnvironment::plant_age() {
	for (unsigned int i = 0; i < plants.size(); i++) {
		plants[i]->aged();
	}
}


