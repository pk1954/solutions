# NNetModel
# Created Wed Mar 17 19:01:10 2021

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

NrOfShapes = 19

CreateShape 0 outputNeuron(285.686|17.3107)
CreateShape 2 knot(92.184|-107.968)
CreateShape 3 knot(304.804|-133.401)
CreateShape 5 outputNeuron(-15.1929|0.733966)
CreateShape 6 knot(-29.5993|-55.2864)
CreateShape 8 outputNeuron(116.507|307.198)
CreateShape 9 knot(206.298|608.189)
CreateShape 11 outputNeuron(399.856|274.151)
CreateShape 12 knot(524.495|482.083)
CreateShape 14 outputNeuron(813.366|155.376)
CreateShape 15 knot(867.621|372.107)
CreateShape 17 knot(358.798|490.882)
CreateShape 1 pipe(2->0)
CreateShape 4 pipe(3->0)
CreateShape 7 pipe(6->5)
CreateShape 10 pipe(9->8)
CreateShape 13 pipe(12->11)
CreateShape 16 pipe(15->14)
CreateShape 18 pipe(17->11)



NrOfTracks 0

