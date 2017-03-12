#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl(): numSegs(0){}
	~MapLoaderImpl();
	bool load(string mapFile); // load mapFile info into streetSegs
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;

private:
	size_t numSegs;
	vector<StreetSegment*> streetSegs;
	//void process(string &rawLine);
};

// DESTRUCTOR
MapLoaderImpl::~MapLoaderImpl()
{
	for (int i = 0; i < numSegs; i++)
		delete streetSegs[i];
}

// LOADING FILE
bool MapLoaderImpl::load(string mapFile)
{
	ifstream infile(mapFile);
	if (!infile) // failed to open file
		return false;

	int headerCount = 2;
	int bodyCount = -1;
	string rawLine;
	while (getline(infile, rawLine)) // read line
	{
		// HEADER
		if (headerCount == 2) // street segment name
		{
			StreetSegment* temp;
			temp->streetName = rawLine;
			streetSegs.push_back(temp);
			headerCount--;
			continue;
		} else if (headerCount == 1) // starting/ending geo-coordinates
		{
			//process(rawLine);

			// finding start lat
			int pos = rawLine.find(", ");
			string startLat = rawLine.substr(0, pos);
			rawLine.erase(0, pos + 2); // clear the portion copied into startLat

			// finding start long
			pos = rawLine.find(" ");
			string startLong = rawLine.substr(0, pos);
			rawLine.erase(0, pos + 1); // clear the portion copied in startLong

			// finding end lat
			pos = rawLine.find(",");
			string endLat = rawLine.substr(0, pos);
			rawLine.erase(0, pos + 1); // clear the portion copied into endLat

			// finding end long
			string endLong = rawLine; // what remains of rawLine is endLong


			// storing start coordinate info
			streetSegs[numSegs]->segment.start.latitudeText = startLat;
			streetSegs[numSegs]->segment.start.longitudeText = startLong;

			streetSegs[numSegs]->segment.start.latitude = stod(startLat);
			streetSegs[numSegs]->segment.start.longitude = stod(startLong);

			// storing end coordinate info
			streetSegs[numSegs]->segment.end.latitudeText = endLat;
			streetSegs[numSegs]->segment.end.longitudeText = endLong;

			streetSegs[numSegs]->segment.end.latitude = stod(endLat);
			streetSegs[numSegs]->segment.end.longitude = stod(endLong);

			headerCount = -1;
			continue;
		}

		// BODY
		if (headerCount == -1 && bodyCount == -1)
		{
			bodyCount = stod(rawLine);
			continue;
		} else if (headerCount == -1 && bodyCount > 0)
		{
			// storing attraction name
			string name = rawLine.substr(0, rawLine.find("|"));
			rawLine.erase(0, rawLine.find("|"));

			// storing coords
			string lat = rawLine.substr(0, rawLine.find(", "));
			rawLine.erase(0, rawLine.find(",") + 1);
			string lng = rawLine;

			// attraction
			Attraction a;
			a.name = name;
			a.geocoordinates.latitudeText = lat;
			a.geocoordinates.longitudeText = lng;
			a.geocoordinates.latitude = stod(lat);
			a.geocoordinates.longitude = stod(lng);

			streetSegs[numSegs]->attractions.push_back(a);

			bodyCount--;
			continue;

		} else if (headerCount == -1 && bodyCount == 0)
		{
			numSegs++;
			headerCount = 2;
		}
	}
}

// NUMBER OF SEGMENTS
size_t MapLoaderImpl::getNumSegments() const
{
	return numSegs;
}

// RETURN DESIRED SEGMENT
bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum > 0 && segNum < numSegs)
	{
		seg = *streetSegs[segNum];
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
