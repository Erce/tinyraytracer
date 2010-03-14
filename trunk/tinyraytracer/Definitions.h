#pragma once
#include <limits>

#define Real float
#define EPSILON	0.01f

const static float MAX_FLOAT = 99999999.f;
const static float MIN_FLOAT = -MAX_FLOAT;
#define IntersectionType int
#define 	INTERSECTS_OUTSIDE  -10
#define 	INTERSECTION_NO		 0
#define 	INTERSECTS_INSIDE	 10


#define SAFE_DELETE_ARRAY(a) {if( (a) != NULL ){ delete [] (a);(a) = NULL;} }
#define SAFE_DELETE(a) {if( (a) != NULL ){ delete (a);(a) = NULL;} }