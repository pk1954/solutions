# NNetModel
# Created 17-02-2020 17-44-45
# Computer name: DESKTOP-D6AR9AH
# User name: Peter

Protocol version 1.4

GlobalParameter PulseSpeed = 0.1
GlobalParameter PulseWidth = 2000
GlobalParameter SignalLoss = 0
GlobalParameter Threshold = 20
GlobalParameter PeakVoltage = 10
GlobalParameter RefractoryPeriod = 500
GlobalParameter TimeResolution = 100

NrOfShapes = 14

CreateShape 0 inputNeuron (536|186)
CreateShape 1 neuron (497|808)
CreateShape 2 neuron (734|482)
CreateShape 5 knot (548.128|393.827)
CreateShape 6 knot (496|538)
CreateShape 3 pipeline (5->2)
CreateShape 4 pipeline (6->1)
CreateShape 7 pipeline (2->6)
CreateShape 8 pipeline (6->5)
CreateShape 9 pipeline (0->5)

ShapeParameter InputNeuron 0 PulseRate = 50
