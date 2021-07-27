# NNetModel
# Created Tue Jul 27 00:02:42 2021

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

NrOfNobs = 6

CreateNob 0 inputNeuron(782.692|267.316)
CreateNob 1 inputNeuron(877.308|299.684)
CreateNob 2 outputNeuron(708.755|476.655)
CreateNob 3 outputNeuron(804.245|506.345)
CreateNob 4 connector{2:2,3}
CreateNob 5 connector{2:0,1}

NobParameter InputNeuron 0 PulseRate = 50
NobParameter InputNeuron 1 PulseRate = 50


NrOfTracks 0

