
/*
Authors: Andrew Yoder, Thomas Stryjski, Zachary O'Brien
Email: aby7159@rit.edu, tgs9181@rit.edu, zjo5244@rit.edu
Date: 12/8/18
Class: EEEE-346-01
Assignment: Project 3

Purpose: derived class for small fungi
*/

#include "MicroOrganism.h"
#include "Bacteria.h"
#pragma once

class Fungus:public MicroOrganism {

	protected:
		double fertility;        
        
    public:
		//constructors
        Fungus(double x, double y, double);

        //getters
		double get_fertility();

		//setters
		void set_fertility();
        
		//others
		void reproduce(Fungus *O, double x_max, double y_max, double z_max);
        void aged();
		Fungus operator+(Bacteria *b);


};

