# NNetModel
# Created Mon Mar 15 13:20:00 2021

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

NrOfShapes = 8

CreateShape 0 outputNeuron(196.637|152.032)
CreateShape 2 knot(92.184|-107.968)
CreateShape 3 knot(240.063|-49.9719)
CreateShape 5 neuron(4.84362|134.544)
CreateShape 6 knot(-29.5993|-55.2864)
CreateShape 1 pipe(2->0)
CreateShape 4 pipe(3->0)
CreateShape 7 pipe(6->5)



NrOfTracks 0

