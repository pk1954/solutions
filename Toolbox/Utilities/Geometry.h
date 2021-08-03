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

	if ((P.GetXvalue() - A.GetXvalue()) * BA.GetXvalue() + (P.GetYvalue() - A.GetYvalue()) * BA.GetYvalue() < 0.0f) return false;
	if ((P.GetXvalue() - B.GetXvalue()) * BA.GetXvalue() + (P.GetYvalue() - B.GetYvalue()) * BA.GetYvalue() > 0.0f) return false;
	if ((P.GetXvalue() - A.GetXvalue()) * DA.GetXvalue() + (P.GetYvalue() - A.GetYvalue()) * DA.GetYvalue() < 0.0f) return false;
	if ((P.GetXvalue() - D.GetXvalue()) * DA.GetXvalue() + (P.GetYvalue() - D.GetYvalue()) * DA.GetYvalue() > 0.0f) return false;

	return true;
}

template <typename T>
bool IsPointInRect2  // check if point is in given rectangle
(
	T const point,  // The point in question
	T const start,  // The center of one side of the reactangle
	T const end,    // The center of the opposite side of the reactangle
	T const ext     // The vector from start(end) to a corner of that side
)
{
	T const corner1 { start + ext };
	T const corner2 { start - ext };
	T const corner3 { end   + ext };

	return IsPointInRect< T >(point, corner1, corner2, corner3);
}
