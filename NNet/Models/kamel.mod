# NNetModel
# Created Thu Mar 17 17:53:48 2022

# Computer name: DESKTOP-4-PETER
# User name: Peter

Protocol version 1.8



GlobalParameter StimulusMaxFreq = 50
GlobalParameter StimulusMaxTime = 500000
GlobalParameter BaseFrequency = 100
GlobalParameter PulseSpeed = 1
GlobalParameter PulseWidth = 2000
GlobalParameter Threshold = 20
GlobalParameter PeakVoltage = 10
GlobalParameter RefractoryPeriod = 500
GlobalParameter TimeResolution = 100

NrOfNobs =  7

CreateNob 0 inputNeuron( 950.258| -1226.48)
Voltage 0 inputNeuron 10.4
CreateNob 2 outputNeuron( 50.2565| 645.127)
Voltage 2 outputNeuron 0
CreateNob 3 inputNeuron( 21.0258| 310.154)
Voltage 3 inputNeuron 10.4
CreateNob 4 knot( 62.8719| 390.769)
Voltage 4 knot 0
CreateNob 1 pipe(0->4)
Voltage 1 pipe(18: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 5 pipe(3->4)
Voltage 5 pipe(1: 0)
CreateNob 6 pipe(4->2)
Voltage 6 pipe(3: 0, 0, 0)



NrOfTracks 2
Signal  track  0 source 101(( 231.147| 66.3152),59.848)
Signal  track  1 source 101(( 47.4873| 511.051),59.848)

