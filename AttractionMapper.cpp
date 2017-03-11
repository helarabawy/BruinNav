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
	// all street segments
	StreetSegment* streetSegmentsWithAttractions = new StreetSegment[ml.getNumSegments()];

	// array with street segments
	int numWithoutAttractions = 0;

	// storing segments with attractions
	for (int i = 0; i < ml.getNumSegments(); i++)
	{
		StreetSegment temp;
		if (ml.getSegment(i, temp))
		{
			if (temp.attractions.size() > 0)
			{
				streetSegmentsWithAttractions[i - numWithoutAttractions] = temp;
			} else
				numWithoutAttractions++;
		}
	}

	// storing each attraction
	for (int i = 0; i < (ml.getNumSegments() - numWithoutAttractions); i++)
	{
		for (int j = 0; j < streetSegmentsWithAttractions[i].attractions.size(); j++)
		{
			map->associate(streetSegmentsWithAttractions[i].attractions[j], streetSegmentsWithAttractions[i].segment);
		}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	GeoCoord* temp = map->find(attraction);
	if (temp != nullptr)
	{
		gc = *temp;
		return true;
	}

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
