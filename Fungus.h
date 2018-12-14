
/*
Authors: Andrew Yoder, Thomas Stryjski, Zachary O'Brien
Email: aby7159@rit.edu, tgs9181@rit.edu, zjo5244@rit.edu
Date: 12/8/18
Class: EEEE-346-01
Assignment: Project 3

Purpose: derived class for small fungi
*/

#include "MicroOrganism.h"
#pragma once

class Fungus:public MicroOrganism {

	protected:
		double fertility;        
        
    public:
        Fungus(double x, double y, double);
        void set_fertility();
        void reproduce(Organism *O);
        void aged(int AGE);
        double fRand(double fMin, double fMax);    	


};

