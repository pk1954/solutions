// NNetModel.h 
//
// NNetModel

#pragma once

#include "util.h"
#include "MoreTypes.h"
#include "observable.h"
#include "observerInterface.h"
#include "ParameterType.h"
#include "NNetParameters.h"
#include "ModelDescription.h"
#include "UPSigGenList.h"
#include "UPSensorList.h"
#include "MonitorData.h"
#include "UPNobList.h"
#include "Pipe.h"

using std::unique_ptr;
using std::move;

class BaseKnot;

class NNetModel
{
public:

	// const functions

	bool operator==(NNetModel const &) const;

	template <Nob_t T>
	T GetNobConstPtr(NobId const id) const
	{
		Nob const * const pNob { GetConstNob(id) };
		return (pNob && HasType<T>(* pNob)) ? static_cast<T>(pNob) : nullptr;
	}

	void CheckModel() const;
	void CheckId(NobId const) const;
	void DumpModel (char const * const, int const) const;

	float GetParameter(ParamType::Value const p) const { return m_param.GetParameterValue(p); }

	Nob      const * GetConstNob    (NobId const) const;
	BaseKnot const * GetStartKnotPtr(NobId const) const;
	BaseKnot const * GetEndKnotPtr  (NobId const) const;

	NobId GetStartKnotId(NobId const) const;
	NobId GetEndKnotId  (NobId const) const;

	wstring GetModelFilePath() const { return m_wstrModelFilePath; }
	size_t  Size()             const { return m_Nobs.Size(); }

	bool  GetDescriptionLine(int const, wstring &) const;
	
	NobId FindNobAt(MicroMeterPnt const & umPoint, auto const & crit) const // Template!
	{	
		NobId idRes { NO_NOB };

		idRes = m_Nobs.FindNobAt(umPoint, [&crit](Nob const & s) { return s.IsIoConnector() && crit(s); });
		if (IsDefined(idRes))
			return idRes;

		idRes = m_Nobs.FindNobAt(umPoint, [&crit](Nob const & s) { return s.IsAnyNeuron() && (!s.HasParentNob()) && crit(s); });
		if (IsDefined(idRes))
			return idRes;

		idRes = m_Nobs.FindNobAt(umPoint, [&crit](Nob const & s) { return s.IsKnot() && crit(s); }); 	
		if (IsDefined(idRes))
			return idRes;

		idRes = m_Nobs.FindNobAt(umPoint, [&crit](Nob const & s) { return s.IsPipe() && crit(s); });
		if (IsDefined(idRes))
			return idRes;

		return NO_NOB;
	}

	// functions affecting Nob data, not model data

	void RecalcAllNobs() const;
	void ClearDynamicData();

    // access functions to members 

	UPSigGenList const & GetSigGenList()  const { return m_sigGenList; }
	UPSigGenList       & GetSigGenList()        { return m_sigGenList; }
	UPSensorList const & GetSensorList()  const { return m_sensorList; }
	UPSensorList       & GetSensorList()        { return m_sensorList; }
	UPNobList    const & GetUPNobs()      const { return m_Nobs; }
	UPNobList          & GetUPNobs()            { return m_Nobs; }
	MonitorData  const & GetMonitorData() const { return m_monitorData; }
	MonitorData        & GetMonitorData()       { return m_monitorData; }
	Param        const & GetParams()      const { return m_param; }
	Param              & GetParams()            { return m_param; }

	// non const functions

	Nob * GetNob (NobId const);

	bool  Compute();
	void  ResetModel();
	float SetParam(ParamType::Value const, float const);
	void  SelectSubtree(BaseKnot &, bool const);
	void  Reconnect(NobId const);

	void DeselectAllNobs     () const               { m_Nobs.SelectAllNobs(false); }
	void SetModelFilePath    (wstring const & wstr) { m_wstrModelFilePath = wstr; }
	void AddDescriptionLine  (wstring const & wstr) { m_description.AddDescriptionLine(wstr); }
	void DescriptionComplete ()                     { m_description.DescriptionComplete(); }
	void SetDescriptionUI    (DescriptionUI & i)    { m_description.SetDescriptionUI(i); }
	void SetHighSigObservable(Observable * obs)     { m_monitorData.SetHighSigObservable(obs); }

private:

	UPNobList        m_Nobs;
	UPSigGenList     m_sigGenList;
	UPSensorList     m_sensorList;
	ModelDescription m_description;
	MonitorData      m_monitorData;
	Param            m_param;
	wstring          m_wstrModelFilePath { L"" };
};
