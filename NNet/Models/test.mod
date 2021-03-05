# NNetModel
# Created Fri Mar  5 21:57:53 2021

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

NrOfShapes = 9

CreateShape 0 outputNeuron(341.173|62.6437)
CreateShape 1 outputNeuron(196.637|152.032)
CreateShape 2 inputNeuron(130.659|-118.814)
CreateShape 3 inputNeuron(441.944|-101.06)
CreateShape 6 neuron(593.814|-123.137)
CreateShape 7 knot(598.599|-6.86806)
CreateShape 4 pipe(3->0)
CreateShape 5 pipe(2->1)
CreateShape 8 pipe(6->7)

ShapeParameter InputNeuron 2 PulseRate = 50
ShapeParameter InputNeuron 3 PulseRate = 50


NrOfTracks 0

