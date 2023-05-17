// SignalParameters.ixx
//
// NNetModel

export module NNetModel:SignalParameters;

import Observable;
import Types;
import :SigGenStaticData;

export class SignalParameters : public Observable
{
public:

	fMicroSecs TimeResolution() const { return m_usResolution; }
	
	void SetResolution(fMicroSecs const us) { m_usResolution = us; }

	SigGenStaticData & GetSigGenStaticData() 
	{ 
		return m_sigGenData; 
	}

	void SetSigGenStaticData(SigGenStaticData const& data)
	{
		m_sigGenData = data;
		NotifyAll(false);
	}

private:
	SigGenStaticData m_sigGenData   { };
	fMicroSecs       m_usResolution { 100._MicroSecs };
};
