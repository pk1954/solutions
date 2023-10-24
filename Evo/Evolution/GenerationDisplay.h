// GenerationDisplay.h
//
// Evolution

module;

#include "EvoReadBuffer.h"
#include "ObserverInterface.h"

class StatusBar;

class GenerationDisplay : public ObserverInterface
{
public:
	GenerationDisplay(StatusBar *, EvoReadBuffer *, int);

	virtual void Notify(bool const);

private:
	class RefreshRate;

	RefreshRate * m_pRefreshRate;
};

