// ProtocolServer.ixx
//
// EvoWindows

export module ProtocolServer;

import ObserverInterface;
import EvoCoreLib;
import HistorySystem;

export class ProtocolServer : public ObserverInterface
{
public:
	ProtocolServer();
	~ProtocolServer();

	void Start(HistorySystem * const pHistSys)
	{
		m_pHistorySystem = pHistSys;
	}

	virtual void Notify(bool const);

private:
	HistorySystem * m_pHistorySystem;
};
