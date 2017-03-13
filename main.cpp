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

// TESTING SEGMENT MAPPER
void example(const MapLoader& ml)
{
	SegmentMapper sm;
	sm.init(ml);

	GeoCoord lookMeUp("34.0572000", "-118.4417620");

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
	}
}
int main()
{
	MapLoader ml;
	if (ml.load("mapdata.txt"))
		cerr << "Map loaded successfully" << endl;
	else
		cerr << "Map failed to load" << endl;

	example(ml);
}

