// InputConnector.ixx
//
// NNetModel

module;

#include <memory>
#include <vector>

export module NNetModel:InputConnector;

import DrawContext;
import :tHighlight;
import :SignalGenerator;
import :NNetParameters;
import :NobType;
import :Nob;
import :IoConnector;
import :InputLine;
import :IoLine;
import :PosNob;

using std::unique_ptr;
using std::vector;

export class InputConnector : public IoConnector
{
public:

	static bool TypeFits(NobType const type) { return type.IsInputConnectorType(); }

	InputConnector(vector<IoLine *> &&);

	~InputConnector() final = default;

	InputLine & GetElem(size_t const) const;

	void SetSigGen(SignalGenerator* const);

	SignalGenerator      * GetSigGen();
	SignalGenerator const* GetSigGenC() const;

	void CollectInput() final { /* */ };
	void Reconnect()    final { /* nothing to connect */ };

	bool FixOpenLinks(PushFunc const&) final { return false; };

	NobIoMode GetIoMode() const final { return NobIoMode::input; }

	void DrawExterior(DrawContext const&, tHighlight const) const final;
};
