// HistModelData.ixx
//
// History

export module HistoryLib:HistModelData;

import std;
import :ModelData;
import :GenerationCmd;

using std::unique_ptr;
using std::make_unique;

class HistModelData : public ModelData
{
public:
	virtual ~HistModelData() {};

	virtual void OnAppCommand(GenerationCmd const) = 0;
};
