# NNetModel
# Created Fri Mar  5 17:12:23 2021

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

CreateShape 0 outputNeuron(405.29|154.035)
CreateShape 1 outputNeuron(196.637|152.032)
CreateShape 2 inputNeuron(200.04|-125.513)
CreateShape 3 inputNeuron(441.944|-101.06)
CreateShape 4 pipe(3->0)
CreateShape 5 pipe(2->1)

ShapeParameter InputNeuron 2 PulseRate = 50
ShapeParameter InputNeuron 3 PulseRate = 50


NrOfTracks 0

