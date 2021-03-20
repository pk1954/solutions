// ConnectionNeuron.h
//
// NNetModel

#pragma once

#include "MoreTypes.h"
#include "MicroMeterPosDir.h"
#include "Neuron.h"

using std::wcout;
using std::endl;

class ConnectionNeuron : public Neuron
{
public:
	ConnectionNeuron( MicroMeterPoint const & upCenter, ShapeType const type )
		: Neuron( upCenter, type )
	{
		wcout << __func__ << L"(" << upCenter << L", " << type << L")" << endl;
	}

	void Check() const
	{
		Neuron::CheckShape();
	}

	MicroMeterPoint const GetDirVector() const
	{
		MicroMeterPoint umVector { m_direction.IsNull() ? determineVector() : Radian2Vector(m_direction) };
		return Normalize(umVector) * GetExtension().GetValue();
	}

	MicroMeterPosDir const GetPosDir() const
	{
		return MicroMeterPosDir( GetPosition(), Vector2Radian(GetDirVector()) );
	}

	void SetPosDir( MicroMeterPosDir const posDir )
	{
		wcout << __func__ << posDir << endl;
		m_direction = posDir.GetDir();
		SetPosition ( posDir.GetPos() );
	}

	void UnlockDirection() 
	{
		m_direction.Set2Null();
	}

private:
	MicroMeterPoint const determineVector() const;

	Radian m_direction { Radian::NULL_VAL() };
};
