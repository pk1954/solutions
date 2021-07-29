# NNetModel
# Created Thu Jul 29 23:37:40 2021

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

NrOfNobs = 14

CreateNob 0 inputNeuron(475.835|88.556)
CreateNob 1 inputNeuron(667.707|319.176)
CreateNob 2 outputNeuron(310.569|449.461)
CreateNob 3 outputNeuron(597|538.665)
CreateNob 4 outputNeuron(692.477|568.399)
CreateNob 5 outputNeuron(406.046|479.196)
CreateNob 6 outputNeuron(501.523|508.93)
CreateNob 7 outputNeuron(787.954|598.133)
CreateNob 9 inputNeuron(731.664|396.048)
CreateNob 10 inputNeuron(539.793|165.429)
CreateNob 11 inputNeuron(603.749|242.302)
CreateNob 12 inputNeuron(795.621|472.921)
CreateNob 8 outputConnector{6:2,3,4,5,6,7}
CreateNob 13 inputConnector{6:0,1,9,10,11,12}

NobParameter InputNeuron 0 PulseRate = 50
NobParameter InputNeuron 1 PulseRate = 50
NobParameter InputNeuron 9 PulseRate = 50
NobParameter InputNeuron 10 PulseRate = 50
NobParameter InputNeuron 11 PulseRate = 50
NobParameter InputNeuron 12 PulseRate = 50


NrOfTracks 0

