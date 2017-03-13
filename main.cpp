// The main.cpp you can use for testing will replace this file soon.

#include "provided.h"
#include "MyMap.h"
#include <iostream>
#include <string>
using namespace std;

// TO TEST MyMap.h
/*
void foo()
{
	MyMap<string,double> nameToGPA;  // maps student name to GPA
	// add new items to the binary search tree-based map
	nameToGPA.associate("Carey", 3.5);  // Carey has a 3.5 GPA
	nameToGPA.associate("David", 3.99); // David beat Carey
	nameToGPA.associate("Abe", 3.2);

	// Abe has a 3.2 GPA
	double* davidsGPA = nameToGPA.find("David");
	if (davidsGPA != nullptr)
	{
		cerr << "david's GPA: " << *davidsGPA << endl;
		*davidsGPA = 1.5;
		cerr << "david's new GPA: " << *davidsGPA << endl;
	}
	// after a re-grade of David’s exam
	nameToGPA.associate("Carey", 4.0);
	// Carey deserves a 4.0
	// replaces old 3.5 GPA


	cerr << "david's new GPA: " << *davidsGPA << endl;


	double* lindasGPA = nameToGPA.find("Linda");
	if (lindasGPA == nullptr)
		cout << "Linda is not in the roster!" << endl;
	else
		cout << "Linda’s GPA is: " << *lindasGPA << endl;
}

*/

// TESTING ATTRACTION MAPPER
/*
void example(const MapLoader& ml)
{
	AttractionMapper am;
	am.init(ml);

	GeoCoord fillMe;
	string attraction = "The Beverly Hills Litigation Group";

	bool found = am.getGeoCoord(attraction, fillMe);
	if (!found)
	{
		cout << "No geolocation found for " << attraction << endl;
		return;
	}

	cout << "The location of " << attraction << " is " <<
			fillMe.latitude << ", " << fillMe.longitude << endl;
}
*/

int main()
{
	MapLoader test;

	if (test.load("mapdata.txt"))
		cerr << "loaded successfully" << endl;
	else
		cerr << "failed to load" << endl;

	example(test);
			//foo();
}

