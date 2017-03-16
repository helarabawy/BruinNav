#ifndef SUPPORT_H
#define SUPPORT_H

#include "provided.h"
#include <string>
#include <ctype.h>
#include <algorithm>

using namespace std;

struct NavPoint {
	GeoCoord *thisPoint;
	double distance;
	int priority;
	double angle;
	std::string streetName = "";
	std::string capitalizedStreet = "";
	NavPoint *previousPoint;
	bool process = false;
	const int INFINITE = 9999999;

	NavPoint(const SegmentMapper& sm, GeoCoord& loc, NavPoint* previous = nullptr) {
		//create a geolocation
		thisPoint = new GeoCoord;
		thisPoint->latitude = loc.latitude;
		thisPoint->longitude = loc.longitude;

		//set previous points
		previousPoint = previous;

		//sets distance
		calculateDistance();

		//set priority
		priority = INFINITE;


		setStreetName(sm);
	}

	void setStreetName(const SegmentMapper& sm) {
		if (previousPoint == nullptr) {
			std::vector<StreetSegment> temp = sm.getSegments(*thisPoint);
			if (!temp.empty()) {
				streetName = temp.at(0).streetName;
				capitalizedStreet = streetName;

				transform(streetName.begin(), streetName.end(), streetName.begin(), ::tolower);

			}
		}
		else {
			std::vector<StreetSegment> temp = sm.getSegments(*thisPoint);

			for (StreetSegment ssg : temp) {

				capitalizedStreet = ssg.streetName;

				transform(streetName.begin(), streetName.end(), streetName.begin(), ::tolower);

				GeoSegment gs;
				gs.start = *(previousPoint->thisPoint);
				gs.end = *thisPoint;
				angle = angleOfLine(gs);
			}
		}
	}

	void calculateDistance() {
		if (previousPoint == nullptr) {
			distance = 0;
		}
		else {
			distance = distanceEarthMiles(*thisPoint, *(previousPoint->thisPoint));
		}
	}

	void calculatePriority(const GeoCoord& end, NavPoint* np) {

		int f, g = distanceEarthMiles(*thisPoint, end);
		if (previousPoint == nullptr) {
			f = 0;
		}
		else {
			f = np->priority;
		}


		if (g + distance < priority) {
			priority = g + distance;
			previousPoint = np;
		}
	}

	~NavPoint() {
	}
};

inline
bool operator<(const GeoCoord& a, const GeoCoord& b) // overload < GeoCoord
{
	if (a.latitude < b.latitude) // decide on lat first
	{
		return true; // return true
	}
	else if (a.latitude == b.latitude) // if the lats are equal
	{
		if (a.longitude < b.longitude) // if a's long is less
		{
			return true; // yep!
		}
		else // otherwise
		{
			return false; // nope!
		}
	}
	else // if they are oppose
	{
		return false; // return false
	}
}

inline
bool operator==(const GeoSegment& a, const GeoSegment& b)
{
	//returns true if both GeoCoord's have the same latitude and longitude
	return ((a.start.latitude == b.start.latitude) && (a.start.longitude == b.start.longitude)) || ((a.end.latitude == b.end.latitude) && (a.end.longitude == b.end.longitude));
}

inline
bool operator==(const GeoCoord& a, const GeoCoord& b) // overload for == GeoCoord
{
	return (a.latitude == b.latitude && a.longitude == b.longitude); // return if lats and longs are ==
}

inline
bool operator== (const StreetSegment& a, const StreetSegment& b) // operator overload for StreetSegment ==
{
	return a.segment.start == b.segment.start && a.segment.end == b.segment.end; // return if starts and ends are ==
}

inline
std::string directionOfLine(double angle) // get the directino of the line!
{
	std::string direction;
	if (0 <= angle && angle <= 22.5) // angle between
	{
		direction = "east"; // return east
	}
	if (22.5 < angle && angle <= 67.5) // angle between
	{
		direction = "northeast"; // return value
	}
	if (67.5 < angle && angle <= 112.5) // angle between
	{
		direction = "north"; // return value
	}
	if (112.5 < angle && angle <= 157.5) // angle between
	{
		direction = "northwest"; // return value
	}
	if (157.5 < angle && angle <= 202.5) // angle between
	{
		direction = "west"; // return value
	}
	if (202.5 < angle && angle <= 247.5) // get value
	{
		direction = "southwest"; // return value
	}
	if (247.5 < angle && angle <= 292.5) // if value between
	{
		direction = "south"; // return this
	}
	if (292.5 < angle && angle <= 337.5) // if angle between these
	{
		direction = "southeast"; // return this value
	}
	if (337.5 < angle && angle < 360) // angle between
	{
		direction = "east"; // return value
	}
	return direction; // by default return dir
}

inline
string transform(string name)
{
	string modified = "";
	// first character of string
	if (isalpha(name[0]))
		modified += toupper(name[0]);
	else
		modified += name[0];

	// rest of name
	for (int i = 1; i < name.size(); i++)
	{
		if (isalpha(name[i]))
		{
			if (name[i - 1] == ' ' || name[i - 1] == '-' || name[i - 1] == ';')
			{
				modified += toupper(name[i]); // beginning of a word
			} else
			{
				modified += tolower(name[i]); // middle of word
			}
		} else
			modified += tolower(name[i]);
	}
	return modified;
}

inline
int findDelimitingSpace(std::string s) {

		for (int i = 1, len = s.length(); i < len; i++) {
			if (s[i] == ' ') {
				if (s[i - 1] != ' ') {
					return i;
				}
			}
		}
		return -1;
}

inline
void removeSpaces(std::string s)
{
	string mod = "";
	for (int i = 0; i < s.size(); i++)
	{
		if (s.at(i) != ' ')
		{
			mod += s.at(i);
		}
	}
	mod = s;
}


class Compare
{
public:
	bool operator() (NavPoint* a, NavPoint* b)
	{
		return a->priority > b->priority;
	}
};



#endif
