# NNetModel
# Created Sat Jun 18 22:59:30 2022

# Computer name: DESKTOP-4-PETER
# User name: Peter

Protocol version 1.8

GlobalParameter StimulusMaxTime = 500000
GlobalParameter StimulusMaxFreq = 50
GlobalParameter StimulusMaxVolt = 20
GlobalParameter inputBaseFreq = 10
GlobalParameter inputBaseVolt = 10
GlobalParameter PeakVoltage = 10
GlobalParameter Threshold = 14
GlobalParameter PulseSpeed = 0.1
GlobalParameter SpikeWidth = 2000
GlobalParameter RefractoryPeriod = 500
GlobalParameter FilterSize = 100
GlobalParameter TimeResolution = 100
NrOfNobs = 3
CreateNob 0 inputLine( 400| 200)
CreateNob 1 outputLine( 400| 800)
CreateNob 2 pipe(0->1)
Voltage 0 inputLine 0
Voltage 1 outputLine 0
Voltage 2 pipe(60: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
SignalGenerator "Standard"      10  50      10  20   500000
SignalGenerator "SigGen 1"      10  29.5      10  20   1.09e+06
ActiveSigGen  1
NrOfTracks 0
SimulationTime  0
