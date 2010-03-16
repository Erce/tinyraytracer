
/*
*
*	Author : Umut Riza ERTURK
*	March 2010
*	
*	Using inline functions for efficiency reasons
*/

#pragma once
#include "Definitions.h"
#include "MathUtil.h"

class Vector3
{
public:

	union 
	{
		struct  
		{
			Real x;
			Real y;
			Real z;
		};
		struct
		{
			Real r;
			Real g;
			Real b;
		};
		struct
		{
			Real data[3];
		};

	};


	inline Vector3(void){};
	inline Vector3(const Vector3& vec)
	{
		x =vec.x;
		y =vec.y;
		z =vec.z;
	};
	inline Vector3( const Real fX, const Real fY, const Real fZ )
	{
		x =fX;
		y =fY;
		z =fZ;
	}
	inline Vector3( const Real f )
	{
		x = y = z = f;
	}
	~Vector3(void){};


    inline Vector3& operator = ( const Vector3& vec )
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
        return *this;
    }

    inline Vector3 operator + ( const Vector3& vec ) const
    {
        return Vector3(
            x + vec.x,
            y + vec.y,
            z + vec.z);
    }

    inline Vector3 operator - ( const Vector3& vec ) const
    {
        return Vector3(
            x - vec.x,
            y - vec.y,
            z - vec.z);
    }
    
	inline Vector3 operator * ( const Real rLeftSideValue ) const
    {
        return Vector3(
            x * rLeftSideValue,
            y * rLeftSideValue,
            z * rLeftSideValue);
    }

    inline Vector3 operator / ( const Real rLeftSideValue ) const
    {       

        Real fInv = 1.f / rLeftSideValue;

        return Vector3(
            x * fInv,
            y * fInv,
            z * fInv);
    }

    inline Vector3 operator - () const
    {
        return Vector3(-x, -y, -z);
    }
        inline friend Vector3 operator * ( const Real rLeftSideValue, const Vector3& vRightSide )
        {
            return Vector3(
                rLeftSideValue * vRightSide.x,
                rLeftSideValue * vRightSide.y,
                rLeftSideValue * vRightSide.z);
        }

        inline friend Vector3 operator / ( const Real rLeftSideValue, const Vector3& vRightSide )
        {
            return Vector3(
                rLeftSideValue / vRightSide.x,
                rLeftSideValue / vRightSide.y,
                rLeftSideValue / vRightSide.z);
        }

        inline friend Vector3 operator + (const Vector3& leftSideValue, const Real rightSideValue)
        {
            return Vector3(
                leftSideValue.x + rightSideValue,
                leftSideValue.y + rightSideValue,
                leftSideValue.z + rightSideValue);
        }

		inline friend Vector3 operator * (const Vector3& leftSideValue, const Vector3& rightSideValue)
		{
			return Vector3(
				leftSideValue.x * rightSideValue.x,
				leftSideValue.y * rightSideValue.y,
				leftSideValue.z * rightSideValue.z);
		}

        inline friend Vector3 operator + (const Real leftSideValue, const Vector3& rightSideValue)
        {
            return Vector3(
                leftSideValue + rightSideValue.x,
                leftSideValue + rightSideValue.y,
                leftSideValue + rightSideValue.z);
        }

        inline friend Vector3 operator - (const Vector3& leftSideValue, const Real rightSideValue)
        {
            return Vector3(
                leftSideValue.x - rightSideValue,
                leftSideValue.y - rightSideValue,
                leftSideValue.z - rightSideValue);
        }

        inline friend Vector3 operator - (const Real leftSideValue, const Vector3& rightSideValue)
        {
            return Vector3(
                leftSideValue - rightSideValue.x,
                leftSideValue - rightSideValue.y,
                leftSideValue - rightSideValue.z);
        }

        // arithmetic updates
        inline Vector3& operator += ( const Vector3& vRightSide )
        {
            x += vRightSide.x;
            y += vRightSide.y;
            z += vRightSide.z;

            return *this;
        }

        inline Vector3& operator += ( const Real rLeftSideValue )
        {
            x += rLeftSideValue;
            y += rLeftSideValue;
            z += rLeftSideValue;
            return *this;
        }

        inline Vector3& operator -= ( const Vector3& vRightSide )
        {
            x -= vRightSide.x;
            y -= vRightSide.y;
            z -= vRightSide.z;
            return *this;
        }

        inline Vector3& operator -= ( const Real rLeftSideValue )
        {
            x -= rLeftSideValue;
            y -= rLeftSideValue;
            z -= rLeftSideValue;
            return *this;
        }

        inline Vector3& operator *= ( const Real rLeftSideValue )
        {
            x *= rLeftSideValue;
            y *= rLeftSideValue;
            z *= rLeftSideValue;
            return *this;
        }

        inline Vector3& operator *= ( const Vector3& vRightSide )
        {
            x *= vRightSide.x;
            y *= vRightSide.y;
            z *= vRightSide.z;

            return *this;
        }

        inline Vector3& operator /= ( const Real rLeftSideValue )
        {
            Real fInv = 1.f / rLeftSideValue;
            x *= fInv;
            y *= fInv;
            z *= fInv;

            return *this;
        }
        __forceinline Real length () const
        {
            return MathUtil::sqrtX( squaredLength() );
        }
        __forceinline Real squaredLength () const
        {
            return ( x*x + y*y + z*z );
        }
        __forceinline Real dot(const Vector3& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }
        __forceinline void normalise()
        {
			(*this) *= 1.f / length();
        }
        __forceinline Vector3 crossProduct( const Vector3& vRightSide ) const
        {
            return Vector3(
                y * vRightSide.z - z * vRightSide.y,
                z * vRightSide.x - x * vRightSide.z,
                x * vRightSide.y - y * vRightSide.x);
        }
        __forceinline Vector3 normalisedCopy(void) const
        {
            Vector3 ret = *this;
            ret.normalise();
            return ret;
        }
        __forceinline Vector3 reflect(const Vector3& normal) const
        {
            return Vector3( *this - ( (2 * this->dot(normal)) * normal ) );
        }
        __forceinline void reflectItself(const Vector3& normal)
        {
            *this -= ( (2 * this->dot(normal)) * normal ) ;
        }
        static const Vector3 VECTOR_ZERO;
        static const Vector3 VECTOR_UNIT_X;
		static const Vector3 VECTOR_NEGATIVE_UNIT_X;
        static const Vector3 VECTOR_UNIT_Y;
		static const Vector3 VECTOR_NEGATIVE_UNIT_Y;
        static const Vector3 VECTOR_UNIT_Z;
        static const Vector3 VECTOR_NEGATIVE_UNIT_Z;
};
