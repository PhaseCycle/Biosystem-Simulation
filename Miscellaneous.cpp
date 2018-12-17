/*
Authors: Andrew Yoder, Thomas Stryjski, Zachary O'Brien
Email: aby7159@rit.edu, tgs9181@rit.edu, zjo5244@rit.edu
Date: 12/8/18
Class: EEEE-346-01
Assignment: Project 3

Purpose: Misclaneous Functions
*/

#include "Miscellaneous.h"

double fRand(double fMin, double fMax) {
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

//Credit goes to https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
//for showing how to implement this random number generator