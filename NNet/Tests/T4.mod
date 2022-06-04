# NNetModel
# Created Sun Jun 13 12:37:05 2021

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

NrOfNobs = 19

CreateNob 0 inputNeuron(340.978|559.888)
CreateNob 1 outputNeuron(337.643|489.968)
CreateNob 3 knot(420|800)
CreateNob 4 knot(345.501|-3.48833)
CreateNob 7 knot(536.199|9.08897)
CreateNob 8 knot(575.341|884.764)
CreateNob 10 outputNeuron(437.529|485.204)
CreateNob 11 inputNeuron(440.864|555.125)
CreateNob 12 inputNeuron(540.75|550.361)
CreateNob 13 outputNeuron(537.416|480.44)
CreateNob 15 knot(720.755|840)
CreateNob 16 knot(646.255|36.5117)
CreateNob 2 pipe(0->3)
CreateNob 5 pipe(4->1)
CreateNob 6 pipe(7->10)
CreateNob 9 pipe(11->8)
CreateNob 14 pipe(12->15)
CreateNob 17 pipe(16->13)
CreateNob 18 closedConnector { 3: 0 11 12 } { 3: 1 10 13 }

NobParameter InputNeuron 0 PulseRate = 50
NobParameter InputNeuron 11 PulseRate = 50
NobParameter InputNeuron 12 PulseRate = 50


NrOfTracks 0

