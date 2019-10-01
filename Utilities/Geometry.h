// Geometry.h
//
// Utilities

#pragma once

template <typename T>
bool IsPointInRect  // check if point is in given rectangle
(
	T const P,  // The point in question
	T const A,  // Any 3 corners of the rectangle
	T const B,  //
	T const D   //
)
{
	T const BA = B - A;
	T const DA = D - A;

	if ((P.GetXvalue() - A.GetXvalue()) * BA.GetXvalue() + (P.GetYvalue() - A.GetYvalue()) * BA.GetYvalue() < 0.0) return false;
	if ((P.GetXvalue() - B.GetXvalue()) * BA.GetXvalue() + (P.GetYvalue() - B.GetYvalue()) * BA.GetYvalue() > 0.0) return false;
	if ((P.GetXvalue() - A.GetXvalue()) * DA.GetXvalue() + (P.GetYvalue() - A.GetYvalue()) * DA.GetYvalue() < 0.0) return false;
	if ((P.GetXvalue() - D.GetXvalue()) * DA.GetXvalue() + (P.GetYvalue() - D.GetYvalue()) * DA.GetYvalue() > 0.0) return false;

	return true;
}
