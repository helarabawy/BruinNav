#include "provided.h"
#include "support.h"
#include <string>
#include <cctype>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class MapLoaderImpl
{
	public:
		MapLoaderImpl(){}
		~MapLoaderImpl(){}
		bool load(string mapFile); // load mapFile info into streetSegs
		size_t getNumSegments() const;
		bool getSegment(size_t segNum, StreetSegment& seg) const;

	private:
		size_t numSegs = 0;
		vector<StreetSegment> streetSegs;
};


// LOADING FILE
bool MapLoaderImpl::load(string mapFile)
{
	ifstream infile(mapFile);
	if (!infile) // failed to open file
	{
		return false;
	}

	int headerCount = 2;
	int bodyCount = 0;
	string rawLine;

	while (getline(infile, rawLine)) // read line
	{
		// HEADER
		if (headerCount == 2) // street segment name
		{
			StreetSegment newSeg;
			streetSegs.push_back(newSeg);
			streetSegs.at(numSegs).streetName = transform(rawLine);
			headerCount--;
			continue;
		} else if (headerCount == 1) // starting/ending geo-coordinates
		{
			// finding start lat
			size_t pos = rawLine.find(", ");
			string startLat;
			if (pos >= rawLine.size()) // ","
			{
				pos = rawLine.find(",");
				startLat = rawLine.substr(0, pos);
				rawLine.erase(0, pos + 1); // clear the portion copied into startLat
			} else // ", "
			{
				startLat = rawLine.substr(0, pos);
				rawLine.erase(0, pos + 2); // clear the portion copied into startLat
			}


			// finding start long
			pos = rawLine.find(" ");
			string startLong = rawLine.substr(0, pos);
			rawLine.erase(0, pos + 1); // clear the portion copied in startLong

			// finding end lat
			pos = rawLine.find(", ");
			string endLat;
			if (pos >= rawLine.size()) // ","
			{
				pos = rawLine.find(",");
				endLat = rawLine.substr(0, pos);
				rawLine.erase(0, pos + 1); // clear the portion copied into endLat
			} else // ", "
			{
				endLat = rawLine.substr(0, pos);
				rawLine.erase(0, pos + 2); // clear the portion copied into endLat
			}


			// finding end long
			string endLong = rawLine; // what remains of rawLine is endLong

			// storing start coordinate info
			streetSegs[numSegs].segment.start.latitudeText = startLat; ////cerr << "START LAT: " << startLat << endl;
			streetSegs[numSegs].segment.start.longitudeText = startLong;  ////cerr << "START LONG: " << startLong << endl;

			streetSegs[numSegs].segment.start.latitude = stod(startLat);  ////cerr << "START LAT(d): " << stod(startLat) << endl;
			streetSegs[numSegs].segment.start.longitude = stod(startLong);  ////cerr << "START LONG(d): " << stod(startLong) << endl;

			// storing end coordinate info
			streetSegs[numSegs].segment.end.latitudeText = endLat; ////cerr << "END LAT: " << endLat << endl;
			streetSegs[numSegs].segment.end.longitudeText = endLong; ////cerr << "END LONG: " << endLong << endl;

			streetSegs[numSegs].segment.end.latitude = stod(endLat); ////cerr << "END LAT(d): " << stod(endLat) << endl;
			streetSegs[numSegs].segment.end.longitude = stod(endLong); ////cerr << "END LONG(d): " << stod(endLat) << endl;

			////cerr << "---------------------------------------------" << endl;
			headerCount--;
			continue;

		} else if (headerCount == 0)
		{
			// BODY
			bodyCount = stoi(rawLine);
			//cerr << "---------------------------------------------" << endl << "Attraction Count: " << bodyCount << endl;
			headerCount--;

			if (bodyCount == 0) // setting body count
			{
				headerCount = 2;
				numSegs++;
			}
			continue;

		} else
		{
				// storing attraction name
				string name = rawLine.substr(0, rawLine.find("|"));

				rawLine.erase(0, rawLine.find("|"));
				// storing coords
				string lat = rawLine.substr(1, rawLine.find(", "));
				rawLine.erase(0, rawLine.find(",") + 1);
				string lng = rawLine;

				// attraction
				Attraction a;
				a.name = transform(name);
				a.geocoordinates.latitudeText = lat;
				a.geocoordinates.longitudeText = lng;
				a.geocoordinates.latitude = stod(lat);
				a.geocoordinates.longitude = stod(lng);

				streetSegs[numSegs].attractions.push_back(a);

				bodyCount--;


			// resetting header count when bodyCount reaches 0
			if (bodyCount == 0)
			{
				numSegs++;
				headerCount = 2;
			}
		}
	}
	return true;
}

// NUMBER OF SEGMENTS
size_t MapLoaderImpl::getNumSegments() const
{
	return numSegs;
}

// RETURN DESIRED SEGMENT
bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum < numSegs)
	{
		seg = streetSegs.at(segNum);
		return true;
	} else
		return false;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
