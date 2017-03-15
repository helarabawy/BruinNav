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
			GeoCoord* source;
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
			n->source = nullptr;
			n->destination = nullptr;
			n->totalCost = 0;
			n->streetName = "";

			return n;
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
				n->totalCost = distanceEarthMiles(*n->source, *n->gc)
				+ distanceEarthMiles(*n->gc, *n->destination );
			}
		}

		// Helper function to find the optimal route
		Node* searchForOptimalRoute(GeoCoord &start, GeoCoord &end, string destination) const
		{
			// to compare cost of paths
			struct CompareCost{
				bool operator()(const Node* lhs, const Node* rhs)
				{
					return lhs->totalCost < rhs->totalCost;
				}
			};

			// priority queue to keep track of optimal route
			priority_queue<Node*, vector<Node*>, const CompareCost> route;


			// storing options at each GeoCoordinate
			vector<StreetSegment> options = sm.getSegments(start);

			// first step should always have 2 options

			// start of StreetSeg
			Node* n1 = newNode();
			n1->streetName = options[0].streetName;
			n1->gc = &(options[0].segment.start);
			n1->source = &start;
			n1->destination = &end;
			setTotalCost(n1);

			// end of StreetSeg
			Node* n2 = newNode();
			n2->streetName = options[0].streetName;
			n2->gc = &(options[0].segment.end);
			n2->source = &start;
			n2->destination = &end;
			setTotalCost(n2);

			// push first two options into route options
			route.push(n1);
			route.push(n2);

			// look for different paths
			bool foundRoute = false;
			while (!route.empty() && !foundRoute)
			{
				// taking the node with the best cost
				Node* front = route.top();
				route.pop();

				// evaluating this top node's options
				options = sm.getSegments(*(front->gc));

				// fill queue with all options
				for (int i = 0; i < options.size(); i++)
				{
					Node* n = newNode();
					n->streetName = options[i].streetName;
					n->gc = &(options[i].segment.end);
					n->source = &start;
					n->destination = &end;
					setTotalCost(n);

					// push into route options
					route.push(n);

					// does this option contain the destination
					for (int j = 0; j < options[i].attractions.size(); j++)
					{
						// storing current attraction
						string attraction = options[i].attractions[j].name;
						if (attraction == destination)
						{
							// ensuring found destination is top of the queue
							n->totalCost = 0;
							foundRoute = true;
						}
					}
				}
			}

			if (foundRoute)
				return route.top();
			else
				return nullptr;

		}

		void routeToDirections(Node* optimizedRoute, vector<NavSegment> &directions) const{}
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
	Node* optimizedRoute = searchForOptimalRoute(startGC, endGC, end);
	if (optimizedRoute == nullptr)
	{
		return NAV_NO_ROUTE;
	}

	// translate route to workable directions for user
	routeToDirections(optimizedRoute, directions);

	return NAV_SUCCESS;
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
