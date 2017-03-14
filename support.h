/*
 * support.h
 *
 *  Created on: Mar 12, 2017
 *      Author: helarabawy
 */

#ifndef SUPPORT_H_
#define SUPPORT_H_

#include <string>
#include "provided.h"

using namespace std;

inline
bool operator==(GeoCoord lhs, GeoCoord rhs)
{
	if ( (lhs.latitude == rhs.latitude) &&
		 (lhs.longitude == rhs.longitude) )
		return true;
	else
		return false;
}

inline
bool operator<(GeoCoord lhs, GeoCoord rhs)
{
	if (lhs.latitude < rhs.latitude)
		return true;
	if (lhs.latitude == rhs.latitude && lhs.longitude < rhs.longitude)
		return true;

	return false;
}

inline
void toLowercase(string& name)
{
	for (int i = 0; i < name.size(); i++)
	{
		if (isalpha(name[i]))
		{
			name[i] = tolower(name[i]);
		}
	}
}

#endif /* SUPPORT_H_ */
