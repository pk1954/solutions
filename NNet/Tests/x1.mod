# NNetModel
# Created Fri Aug 13 00:08:02 2021

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

NrOfNobs = 72

CreateNob 0 outputNeuron(392.08|563.396)
CreateNob 3 outputNeuron(491.553|573.655)
CreateNob 4 outputNeuron(591.025|583.913)
CreateNob 7 outputNeuron(690.498|594.172)
CreateNob 12 neuron(775.03|214.437)
CreateNob 13 neuron(666.636|172.255)
CreateNob 14 neuron(568.38|190.913)
CreateNob 15 neuron(454.915|118.311)
CreateNob 16 neuron(634.137|-1170.78)
CreateNob 17 neuron(747.602|-1098.18)
CreateNob 18 neuron(845.858|-1116.84)
CreateNob 19 neuron(954.253|-1074.66)
CreateNob 20 inputNeuron(641.124|-1802.78)
CreateNob 23 inputNeuron(741.069|-1799.47)
CreateNob 24 inputNeuron(841.015|-1796.16)
CreateNob 27 inputNeuron(940.959|-1792.85)
CreateNob 32 neuron(811.637|-468.155)
CreateNob 33 neuron(681.793|-487.067)
CreateNob 34 neuron(581.847|-490.379)
CreateNob 35 neuron(481.902|-493.69)
CreateNob 36 neuron(1254.35|-347.656)
CreateNob 37 neuron(1354.29|-344.345)
CreateNob 38 neuron(1454.24|-341.033)
CreateNob 39 neuron(1584.08|-322.121)
CreateNob 44 inputNeuron(1713.4|-1646.82)
CreateNob 47 inputNeuron(1613.46|-1650.13)
CreateNob 48 inputNeuron(1513.51|-1653.44)
CreateNob 51 inputNeuron(1413.57|-1656.75)
CreateNob 52 neuron(1726.7|-928.626)
CreateNob 53 neuron(1618.3|-970.806)
CreateNob 54 neuron(1520.05|-952.146)
CreateNob 55 neuron(1406.58|-1024.75)
CreateNob 56 neuron(1227.36|264.345)
CreateNob 57 neuron(1340.82|336.947)
CreateNob 58 neuron(1439.08|318.289)
CreateNob 59 neuron(1547.47|360.471)
CreateNob 64 outputNeuron(1462.94|740.206)
CreateNob 67 outputNeuron(1363.47|729.947)
CreateNob 68 outputNeuron(1264|719.689)
CreateNob 71 outputNeuron(1164.52|709.43)
CreateNob 1 pipe(15->0)
CreateNob 2 pipe(14->3)
CreateNob 5 pipe(13->4)
CreateNob 6 pipe(12->7)
CreateNob 8 pipe(32->12)
CreateNob 9 pipe(33->13)
CreateNob 10 pipe(34->14)
CreateNob 11 pipe(35->15)
CreateNob 21 pipe(20->16)
CreateNob 22 pipe(23->17)
CreateNob 25 pipe(24->18)
CreateNob 26 pipe(27->19)
CreateNob 28 pipe(19->32)
CreateNob 29 pipe(18->33)
CreateNob 30 pipe(17->34)
CreateNob 31 pipe(16->35)
CreateNob 40 pipe(55->36)
CreateNob 41 pipe(54->37)
CreateNob 42 pipe(53->38)
CreateNob 43 pipe(52->39)
CreateNob 45 pipe(44->52)
CreateNob 46 pipe(47->53)
CreateNob 49 pipe(48->54)
CreateNob 50 pipe(51->55)
CreateNob 60 pipe(36->56)
CreateNob 61 pipe(37->57)
CreateNob 62 pipe(38->58)
CreateNob 63 pipe(39->59)
CreateNob 65 pipe(59->64)
CreateNob 66 pipe(58->67)
CreateNob 69 pipe(57->68)
CreateNob 70 pipe(56->71)

NobParameter InputNeuron 20 PulseRate = 50
NobParameter InputNeuron 23 PulseRate = 50
NobParameter InputNeuron 24 PulseRate = 50
NobParameter InputNeuron 27 PulseRate = 50
NobParameter InputNeuron 44 PulseRate = 50
NobParameter InputNeuron 47 PulseRate = 50
NobParameter InputNeuron 48 PulseRate = 50
NobParameter InputNeuron 51 PulseRate = 50


NrOfTracks 1
Signal  track 0source 101((950.297|-504.12),250)

