# NNetModel
# Created Wed Aug 11 17:24:12 2021

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

NrOfNobs = 12

CreateNob 0 inputNeuron(1066.36|263.583)
CreateNob 1 inputNeuron(666.437|255.617)
CreateNob 2 inputNeuron(966.377|261.591)
CreateNob 3 inputNeuron(866.397|259.6)
CreateNob 4 inputNeuron(766.417|257.609)
CreateNob 7 outputNeuron(779.861|9.40335)
CreateNob 8 outputNeuron(879.708|14.9379)
CreateNob 9 outputNeuron(979.555|20.4727)
CreateNob 10 outputNeuron(680.015|3.86862)
CreateNob 11 outputNeuron(1079.41|26.008)
CreateNob 5 inputConnector{5:0,4,7,8,3}
CreateNob 6 outputConnector{5:72,68,67,64,71}

NobParameter InputNeuron 0 PulseRate = 50
NobParameter InputNeuron 1 PulseRate = 50
NobParameter InputNeuron 2 PulseRate = 50
NobParameter InputNeuron 3 PulseRate = 50
NobParameter InputNeuron 4 PulseRate = 50


NrOfTracks 0

