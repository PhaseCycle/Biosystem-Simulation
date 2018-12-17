/*
Authors: Andrew Yoder, Thomas Stryjski, Zachary O'Brien
Email: aby7159@rit.edu, tgs9181@rit.edu, zjo5244@rit.edu
Date: 12/4/18
Class: EEEE-346-01
Assignment: Project 3

Purpose: Class for ennvorments on microscopic scale;
*/
#include "MicroEnvironment.h"
#include "Miscellaneous.h"

using namespace std;


//constructors
MicroEnvironment::MicroEnvironment() {
        //Environment(); WE can't use constructors created in abstract classes
        //setting inherited variables manually, since we can't inherit default values
        run_to_time = 10;
        min_temp = 0;
        max_temp = 100;
        time = 0;

        double amplitude = (max_temp - min_temp) / 2;
        double center = (max_temp + min_temp) / 2;
        Sinusoid temporary (amplitude, 10, 0, center);
        temp = temporary;

        Sinusoid binaryoscillator(0.5, 10, 0, 0.5);
        sunlight = binaryoscillator;

        setBounds(50, 50, 50);

        spawn_bacteria(10);
        spawn_fungi(10);
}
MicroEnvironment::MicroEnvironment(int t, double min_t, double max_t, double x, double y, double z, int num_bacteria, int num_fungus) {
        //Environment(temp, t, max_t, min_t);
        //setting inherited variables manually, since we can't inherit default values
        run_to_time = t;
        min_temp = min_t;
        max_temp = max_t;
        time = 0;

        double amplitude = (max_temp - min_temp) / 2;
        double center = (max_temp + min_temp) / 2;
        Sinusoid temporary(amplitude, 10, 0, center);
        temp = temporary;

        Sinusoid binaryoscillator(0.5, 10, 0, 0.5);
        sunlight = binaryoscillator;

        setBounds(x, y, z);

        spawn_bacteria(num_bacteria);
        spawn_fungi(num_fungus);
}

//deconstructors
MicroEnvironment::~MicroEnvironment()
{
	for (int i = 0; i < fungi.size(); i++) {
		delete (fungi[i]);
	}
	fungi.clear();

	for (int j = 0; j < bacteria.size(); j++) {
		delete (bacteria[j]);
	}
	bacteria.clear();
}

//getters
int MicroEnvironment::bacteria_pop() {
        return bacteria.size();
}
int MicroEnvironment::fungus_pop() {
        return fungi.size();
}
double MicroEnvironment::get_sunlight(double x, double y, double z) {
        double s = 1 - ((z_max - z) / (z_max + z_max));//spatial function
        return sunlight.func(time) * s; //sunlight is a function of time and depth, with zMax plane having full sunlight and decreasing linearly to 0
        //In reality light penetrates water non-linearly, but since the change in depth is so small at a microscopic level, we can approximate linearly here, and
        //exaggerate the change in light in water depth by assuming other impurities are in the water, so that we can play around with the dynamic between
        //the bacteria feeding off both sunlight and chemicals (think of this bacteria as a chemotroph and phototroph)
}
double MicroEnvironment::get_chemical(double x, double y, double z) {
        double a = -z / (pow(x, 2) + pow(y, 2));// Derived from special case parabolic ellipsoid (meant to represent a vent or plume of some kind) z = a*x^2 + b*x^2
        //the sharper the parabola to the z-axis, the greater the a-constant
        if (a < 0) {
                a = 0; //see formula below. This was added so that no reverse-but-equal-in-magnitude parabolic ellipsoid concentration would exist for z > 0
        }
        double chemicalConcentration = (1 / (-1 * (a + 1))) + 1;//this equation scales what would virtually be an infinite number toward the z-axis to a value between 0 and 1
        //think of how PH is a scaled measurement of what would normally be huge values representing concentration. A log graph was not used in this case because
        // a log graph has no horizontal asymptote to map infinite concentration to.
        return chemicalConcentration;
}

//setters
void MicroEnvironment::setBounds(double x, double y, double z) {
        if (x > 0)
                x_max = x;
        else if (x < 0)
                x_max = -x;
        else
                x_max = 50;

        if (y > 0)
                y_max = y;
        else if (y < 0)
                y_max = -y;
        else
                y_max = 50;

        if (z > 0)
                z_max = z;
        else if (z < 0)
                z_max = -z;
        else
                z_max = 50;
}

//others
void MicroEnvironment::event() {
        bacteria_move();
        fungus_eat_move();
        fungus_die();

		set_bacteria_variables();
		set_fungus_variables();
		bacteria_reproduce();
        fungus_reproduce();

		bacteria_age();
		fungus_age();
		time++;
}
void MicroEnvironment::print() {
        cout << time << "\t" << fungus_pop() << "\t" << bacteria_pop() << endl;
}

//Utility functions
//Independent
double MicroEnvironment::fix_x_cord(double x_old) {

	if (x_old > x_max)
		return x_max;
	else if (x_old < -(x_max))
		return -(x_max);
	else
		return x_old;

}
double MicroEnvironment::fix_y_cord(double y_old) {

	if (y_old > y_max)
		return y_max;
	else if (y_old < -(y_max))
		return -(y_max);
	else
		return y_old;

}
double MicroEnvironment::fix_z_cord(double z_old) {

	if (z_old > z_max)
		return z_max;
	else if (z_old < -(z_max))
		return -(z_max);
	else
		return z_old;

}
bool MicroEnvironment::within_bounds(MicroOrganism &O) {
        double x_cord = O.getLocation().getX();
        double y_cord = O.getLocation().getY();
        double z_cord = O.getLocation().getZ();
        if ((x_cord > x_max) || (x_cord < -(x_max)))
                return false;
        if ((y_cord > y_max) || (y_cord < -(y_max)))
                return false;
        if ((z_cord > z_max) || (z_cord < -(z_max)))
                return false;
        return true;

}
void MicroEnvironment::spawn_bacteria(int num) {
        double x, y, z;
        for (int i = 0; i < num; i++) {
                x = fRand(-(x_max), x_max);
                y = fRand(-(y_max), y_max);
                z = fRand(-(z_max), z_max);
                bacteria.push_back(new Bacteria(x, y, z));
        }
}
void MicroEnvironment::spawn_fungi(int num) {
        double x, y, z;
        for (int i = 0; i < num; i++) {
                x = fRand(-(x_max), x_max);
                y = fRand(-(y_max), y_max);
                z = fRand(-(z_max), z_max);
                fungi.push_back(new Fungus(x, y, z));
        }
}

//Summarizing Functions ->  Made to make event() more readable
//Fungus Actions

void MicroEnvironment::fungus_eat_move() {
        Bacteria *closest, *temp;
        double dist_temp, dist_closest;
        int index;
        for (unsigned int i = 0; i < fungi.size(); i++) {
				if (fungi.empty()) {
					break;
				}
                dist_closest = *(fungi[i]) - *(bacteria[0]);
                closest = bacteria[0];
                temp = closest;
                index = 0;
                for (unsigned int j = 0; j < bacteria.size(); j++) {
                        temp = bacteria[j];
                        dist_temp = *(fungi[i]) - *temp;
                        if (dist_temp < dist_closest) {
                                dist_closest = dist_temp;
                                closest = temp;
                                index = j;
                        }
                }
                if (dist_closest <= fungi[i]->get_movement()) { //closest plant is within movement range
                        *fungi[i] + temp;
                        bacteria.erase(bacteria.begin() + index);              //PLANT DELETED HERE
                }
                else if (dist_closest <= fungi[i]->get_visibility()) { // closest plant is within visability range
                        double new_x = fungi[i]->getLocation().getX() + fungi[i]->get_movement() * fungi[i]->unit_x(*temp);
                        double new_y = fungi[i]->getLocation().getY() + fungi[i]->get_movement() * fungi[i]->unit_y(*temp);
                        double new_z = fungi[i]->getLocation().getZ() + fungi[i]->get_movement() * fungi[i]->unit_z(*temp);
                        fungi[i]->setLocation(new_x, new_y, new_z);
                }
                else { //plant is not within visibility range (randomly moves around);
					double theta, phi, new_x, new_y, new_z;
					do {
						theta = fRand(0, 2 * 3.14159265);
						phi = fRand(0, 2 * 3.14159265);

						new_x = fungi[i]->getLocation().getX() + fungi[i]->get_movement() * sin(phi) * cos(theta);
						new_y = fungi[i]->getLocation().getY() + fungi[i]->get_movement() * sin(phi) * sin(theta);
						new_z = fungi[i]->getLocation().getZ() + fungi[i]->get_movement() * cos(phi);
					} while ((new_x > x_max) || (new_x < -(x_max)) || (new_y > y_max) || (new_y < -(y_max)) || (new_z > z_max) || (new_z < -(z_max)));
                        fungi[i]->setLocation(new_x, new_y, new_z);
                }
        }
		for (unsigned int i = 0; i < fungi.size(); i++) {
			fungi[i]->dec_con_time_counter();
		}
}
void MicroEnvironment::fungus_die() {
        for (unsigned int i = 0; i < fungi.size();) { //techincally a proper declaration, see below
                if (fungi[i]->get_con_food_counter() == 0) {
                        fungi[i]->set_con_food_counter(fungi[i]->get_con_amount());
                        fungi[i]->set_con_time_counter(fungi[i]->get_con_time());
                        i++; //manual incrementation
                }
                else if (fungi[i]->get_con_time_counter() == 0) {
						
                        fungi.erase(fungi.begin() + i);
                        //when animal is deleted, every element is shifted to fill the gap, so we want to reasses the same index
                }
				else {
					i++; //if it's not time for a fungus to starve, we need to move on to check the next fungus
				}
        }
}
void MicroEnvironment::set_fungus_variables() {
	for (unsigned int i = 0; i < fungi.size(); i++) {
		fungi[i]->set_temp(temp.func(time));
	}
}
void MicroEnvironment::fungus_reproduce() {
	int fu = fungi.size();
    for (int i = 0; i < fu; i++) {
		if (fungi[i]->get_rep_counter() == 0) {
			int constant_rep_amount = int(fungi[i]->get_rep_amount() * fungi[i]->get_fertility());
				for (int j = 0; j < constant_rep_amount; j++) {
								Fungus *f = new Fungus(0, 0, 0);
                                fungi[i]->reproduce(f, x_max, y_max, z_max);
                                fungi.push_back(f);
                }
		}
                fungi[i]->dec_rep_counter();
	}
}
void MicroEnvironment::fungus_age() {
	for (unsigned int i = 0; i < fungi.size(); i++) { //for loop is unsigned because vector index are undsigned
		fungi[i]->aged();
	}
}

//Bacteria Actions
void MicroEnvironment::bacteria_move() {
        for (int i = 0; i < bacteria.size(); i++) {
                //simplify notation
                double locX = bacteria[i]->getLocation().getX();
                double locY = bacteria[i]->getLocation().getY();
                double locZ = bacteria[i]->getLocation().getZ();
                double vis = bacteria[i]->get_visibility();
				
				double x_pos = fix_x_cord(locX + vis);
				double x_neg = fix_x_cord(locX - vis);
				double y_pos = fix_y_cord(locY + vis);
				double y_neg = fix_y_cord(locY - vis);
				double z_pos = fix_z_cord(locZ + vis);
				double z_neg = fix_z_cord(locZ - vis);

                //sample sunlight at cardinal directions
                double s_at_self = get_sunlight(locX, locY, locZ);
                double s_at_xpos = get_sunlight(x_pos, locY, locZ);
                double s_at_xneg = get_sunlight(x_neg, locY, locZ);
                double s_at_ypos = get_sunlight(locX, y_pos, locZ);
                double s_at_yneg = get_sunlight(locX, y_neg, locZ);
                double s_at_zpos = get_sunlight(locX, locY, z_pos);
                double s_at_zneg = get_sunlight(locX, locY, z_neg);
                double s_x_choose;
                double s_y_choose;
                double s_z_choose;
                double s_at_choose;
                double s_fertility;

                //sample chemical at cardinal directions
                double c_at_self = get_chemical(locX, locY, locZ);
                double c_at_xpos = get_chemical(x_pos, locY, locZ);
                double c_at_xneg = get_chemical(x_neg, locY, locZ);
                double c_at_ypos = get_chemical(locX, y_pos, locZ);
                double c_at_yneg = get_chemical(locX, y_neg, locZ);
                double c_at_zpos = get_chemical(locX, locY, z_pos);
                double c_at_zneg = get_chemical(locX, locY, z_neg);
                double c_x_choose;
                double c_y_choose;
                double c_z_choose;
                double c_at_choose;
                double c_fertility;

                int ifsimp = 1; //only added this so i could shrink the choosing protocol
                //choosing protocol for sunshine:
                if (ifsimp == 1) {//chosing stuff for sunlight
                        //choosing x
                        if (s_at_xpos != s_at_xneg) {
                                if (s_at_xpos > s_at_xneg) {
                                        s_x_choose = x_pos;
                                }
                                else {
                                        s_x_choose = x_neg;
                                }
                        }
                        else {
                                s_x_choose = locX;
                        }

                        //choosing y
                        if (s_at_ypos != s_at_yneg) {
                                if (s_at_ypos > s_at_yneg) {
                                        s_y_choose = y_pos;
                                }
                                else {
                                        s_y_choose = y_neg;
                                }
                        }
                        else {
                                s_y_choose = locY;
                        }

                        //choosing z
                        if (s_at_zpos != s_at_zneg) {
                                if (s_at_zpos > s_at_zneg) {
                                        s_z_choose = z_pos;
                                }
                                else {
                                        s_z_choose = z_neg;
                                }
                        }
                        else {
                                s_z_choose = locZ;
                        }
                }
                //same protocol for chemical:
                if (ifsimp == 1) {
                        if (c_at_xpos != c_at_xneg) {
                                if (c_at_xpos > c_at_xneg) {
                                        c_x_choose = x_pos;
                                }
                                else {
                                        c_x_choose = x_neg;
                                }
                        }
                        else {
                                c_x_choose = locX;
                        }

                        //choosing y
                        if (c_at_ypos != c_at_yneg) {
                                if (c_at_ypos > c_at_yneg) {
                                        c_y_choose = y_pos;
                                }
                                else {
                                        c_y_choose = y_neg;
                                }
                        }
                        else {
                                c_y_choose = locY;
                        }

                        //choosing z
                        if (c_at_zpos != c_at_zneg) {
                                if (c_at_zpos > c_at_zneg) {
                                        c_z_choose = z_pos;
                                }
                                else {
                                        c_z_choose = z_neg;
                                }
                        }
                        else {
                                c_z_choose = locZ;
                        }
                }

                if (c_x_choose != s_x_choose || c_y_choose != s_y_choose || c_z_choose != s_z_choose) {
                        //updating parameters for sunshine recommeded destination and chemical recommened destination fertility calculations
                        //because using the method set_fertility
                        s_at_choose = get_sunlight(s_x_choose, s_y_choose, s_z_choose);
                        c_at_choose = get_chemical(s_x_choose, s_y_choose, s_z_choose);
                        bacteria[i]->set_sunlight(s_at_choose);
                        bacteria[i]->set_chemical(c_at_choose);
                        bacteria[i]->set_fertility();
                        s_fertility = bacteria[i]->get_fertility();

                        s_at_choose = get_sunlight(c_x_choose, c_y_choose, c_z_choose);
                        c_at_choose = get_chemical(c_x_choose, c_y_choose, c_z_choose);
                        bacteria[i]->set_sunlight(s_at_choose);
                        bacteria[i]->set_chemical(c_at_choose);
                        bacteria[i]->set_fertility();
                        c_fertility = bacteria[i]->get_fertility();
                        if (s_fertility > c_fertility) {
                                bacteria[i]->setLocation(s_x_choose, s_y_choose, s_z_choose);
                        }
                        else if (s_fertility < c_fertility) {
                                bacteria[i]->setLocation(c_x_choose, c_y_choose, c_z_choose);
                        }
                        else {
							double theta, phi, new_x, new_y, new_z;
							do {
								theta = fRand(0, 2 * 3.14159265);
								phi = fRand(0, 2 * 3.14159265);

								new_x = locX + bacteria[i]->get_movement() * sin(phi) * cos(theta);
								new_y = locY + bacteria[i]->get_movement() * sin(phi) * sin(theta);
								new_z = locZ + bacteria[i]->get_movement() * cos(phi);

								
							} while ((new_x > x_max) || (new_x < -(x_max)) || (new_y > y_max) || (new_y < -(y_max)) || (new_z > z_max) || (new_z < -(z_max)));
							bacteria[i]->setLocation(new_x, new_y, new_z);
						}
                }
                else { //plant notices no change in chemical in cardinal directions at visibility range (randomly moves around);
						double theta, phi, new_x, new_y, new_z;
						do {
								theta = fRand(0, 2 * 3.14159265);
								phi = fRand(0, 2 * 3.14159265);

								new_x = bacteria[i]->getLocation().getX() + bacteria[i]->get_movement() * sin(phi) * cos(theta);
								new_y = bacteria[i]->getLocation().getY() + bacteria[i]->get_movement() * sin(phi) * sin(theta);
								new_z = bacteria[i]->getLocation().getZ() + bacteria[i]->get_movement() * cos(phi);
						} while ((new_x > x_max) || (new_x < -(x_max)) || (new_y > y_max) || (new_y < -(y_max)) || (new_z > z_max) || (new_z < -(z_max)));
	
						bacteria[i]->setLocation(new_x, new_y, new_z);
				}
        }
}
//currently there is no natural death implemented from lack of sunlight or chemicals.
void MicroEnvironment::set_bacteria_variables() {
	for (unsigned int i = 0; i < bacteria.size(); i++) {
		bacteria[i]->set_temp(temp.func(time));
	}
}
void MicroEnvironment::bacteria_reproduce() {
        int ba = bacteria.size();
		int fer;
		if (ba > 0){
			for (int i = 0; i < ba; i++) {
				bacteria[i]->set_fertility();
				if (bacteria[i]->get_rep_counter() == 0) {
					fer = int(bacteria[i]->get_rep_amount() * bacteria[i]->get_fertility());
					for (int j = 0; j < fer; j++) {
						Bacteria *b = new Bacteria(0, 0, 0);
						bacteria[i]->reproduce(b, x_max, y_max, z_max);
						bacteria.push_back(b);
					}
				}
				bacteria[i]->dec_rep_counter();
			}
        }
}
void MicroEnvironment::bacteria_age() {
	for (unsigned int i = 0; i < bacteria.size(); i++) { //for loop is unsigned because vector index are undsigned
		bacteria[i]->aged();
	}
}

