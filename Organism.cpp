/* 
Authors: Andrew Yoder, Thomas Stryjski, Zachary O'Brien
Email: aby7159@rit.edu, tgs9181@rit.edu, zjo5244@rit.edu
Date: 12/1/18
Class: EEEE-346-01
Assignment: Project 3

Purpose: Executes functions declared in Organism.h
*/

#include "Organism.h"

//getters
int Organism::get_con_amount() {
	return consumption_amount;
}
int Organism::get_con_time() {
	return consumption_time;
}
int Organism::get_rep_amount() {
	return reproduction_amount;
}
int Organism::get_rep_time() {
	return reproduction_time;
}
int Organism::get_age() {
	return age;
}
int Organism::get_rep_counter() {
	return reproduction_counter;
}
int Organism::get_strength() {
	return strength;
}
double Organism::get_movement() {
	return movement;
}
double Organism::get_spawn_distance() {
	return spawn_distance;
}
double Organism::get_visibility() {
	return visibility;
}

//setters
void Organism::set_con_amount(int x) {
	consumption_amount = x;
}
void Organism::set_con_time(int x) {
	consumption_time = x;
}
void Organism::set_rep_amount(int x) {
	reproduction_amount = x;
}
void Organism::set_rep_time(int x) {
	reproduction_time = x;
}
void Organism::set_age(int x) {
	age = x;
}
void Organism::set_rep_counter(int x) {
	reproduction_counter = x;
}
void Organism::set_strenth(int x) {
	strength = x;
}
void Organism::set_movement(double x) {
	movement = x;
}
void Organism::set_spawn_distance(double x) {
	spawn_distance = x;
}
void Organism::set_visibility(double x) {
	visibility = x;
}


void Organism::dec_rep_counter() {
	if (reproduction_counter == 0)
		reproduction_counter = reproduction_time;
	else
		reproduction_counter--;
}
void Organism::deplete_strength() {
	strength--;
}