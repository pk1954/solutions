// NNetModel.h 
//
// NNetModel

#pragma once

#include "util.h"
#include "MoreTypes.h"
#include "observable.h"
#include "ObserverInterface.h"
#include "ParameterType.h"
#include "NNetParameters.h"
#include "ModelDescription.h"
#include "SignalGenerator.h"
#include "UPSigGenList.h"
#include "MonitorData.h"
#include "UPNobList.h"
#include "Pipe.h"
#include "BaseKnot.h"

using std::unique_ptr;
using std::move;

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

	Nob const * GetConstNob(NobId const) const;

	float GetParameter(ParamType::Value const p) const { return m_param.GetParameterValue(p); }

	MicroMeterPnt    GetNobPos      (NobId const id) const { return GetNobConstPtr<Nob  const *>(id)->GetPos         (); }
	BaseKnot const * GetStartKnotPtr(NobId const id) const { return GetNobConstPtr<Pipe const *>(id)->GetStartKnotPtr(); }
	BaseKnot const * GetEndKnotPtr  (NobId const id) const { return GetNobConstPtr<Pipe const *>(id)->GetEndKnotPtr  (); }

	NobId GetStartKnotId(NobId const idPipe) const { return GetStartKnotPtr(idPipe)->GetId(); }
	NobId GetEndKnotId  (NobId const idPipe) const { return GetEndKnotPtr  (idPipe)->GetId(); }

	UPNobList   const & GetUPNobs()        const { return m_Nobs; }
	MonitorData const & GetMonitorData()   const { return m_monitorData; }
	Param       const & GetParams()        const { return m_param; }
	wstring             GetModelFilePath() const { return m_wstrModelFilePath; }
	size_t              Size()             const { return m_Nobs.Size(); }

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
	void RecalcFilters();
	void ClearDynamicData();

	// non const functions

	Nob * GetNob (NobId const);

	bool  Compute();
	void  ResetModel();
	float SetParam(ParamType::Value const, float const);
	void  SelectSubtree(BaseKnot &, bool const);
	void  Reconnect(NobId const);

	SignalGenerator * StdSigGen() { return m_sigGenList.StdSigGen(); }

	UPSigGen NewSigGen   ()                           { return      m_sigGenList.NewSigGen   ();      }
	UPSigGen NewSigGen   (wstring const & name)       { return      m_sigGenList.NewSigGen   (name);  }
	UPSigGen RemoveSigGen(wstring const & name)       { return move(m_sigGenList.RemoveSigGen(name)); }
	UPSigGen RemoveSigGen(SigGenId const id)          { return move(m_sigGenList.RemoveSigGen(id)); }
	UPSigGen RemoveSigGen()                           { return move(m_sigGenList.RemoveSigGen()); }
	bool     IsInList    (wstring const & name) const { return      m_sigGenList.IsInList    (name);  }
	bool     IsValid     (SigGenId const  id  ) const { return      m_sigGenList.IsValid     (id);    }
	SigGenId FindSigGen  (wstring const & name) const { return      m_sigGenList.FindSigGen  (name);  }
	SigGenId PushSigGen  (UPSigGen upSigGen)          { return      m_sigGenList.PushSigGen(move(upSigGen)); }
	UPSigGen PopSigGen   ()                           { return move(m_sigGenList.PopSigGen()); }

	void InsertSigGen(UPSigGen up, SigGenId const id) { return      m_sigGenList.InsertSigGen(move(up), id); }

	SigGenId                GetSigGenIdActive()                const { return m_sigGenList.GetSigGenIdActive(); }
	SigGenId                SetSigGenActive(SigGenId const id)       { return m_sigGenList.SetActive(id); }
	SignalGenerator const * GetSigGen      (SigGenId const id) const { return m_sigGenList.GetSigGen(id); }
	SignalGenerator       * GetSigGen      (SigGenId const id)       { return m_sigGenList.GetSigGen(id); }

	UPNobList             & GetUPNobs()             { return m_Nobs; }
	MonitorData           & GetMonitorData()        { return m_monitorData; }
	Param                 & GetParams()             { return m_param; }
	UPSigGenList    const & GetSigGenList()         { return m_sigGenList; }
	SignalGenerator const * GetSigGenActive() const { return m_sigGenList.GetSigGenActive(); }
	SignalGenerator       * GetSigGenActive()       { return m_sigGenList.GetSigGenActive(); }

	void DeselectAllNobs     () const               { m_Nobs.SelectAllNobs(false); }
	void SetModelFilePath    (wstring const & wstr) { m_wstrModelFilePath = wstr; }
	void AddDescriptionLine  (wstring const & wstr) { m_description.AddDescriptionLine(wstr); }
	void DescriptionComplete ()                     { m_description.DescriptionComplete(); }
	void SetDescriptionUI    (DescriptionUI & i)    { m_description.SetDescriptionUI(i); }
	void SetHighSigObservable(Observable * obs)     { m_monitorData.SetHighSigObservable(obs); }

private:

	UPNobList        m_Nobs;
	UPSigGenList     m_sigGenList;
	ModelDescription m_description;
	MonitorData      m_monitorData;
	Param            m_param;
	wstring          m_wstrModelFilePath { L"" };
};
