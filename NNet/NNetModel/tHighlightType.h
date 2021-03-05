// tHighlightType.h
//
// NNetModel

#pragma once

enum class tHighlightType
{
	normal,
	highlighted,
	targetFit,
	targetNoFit
};

static bool const IsHighlighted( tHighlightType const type )
{
	return type == tHighlightType::highlighted;
}

static bool const IsNormal( tHighlightType const type )
{
	return type == tHighlightType::normal;
}

//static bool const IsTarget( tHighlightType const type )
//{
//	return type == tHighlightType::target;
//}