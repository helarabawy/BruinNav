#include "provided.h"
#include "support.h"
#include "MyMap.h"
#include <string>
using namespace std;

class AttractionMapperImpl
{
	public:
		AttractionMapperImpl() {}
		~AttractionMapperImpl() {}
		void init(const MapLoader& ml);
		bool getGeoCoord(string attraction, GeoCoord& gc) const;

	private:
		MyMap<string, GeoCoord> map;
};

void AttractionMapperImpl::init(const MapLoader& ml)
{
	for (int i = 0; i < ml.getNumSegments(); i++)
	{
		// storing current segment
		StreetSegment temp;
		ml.getSegment(i, temp);

		for (int j = 0; j < temp.attractions.size(); j++)
		{
			string name = temp.attractions[j].name;
			map.associate(name, temp.attractions[j].geocoordinates);
		}
	}
}

// FIND GC AT ATTRACTION
bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	string modAtt = transform(attraction);
	const GeoCoord* temp = map.find(modAtt);

	if (temp == nullptr)
	{
		return false;
	} else
	{
		gc = *temp;
		return true;
	}

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
