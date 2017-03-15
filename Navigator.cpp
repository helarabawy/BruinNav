#include "provided.h"
#include "support.h"
#include <string>
#include <vector>
#include <queue>
using namespace std;

class NavigatorImpl
{
	public:
		NavigatorImpl(){}
		~NavigatorImpl(){}
		bool loadMapData(string mapFile);
		NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
	private:
		// To load map data
		SegmentMapper sm;
		AttractionMapper am;

		// Nodes to keep track of route
		struct Node{
			Node* prev;
			GeoCoord* gc;
			GeoCoord* destination;
			int totalCost;
			string streetName;
		};

		// clean new node with no random values
		Node* newNode() const
		{
			Node* n = new Node;
			n->prev = nullptr;
			n->gc = nullptr;
			n->destination = nullptr;
			n->totalCost = 0;
			n->streetName = "";
		}

		// to calculate total cost
		void setTotalCost(Node* n) const
		{
			if (n->prev != nullptr)
			{
				n->totalCost = n->prev->totalCost
				+ distanceEarthMiles(*n->prev->gc, *n->gc)
				+ distanceEarthMiles(*n->gc, *n->destination );
			} else
			{
				n->totalCost = distanceEarthMiles(*n->prev->gc, *n->gc)
				+ distanceEarthMiles(*n->gc, *n->destination );

			}
		}

		// to compare cost of paths
		struct CompareCost{
			bool operator()(const Node* lhs, const Node* rhs)
			{
				return lhs->totalCost < rhs->totalCost;
			}
		};

		priority_queue<Node*, vector<Node*>, CompareCost> route;

		bool searchForOptimalRoute(GeoCoord &start, GeoCoord &end) const;
		void routeToDirections(vector<NavSegment> &directions) const;
};

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

	// searching for optimal route
	bool routed = searchForOptimalRoute(startGC, endGC);

	if (!routed)
	{
		return NAV_NO_ROUTE;
	}

	// translate route to workable directions for user
	routeToDirections(directions);

	return NAV_SUCCESS;
}

bool NavigatorImpl::searchForOptimalRoute(GeoCoord& start, GeoCoord& end) const
{
	vector<StreetSegment> options = sm.getSegments(start);

	// first step should always have 2 options
	Node* n1 = newNode();
	n1->streetName = options[0].streetName;
	n1->gc = &(options[0].segment.start);
	n1->destination = &end;
	setTotalCost(n1);

	Node* n2 = newNode();
	n2->streetName = options[1].streetName;
	n2->gc = &(options[1].segment.start);
	n2->destination = &end;
	setTotalCost(n2);

	// push first two options into route options
	route.push(n1);
	route.push(n2);

	return false;


}

void NavigatorImpl::routeToDirections(vector<NavSegment>& directions) const
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
