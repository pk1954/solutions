# NNetModel
# Created Thu Jan 21 18:33:36 2021

# Computer name: DESKTOP-D6AR9AH
# User name: Peter

Protocol version 1.7


Description "daffafagafgd"
Description "hfghdhsdhgdh"

GlobalParameter PulseSpeed = 120
GlobalParameter PulseWidth = 2000
GlobalParameter SignalLoss = 0
GlobalParameter Threshold = 20
GlobalParameter PeakVoltage = 10
GlobalParameter RefractoryPeriod = 500
GlobalParameter TimeResolution = 100

NrOfShapes = 3

CreateShape 0 inputNeuron(400|200)
CreateShape 1 neuron(400|800)
CreateShape 2 pipe(0->1)

ShapeParameter InputNeuron 0 PulseRate = 50


NrOfTracks 0

