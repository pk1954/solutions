# NNetModel
# Created Tue Aug 31 21:05:15 2021

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

NrOfNobs = 21

CreateNob 1 knot(918.239|12130.5)
CreateNob 3 knot(1113.56|13410.2)
CreateNob 5 knot(1592.2|13526.4)
CreateNob 7 knot(420.478|13369.9)
CreateNob 9 knot(658.317|13379.4)
CreateNob 11 knot(-198.763|13446.1)
CreateNob 13 knot(87.0951|13384.4)
CreateNob 14 inputNeuron(1205.71|12612.7)
CreateNob 15 neuron(856.641|12560.8)
CreateNob 16 inputNeuron(491.556|12535.6)
CreateNob 17 inputNeuron(318.613|12519.4)
CreateNob 18 inputNeuron(143.891|12465.7)
CreateNob 19 inputNeuron(-41.5045|12433.4)
CreateNob 0 pipe(1->15)
CreateNob 2 pipe(15->3)
CreateNob 4 pipe(14->5)
CreateNob 6 pipe(17->7)
CreateNob 8 pipe(16->9)
CreateNob 10 pipe(19->11)
CreateNob 12 pipe(18->13)
CreateNob 20 closedConnector{6:19,18,17,16,15,14}

NobParameter InputNeuron 14 PulseRate = 50
NobParameter InputNeuron 16 PulseRate = 50
NobParameter InputNeuron 17 PulseRate = 50
NobParameter InputNeuron 18 PulseRate = 50
NobParameter InputNeuron 19 PulseRate = 50


NrOfTracks 3

