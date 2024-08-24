// GenerationDisplay.ixx
//
// Evolution

export module GenerationDisplay;

import ObserverInterface;
import EvoReadBuffer;
import StatusBar;

class GenerationDisplay : public ObserverInterface
{
public:
	GenerationDisplay(StatusBar *, EvoReadBuffer *, int);

	virtual void Notify(bool const);

private:
	class RefreshRate;

	RefreshRate * m_pRefreshRate;
};
