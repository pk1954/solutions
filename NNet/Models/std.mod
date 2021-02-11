# NNetModel
# Created Thu Feb 11 19:17:25 2021

# Computer name: DESKTOP-D6AR9AH
# User name: Peter

Protocol version 1.7



GlobalParameter PulseSpeed = 120
GlobalParameter PulseWidth = 2000
GlobalParameter SignalLoss = 0
GlobalParameter Threshold = 14
GlobalParameter PeakVoltage = 10
GlobalParameter RefractoryPeriod = 500
GlobalParameter TimeResolution = 100

NrOfShapes = 10

CreateShape 0 inputNeuron(400|200)
CreateShape 1 neuron(488.002|615.271)
CreateShape 2 neuron(705.324|438.935)
CreateShape 4 neuron(221.79|523.678)
CreateShape 7 inputNeuron(501.48|676.749)
CreateShape 8 knot(525.594|791.491)
CreateShape 3 pipe(2->1)
CreateShape 5 pipe(4->1)
CreateShape 6 pipe(0->1)
CreateShape 9 pipe(7->8)

ShapeParameter InputNeuron 0 PulseRate = 50
ShapeParameter InputNeuron 7 PulseRate = 50


NrOfTracks 0

