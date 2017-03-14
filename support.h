/*
 * support.h
 *
 *  Created on: Mar 12, 2017
 *      Author: helarabawy
 */

#ifndef SUPPORT_H_
#define SUPPORT_H_

#include "provided.h"

bool operator==(GeoCoord lhs, GeoCoord rhs)
{
	if ( (lhs.latitudeText == rhs.latitudeText) &&
		 (lhs.longitudeText == rhs.longitudeText) )
		return true;
	else
		return false;
}

bool operator<(GeoCoord lhs, GeoCoord rhs)
{
	if (lhs.latitude < rhs.latitude)
		return true;
	if (lhs.latitude == rhs.latitude && lhs.longitude < rhs.longitude)
		return true;

	return false;
}


#endif /* SUPPORT_H_ */
