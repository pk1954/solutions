// InputConnector.ixx
//
// NNetModel

export module NNetModel:InputConnector;

import std;
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

	void AppendMenuItems(AddMenuFunc const&) const override;

	InputLine & GetElem(size_t const) const;

	void SetSigGen(SignalGenerator* const);

	SignalGenerator      * GetSigGen();
	SignalGenerator const* GetSigGenC() const;

	void CollectInput() final { /* */ };
	void Reconnect()    final { /* nothing to connect */ };

	NobIoMode GetIoMode()  const final { return NobIoMode::input; }
	NobType   GetNobType() const final { return NobType::Value::inputConnector; }

	void DrawExterior(DrawContext const&, tHighlight const) const final;

    static unsigned int Size() { return sizeof(InputConnector); }
};
