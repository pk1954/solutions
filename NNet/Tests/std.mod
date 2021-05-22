# NNetModel
# Created Sat May 22 22:14:34 2021

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

NrOfNobs = 18

CreateNob 0 inputNeuron(274.683|563.05)
CreateNob 1 outputNeuron(261.161|321.975)
CreateNob 3 knot(420|800)
CreateNob 4 knot(345.501|-3.48833)
CreateNob 7 knot(536.199|9.08897)
CreateNob 8 knot(575.341|884.764)
CreateNob 10 outputNeuron(360.506|333.396)
CreateNob 11 inputNeuron(373.81|576.234)
CreateNob 12 inputNeuron(607.045|547.199)
CreateNob 13 outputNeuron(662.255|285.215)
CreateNob 15 knot(720.755|840)
CreateNob 16 knot(646.255|36.5117)
CreateNob 2 pipe(0->3)
CreateNob 5 pipe(4->1)
CreateNob 6 pipe(7->10)
CreateNob 9 pipe(11->8)
CreateNob 14 pipe(12->15)
CreateNob 17 pipe(16->13)

NobParameter InputNeuron 0 PulseRate = 50
NobParameter InputNeuron 11 PulseRate = 50
NobParameter InputNeuron 12 PulseRate = 50


NrOfTracks 0

