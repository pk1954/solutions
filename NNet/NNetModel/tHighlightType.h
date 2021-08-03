// tHighlightType.h
//
// NNetModel

#pragma once

enum class tHighlight
{
	normal,
	highlighted,
	targetFit,
	targetNoFit
};

static bool const IsHighlighted(tHighlight const type)
{
	return type == tHighlight::highlighted;
}

static bool const IsNormal(tHighlight const type)
{
	return type == tHighlight::normal;
}

//static bool const IsTarget(tHighlight const type)
//{
//	return type == tHighlight::target;
//}