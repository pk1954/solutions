// NNetModelStorage.ixx
//
// ModelIO

module;

#include <exception>

export module NNetModelStorage;

using std::exception;

export class NNetModelStorage
{
public:

	inline static double const PROTOCOL_VERSION { 2.0 };
};

export struct ProtocollException : public exception
{
	ProtocollException(double const dVersion)
		: m_dVersion(dVersion)
	{}
	double m_dVersion;
};