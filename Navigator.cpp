#include "provided.h"
#include <string>
#include <vector>
#include <queue>
using namespace std;

class NavigatorImpl
{
	public:
		NavigatorImpl();
		~NavigatorImpl();
		bool loadMapData(string mapFile);
		NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
	private:
		SegmentMapper sm;
		AttractionMapper am;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	MapLoader ml;
	bool loaded = ml.load(mapFile);

	if (loaded)
	{
		sm.init(ml);
		am.init(ml);

		return true;
	} else
		return false;
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	// store coordinate of starting point
	GeoCoord startGC;
	am.getGeoCoord(start, startGC);

	// store coordinate of ending point
	GeoCoord endGC;
	am.getGeoCoord(end, endGC);

	bool routed = false;
	while (!routed)
	{
		// create a priority queue ordered by A* algorithm

		// load based on options at each coordinate
	}
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
