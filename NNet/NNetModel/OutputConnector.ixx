// OutputConnector.ixx
//
// NNetModel

module;

#include <vector>

export module NNetModel:OutputConnector;

import DrawContext;
import Types;
import :tHighlight;
import :NobType;
import :IoConnector;

using std::vector;

export class OutputConnector : public IoConnector
{
public:

	static bool TypeFits(NobType const type) { return type.IsOutputConnectorType(); }

	OutputConnector();
	explicit OutputConnector(vector<IoLine*>&);
	explicit OutputConnector(vector<IoLine*>&&);

	~OutputConnector() final = default;

	void CollectInput() final { /* done by output neurons */ };
	void Reconnect()    final { /* nothing to connect */ };

	NobIoMode GetIoMode() const final { return NobIoMode::output; }

	void DrawExterior(DrawContext const&, tHighlight const) const override;

	static unsigned int Size() { return sizeof(OutputConnector); }
};
