#include "provided.h"
#include <vector>
#include <string>
using namespace std;

class SegmentMapperImpl
{
	public:
		SegmentMapperImpl() {map = new MyMap<string, GeoCoord>;}
		~SegmentMapperImpl() {delete map;}
		void init(const MapLoader& ml);
		vector<StreetSegment> getSegments(const GeoCoord& gc) const;

	private:
		MyMap<GeoCoord, vector<StreetSegment>>* map;
};

void SegmentMapperImpl::init(const MapLoader& ml)
{
	for (int i = 0; i < ml.getNumSegments(); i++)
	{
		// accessing each segment
		StreetSegment temp;
		ml.getSegment(i, temp);

		// accessing gc's of each gs
		GeoSegment *gSeg = &temp.segment;
		GeoCoord *start = &gSeg->start;
		GeoCoord *end = &gSeg->end;

		// for start GC

		// finding the vector of streetsegments associated with start coord
	    vector<StreetSegment>* segs = map->find(*start);

		if (segs != nullptr) // found vector!
		{
			segs->push_back(temp); // add this streetsegment to the vector
		}
		else { // didn't find vector!
			vector<StreetSegment> newSegs;
			newSegs.push_back(temp);

			map->associate(start, newSegs);
		}


		// for end GC

		// finding the vector of streetsegments associated with end coord
	    vector<StreetSegment>* segs = map->find(*end);

		if (segs != nullptr) // found vector!
		{
			segs->push_back(temp); // add this streetsegment to the vector
		}
		else { // didn't find vector!
			vector<StreetSegment> newSegs;
			newSegs.push_back(temp);

			map->associate(start, newSegs);
		}

	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	vector<StreetSegment> segments;
	return segments;  // This compiles, but may not be correct
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
