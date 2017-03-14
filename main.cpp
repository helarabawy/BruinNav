// The main.cpp you can use for testing will replace this file soon.

#include "provided.h"
#include "MyMap.h"
#include <iostream>
#include <cassert>
#include <string>
using namespace std;

// TO TEST MyMap.h
/*
void foo()
{

	MyMap<int, string> map;

	// ADDING NEW ITEMS TO MAP

	cerr << "Arti: " << endl; map.associate(1, "Arti");
	cerr << "Matteo: " << endl; map.associate(2, "Matteo");
	cerr << "Dawei: " << endl; map.associate(3, "Dawei");
	cerr << "Akaash: " << endl; map.associate(4, "Akaash");
	cerr << "Mihir: " << endl; map.associate(5, "Mihir");
	cerr << "Erick: " << endl; map.associate(6, "Erik");

	cerr << endl << "SIZE: " << map.size() << endl;

	// TESTING CLEAR
	map.clear();
	assert(map.size() == 0);

	cerr << endl << "------------------------------------------------" << endl;

	cerr << "Arti: " << endl; map.associate(1, "Arti");
	cerr << "Matteo: " << endl; map.associate(2, "Matteo");
	cerr << "Dawei: " << endl; map.associate(3, "Dawei");
	cerr << "Akaash: " << endl; map.associate(4, "Akaash");
	cerr << "Mihir: " << endl; map.associate(5, "Mihir");
	cerr << "Erick: " << endl; map.associate(6, "Erik");

	cerr << endl << "SIZE: " << map.size() << endl;

	// TESTED - CLEAR WORKS
	// TESTED - ASSOCIATING BY ADDING NEW NODES
	// TESTED - SIZE WORKS

	//////////////////////////////////////////////////////////////////

	// TESTING FIND
	string* x;
	x = map.find(1); cerr << "At 1: " << *x << endl;
	x = map.find(2); cerr << "At 2: " << *x << endl;
	x = map.find(3); cerr << "At 3: " << *x << endl;
	x = map.find(4); cerr << "At 4: " << *x << endl;
	x = map.find(5); cerr << "At 5: " << *x << endl;
	x = map.find(6); cerr << "At 6: " << *x << endl;




	MyMap<string,double> nameToGPA;  // maps student name to GPA
	// add new items to the binary search tree-based map
	nameToGPA.associate("Carey", 3.5);  // Carey has a 3.5 GPA
	nameToGPA.associate("David", 3.99); // David beat Carey
	nameToGPA.associate("Zebra", 6.7);
	nameToGPA.associate("Abe", 3.2);


	// Testing added values were correctly initially associated
	double* AbeGPA = nameToGPA.find("Abe");
	double* davidsGPA = nameToGPA.find("David");
	double* careysGPA = nameToGPA.find("Carey");
	double* zGPA = nameToGPA.find("Zebra");

	cerr << *AbeGPA << endl;
	cerr << *davidsGPA << endl;
	cerr << *careysGPA << endl;
	cerr << *zGPA << endl;
	//assert(*davidsGPA == 3.99);
	//assert(*careysGPA == 3.5);


	// TESTING ASSOCIATE BY REPLACING
	nameToGPA.associate("Abe", 3.2);
	assert(*AbeGPA == 3.2);
	nameToGPA.associate("Abe", 3.3);
	assert(*AbeGPA == 3.3);
	nameToGPA.associate("Abe", 3.4);
	assert(*AbeGPA == 3.4);
	nameToGPA.associate("Abe", 3.5);
	assert(*AbeGPA == 3.5);
	nameToGPA.associate("Abe", 3.6);
	assert(*AbeGPA == 3.6);
	nameToGPA.associate("Abe", 3.7);
	assert(*AbeGPA == 3.7);
	nameToGPA.associate("Abe", 5);
	assert(*AbeGPA == 5);

	// Abe has a 3.2 GPA

	cerr << "david's GPA: " << *davidsGPA << endl;
	*davidsGPA = 1.5;
	cerr << "david's new GPA: " << *davidsGPA << endl;

	// after a re-grade of David’s exam
	nameToGPA.associate("Carey", 3.69);
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
	string attraction = "LA Tennis Center";

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

// TESTING SEGMENT MAPPER
/*

void example(const MapLoader& ml)
{
	SegmentMapper sm;

	cerr << "Will Init" << endl;
	sm.init(ml);
	cerr << "Initted" << endl;

	GeoCoord lookMeUp("34.0572000", "-118.4417620");

	cerr << "Searching for segments" << endl;
	std::vector<StreetSegment> vecOfAssociatedSegs(sm.getSegments(lookMeUp));

	if (vecOfAssociatedSegs.empty())
	{
		cout << "Error - no segments found matching this coordinate\n";
		return;
	}

	cout << "Here are all the segments associated with your coordinate: " << endl;

	for (auto s: vecOfAssociatedSegs)
	{
		cout << "Segment's street: " << s.streetName << endl;
		cout << "Segment's start lat/long:" << s.segment.start.latitude << ", "
				<< s.segment.start.longitude << endl;
		cout << "Segment's end lat/long:" << s.segment.end.latitude << ", "
						<< s.segment.end.longitude << endl;
		cout << "This segment has " << s.attractions.size() << " attraction on it." <<  endl;
		cout << "------------------------------------------------------" << endl;
	}
}
*/




int main()
{
	MapLoader ml;
	if (ml.load("mapdata.txt"))
		cerr << "Map loaded successfully" << endl;
	else
		cerr << "Map failed to load" << endl;

//	example(ml);


//	foo();
}

