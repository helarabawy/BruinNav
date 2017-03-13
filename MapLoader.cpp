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
	{
		cerr << "CANNOT OPEN FILE" << endl;
		return false;
	}
	cerr << "SUCCESSFULLY OPENED FILE" << endl;

	int headerCount = 2;
	int bodyCount = -1;
	string rawLine;
	while (getline(infile, rawLine)) // read line
	{
		cerr << "----------------------" << endl;
		cerr << "RAW LINE: " << rawLine << endl;
		cerr << "HEADER COUNT: " << headerCount << endl;

		// HEADER
		if (headerCount == 2 && bodyCount == -1) // street segment name
		{
			StreetSegment* temp = new StreetSegment;
			temp->streetName = rawLine;
			streetSegs.push_back(temp);
			headerCount--;
			cerr << "STREET NAME PROCESSED" << endl;
			continue;
		} else if (headerCount == 1 && bodyCount == -1) // starting/ending geo-coordinates
		{
			// finding start lat
			int pos = rawLine.find(", ");
			string startLat = rawLine.substr(0, pos);
			cerr << "Starting latitude:" << startLat << endl;
			rawLine.erase(0, pos + 2); // clear the portion copied into startLat

			// finding start long
			pos = rawLine.find(" ");
			string startLong = rawLine.substr(0, pos);
			cerr << "Starting longitude:" << startLong << endl;
			rawLine.erase(0, pos + 1); // clear the portion copied in startLong

			// finding end lat
			pos = rawLine.find(",");
			string endLat = rawLine.substr(0, pos);
			cerr << "Ending latitude:" << endLat << endl;
			rawLine.erase(0, pos + 1); // clear the portion copied into endLat

			// finding end long
			string endLong = rawLine; // what remains of rawLine is endLong
			cerr << "Ending longitude:" << endLong << endl;

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
		if (headerCount == -1 && bodyCount == -1) // setting body count
		{
			bodyCount = stod(rawLine);
			cerr << "Setting Bodycount: " <<  bodyCount << endl;
		} else if (headerCount == -1 && bodyCount > 0) // going through body count
		{
			cerr << "Attraction: " << endl;
			// storing attraction name
			string name = rawLine.substr(0, rawLine.find("|"));
			rawLine.erase(0, rawLine.find("|"));
			cerr << name << endl;

			// storing coords
			string lat = rawLine.substr(1, rawLine.find(", "));
			rawLine.erase(0, rawLine.find(",") + 1);
			string lng = rawLine;
			cerr << "Latitude: " << lat << "; Longitude: " << lng << endl;

			// attraction
			Attraction a;
			a.name = name;
			a.geocoordinates.latitudeText = lat;
			a.geocoordinates.longitudeText = lng;
			a.geocoordinates.latitude = stod(lat);
			a.geocoordinates.longitude = stod(lng);

			streetSegs[numSegs]->attractions.push_back(a);

			bodyCount--;
		}

		// resetting header count when bodyCount reaches 0
		if (headerCount == -1 && bodyCount == 0)
		{
			numSegs++;
			headerCount = 2;
			bodyCount = -1;
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
