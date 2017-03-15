#include "provided.h"
#include "support.h"
#include <string>
#include "MyMap.h"
#include <iostream>
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
		// TO LOAD MAP DATA
		SegmentMapper sm;
		AttractionMapper am;

		// NODES TO KEEP TRACK OF ROUTE
		struct Node{
			Node* prev;
			GeoCoord* gc;
			GeoCoord* source;
			GeoCoord* destination;
			double totalCost;
			string streetName;
		};

		// CLEAN INTIALIZED NODE
		Node* newNode() const
		{
			Node* n = new Node;
			n->prev = nullptr;
			n->gc = nullptr;
			n->source = nullptr;
			n->destination = nullptr;
			n->totalCost = 0;
			n->streetName = "";
			// TODO: should I store geosegment instead?
			return n;
		}

		// TO CALCULATE THE TOTAL COST
		void setTotalCost(Node* n) const
		{
			if (n->prev != nullptr)
			{
				n->totalCost = n->prev->totalCost
				+ distanceEarthMiles(*n->prev->gc, *n->gc) // distance of segment
				+ distanceEarthMiles(*n->gc, *n->destination); // distance to destination
			} else
				cerr << "n->prev = null pointer!" << endl;
		}

		// FUNCTION TO FIND OPTIMAL ROUTE
		Node* searchForOptimalRoute(GeoCoord &start, GeoCoord &end, string destination) const
		{
			// to compare cost of paths
			struct CompareCost{
				bool operator()(const Node* lhs, const Node* rhs)
				{
					return lhs->totalCost > rhs->totalCost;
				}
			};

			// priority queue to keep track of optimal route
			priority_queue<Node*, vector<Node*>, const CompareCost> route;

			// map to keep track of all entered nodes
			MyMap<GeoCoord, Node> nodeStore;

			// storing options at each GeoCoordinate
			vector<StreetSegment> options = sm.getSegments(start);

			// all potential routes have same source
			Node* sourceNode = newNode();
			sourceNode->source = sourceNode->gc = &start;
			sourceNode->destination = &end;
			setTotalCost(sourceNode);


			cerr << "NEW NODE: created sourceaNode at " << sourceNode << " ---- COST: " << sourceNode->totalCost << "/// COORDINATES: "<< sourceNode->gc->latitudeText << ", " << sourceNode->gc->longitudeText << endl;
			cerr << "PARENT: " << sourceNode->prev << endl;
			cerr << "------------------------------------------------------------------------" << endl;

			// first step should always have 2 options

			// start of StreetSeg
			Node* n1 = newNode();
			n1->streetName = options[0].streetName;
			n1->gc = &(options[0].segment.start);
			n1->prev = sourceNode;
			n1->source = &start;
			n1->destination = &end;
			setTotalCost(n1);

			cerr << n1->streetName << " /// NEW NODE: created N1 node at " << n1 << " ---- COST: " << n1->totalCost << "/// COORDINATES: "<< n1->gc->latitudeText << ", " << n1->gc->longitudeText <<endl;
			cerr << "PARENT: " << n1->prev << endl;
			cerr << "------------------------------------------------------------------------" << endl;

			// end of StreetSeg
			Node* n2 = newNode();
			n2->streetName = options[0].streetName;
			n2->gc = &(options[0].segment.end);
			n2->prev = sourceNode;
			n2->source = &start;
			n2->destination = &end;
			setTotalCost(n2);


			cerr << n1->streetName << " ///  NEW NODE: created N2 node at " << n2 << " ---- COST: " << n2->totalCost << "/// COORDINATES: "<< n2->gc->latitudeText << ", " << n2->gc->longitudeText <<endl;
			cerr << "PARENT: " << n2->prev << endl;
			cerr << "------------------------------------------------------------------------" << endl;
			// TODO: ACCOMODATE FOR BOTH ATTRACTIONS BEING ON SAME STREETSEG

			cerr << endl << endl;

			// push first two options into route options
			route.push(n1);
			nodeStore.associate(*n1->gc, *n1); // add to node storage
			cerr << "INSERTING node " << n1->streetName << " with cost of " << n1->totalCost << " into the queue" << endl;
			route.push(n2);
			nodeStore.associate(*n2->gc, *n2); // add to node storage
			cerr << "INSERTING node " << n2->streetName << " with cost of " << n2->totalCost << " into the queue" << endl;

			// look for different paths
			bool foundRoute = false;
			while (!route.empty() && !foundRoute)
			{
				cerr << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl << endl;
				cerr << "PRIORITY QUEUE SIZE: " <<route.size() << endl;
				cerr << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl << endl;



				// start and end are in same street seg - wrong
				for (int x = 0; x < options.size(); x++)
				{
					for (int y = 0; y < sm.getSegments(end).size(); y++)
					{
						if (options[x] == sm.getSegments(end)[y])
						{
							foundRoute = true;
						}
					}
				}



				// taking the node with the best cost
				Node* front = route.top();
				route.pop();

				cerr << front->streetName << " ///  FRONT at " << front << " ---- COST: " << front->totalCost << "/// COORDINATES: "<< front->gc->latitudeText << ", " << front->gc->longitudeText <<endl;
				cerr << "PARENT: " << front->prev << endl;
				cerr << "------------------------------------------------------------------------" << endl;


				// evaluating this top node's options
				options = sm.getSegments(*(front->gc));

				// fill queue with all options
				for (int i = 0; i < options.size(); i++)
				{
					// curr segment
					cerr << "STREET SEGMENT #" << i << ": " << options[i].streetName << "/// START COORDS: " << options[i].segment.start.latitudeText << ", " << options[i].segment.start.longitudeText
							<< "/// END COORDS: " << options[i].segment.end.latitudeText << ", " << options[i].segment.end.longitudeText << endl;

					// new node
					Node* n = newNode();

					cerr << "FRONT GEOCOORD: " << front->gc->latitudeText << ", " << front->gc->longitudeText << endl;

					if (*front->gc == options[i].segment.start)
					{
						cerr << "Matching FRONT with Segment START" << endl;
						if (*front->prev->gc == options[i].segment.end)
						{  cerr << "Skipping same segment" << endl;
							continue;
						} else {
							n->gc = new GeoCoord(options[i].segment.end.latitudeText, options[i].segment.end.longitudeText);
							n->prev = front;
							n->source = &start;
							n->destination = &end;
							n->streetName = options[i].streetName;
							setTotalCost(n);
						}
					} else if (*front->gc == options[i].segment.end)
					{
						cerr << "Matching FRONT with Segment END" << endl;
						if (*front->prev->gc == options[i].segment.start)
						{ cerr << "Skipping same segment" << endl;
							continue;
						} else {
							n->gc = new GeoCoord(options[i].segment.start.latitudeText, options[i].segment.start.longitudeText);
							n->prev = front;
							n->source = &start;
							n->destination = &end;
							n->streetName = options[i].streetName;

							setTotalCost(n);
						}
					}


					cerr << n->streetName << " ///  NEW NODE: created node at " << n << " ---- COST: " << n->totalCost << "/// COORDINATES: "<< n->gc->latitudeText << ", " << n->gc->longitudeText <<endl;
					cerr << "PARENT: " << n->prev << endl;
					cerr << "------------------------------------------------------------------------" << endl;


					// push into route options + alter repeating nodes
					Node* nodeFound = nodeStore.find(*n->gc);
					if (nodeFound == nullptr)
					{
						nodeStore.associate(*n->gc, *n);
						route.push(n);
					} else { // node already there
						if (nodeFound->totalCost > n->totalCost)
						{
							nodeFound->totalCost = n->totalCost;
							nodeFound->prev = n->prev;
						}
						// throw n away it's cost is either worse than existing one or altered existing one
					}

					cerr << "INSERTING node " << n->streetName << " with cost of " << n->totalCost << " into the queue" << endl;

					// does this option contain the destination
					for (int j = 0; j < options[i].attractions.size(); j++)
					{
						// storing current attraction
						string attraction = options[i].attractions[j].name;
						if (attraction == destination)
						{
							// ensuring found destination is top of the queue
							foundRoute = true;
						}
					}
				}
			}

			if (foundRoute)
			{
				return route.top();
			}
			else
				return nullptr;

		}

		// TRANSFORM FOUND ROUTE TO WORKABLE NAVSEGMENTS FOR USER TO READ
		void routeToDirections(Node* n, vector<NavSegment> &directions) const
		{
			if (n == nullptr)
				return;

			// recursively go all the way to first command
			routeToDirections(n->prev, directions);

			// reached first command
			if (n->prev->gc == n->source)
			{
				// first move is always proceed
				string direction = getDirection(angleOfLine(GeoSegment(*n->source, *n->gc)));
				directions.push_back(NavSegment(direction, n->streetName,
						distanceEarthMiles(*n->source, *n->gc), GeoSegment(*n->source, *n->gc)));
				return;
			} else if (n->prev->streetName == n->streetName)
			{
				// same street, proceed
				string direction = getDirection(angleBetween2Lines(GeoSegment(*n->prev->gc, *n->gc), GeoSegment(*n->prev->prev->gc, *n->prev->gc)));
				directions.push_back(NavSegment(direction, n->streetName,
						distanceEarthMiles(*n->prev->gc, *n->gc), GeoSegment(*n->prev->gc, *n->gc)));
			} else
			{
				// different street, turn
				string direction = getDirection(angleBetween2Lines(GeoSegment(*n->prev->gc, *n->gc), GeoSegment(*n->prev->prev->gc, *n->prev->gc)));
				directions.push_back(NavSegment(direction, n->streetName));
			}
		}

		// TRANSLATE ANGLE INTO A DIRECTION
		string getDirection(double angle) const
		{
			if (angle >= 0 && angle <= 22.5)
				return "east";
			else if (angle > 22.5 && angle <= 67.5)
				return "northeast";
			else if (angle > 67.5 && angle <= 122.5)
				return "north";
			else if (angle > 112.5 && angle <= 157.5)
				return "northwest";
			else if (angle > 157.5 && angle <= 202.5)
				return "west";
			else if (angle > 202.5 && angle <= 247.5)
				return "southwest";
			else if (angle > 247.5 && angle <= 292.5)
				return "south";
			else if (angle > 292.5 && angle <= 337.5)
				return "southeast";
			else if (angle > 337.5 && angle <= 360)
				return "east";
			else
				return "";
		}
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
