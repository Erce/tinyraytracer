/*
*
*	Author : Umut Riza ERTURK
*	March 2010
*	
*/

#pragma once
#include "Colour3f.h"
class PointLight
{
public:
	PointLight(void){
	};

	PointLight(const Colour3f& colour, const Vector3& position, Real rPower = 1.f)
	{
		m_Colour = colour * rPower;
		m_Position = position;
		
	};
	~PointLight(void){

	};	
	Colour3f		m_Colour;
	Vector3			m_Position;

};
