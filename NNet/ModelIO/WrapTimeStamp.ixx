// WrapTimeStamp.ixx
//
// ModelIO

//module;
//
//#include <iostream>
//#include <ctime>
//
//export module WrapTimeStamp;
//
//import Wrapper;
//import Script;
//import NNetModelIO;
//
//using std::wostream;
//using std::endl;
//
//export class WrapTimeStamp : public Wrapper
//{
//public:
//    using Wrapper::Wrapper;
//
//    void operator() (Script& script) const final
//    {
//        unsigned long const ulScanTime { script.ScrReadUlong() };
//        NNetModelIO::GetImportNMWI().SetScanTime(static_cast<time_t>(ulScanTime));
//    }
//
//    void Write(wostream& out) const final
//    {
//        WriteCmdName(out);
//        out << NNetModelIO::GetExportNMRI().GetScanTime() << endl;
//    };
//};
