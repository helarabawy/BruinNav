#include "provided.h"
#include "MyMap.h"
#include <string>
using namespace std;

class AttractionMapperImpl
{
	public:
		AttractionMapperImpl();
		~AttractionMapperImpl();
		void init(const MapLoader& ml);
		bool getGeoCoord(string attraction, GeoCoord& gc) const;

	private:
		MyMap<string, GeoCoord>* map;
};

AttractionMapperImpl::AttractionMapperImpl()
{
	map = new MyMap<string, GeoCoord>;
}

AttractionMapperImpl::~AttractionMapperImpl()
{
	delete map;
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	size_t totalAttractionsCount = 0;

	// to get total number of attractions
	for (size_t i = 0; i < ml.getNumSegments(); i++)
	{
		// storing current segment
		StreetSegment temp;
		ml.getSegment(i, temp);

		// counting attractions for each segment
		totalAttractionsCount += temp.attractions.size();
	}
cerr << "TOTAL ATTRACTIONS COUNT: " << totalAttractionsCount << endl;

	// storing current segment
	StreetSegment temp;
	size_t ind = 0; //index to keep track of which segment we are on
	ml.getSegment(ind, temp);

	// current segment's attraction count
	size_t currAttraction = temp.attractions.size();

	// associating each attraction name with it's geocoordinates
	for (size_t i = 0; i < totalAttractionsCount; i++)
	{
		cerr << i << ")" << "Current Attractions: " << currAttraction << endl;

		if (currAttraction == 0) // look into another segment's attractions
		{
			ind++;
			i--;
			ml.getSegment(ind, temp);
			currAttraction = temp.attractions.size();
			continue;
		}

		if (currAttraction > 0) // look into all the attractions of this segment
		{
			currAttraction--;
			Attraction* tempAtt = &temp.attractions[currAttraction];
			cerr << "Associating " << tempAtt->name << " and (" << tempAtt->geocoordinates.latitude << ", " << tempAtt->geocoordinates.longitude << ")";
			map->associate(tempAtt->name, tempAtt->geocoordinates);
		}
	}
}

// FIND GC AT ATTRACTION
bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	GeoCoord* temp = map->find(attraction);
	if (temp != nullptr)
	{
		cerr << "Found attraction!!!" << endl;
		gc.latitude = temp->latitude;
		gc.longitude = temp->longitude;
		gc.latitudeText = temp->latitudeText;
		gc.longitudeText = temp->longitudeText;
		return true;
	}

	cerr << "Did not find attraction!!!" << endl;
	return false;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
