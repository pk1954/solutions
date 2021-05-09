# NNetModel
# Created Sun May  9 17:45:00 2021

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

NrOfNobs = 20

CreateNob 0 inputNeuron(467.056|589.691)
CreateNob 1 outputNeuron(434.645|257.162)
CreateNob 3 knot(420|800)
CreateNob 4 knot(345.501|-3.48833)
CreateNob 7 knot(473.044|-18.8449)
CreateNob 8 knot(512.186|856.83)
CreateNob 10 outputNeuron(523.86|269.027)
CreateNob 11 inputNeuron(556.27|601.556)
CreateNob 12 inputNeuron(645.484|613.422)
CreateNob 13 outputNeuron(613.074|280.892)
CreateNob 15 knot(720.755|840)
CreateNob 16 knot(646.255|36.5117)
CreateNob 2 pipe(0->3)
CreateNob 5 pipe(4->1)
CreateNob 6 pipe(7->10)
CreateNob 9 pipe(11->8)
CreateNob 14 pipe(12->15)
CreateNob 17 pipe(16->13)
CreateNob 18 connector{ 3: 1 10 13 }
CreateNob 19 connector{ 3: 0 11 12 }

NobParameter InputNeuron 0 PulseRate = 50
NobParameter InputNeuron 11 PulseRate = 50
NobParameter InputNeuron 12 PulseRate = 50


NrOfTracks 0

