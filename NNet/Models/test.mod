# NNetModel
# Created Mon Feb 22 13:44:51 2021

# Computer name: DESKTOP-D6AR9AH
# User name: Peter

Protocol version 1.7



GlobalParameter PulseSpeed = 0.1
GlobalParameter PulseWidth = 2000
GlobalParameter SignalLoss = 0
GlobalParameter Threshold = 14
GlobalParameter PeakVoltage = 10
GlobalParameter RefractoryPeriod = 500
GlobalParameter TimeResolution = 100

NrOfShapes = 6

CreateShape 0 inputNeuron(447.262|127.769)
CreateShape 1 inputNeuron(266.256|131.021)
CreateShape 2 neuron(455.679|431.458)
CreateShape 3 neuron(266.253|427.719)
CreateShape 4 pipe(0->2)
CreateShape 5 pipe(1->3)

ShapeParameter InputNeuron 0 PulseRate = 50
ShapeParameter InputNeuron 1 PulseRate = 50


NrOfTracks 0

