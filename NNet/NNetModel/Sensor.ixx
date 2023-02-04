// Sensor.ixx
//
// NNetModel

export module NNetModel:Sensor;

import Types;
import DrawContext;
import :NNetParameters;
import :SignalSource;
import :UPNobList;
import :Pipe;

using std::vector;
using std::wostream;

export class Sensor : public SignalSource
{
public:
    enum class Type
    {
        microSensor,
        macroSensor
    };

    virtual Type SensorType() const = 0;
    virtual void RotateSensor(MicroMeterPnt const&, Radian const) = 0;

    SignalSource::Type SignalSourceType() const final { return SignalSource::Type::sensor; }

    bool IsMicroSensor() const { return SensorType() == Type::microSensor; }
    bool IsMacroSensor() const { return SensorType() == Type::macroSensor; }
};

