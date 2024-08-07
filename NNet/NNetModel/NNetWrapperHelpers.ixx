// NNetWrapperHelpers.ixx
//
// NNetModel

export module NNetWrapperHelpers;

import std;
import Types;
import RunTime;
import NNetModel;
import Signals;

using std::unique_ptr;
using std::wostream;
using std::setprecision;

export unique_ptr<NobIdList> ScrReadNobIdList          (Script &);
export NobId                 ScrReadNobId              (Script &);
export SignalId              ScrReadSignalId           (Script &);
export SigGenId              ScrReadSigGenId           (Script &);
export NobType               ScrReadNobType            (Script &);
export ParamType::Value      ScrReadParamType          (Script &);
export mV                    ScrReadVoltage            (Script &);
export SigGenStaticData      ScrReadSigGenStaticData   (Script &);
