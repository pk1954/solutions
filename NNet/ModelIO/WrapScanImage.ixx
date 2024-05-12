// WrapScanImage.ixx
//
// ModelIO

module;

#include <memory>
#include <iostream>
#include <iomanip>
#include <string>

export module WrapScanImage;

import IoUtil;
import Raster;
import NNetModelIO;
import NNetModel;
import Wrapper;

using std::unique_ptr;
using std::make_unique;
using std::endl;
using std::setw;

export class WrapScanImage : public Wrapper
{
public:
    using Wrapper::Wrapper;

    void operator() (Script& script) const final
    {
        Raster            const & raster  { NNetModelIO::GetImportNMWI().GetScanRaster() };
        unique_ptr<ScanImageByte> upImage { make_unique<ScanImageByte>(raster.Size())};
        script.ScrReadSpecial(CURLY_OPEN_BRACKET);
        raster.Apply2AllC
        (
            [&script, &upImage](RasterPoint const &rp)
            {
                if (rp.m_x == 0)
                    script.ScrReadSpecial(CURLY_OPEN_BRACKET);
                upImage->Set(rp, static_cast<ColIndex>(script.ScrReadUchar()));
                if (rp.m_x == upImage->Width() - 1)
                    script.ScrReadSpecial(CURLY_CLOSE_BRACKET);
            }
        );
        script.ScrReadSpecial(CURLY_CLOSE_BRACKET);
        NNetModelIO::GetImportNMWI().ReplaceScanImage(move(upImage));
    }

    void Write(wostream& out) const final
    {
        if (ScanImageByte const* pImage { NNetModelIO::GetExportNMRI().GetScanImageC() })
        {
            WriteCmdName(out);
            out << endl << CURLY_OPEN_BRACKET << endl;
            pImage->Size().VisitAllRasterPointsC
            (
                [&out, pImage](RasterPoint const &rp)
                {
                    if (rp.m_x == 0)
                        out << L"   " << CURLY_OPEN_BRACKET << SPACE;
                    out << setw(3) << pImage->Get(rp) << SPACE;
                    if (rp.m_x == pImage->Width() - 1)
                        out << CURLY_CLOSE_BRACKET << endl;
                }
            );
            out << CURLY_CLOSE_BRACKET << endl;
        }
    }
};
