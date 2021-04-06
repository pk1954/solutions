# NNetModel
# Created Tue Apr  6 17:04:49 2021

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

NrOfShapes = 20

CreateShape 0 inputNeuron(357.445|550.693)
CreateShape 1 outputNeuron(349.289|749.289)
CreateShape 4 outputNeuron(420|820)
CreateShape 5 inputNeuron(428.152|621.407)
CreateShape 6 inputNeuron(498.859|692.121)
CreateShape 7 outputNeuron(490.711|890.711)
CreateShape 10 outputNeuron(600.585|447.135)
CreateShape 11 inputNeuron(758.095|409.758)
CreateShape 12 inputNeuron(635.914|249.059)
CreateShape 13 outputNeuron(521.597|385.809)
CreateShape 16 outputNeuron(442.609|324.483)
CreateShape 17 inputNeuron(551.616|190.043)
CreateShape 2 pipe(0->1)
CreateShape 3 pipe(5->4)
CreateShape 8 pipe(6->7)
CreateShape 9 pipe(11->10)
CreateShape 14 pipe(12->13)
CreateShape 15 pipe(17->16)
CreateShape 18 connector{ 3: 10 13 16 }

CreateShape 19 connector{ 3: 0 5 6 }


ShapeParameter InputNeuron 0 PulseRate = 50
ShapeParameter InputNeuron 5 PulseRate = 50
ShapeParameter InputNeuron 6 PulseRate = 50
ShapeParameter InputNeuron 11 PulseRate = 50
ShapeParameter InputNeuron 12 PulseRate = 50
ShapeParameter InputNeuron 17 PulseRate = 50


NrOfTracks 0

