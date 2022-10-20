// WrapCreateNob.ixx
//
// ModelIO

module;

#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

export module WrapCreateNob;

import NNetWrapperBase;
import ErrHndl;
import Types;
import IoConstants;
import WrapBase;
import Script;
import NNetModelIO;
import NNetWrapperHelpers;
import NNetModel;

using std::vector;
using std::wstring;
using std::wostream;
using std::unique_ptr;
using std::make_unique;
using std::wcout;
using std::endl;

export class WrapCreateNob : public NNetWrapperBase
{
public:
    using NNetWrapperBase::NNetWrapperBase;

    void operator() (Script & script) const final
    {
        createNob(script);
    }

    void Write(wostream & out) const final
    {
        NNetModelReaderInterface const& nmri{ m_modelIO.GetExportNMRI() };
        nmri.Apply2AllC<BaseKnot   >([this, &out](BaseKnot    const& s) { writeNob(out, s); });
        nmri.Apply2AllC<Pipe       >([this, &out](Pipe        const& s) { writeNob(out, s); });
        nmri.Apply2AllC<IoConnector>([this, &out](IoConnector const& s) { writeNob(out, s); });
    };

private:

    UPNob createPipe(Script& script) const
    {
        script.ScrReadSpecial(OPEN_BRACKET);
        NobId const idStart { ScrReadNobId(script) };
        for (int i = 0; i < FROM_TO.size(); i++)
            script.ScrReadSpecial(FROM_TO[i]);
        NobId const idEnd { ScrReadNobId(script) };
        script.ScrReadSpecial(CLOSE_BRACKET);
        UPNobList const& list { m_modelIO.GetImportNMWI().GetUPNobs() };
        NNetModelIO::CheckImportedNobId(script, list, idStart);
        NNetModelIO::CheckImportedNobId(script, list, idEnd);
        if (idStart == idEnd)
        {
            wcout << "+++ Pipe has identical start and end point" << endl;
            wcout << "+++ " << L": " << idStart << L" -> " << idEnd << endl;
            wcout << "+++ Pipe ignored" << endl;
            throw ScriptErrorHandler::ScriptException(999, wstring(L"Error reading Pipe"));
            return nullptr;
        }
        else
        {
            BaseKnot * const pKnotStart { m_modelIO.GetImportNMWI().GetNobPtr<BaseKnot*>(idStart) };
            BaseKnot * const pKnotEnd   { m_modelIO.GetImportNMWI().GetNobPtr<BaseKnot*>(idEnd) };
            unique_ptr<Pipe> upPipe     { make_unique<Pipe>(pKnotStart, pKnotEnd) };
            pKnotStart->AddOutgoing(*upPipe.get());
            pKnotEnd  ->AddIncoming(*upPipe.get());
            return move(upPipe);
        }
    }

   Nob * createNob(Script& script) const
    {
        NobId   const idFromScript { ScrReadNobId(script) };
        NobType const nobType      { ScrReadNobType(script) };
        UPNob         upNob{};

        switch (nobType.GetValue())
        {
            using enum NobType::Value;
        case inputConnector:
        case outputConnector:
            upNob = createIoConnector(script, nobType);
            break;
        case inputLine:
        case outputLine:
        case neuron:
        case knot:
            upNob = createBaseKnot(script, nobType);
            break;
        case pipe:
            upNob = createPipe(script);
            break;
        default:
            assert(false);  // handled in ScrReadNobType
            break;
        }

        Nob * pNob { nullptr };
        if (upNob)
        {
            UPNobList& list{ m_modelIO.GetImportNMWI().GetUPNobs() };
            upNob->SetId(idFromScript);
            pNob = upNob.get();
            list.SetNob2Slot(idFromScript, move(upNob));
        }

        if (pNob && nobType.IsIoConnectorType())
        {
            static_cast<IoConnector*>(pNob)->SetParentPointers();
        }

        return pNob;
    }

    UPNob createBaseKnot(Script& script, NobType const nobType) const
    {
        MicroMeterPnt const umPosition(ScrReadMicroMeterPnt(script));
        return BaseKnotFactory::Make(umPosition, nobType);
    }

    UPNob createIoConnector(Script& script, NobType const nobType) const
    {
        bool            bOK = true;
        vector<IoLine*> ioLineList;
        script.ScrReadSpecial(LIST_OPEN_BRACKET);
        int const iNrOfElements{ script.ScrReadInt() };
        script.ScrReadSpecial(NR_SEPARATOR);
        for (int iElem{ 0 };;)
        {
            NobId    const id { ScrReadNobId(script) };
            IoLine * const pIoLine{ m_modelIO.GetImportNMWI().GetNobPtr<IoLine*>(id) };
            if (!pIoLine)
            {
                wcout << L"# +++ unexpected NobType. Ignoring IoConnector" << endl;
                bOK = false;
            }
            if (bOK)
                ioLineList.push_back(pIoLine);
            if (++iElem == iNrOfElements)
                break;
            script.ScrReadSpecial(ID_SEPARATOR);
        }
        script.ScrReadSpecial(LIST_CLOSE_BRACKET);
        unique_ptr<IoConnector> upIoConnector;
        if (bOK)
        {
            if (nobType.IsInputConnectorType())
                upIoConnector = make_unique<InputConnector> (m_modelIO.GetImportNMWI().GetParams(), move(ioLineList));
            else
                upIoConnector = make_unique<OutputConnector>(move(ioLineList));
            upIoConnector->AlignDirection();
        }
        return move(upIoConnector);
    }

    void writePipe(wostream& out, Pipe const& pipe) const
    {
        out << OPEN_BRACKET
            << m_modelIO.GetCompactIdVal(pipe.GetStartKnotId())
            << FROM_TO
            << m_modelIO.GetCompactIdVal(pipe.GetEndKnotId())
            << CLOSE_BRACKET;
    }

    void writeIoConnector(wostream& out, IoConnector const& conn) const
    {
        assert(conn.Size() > 0);
        size_t const iLast{ conn.Size() - 1 };
        out << LIST_OPEN_BRACKET << conn.Size() << NR_SEPARATOR;
        for (size_t i = 0;; ++i)
        {
            out << m_modelIO.GetCompactIdVal(conn.GetElem(i).GetId());
            if (i == iLast)
                break;
            out << ID_SEPARATOR;
        }
        out << LIST_CLOSE_BRACKET;
    }

    void writeNob(wostream& out, Nob const& nob) const
    {
        using enum NobType::Value;
        if (nob.IsDefined())
        {
            WriteCmdName(out);
            out << m_modelIO.GetCompactIdVal(nob.GetId()) << L" " << nob.GetName();
            switch (nob.GetNobType().GetValue())
            {
            case inputLine:
            case outputLine:
            case neuron:
            case knot:
                out << static_cast<BaseKnot const&>(nob).GetPos();
                break;

            case pipe:
                writePipe(out, static_cast<Pipe const&>(nob));
                break;

            case inputConnector:
            case outputConnector:
                writeIoConnector(out, static_cast<IoConnector const&>(nob));
                break;

            default:
                assert(false);
                break;
            }
            out << endl;
        }
    }
};
