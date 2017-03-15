#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <vector>
#include <iostream>
#include <string>
using namespace std;

class SegmentMapperImpl
{
	public:
		SegmentMapperImpl() {}
		~SegmentMapperImpl() {}
		void init(const MapLoader& ml);
		vector<StreetSegment> getSegments(const GeoCoord& gc) const;

	private:
		MyMap<GeoCoord, vector<StreetSegment>> map;
};

void SegmentMapperImpl::init(const MapLoader& ml)
{

	for (int i = 0; i < ml.getNumSegments(); i++)
	{
		// accessing each segment
		StreetSegment temp;
		ml.getSegment(i, temp);

		// accessing gc's of each gs
		GeoSegment gSeg = temp.segment;
		GeoCoord start = gSeg.start;
		GeoCoord end = gSeg.end;

		// START GC

		// finding the vector of streetsegments associated with start coord
		vector<StreetSegment>* segs = map.find(start);

		if (segs != nullptr) // found vector!
		{
			segs->push_back(temp); // add this streetsegment to the vector
		}
		else
		{ // didn't find vector!
			vector<StreetSegment> newSegs;
			newSegs.push_back(temp);
			map.associate(start, newSegs);
		}

		// END GC

		segs = map.find(end);

		// make sure start does not equal end
		if (!(start == end))
		{
			// finding the vector of streetsegments associated with end coord
			segs = map.find(end);

			if (segs != nullptr) // found vector!
			{
				segs->push_back(temp); // add this streetsegment to the vector
			}
			else { // didn't find vector!
				vector<StreetSegment> newSegs;
				newSegs.push_back(temp);
				map.associate(end, newSegs);
			}
		}

		// ATTRACTIONS
		for (int j = 0; j < temp.attractions.size(); j++)
		{
			Attraction att = temp.attractions[j];

			segs = map.find(att.geocoordinates);

			if (segs != nullptr) // found vector!
			{
				segs->push_back(temp); // add this streetsegment to the vector
			}
			else { // didn't find vector!
				vector<StreetSegment> newSegs;
				newSegs.push_back(temp);
				map.associate(att.geocoordinates, newSegs);
			}

		}
	}
}


vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	const vector<StreetSegment>* temp = map.find(gc);

	if (temp == nullptr)
	{
		vector<StreetSegment> empty;
		return empty;
	} else
		return *temp;

}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
