#include "provided.h"
#include "support.h"
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

		//priority_queue<>route;

		bool searchForOptimalRoute(GeoCoord &start, GeoCoord &end);
		void routeToDirections(vector<NavSegment> &directions);
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
	if (ml.load(mapFile))
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
	if (!am.getGeoCoord(start, startGC))
		return NAV_BAD_SOURCE;

	// store coordinate of ending point
	GeoCoord endGC;
	if (!am.getGeoCoord(end, endGC))
		return NAV_BAD_DESTINATION;

	// current GC being explored
	GeoCoord currGC = startGC;

	// searching for optimal route
	bool routed = searchForOptimalRoute(startGC, endGC);

	if (!routed)
	{
		return NAV_NO_ROUTE;
	}

	// translate route to workable directions for user
	routeToDirections(&directions);

	return NAV_SUCCESS;
}

bool NavigatorImpl::searchForOptimalRoute(GeoCoord& start, GeoCoord& end)
{
	// TODO: do this
	return false;
}

void NavigatorImpl::routeToDirections(vector<NavSegment>& directions)
{
	// TODO: do this
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
