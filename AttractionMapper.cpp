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
	int totalAttractionsCount = 0;
	// to get total number of attractions
	for (int i = 0; i < ml.getNumSegments(); i++)
	{
		// storing current segment
		StreetSegment temp;
		ml.getSegment(i, temp);

		// counting attractions for each segment
		totalAttractionsCount += temp.attractions.size();
	}

	// store attraction pointers
	Attraction* attractions[totalAttractionsCount];

		// storing current segment
		StreetSegment temp;
		int ind = 0;
		ml.getSegment(ind, temp);

		int currAttraction = temp.attractions.size();

	for (int i = 0; i < totalAttractionsCount; i++)
	{
		if (currAttraction == 0) // look into another segment's attractions
		{
			ind++;
			ml.getSegment(ind, temp);
			currAttraction = temp.attractions.size();
		}

		if (currAttraction > 0) // look into all the attractions of this segment
		{
			currAttraction--;
			Attraction* tempAtt = temp.attractions[currAttraction];
			attractions[i] = tempAtt;
		}
	}

	// associating each street segment with it's geo segment
	for (int i = 0; i < (ml.getNumSegments() - numWithoutAttractions); i++)
	{
		for (int j = 0; j < streetSegmentsWithAttractions[i].attractions.size(); j++)
		{
			map->associate(streetSegmentsWithAttractions[i].attractions[j], streetSegmentsWithAttractions[i].segment);
		}
	}
*/
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
