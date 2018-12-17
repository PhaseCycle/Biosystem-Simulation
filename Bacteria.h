/*
Authors: Andrew Yoder, Thomas Stryjski, Zachary O'Brien
Email: aby7159@rit.edu, tgs9181@rit.edu, zjo5244@rit.edu
Date: 12/1/18
Class: EEEE-346-01
Assignment: Project 3

Purpose: derived class for microorganisms; chemotroph/phototroph
*/
#include "MicroOrganism.h"
#include <cstdlib>
#pragma once

class Bacteria :public MicroOrganism {

	protected: 
		double sunlight;
		double chemical;
		double fertility;
		
	public:
		//constructors
		Bacteria(double x, double y, double z);

		//getters
		
		double get_fertility();

		//setters
		void set_sunlight(double x);
		void set_chemical(double x);
		void set_fertility(); //determines reproduction values as a function of temperature and sunlight

		//other
		void reproduce(Bacteria *O, double x_max, double y_max, double z_max);
		void aged();

	

};
