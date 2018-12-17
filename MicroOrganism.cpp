/*
Authors: Andrew Yoder, Thomas Stryjski, Zachary O'Brien
Email: aby7159@rit.edu, tgs9181@rit.edu, zjo5244@rit.edu
Date: 12/1/18
Class: EEEE-346-01
Assignment: Project 3

Purpose: derived class for organisms in micro-enviornments


*/

#include "MicroOrganism.h"
#include <cmath>

//getters
Point_3D MicroOrganism::getLocation() {
	return l;
}
double MicroOrganism::get_temp() {
	return temperature;
}

//setters
void MicroOrganism::setLocation(double x_new, double y_new, double z_new){
	l.setCoordinates(x_new, y_new, z_new);
}
void MicroOrganism::setLocation(Point_3D p) {
	l.setCoordinates(p.getX(), p.getY(), p.getZ());
}
void MicroOrganism::set_temp(double t) {
	temperature = t;
}

//Operator Overloading
//(-) and associated methods
double MicroOrganism::operator-(MicroOrganism & O){
	double x, y, z, result;

	x = delta_x(O);
	y = delta_y(O);
	z = delta_z(O);

	result = sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
	return result;
}
double MicroOrganism::delta_x(MicroOrganism &O) {
	return O.getLocation().getX() - l.getX();
}
double MicroOrganism::delta_y(MicroOrganism &O) {
	return O.getLocation().getY() - l.getY();
}
double MicroOrganism::delta_z(MicroOrganism &O) {
	return O.getLocation().getZ() - l.getZ();
}
double MicroOrganism::unit_x(MicroOrganism &O) {
	return delta_x(O) / (*this - O);
}
double MicroOrganism::unit_y(MicroOrganism &O) {
	return delta_y(O) / (*this - O);
}
double MicroOrganism::unit_z(MicroOrganism &O) {
	return delta_z(O) / (*this - O);
}