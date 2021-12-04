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

static bool IsHighlighted(tHighlight const type)
{
	return type == tHighlight::highlighted;
}

static bool IsNormal(tHighlight const type)
{
	return type == tHighlight::normal;
}
