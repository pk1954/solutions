// tHighlightType.h
//
// NNetModel

#pragma once

enum class tHighlightType
{
	normal,
	selectedTemp,  // temporarily selected, cursor over shape
	selectedPerm,  // permenently selected
	target
};

static bool const IsSelected( tHighlightType const type )
{
	return (type == tHighlightType::selectedTemp) || (type == tHighlightType::selectedPerm);
}

static bool const IsNormal( tHighlightType const type )
{
	return type == tHighlightType::normal;
}

static bool const IsTarget( tHighlightType const type )
{
	return type == tHighlightType::target;
}