#ifndef __MATH__
#define __MATH__

//////////////////////////////////////////////////////////////////////////
// File : Math.h
// Author : Francesco Rinaldi
// Date : 11 / 20 / 05
// Purpose : To define Operation types for Vector, Matrix, and Quaternions
//			 also define mathematical constance. 
//		     Class Triangle is defined here as well
//////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////
// redefine float Type
#define Scalar  float

//////////////////////////////////////////////////////////////////////////
// Misc. constants
const Scalar _Gravity_			= -32.174f;				// acceleration due to gravity, ft/s^2
const Scalar _AirResistance_	=  0.0023769f;			// density of air at sea level, slugs/ft^3
const Scalar _FloatTolerance_	= 0.000001f;			// Scalar type tolerance 
const Scalar _PI_				= 3.14159265f;			// Every one love PI 
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Misc. Key functions
// prototypes
inline Scalar DegreesToRadians(const Scalar Deg);
inline Scalar RadiansToDegrees(const Scalar Rad);
// definitions
inline Scalar DegreesToRadians(const Scalar Deg)
{
	return Deg * _PI_ / 180.0f;
}
inline Scalar RadiansToDegrees(const Scalar Rad)
{
	return Rad * 180.0f / _PI_;
}
inline Scalar GetRandomScalar(Scalar low, Scalar high)
{
	if (low >= high)
		return low;
	Scalar f = (rand() % 10000) * 0.0001f;
	return (f * (high - low) + low);
}
//////////////////////////////////////////////////////////////////////////
// Class : CVector 
//			Defines a 3 component vector, x, y, z
class CVector3f
{
public:
	// 3 components
	Scalar x, y, z, w;

	CVector3f(void);
	~CVector3f(void);
	CVector3f(const CVector3f&);
	CVector3f(Scalar s);
	CVector3f(Scalar fx, Scalar fy, Scalar fz);
	//CVector3f& operator=(CVector3f& v);
	CVector3f& operator=(const CVector3f& v);
	CVector3f& operator=(Scalar s);
	
	CVector3f operator-(void);			// overload negation

	// ret = this (op) v
	CVector3f operator+(const CVector3f& v);		
	CVector3f operator-(const CVector3f& v);
	CVector3f operator*(const CVector3f& v);
	CVector3f operator/(const CVector3f& v);

	CVector3f operator+(const Scalar s);		
	CVector3f operator-(const Scalar s);
	CVector3f operator*(const Scalar s);
	CVector3f operator/(const Scalar s);

	void operator+=(CVector3f v);
	void operator-=(CVector3f v);
	void operator*=(CVector3f v);
	void operator/=(CVector3f v);

	void operator+=(Scalar s);
	void operator-=(Scalar s);
	void operator*=(Scalar s);
	void operator/=(Scalar s);

	bool operator!=(CVector3f v);
	bool operator==(CVector3f v);

	void  Normalize();
	void  Normalize(CVector3f &v);		// normalize the referenced vector
	Scalar Magnitude();

	Scalar    DotProduct(CVector3f v);
	CVector3f CrossProduct(CVector3f v1, CVector3f v2);		// v1 X v2 return a new Vector
	CVector3f CrossProduct(CVector3f v);					// this X v  return a new Vector
	Scalar	  TriplefloatCrossproduct(CVector3f v1, CVector3f v2, CVector3f v3);
	void	  MakeZero();

	 CVector3f GetRotatedX(float angle);
	 CVector3f GetRotatedY(float angle);
	 CVector3f GetRotatedZ(float angle);

	 void RotateX(float angle);
	 void RotateY(float angle);
	 void RotateZ(float angle);
	 void MakeRandom(float maxX = 10, float minX = 10, 
						   float maxY = 10, float minY = 10, 
						   float maxZ = 10, float minZ = 10);

     Scalar GetTweenAngleRadians(CVector3f vec);
	 Scalar GetTweenAngleDegrees(CVector3f vec);
};

//////////////////////////////////////////////////////////////////////////
// Matrix3x3
//			 3 x 3 Matrix class and functions

class CMatrix3x3
{
public:
	// elements eij : i -> row  j -> col
	Scalar e11, e12, e13,
		  e21, e22, e23,
		  e31, e32, e33;

	CMatrix3x3(void);
	~CMatrix3x3(void){}
	CMatrix3x3(const CMatrix3x3&);
	CMatrix3x3(Scalar e11, Scalar e12, Scalar e13,
			  Scalar e21, Scalar e22, Scalar e23,
		      Scalar e31, Scalar e32, Scalar e33);
	CMatrix3x3(CVector3f row1, CVector3f row2, CVector3f row3);
	
	CMatrix3x3& operator=(const CMatrix3x3&);
	
	Scalar	   Determinant(void);
	CMatrix3x3 Transpose(void);
	CMatrix3x3 Inverse(void);
	void	   MakeMeIdentity();
	CMatrix3x3 GetIdentity();
	void	   MakeMeRotationX(Scalar ang);
	void	   MakeMeRotationY(Scalar ang);
	void	   MakeMeRotationZ(Scalar ang);


	CMatrix3x3& operator+=(CMatrix3x3 M3x3);
	CMatrix3x3& operator-=(CMatrix3x3 M3x3);
	CMatrix3x3& operator*=(Scalar s);
	CMatrix3x3& operator/=(Scalar s);
};

inline CMatrix3x3::CMatrix3x3(void)
{
	e11 = e12 = e13 = 
	e21 = e22 = e23 = 
	e31 = e32 = e33 = 0.0f;
	MakeMeIdentity();
}

inline CMatrix3x3::CMatrix3x3(const CMatrix3x3& Mat3x3)
{
	e11 = Mat3x3.e11;
	e12 = Mat3x3.e12;
	e13 = Mat3x3.e13;
	e21 = Mat3x3.e21;
	e22 = Mat3x3.e22;
	e23 = Mat3x3.e23;
	e31 = Mat3x3.e31;
	e32 = Mat3x3.e32;
	e33 = Mat3x3.e33;
}

inline CMatrix3x3::CMatrix3x3(Scalar e11, Scalar e12, Scalar e13, 
							  Scalar e21, Scalar e22, Scalar e23, 
							  Scalar e31, Scalar e32, Scalar e33)
{
	e11 = e11;
	e12 = e12;
	e13 = e13;
	e21 = e21;
	e22 = e22;
	e23 = e23;
	e31 = e31;
	e32 = e32;
	e33 = e33;
}

inline CMatrix3x3::CMatrix3x3(CVector3f row1, CVector3f row2, CVector3f row3)
{
	e11 = row1.x; e12 = row1.y; e13 = row1.z;
	e21 = row2.x; e22 = row2.y; e23 = row2.z;
	e31 = row3.x; e32 = row3.y; e33 = row3.z;
}

inline CMatrix3x3& CMatrix3x3::operator=(const CMatrix3x3& M3x3)
{
	e11 = M3x3.e11;
	e12 = M3x3.e12;
	e13 = M3x3.e13;
	e21 = M3x3.e21;
	e22 = M3x3.e22;
	e23 = M3x3.e23;
	e31 = M3x3.e31;
	e32 = M3x3.e32;
	e33 = M3x3.e33;
	return *this;
}

inline Scalar CMatrix3x3::Determinant(void)
{
	return (
			e11*e22*e33 - 
			e11*e32*e23 +
			e21*e32*e13 -
			e21*e12*e33 +
			e31*e12*e23 - 
			e31*e22*e13
		);
}

inline CMatrix3x3 CMatrix3x3::Transpose(void)
{
	return CMatrix3x3(e11, e21, e31, e12, e22, e32, e13, e23, e33);
}

inline CMatrix3x3 CMatrix3x3::Inverse(void)
{
	// this represents the cofactor by taking the determinant 
	// of the minor of each element
	Scalar d = Determinant();

	if (d == 0) d = 1;

	// M^-1 = 1/det[M]  | 	e11  e12  e13 | 
	//					|	e21  e22  e23 |
	//					|	e31  e32  e33 |

	return CMatrix3x3(	
		 (e22*e33-e23*e32)/d,
		-(e12*e33-e13*e32)/d,
		 (e12*e23-e13*e22)/d,
		-(e21*e33-e23*e31)/d,
		 (e11*e33-e13*e31)/d,
		-(e11*e23-e13*e21)/d,
		 (e21*e32-e22*e31)/d,
		-(e11*e32-e12*e31)/d,
		 (e11*e22-e12*e21)/d 
		);	
}

inline CMatrix3x3 CMatrix3x3::GetIdentity()
{
	return CMatrix3x3(CVector3f(1.0f, 0.0f, 0.0f), CVector3f(0.0f, 1.0f, 0.0f), CVector3f(0.0f, 0.0f, 1.0f));
}
inline void CMatrix3x3::MakeMeIdentity()
{
	e11 = 1.0f; e12 = 0.0f; e13 = 0.0f;
	e21 = 0.0f; e22 = 1.0f; e23 = 0.0f;
	e31 = 0.0f; e32 = 0.0f; e33 = 1.0f;
}
inline void CMatrix3x3::MakeMeRotationX(Scalar ang)
{
	float cosTheta, sinTheta;
	cosTheta = cos(ang);
	sinTheta = sin(ang);
	e11 = 1.0f;	e12 = 0.0f;		e13 = 0.0f;
	e21 = 0.0f;	e22 = cosTheta;	e23 = -sinTheta;
	e31 = 0.0f;	e32 = sinTheta;	e33 = cosTheta;

}
inline void CMatrix3x3::MakeMeRotationY(Scalar ang)
{
	float cosTheta, sinTheta;
	cosTheta = cos(ang);
	sinTheta = sin(ang);
	e11 = cosTheta;		e12 = 0.0f;		e13 = sinTheta;
	e21 = 0.0f;			e22 = 1.0f;		e23 = 0.0f;
	e31 = -sinTheta;	e32 = 0.0f;		e33 = cosTheta;
}
inline void CMatrix3x3::MakeMeRotationZ(Scalar ang)
{
	float cosTheta, sinTheta;
	cosTheta = cos(ang);
	sinTheta = sin(ang);
	e11 = cosTheta;		e12 = -sinTheta;	e13 = 0.0f;
	e21 = sinTheta;		e22 = cosTheta;		e23 = 0.0f;
	e31 = 0.0f;			e32 = 0.0f;			e33 = 1.0f;
}
inline CMatrix3x3& CMatrix3x3::operator+=(CMatrix3x3 M3x3)
{
	e11 += M3x3.e11;
	e12 += M3x3.e12;
	e13 += M3x3.e13;
	e21 += M3x3.e21;
	e22 += M3x3.e22;
	e23 += M3x3.e23;
	e31 += M3x3.e31;
	e32 += M3x3.e32;
	e33 += M3x3.e33;
	return *this;
}
inline CMatrix3x3& CMatrix3x3::operator-=(CMatrix3x3 M3x3)
{
	e11 -= M3x3.e11;
	e12 -= M3x3.e12;
	e13 -= M3x3.e13;
	e21 -= M3x3.e21;
	e22 -= M3x3.e22;
	e23 -= M3x3.e23;
	e31 -= M3x3.e31;
	e32 -= M3x3.e32;
	e33 -= M3x3.e33;
	return *this;
}
inline CMatrix3x3& CMatrix3x3::operator*=(Scalar scalar)
{
	e11 *= scalar;
	e12 *= scalar;
	e13 *= scalar;
	e21 *= scalar;
	e22 *= scalar;
	e23 *= scalar;
	e31 *= scalar;
	e32 *= scalar;
	e33 *= scalar;
	return *this;
}
inline CMatrix3x3& CMatrix3x3::operator/=(Scalar scalar)
{
	if (scalar == 0.0f)
		return *this;
	e11 /= scalar;
	e12 /= scalar;
	e13 /= scalar;
	e21 /= scalar;
	e22 /= scalar;
	e23 /= scalar;
	e31 /= scalar;
	e32 /= scalar;
	e33 /= scalar;
	return *this;
}
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Additional Matrix Operations, will come in handy
// protypes;
inline CMatrix3x3 operator+(CMatrix3x3 M3x3a, CMatrix3x3 M3x3b);
inline CMatrix3x3 operator-(CMatrix3x3 M3x3a, CMatrix3x3 M3x3b);
inline CMatrix3x3 operator*(CMatrix3x3 M3x3, CMatrix3x3 M3x3b);
inline CMatrix3x3 operator/(CMatrix3x3 M3x3, Scalar scalar);
inline CMatrix3x3 operator*(CMatrix3x3 M3x3, Scalar scalar);
inline CMatrix3x3 operator*(Scalar scalar, CMatrix3x3 M3x3);

inline CVector3f operator*(CMatrix3x3 M3x3, CVector3f v3f);
inline CVector3f operator*(CVector3f v3f, CMatrix3x3 M3x3);

// Definitions
inline CMatrix3x3 operator+(CMatrix3x3 M3x3a, CMatrix3x3 M3x3b)
{
	return CMatrix3x3(M3x3a.e11+M3x3b.e11, M3x3a.e12+M3x3b.e12, M3x3a.e13+M3x3b.e13,
					  M3x3a.e21+M3x3b.e21, M3x3a.e22+M3x3b.e22, M3x3a.e23+M3x3b.e23,
					  M3x3a.e31+M3x3b.e31, M3x3a.e32+M3x3b.e32, M3x3a.e33+M3x3b.e33);
}
inline CMatrix3x3 operator-(CMatrix3x3 M3x3a, CMatrix3x3 M3x3b)
{
	return CMatrix3x3(M3x3a.e11-M3x3b.e11, M3x3a.e12-M3x3b.e12, M3x3a.e13-M3x3b.e13,
					  M3x3a.e21-M3x3b.e21, M3x3a.e22-M3x3b.e22, M3x3a.e23-M3x3b.e23,
					  M3x3a.e31-M3x3b.e31, M3x3a.e32-M3x3b.e32, M3x3a.e33-M3x3b.e33);
}
inline CMatrix3x3 operator*(CMatrix3x3 M3x3a, CMatrix3x3 M3x3b)
{
	// Matrix multiply is row and col dependent
	// row1 * col1 ... row3 * col3
	// 11 12 13     11 12 13   
	// 21 22 23  *  21 22 23  
	// 31 32 33     31 32 33     

	return CMatrix3x3(
					  // Row 1
					  M3x3a.e11*M3x3b.e11 + M3x3a.e12*M3x3b.e21 + M3x3a.e13*M3x3b.e31, 
					  M3x3a.e11*M3x3b.e12 + M3x3a.e12*M3x3b.e22 + M3x3a.e13*M3x3b.e32,
					  M3x3a.e11*M3x3b.e13 + M3x3a.e12*M3x3b.e23 + M3x3a.e13*M3x3b.e33, 
					  // Row 2
					  M3x3a.e21*M3x3b.e13 + M3x3a.e22*M3x3b.e23 + M3x3a.e23*M3x3b.e33,
					  M3x3a.e21*M3x3b.e12 + M3x3a.e22*M3x3b.e22 + M3x3a.e23*M3x3b.e32,
                      M3x3a.e21*M3x3b.e13 + M3x3a.e22*M3x3b.e23 + M3x3a.e23*M3x3b.e33,
					  // Row 3
					  M3x3a.e31*M3x3b.e13 + M3x3a.e32*M3x3b.e23 + M3x3a.e33*M3x3b.e33,
					  M3x3a.e31*M3x3b.e12 + M3x3a.e32*M3x3b.e22 + M3x3a.e33*M3x3b.e32,
                      M3x3a.e31*M3x3b.e13 + M3x3a.e32*M3x3b.e23 + M3x3a.e33*M3x3b.e33			
					 );
}
inline CMatrix3x3 operator/(CMatrix3x3 M3x3, Scalar scalar)
{
	if (scalar == 0.0f)
		return M3x3;
	return CMatrix3x3
		(
			M3x3.e11/scalar, M3x3.e12/scalar, M3x3.e13/scalar,
			M3x3.e21/scalar, M3x3.e22/scalar, M3x3.e23/scalar,
			M3x3.e31/scalar, M3x3.e32/scalar, M3x3.e33/scalar
		);
}
inline CMatrix3x3 operator*(CMatrix3x3 M3x3, Scalar scalar)
{
	return CMatrix3x3
		(
			M3x3.e11*scalar, M3x3.e12*scalar, M3x3.e13*scalar,
			M3x3.e21*scalar, M3x3.e22*scalar, M3x3.e23*scalar,
			M3x3.e31*scalar, M3x3.e32*scalar, M3x3.e33*scalar
		);
}
inline CMatrix3x3 operator*(Scalar scalar, CMatrix3x3 M3x3)
{
	return CMatrix3x3
		(
		M3x3.e11*scalar, M3x3.e12*scalar, M3x3.e13*scalar,
		M3x3.e21*scalar, M3x3.e22*scalar, M3x3.e23*scalar,
		M3x3.e31*scalar, M3x3.e32*scalar, M3x3.e33*scalar
		);
}
inline CVector3f operator*(CMatrix3x3 M3x3, CVector3f v3f)
{
	return CVector3f(M3x3.e11*v3f.x + M3x3.e12*v3f.y + M3x3.e13*v3f.z,
					 M3x3.e21*v3f.x + M3x3.e22*v3f.y + M3x3.e23*v3f.z, 
					 M3x3.e31*v3f.x + M3x3.e32*v3f.y + M3x3.e33*v3f.z);
}

inline CVector3f operator*(CVector3f v3f, CMatrix3x3 M3x3)
{
	return CVector3f(M3x3.e11*v3f.x + M3x3.e12*v3f.y + M3x3.e13*v3f.z,
				     M3x3.e21*v3f.x + M3x3.e22*v3f.y + M3x3.e23*v3f.z, 
				     M3x3.e31*v3f.x + M3x3.e32*v3f.y + M3x3.e33*v3f.z);
}

//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Class CQuaternion
//				Description of a Quaternion, contains a vector3f and a float
//				this is meant to simplify rotations, this is my first with 
//				this subject so lets see if it works.  hehe
class CQuaternion
{
public:
	CVector3f v;		// vector component : x, y, z
	Scalar	  n;		// Scalarcomponent

	CQuaternion(void);
	~CQuaternion(void);
	CQuaternion(const CQuaternion& q);
	CQuaternion(Scalar n, Scalar x, Scalar y, Scalar z);
	CQuaternion& operator=(const CQuaternion& q);

	Scalar Magnitude();
	CVector3f GetVector();
	Scalar	  GetScalar();

	CQuaternion operator+=(CQuaternion q);
	CQuaternion operator-=(CQuaternion q);
	CQuaternion operator*=(Scalar s);
	CQuaternion operator/=(Scalar s);
	CQuaternion operator-(void);	// return -x, -y, -z n

};

inline CQuaternion::CQuaternion(void)
{
	n = 0.0f;
	v.x = v.y = v.z = 0.0f;
}
inline CQuaternion::~CQuaternion(void){}
inline CQuaternion::CQuaternion(const CQuaternion& q)
{
	n = q.n;
	v.x = q.v.x;
	v.y = q.v.y;
	v.z = q.v.z;

}
inline CQuaternion::CQuaternion(Scalar n, Scalar x, Scalar y, Scalar z)
{
	n = n; v.x = x; v.y = y; v.z = z;
}
inline CQuaternion& CQuaternion::operator=(const CQuaternion& q)
{
	n = q.n; v.x = q.v.x; v.y = q.v.y; v.z = q.v.z;
	return *this;
}

inline Scalar CQuaternion::Magnitude()
{
	return (float)sqrt(n*n + v.x*v.x + v.y*v.y + v.z*v.z);
}
inline CVector3f CQuaternion::GetVector()
{
	return CVector3f(v);
}
inline Scalar	  CQuaternion::GetScalar()
{
	return n;
}

inline CQuaternion CQuaternion::operator+=(CQuaternion q)
{
	n	+= q.n;
	v.x += q.v.x;
	v.y += q.v.y;
	v.z += q.v.z;
	return *this;
}
inline CQuaternion CQuaternion::operator-=(CQuaternion q)
{
	n	-= q.n;
	v.x -= q.v.x;
	v.y -= q.v.y;
	v.z -= q.v.z;
	return *this;
}
inline CQuaternion CQuaternion::operator*=(Scalar s)
{
	n	*= s;
	v.x *= s;
	v.y *= s;
	v.z *= s;
	return *this;
}
inline CQuaternion CQuaternion::operator/=(Scalar s)
{
	if (s == 0)
		return *this;
	n	/= s;
	v.x /= s;
	v.y /= s;
	v.z /= s;
	return *this;
}
inline CQuaternion CQuaternion::operator-(void)	// return -x, -y, -z n
{
	return CQuaternion(n, -v.x, -v.y, -v.z);
}

//	end of Quaternion Class
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Additional Quaternion operations
// prototypes
inline CQuaternion operator+(CQuaternion q1, CQuaternion q2);
inline CQuaternion operator-(CQuaternion q1, CQuaternion q2);
inline CQuaternion operator*(CQuaternion q1, CQuaternion q2);
inline CQuaternion operator*(CQuaternion q1, Scalar s);
inline CQuaternion operator*(Scalar s, CQuaternion q1);
inline CQuaternion operator*(CQuaternion q1, CVector3f v1);
inline CQuaternion operator*(CVector3f v1, CQuaternion q1);
inline CQuaternion operator/(CQuaternion q1, Scalar s);
inline Scalar GetAngle(CQuaternion q);
inline CVector3f GetAxis(CQuaternion q);
inline CQuaternion QRotate(CQuaternion q1, CQuaternion q2);
inline CVector3f QRotate(CQuaternion q, CVector3f v);
inline CQuaternion MakeQFromEulerAngles(Scalar x, Scalar y, Scalar z);
inline CVector3f   MakeEulerAnglesFromQ(CQuaternion q);
inline CMatrix3x3  MakeMatrixFromQ(CQuaternion q);

// Definitions
inline CQuaternion operator+(CQuaternion q1, CQuaternion q2)
{
	return CQuaternion(q1.n+q2.n, q1.v.x+q2.v.x, q1.v.y+q2.v.y, q1.v.z+q2.v.z);
}
inline CQuaternion operator-(CQuaternion q1, CQuaternion q2)
{
	return CQuaternion(q1.n-q2.n, q1.v.x-q2.v.x, q1.v.y-q2.v.y, q1.v.z-q2.v.z);
}
inline CQuaternion operator*(CQuaternion q1, CQuaternion q2)
{
	return CQuaternion(q1.n*q2.n   - q1.v.x*q2.v.x - q1.v.y*q2.v.y - q1.v.z*q2.v.z,
					   q1.n*q2.v.x + q1.v.x*q2.n + q1.v.y*q2.v.z - q1.v.z*q2.v.y,
					   q1.n*q2.v.y + q1.v.y*q2.n + q1.v.z*q2.v.x - q1.v.x*q2.v.z,
					   q1.n*q2.v.z + q1.v.z*q2.n + q1.v.x*q2.v.y - q1.v.y*q2.v.x);
}
inline CQuaternion operator*(CQuaternion q1, Scalar s)
{
	return CQuaternion(q1.n * s, q1.v.x * s, q1.v.y * s, q1.v.z * s);
}
inline CQuaternion operator*(Scalar s, CQuaternion q1)
{
	return CQuaternion(q1.n * s, q1.v.x * s, q1.v.y * s, q1.v.z * s);
}
inline CQuaternion operator*(CQuaternion q1, CVector3f v1)
{
	return	CQuaternion(	-(q1.v.x*v1.x + q1.v.y*v1.y + q1.v.z*v1.z),
								q1.n*v1.x + q1.v.y*v1.z - q1.v.z*v1.y,
								q1.n*v1.y + q1.v.z*v1.x - q1.v.x*v1.z,
								q1.n*v1.z + q1.v.x*v1.y - q1.v.y*v1.x);
}
inline CQuaternion operator*(CVector3f v1, CQuaternion q1)
{
	return	CQuaternion(	-(q1.v.x*v1.x + q1.v.y*v1.y + q1.v.z*v1.z),
								q1.n*v1.x + q1.v.y*v1.z - q1.v.z*v1.y,
								q1.n*v1.y + q1.v.z*v1.x - q1.v.x*v1.z,
								q1.n*v1.z + q1.v.x*v1.y - q1.v.y*v1.x);
}
inline CQuaternion operator/(CQuaternion q1, Scalar s)
{
	if (s == 0.0f)
		return q1;
	return CQuaternion(q1.n/s, q1.v.x/s, q1.v.y/s, q1.v.z/s);
}
inline Scalar GetAngle(CQuaternion q)
{
	return (float)2*acos(q.n);
}
inline CVector3f GetAxis(CQuaternion q)
{
	CVector3f vec;
	Scalar	  m;
	vec = q.GetVector();
	m	= vec.Magnitude();

	if (m <= _FloatTolerance_)
		return CVector3f();
	else
		return vec/m;
}
inline CQuaternion QRotate(CQuaternion q1, CQuaternion q2)
{
	return q1*q2*(-q1);
}
inline CVector3f QRotate(CQuaternion q, CVector3f v)
{
	CQuaternion temp;
	temp = q*v*(-q);
	return temp.GetVector();
}
inline CQuaternion MakeQFromEulerAngles(Scalar x, Scalar y, Scalar z)
{
	//////////////////////////////////////////////////////////////////////////
	// qroll  = { cos(roll/2), [sin(roll/2)]i + 0j + 0k }
	// qpitch = { cos(pitch/2), 0i + [sin(pitch/2)j + 0k }
	// qyaw	  = { cos(yaw/2), 0i + 0j + [sin(yaw/2)k }
	// qfinal = qyaw * qpitch * qroll
	//////////////////////////////////////////////////////////////////////////

	CQuaternion q;
	Scalar roll  = DegreesToRadians(x);
	Scalar pitch = DegreesToRadians(y);
	Scalar yaw   = DegreesToRadians(z);

	Scalar	cyaw, cpitch, croll, syaw, spitch, sroll;
	Scalar	cyawcpitch, syawspitch, cyawspitch, syawcpitch;

	cyaw = cos(0.5f * yaw);
	cpitch = cos(0.5f * pitch);
	croll = cos(0.5f * roll);
	syaw = sin(0.5f * yaw);
	spitch = sin(0.5f * pitch);
	sroll = sin(0.5f * roll);

	cyawcpitch = cyaw*cpitch;
	syawspitch = syaw*spitch;
	cyawspitch = cyaw*spitch;
	syawcpitch = syaw*cpitch;

	q.n   = (Scalar) (cyawcpitch * croll + syawspitch * sroll);
	q.v.x = (Scalar) (cyawcpitch * sroll - syawspitch * croll); 
	q.v.y = (Scalar) (cyawspitch * croll + syawcpitch * sroll);
	q.v.z = (Scalar) (syawcpitch * croll - cyawspitch * sroll);

	return q;
}
inline CVector3f   MakeEulerAnglesFromQ(CQuaternion q)
{
	//////////////////////////////////////////////////////////////////////////
	// R = e11 e12 e13
	//	   e21 e22 e23
	//	   e31 e32 e33
	// q = n, i + j + k
	// R11 = n^2 + x^2 - y^2 - z^2
	// R21 = 2xy + 2zn
	// R31 = 2zx = 2yn
	// R12 = 2xy - 2zn
	// R22 = n^2 - x^2 + y^2 - z^2
	// R32 = 2zy + 2xn
	// R13 = 2xz + 2yn
	// R23 = 2yz - 2xn
	// R33 = n^2 - x^2 - y^2 + z^2
	double	r11, r21, r31, r32, r33, r12, r13;
	double	q00, q11, q22, q33;
	double	tmp;
	CVector3f	u;

	q00 = q.n * q.n;
	q11 = q.v.x * q.v.x;
	q22 = q.v.y * q.v.y;
	q33 = q.v.z * q.v.z;

	r11 = q00 + q11 - q22 - q33;
	r21 = 2 * (q.v.x*q.v.y + q.n*q.v.z);
	r31 = 2 * (q.v.x*q.v.z - q.n*q.v.y);
	r32 = 2 * (q.v.y*q.v.z + q.n*q.v.x);
	r33 = q00 - q11 - q22 + q33;

	tmp = fabs(r31);
	if(tmp > 0.999999)
	{
		r12 = 2 * (q.v.x*q.v.y - q.n*q.v.z);
		r13 = 2 * (q.v.x*q.v.z + q.n*q.v.y);

		u.x = RadiansToDegrees(0.0f); //roll
		u.y = RadiansToDegrees((float) (-(_PI_/2) * r31/tmp)); // pitch
		u.z = RadiansToDegrees((float) atan2(-r12, -r31*r13)); // yaw
		return u;
	}

	u.x = RadiansToDegrees((float) atan2(r32, r33)); // roll
	u.y = RadiansToDegrees((float) asin(-r31));		 // pitch
	u.z = RadiansToDegrees((float) atan2(r21, r11)); // yaw
	return u;
}
inline CMatrix3x3  MakeMatrixFromQ(CQuaternion q)
{
	Scalar	qnn, qxx, qyy, qzz;

	CMatrix3x3	m;

	qnn = q.n * q.n;
	qxx = q.v.x * q.v.x;
	qyy = q.v.y * q.v.y;
	qzz = q.v.z * q.v.z;

	m.e11 = qnn + qxx - qyy - qzz;
	m.e21 = 2 * (q.v.x*q.v.y + q.n*q.v.z);
	m.e31 = 2 * (q.v.x*q.v.z - q.n*q.v.y);

	m.e12 = 2 * (q.v.x*q.v.y - q.n*q.v.z);
	m.e22 = qnn - qxx + qyy - qzz;
	m.e32 = 2 * (q.v.y*q.v.z + q.n*q.v.x);

	m.e13 = 2 * (q.v.x*q.v.z + q.n*q.v.y);
	m.e23 = 2 * (q.v.y*q.v.z - q.n*q.v.x);
	m.e33 = qnn - qxx - qyy + qzz;

	return m;
}

//  end of Additional Quaternion operations
//////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//// Class CTriangle 
////			represents a triangle in 3D space
class CTriangle
{
public:
	CVector3f pt[3];
	CVector3f n;
	CTriangle(void)
	{
		pt[0].MakeZero(); pt[1].MakeZero(); pt[2].MakeZero(); n.MakeZero();

	}
	~CTriangle(void){}
	CTriangle(CTriangle& tri);
	CTriangle(CVector3f p0, CVector3f p1, CVector3f p2);
	CTriangle &operator=(CTriangle& tri);
public:
	void CreateNormal();
	CVector3f GetCenter();
};

inline CVector3f CTriangle::GetCenter()
{
	CVector3f centroid;
	for (int vert = 0; vert < 3; ++vert)
	{
		centroid += pt[vert];
	}
	centroid /= 3;
	return centroid;
}

inline CTriangle::CTriangle(CTriangle& tri)
{
	pt[0] = tri.pt[0]; pt[1] = tri.pt[1]; pt[2] = tri.pt[2]; n = tri.n;
}
inline CTriangle::CTriangle(CVector3f p0, CVector3f p1, CVector3f p2)
{
	pt[0] = p0; pt[1] = p1; pt[2] = p2; 
	CreateNormal();
}
inline CTriangle &CTriangle::operator=(CTriangle& tri)
{
	pt[0] = tri.pt[0]; pt[1] = tri.pt[1]; pt[2] = tri.pt[2]; n = tri.n;
	return *this;
}
inline void CTriangle::CreateNormal()
{
	//      pt1
	//   pt2    pt3

	CVector3f edge1 = pt[1] - pt[0];
	CVector3f edge2 = pt[2] - pt[0];
	n = edge1.CrossProduct(edge2);
}

//////////////////////////////////////////////////////////////////////////
// ***********************************************************************
// ***********************************************************************


// ***********************************************************************
// ***********************************************************************
//////////////////////////////////////////////////////////////////////////
// Vector operations and methods
inline void  CVector3f::Normalize()
{
	Scalar mag = Magnitude();
	if (mag)
	{
		x /= mag;
		y /= mag;
		z /= mag;
	}
}

inline void  CVector3f::Normalize(CVector3f &v)		// normalize the referenced vector
{
	Scalar mag = v.Magnitude();
	if (mag)
	{
		v.x /= mag;
		v.y /= mag;
		v.z /= mag;
	}
}

inline Scalar CVector3f::Magnitude()
{
	return (float)sqrt((x*x)+(y*y)+(z*z));
}

inline Scalar CVector3f::DotProduct(CVector3f v)
{
	// this dot v
	return ((x * v.x)+(y * v.y)+(z * v.z));
}

inline void CVector3f::MakeZero()
{
	x = y = z = 0.0f;
}
// sets and returns *this
inline CVector3f CVector3f::CrossProduct(CVector3f v1, CVector3f v2)
{
	// this = v1.x	 v1.y	v1.z
	//			   \
	//		  v2.x	 v2.y	v2.z	
	x = v1.y * v2.z - v1.z * v2.y;
	y = v1.z * v2.z - v1.z * v2.x;
	z = v1.x * v2.y - v1.y * v2.x;

	return (CVector3f(x, y, z));
}

inline CVector3f CVector3f::CrossProduct(CVector3f v)
{
	return (CVector3f(y * v.z - z * v.y,
		z * v.z - z * v.x, 
		x * v.y - y * v.x));
}

inline Scalar CVector3f::TriplefloatCrossproduct(CVector3f v1, CVector3f v2, CVector3f v3)
{
	// triple Scalarproduct (v1 dot (v2 cross v3))
	return Scalar(	(v1.x * (v2.y * v3.z - v2.z * v3.y)) +
		(v1.y * (-v2.x * v3.z + v2.z * v3.x)) +
		(v1.z * (v2.x * v3.y - v2.y * v3.x)) );

	//u*(v^w);
}

inline CVector3f::CVector3f(void){ MakeZero(); }
inline CVector3f::~CVector3f(void){
}
inline CVector3f::CVector3f(const CVector3f&v) 
{
	x = v.x; y = v.y; z = v.z;
}
inline CVector3f::CVector3f(Scalar fx, Scalar fy, Scalar fz)
{
	x = fx; y = fy; z = fz;
}
inline CVector3f::CVector3f(Scalar s)
{
	x = s; y = s; z = s;
}
//inline CVector3f& CVector3f::operator=(CVector3f& v)
//{
//	x = v.x; y = v.y; z = v.z;
//	return *this;
//}
inline CVector3f& CVector3f::operator=(const CVector3f& v)
{
	x = v.x; y = v.y; z = v.z;
	return *this;
}
inline CVector3f& CVector3f::operator=(Scalar s)
{
	x = s; y = s; z = s;
	return *this;
}
inline CVector3f CVector3f::operator-(void)			// overload negation
{
	CVector3f vec = *this;
	return CVector3f(vec*-1);	
}

// ret = this (op) v
inline CVector3f CVector3f::operator+(const CVector3f& v)
{
	return CVector3f((x + v.x), (y + v.y), (z + v.z));
}

inline CVector3f CVector3f::operator-(const CVector3f& v)
{
	return CVector3f((x - v.x), (y - v.y), (z - v.z));
}
inline CVector3f CVector3f::operator*(const CVector3f& v)
{
	return CVector3f((x * v.x), (y * v.y), (z * v.z));
}
inline CVector3f CVector3f::operator/(const CVector3f& v)
{
	CVector3f t;
	if (v.x == 0.0f || v.y == 0.0f || v.z == 0.0f)
		return CVector3f(v.x, v.y, v.z);

	return CVector3f(x/v.x, y/v.y, z/v.z);
}

inline CVector3f CVector3f::operator+(const Scalar s)
{
	return CVector3f(x + s, y + s, z + s);
}
inline CVector3f CVector3f::operator-(const Scalar s)
{
	return CVector3f(x - s, y - s, z - s);
}
inline CVector3f CVector3f::operator*(const Scalar s)
{
	return CVector3f(x * s, y * s, z * s);
}
inline CVector3f CVector3f::operator/(const Scalar s)
{
	if (s == 0.0f)
		return *this;
	return CVector3f(x / s, y / s, z / s);
}

inline void CVector3f::operator+=(CVector3f v)
{
	x += v.x; 
	y += v.y; 
	z += v.z;
}
inline void CVector3f::operator-=(CVector3f v)
{
	x -= v.x; 
	y -= v.y; 
	z -= v.z;
}
inline void CVector3f::operator*=(CVector3f v)
{
	x *= v.x; y *= v.y; z *= v.z;
}
inline void CVector3f::operator/=(CVector3f v)
{
	if (v.x != 0.0f)
		x /= v.x; 
	if (v.y != 0.0f)
		y /= v.y; 
	if (v.z != 0.0f)
		z /= v.z;
}

inline void CVector3f::operator+=(Scalar s)
{
	x += s; y += s; z += s;
}
inline void CVector3f::operator-=(Scalar s)
{
	x -= s; y -= s; z -= s;
}
inline void CVector3f::operator*=(Scalar s)
{
	x *= s; y *= s; z *= s;
}
inline void CVector3f::operator/=(Scalar s)
{
	if (s == 0.0f)
		return;
	x /= s; y /= s; z /= s;
}

inline bool CVector3f::operator!=(CVector3f v)
{
	// return true if they are not equal
	bool Isit = true;
	if (x == v.x && y == v.y && z == v.z)  // if they are all equal
		Isit = false;					// return they are not equal
	return Isit;
}
inline bool CVector3f::operator==(CVector3f v)
{
	// return true if they are equal
	bool Isit = false;
	if (x == v.x && y == v.y && z == v.z)  // if they are all equal
		Isit = true;					// return they are not equal
	return Isit;
}

inline CVector3f CVector3f::GetRotatedX(float angle)
{
	CVector3f Vp;
	CMatrix3x3 M;
	M.MakeMeRotationX(angle);
	Vp = M * CVector3f(x, y, z);
	return Vp;
}

inline CVector3f CVector3f::GetRotatedY(float angle)
{
	CVector3f Vp;
	CMatrix3x3 M;
	M.MakeMeRotationY(angle);
	Vp = M * CVector3f(x, y, z);
	return Vp;
}


inline CVector3f CVector3f::GetRotatedZ(float angle)
{
	CVector3f Vp;
	CMatrix3x3 M;
	M.MakeMeRotationZ(angle);
	Vp = M * CVector3f(x, y, z);
	return Vp;
}

inline void CVector3f::RotateX(float angle)
{
	CMatrix3x3 M;
	M.MakeMeRotationX(angle);
	*this = M * (*this);
}

inline void CVector3f::RotateY(float angle)
{
	CMatrix3x3 M;
	M.MakeMeRotationY(angle);
	*this = M * (*this);
}

inline void CVector3f::RotateZ(float angle)
{
	CMatrix3x3 M;
	M.MakeMeRotationZ(angle);
	*this = M * (*this);
}

inline Scalar CVector3f::GetTweenAngleRadians(CVector3f vec)
{
	CVector3f a;
	CVector3f b;

	a = *this;
	b = vec;

	a.Normalize();
	b.Normalize();

	float cosAng = a.DotProduct(b);
	float radAngle = acos(cosAng);
	return radAngle;
}

inline Scalar CVector3f::GetTweenAngleDegrees(CVector3f vec)
{
	float degAngle = GetTweenAngleRadians(vec);
	return RadiansToDegrees(degAngle);
}

inline void CVector3f::MakeRandom(float maxX /* = 10 */, float minX /* = 10 */, 
									   float maxY /* = 10 */, float minY /* = 10 */, 
									   float maxZ /* = 10 */, float minZ /* = 10 */)
{
	CVector3f randVec;
	randVec.x = GetRandomScalar(minX, maxX);
	randVec.y = GetRandomScalar(minY, maxY);
	randVec.z = GetRandomScalar(minZ, maxZ);

	*this = randVec;
}
//inline CVector3f CVector3f::GetRandomVector(int maxX /* = 10 */, int minX /* = 10 */, 
//											int maxY /* = 10 */, int minY /* = 10 */, 
//											int maxZ /* = 10 */, int minZ /* = 10 */)
//{
//	CVector3f vNew(float(rand() % maxX-minX), float(rand() % maxY - minY), float(rand() % maxZ - minZ));
//	return vNew;
//}
//
//////////////////////////////////////////////////////////////////////////



#endif