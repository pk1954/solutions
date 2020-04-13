# NNetModel
# Created 07-04-2020 13-34-52
# Computer name: DESKTOP-D6AR9AH
# User name: Peter

Protocol version 1.5

GlobalParameter PulseSpeed = 120
GlobalParameter PulseWidth = 2000
GlobalParameter SignalLoss = 0
GlobalParameter Threshold = 14
GlobalParameter PeakVoltage = 10
GlobalParameter RefractoryPeriod = 500
GlobalParameter TimeResolution = 100

NrOfShapes = 43

CreateShape 0 neuron (400|800)
CreateShape 1 knot (492.763|395.191)
CreateShape 2 knot (379.212|-51.3238)
CreateShape 3 knot (495.763|494.191)
CreateShape 5 knot (495.763|586.191)
CreateShape 7 knot (479.763|677.191)
CreateShape 10 knot (188.329|434.493)
CreateShape 12 knot (440.686|68.837)
CreateShape 14 knot (411.306|25.127)
CreateShape 16 knot (478.386|146.837)
CreateShape 18 knot (475.786|244.337)
CreateShape 20 knot (490.086|322.337)
CreateShape 23 neuron (251.328|805.376)
CreateShape 25 inputNeuron (455.614|-254.472)
CreateShape 27 inputNeuron (293.546|-260.657)
CreateShape 4 pipe (1->3)
CreateShape 6 pipe (3->5)
CreateShape 8 pipe (5->7)
CreateShape 9 pipe (7->0)
CreateShape 11 pipe (2->10)
CreateShape 13 pipe (14->12)
CreateShape 15 pipe (2->14)
CreateShape 17 pipe (12->16)
CreateShape 19 pipe (16->18)
CreateShape 21 pipe (18->20)
CreateShape 22 pipe (20->1)
CreateShape 24 pipe (10->23)
CreateShape 26 pipe (25->2)
CreateShape 28 pipe (27->2)

ShapeParameter InputNeuron 25 PulseRate = 50
ShapeParameter InputNeuron 27 PulseRate = 50

TriggerSound 0 720 Hertz 100 msec 
TriggerSound 23 1440 Hertz 100 msec 
