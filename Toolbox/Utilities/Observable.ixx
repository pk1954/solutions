// Observable.ixx : 
//
// Toolbox\Utilities

export module Observable;

import ViewCollection;
import ObserverInterface;

export class Observable
{
public:

	void RegisterObserver(ObserverInterface & observer)
	{
		m_observers.Register(observer);
	}

	void UnregisterObserver(ObserverInterface const & observer)
	{
		m_observers.Unregister(observer);
	}

	void UnregisterAllObservers()
	{
		m_observers.Clear();
	}

	void NotifyAll(bool const bImmediately)
	{
		m_observers.NotifyAll(bImmediately);
	}

private:
	ViewCollection m_observers;
};
