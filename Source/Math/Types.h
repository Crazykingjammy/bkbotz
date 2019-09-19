#ifndef __Types_h__
#define __Types_h__


class EXColor
{
public:
	float r,g,b,a;

public:

	// A Default Constructor
	EXColor() { r = g = b = a = 2.0f;}


	//Constructor to initialize data at instance.
	EXColor(float R, float G, float B, float A)
	{ r = R; g = G; b = B; a = A; }

	void operator=(float num)
	{  r = g = b = a = num; }
	void operator=(EXColor color)
	{ r = color.r; g = color.g; b = color.b; a = color.a;  }

	void operator*=(float num)
	{ r = r * num; g = g * num; b = b * num; a = a * num; }
	
	void operator*=(EXColor color)
	{ r = r * color.r; g = g * color.g; b = b * color.b; a = a * color.a; }

	void operator+=(float num)
	{r = r + num; g = g + num; b = b + num; a = a + num; }

	void operator-=(float num)
	{r = r - num; g = g - num; b = b - num; a = a - num; }

	EXColor operator+(EXColor color)
	{ return EXColor(color.r + r, color.g + g, color.b + b, color.a + a); }

	EXColor operator+(float num)
	{ return EXColor(num + r, num + g, num + b, num + a); }

	EXColor operator-(EXColor color)
	{ return EXColor(r - color.r, g - color.g, b - color.b, a - color.a);  }
	
	EXColor operator-(float num)
	{ return EXColor(r - num, g - num, b - num, a - num); }

	EXColor operator*(EXColor color)
	{ return EXColor(color.r * r, color.g * g, color.b * b, color.a * a); }

	EXColor operator*(float num)
	{ return EXColor(num * r, num * g, num * b, num * a); }

	EXColor operator/(EXColor color)
	{ return EXColor(r / color.r, g / color.g, b / color.b, a / color.a);  }



};




#endif