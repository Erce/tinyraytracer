#pragma once
#include "Definitions.h"
#include "MathUtil.h"

class Vector3
{
public:

	Real x;
	Real y;
	Real z;


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
		x =f;
		y =f;
		z =f;
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
    
	inline Vector3 operator * ( const Real fScalar ) const
    {
        return Vector3(
            x * fScalar,
            y * fScalar,
            z * fScalar);
    }

    inline Vector3 operator / ( const Real fScalar ) const
    {       

        Real fInv = 1.f / fScalar;

        return Vector3(
            x * fInv,
            y * fInv,
            z * fInv);
    }

    inline Vector3 operator - () const
    {
        return Vector3(-x, -y, -z);
    }
        // overloaded operators to help Vector3
        inline friend Vector3 operator * ( const Real fScalar, const Vector3& rkVector )
        {
            return Vector3(
                fScalar * rkVector.x,
                fScalar * rkVector.y,
                fScalar * rkVector.z);
        }

        inline friend Vector3 operator / ( const Real fScalar, const Vector3& rkVector )
        {
            return Vector3(
                fScalar / rkVector.x,
                fScalar / rkVector.y,
                fScalar / rkVector.z);
        }

        inline friend Vector3 operator + (const Vector3& lhs, const Real rhs)
        {
            return Vector3(
                lhs.x + rhs,
                lhs.y + rhs,
                lhs.z + rhs);
        }

		inline friend Vector3 operator * (const Vector3& lhs, const Vector3& rhs)
		{
			return Vector3(
				lhs.x * rhs.x,
				lhs.y * rhs.y,
				lhs.z * rhs.z);
		}

        inline friend Vector3 operator + (const Real lhs, const Vector3& rhs)
        {
            return Vector3(
                lhs + rhs.x,
                lhs + rhs.y,
                lhs + rhs.z);
        }

        inline friend Vector3 operator - (const Vector3& lhs, const Real rhs)
        {
            return Vector3(
                lhs.x - rhs,
                lhs.y - rhs,
                lhs.z - rhs);
        }

        inline friend Vector3 operator - (const Real lhs, const Vector3& rhs)
        {
            return Vector3(
                lhs - rhs.x,
                lhs - rhs.y,
                lhs - rhs.z);
        }

        // arithmetic updates
        inline Vector3& operator += ( const Vector3& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;
            z += rkVector.z;

            return *this;
        }

        inline Vector3& operator += ( const Real fScalar )
        {
            x += fScalar;
            y += fScalar;
            z += fScalar;
            return *this;
        }

        inline Vector3& operator -= ( const Vector3& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;
            z -= rkVector.z;

            return *this;
        }

        inline Vector3& operator -= ( const Real fScalar )
        {
            x -= fScalar;
            y -= fScalar;
            z -= fScalar;
            return *this;
        }

        inline Vector3& operator *= ( const Real fScalar )
        {
            x *= fScalar;
            y *= fScalar;
            z *= fScalar;
            return *this;
        }

        inline Vector3& operator *= ( const Vector3& rkVector )
        {
            x *= rkVector.x;
            y *= rkVector.y;
            z *= rkVector.z;

            return *this;
        }

        inline Vector3& operator /= ( const Real fScalar )
        {
            Real fInv = 1.f / fScalar;

            x *= fInv;
            y *= fInv;
            z *= fInv;

            return *this;
        }
        __forceinline Real length () const
        {
            return Math::sqrtX( squaredLength() );
        }
        __forceinline Real squaredLength () const
        {
            return ( x*x + y*y + z*z );
        }
        __forceinline Real length(const Vector3& rhs) const
        {
            return (*this - rhs).length();
        }
        __forceinline Real lengthSquare(const Vector3& rhs) const
        {
            return (*this - rhs).squaredLength();
        }
        __forceinline Real dot(const Vector3& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }
        __forceinline void normalise()
        {
			(*this) /= Math::sqrtX(x*x + y*y + z*z);
        }
        __forceinline Vector3 crossProduct( const Vector3& rkVector ) const
        {
            return Vector3(
                y * rkVector.z - z * rkVector.y,
                z * rkVector.x - x * rkVector.z,
                x * rkVector.y - y * rkVector.x);
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
        static const Vector3 ZERO;
        static const Vector3 UNIT_X;
        static const Vector3 UNIT_Y;
        static const Vector3 UNIT_Z;
        static const Vector3 NEGATIVE_UNIT_X;
        static const Vector3 NEGATIVE_UNIT_Y;
        static const Vector3 NEGATIVE_UNIT_Z;
        static const Vector3 UNIT_SCALE;
};
