/*
Authors: Andrew Yoder, Thomas Stryjski, Zachary O'Brien
Email: aby7159@rit.edu, tgs9181@rit.edu, zjo5244@rit.edu
Date: 12/1/18
Class: EEEE-346-01
Assignment: Project 3

Purpose: derived class for organisms in micro-enviornments

*/
#pragma once

#include "Organism.h"
#include "Point_3D.h"

class MicroOrganism :public Organism {
	protected:
		Point_3D l;
		double temperature;

	public:
		//getters
		Point_3D getLocation();
		double get_temp();

		//setters
		void setLocation(double x_new, double y_new, double z_new);
		void setLocation(Point_3D p);
		void set_temp(double t);

		//Operator Overloading
		//(-) and associated methods
		double operator-(MicroOrganism &O);
		double delta_x(MicroOrganism &O);
		double delta_y(MicroOrganism &O);
		double delta_z(MicroOrganism &O);
		double unit_x(MicroOrganism &O);
		double unit_y(MicroOrganism &O);
		double unit_z(MicroOrganism &O);

		//other
		//virtual void reproduce(MicroOrganism *O) = 0;
};
