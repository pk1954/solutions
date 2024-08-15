// NNetModelStorage.ixx
//
// ModelIO

export module NNetModelStorage;

import std;

using std::exception;

export class NNetModelStorage
{
public:

	inline static double const PROTOCOL_VERSION { 3.0 };
};

export struct ProtocollException : public exception
{
	ProtocollException(double const dVersion)
		: m_dVersion(dVersion)
	{}
	double m_dVersion;
};