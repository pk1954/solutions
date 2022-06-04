# NNetModel
# Created Fri Aug 13 00:06:50 2021

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

NrOfNobs = 50

CreateNob 0 outputNeuron(534.208|862.004)
CreateNob 3 outputNeuron(633.126|876.67)
CreateNob 4 outputNeuron(732.045|891.335)
CreateNob 7 outputNeuron(830.964|906.001)
CreateNob 8 outputNeuron(929.883|920.667)
CreateNob 11 inputNeuron(1100.69|-469.62)
CreateNob 12 inputNeuron(1001.77|-484.285)
CreateNob 15 inputNeuron(902.848|-498.951)
CreateNob 16 inputNeuron(803.929|-513.617)
CreateNob 19 inputNeuron(705.01|-528.282)
CreateNob 20 neuron(962.385|338.996)
CreateNob 21 neuron(863.366|325.022)
CreateNob 22 neuron(677.321|303.674)
CreateNob 23 neuron(774.478|304.451)
CreateNob 24 neuron(566.308|283.102)
CreateNob 25 neuron(676.138|-1306.09)
CreateNob 26 neuron(1009.73|-1300.42)
CreateNob 27 neuron(912.571|-1301.2)
CreateNob 28 neuron(1098.62|-1279.85)
CreateNob 29 neuron(1197.63|-1265.88)
CreateNob 30 inputNeuron(881.451|-2159.57)
CreateNob 33 inputNeuron(980.37|-2144.91)
CreateNob 34 inputNeuron(1079.29|-2130.24)
CreateNob 37 inputNeuron(1178.21|-2115.58)
CreateNob 38 inputNeuron(1277.13|-2100.91)
CreateNob 41 outputNeuron(1141.64|-718.17)
CreateNob 42 outputNeuron(1042.72|-732.836)
CreateNob 45 outputNeuron(943.804|-747.502)
CreateNob 46 outputNeuron(844.885|-762.167)
CreateNob 49 outputNeuron(745.967|-776.833)
CreateNob 1 pipe(24->0)
CreateNob 2 pipe(22->3)
CreateNob 5 pipe(23->4)
CreateNob 6 pipe(21->7)
CreateNob 9 pipe(20->8)
CreateNob 10 pipe(11->20)
CreateNob 13 pipe(12->21)
CreateNob 14 pipe(16->22)
CreateNob 17 pipe(15->23)
CreateNob 18 pipe(19->24)
CreateNob 31 pipe(30->25)
CreateNob 32 pipe(34->26)
CreateNob 35 pipe(33->27)
CreateNob 36 pipe(37->28)
CreateNob 39 pipe(38->29)
CreateNob 40 pipe(29->41)
CreateNob 43 pipe(28->42)
CreateNob 44 pipe(26->45)
CreateNob 47 pipe(27->46)
CreateNob 48 pipe(25->49)

NobParameter InputNeuron 11 PulseRate = 50
NobParameter InputNeuron 12 PulseRate = 50
NobParameter InputNeuron 15 PulseRate = 50
NobParameter InputNeuron 16 PulseRate = 50
NobParameter InputNeuron 19 PulseRate = 50
NobParameter InputNeuron 30 PulseRate = 50
NobParameter InputNeuron 33 PulseRate = 50
NobParameter InputNeuron 34 PulseRate = 50
NobParameter InputNeuron 37 PulseRate = 50
NobParameter InputNeuron 38 PulseRate = 50


NrOfTracks 0

