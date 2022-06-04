# NNetModel
# Created Tue Apr 20 14:38:55 2021

# Computer name: DESKTOP-D6AR9AH
# User name: Peter

Protocol version 1.7



GlobalParameter PulseSpeed = 120
GlobalParameter PulseWidth = 2000
GlobalParameter SignalLoss = 0
GlobalParameter Threshold = 20
GlobalParameter PeakVoltage = 10
GlobalParameter RefractoryPeriod = 500
GlobalParameter TimeResolution = 100

NrOfShapes = 28

CreateShape 0 inputNeuron(488.13|563.746)
CreateShape 1 outputNeuron(349.289|749.289)
CreateShape 4 outputNeuron(420|820)
CreateShape 5 inputNeuron(551.767|627.388)
CreateShape 6 inputNeuron(615.403|691.031)
CreateShape 7 outputNeuron(490.711|890.711)
CreateShape 10 outputNeuron(727.267|466.922)
CreateShape 11 inputNeuron(881.71|396.445)
CreateShape 12 inputNeuron(759.529|255.04)
CreateShape 13 outputNeuron(656.177|411.728)
CreateShape 16 outputNeuron(585.088|356.535)
CreateShape 17 inputNeuron(675.231|196.024)
CreateShape 18 inputNeuron(1121.92|334.02)
CreateShape 19 knot(1261.49|608.476)
CreateShape 21 knot(1516.69|404.113)
CreateShape 23 knot(1199.68|437.011)
CreateShape 2 pipe(0->1)
CreateShape 3 pipe(5->4)
CreateShape 8 pipe(6->7)
CreateShape 9 pipe(11->10)
CreateShape 14 pipe(12->13)
CreateShape 15 pipe(17->16)
CreateShape 20 pipe(23->19)
CreateShape 22 pipe(19->21)
CreateShape 24 pipe(21->23)
CreateShape 25 pipe(18->23)
CreateShape 26 connector{ 3: 0 5 6 }
CreateShape 27 connector{ 3: 10 13 16 }

ShapeParameter InputNeuron 0 PulseRate = 50
ShapeParameter InputNeuron 5 PulseRate = 50
ShapeParameter InputNeuron 6 PulseRate = 50
ShapeParameter InputNeuron 11 PulseRate = 50
ShapeParameter InputNeuron 12 PulseRate = 50
ShapeParameter InputNeuron 17 PulseRate = 50
ShapeParameter InputNeuron 18 PulseRate = 50


NrOfTracks 0

