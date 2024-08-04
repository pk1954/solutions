// IoPixelTypes.ixx
//
// Toolbox\IoUtil

export module IoUtil:IoPixelTypes;

import std;
import Types;
import Script;

using std::wostream;

export wostream& operator<< (wostream&, MicroMeterPnt    const&);
export wostream& operator<< (wostream&, MicroMeterRect   const&);
export wostream& operator<< (wostream&, MicroMeterCircle const&);

export PIXEL         ScrReadPixel(Script&);
export PixelPoint    ScrReadPixelPoint(Script&);
export PixelRectSize ScrReadPixelRectSize(Script&);
export PixelRect     ScrReadPixelRect(Script&);
export fPixel        ScrReadfPixel(Script&);
export fPixelPoint   ScrReadfPixelPoint(Script&);
