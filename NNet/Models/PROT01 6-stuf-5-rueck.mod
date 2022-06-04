# NNetModel
# Created Fri Jun  3 16:49:06 2022

# Computer name: DESKTOP-4-PETER
# User name: Peter

Protocol version 1.8


Description "4-stufiges verstärkermodul mit parallelem 2-stufigem modul. geringe verknüpfung. monitormehr-stufiges"
Description " verstärkermodul mit parallelem 2-stufigem modul. geringe verknüpfung. monitor4-stufiges verstärkermo"
Description "dul mit parallelem 2-stufigem modul. geringe verknüpfung. monitormehr-stufiges verstärkermodul mit pa"
Description "rallelem 2-stufigem modul. geringe verknüpfung. monitorprototyp01 monitor.parallelem 2-stufigem modu"
Description "l. geringe verknüpfung. monitormehr-"
Description " 1rückkopplungs neuron 1rückk(*pnob != *pnobother)(*pnob != *pnobother)(*pnob != *pnobother)(*pnob "
Description "!= *pnobother)(*pnob != *pnobother)(*pnob != *pnobother)(*pnob )(*pnob != *pnobother)(*pnob "
Description "!= *pnobother)(*pnob != *pnobother)(*pnob != *pnobother)(*pnob != *pnobother)(*pnob != *pnobother)("
Description "*pnob != *pnobother)"
Description ""
Description "ückk(*pnob != *pnobother)(*pnob != *pnobother)(*pnob != *pnobother)(*pnob "
Description "!= *pnobother)(*pnob != *pnobother)(*pnob != *pnobother)(*pnob != *pnobother)(*pnob != *pnobother)("
Description "*pnob != *pnobother)"
Description ""

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

NrOfNobs =  606

CreateNob 0 inputLine( 336.722| 228.117)
Voltage 0 inputLine 0
CreateNob 3 inputLine( 462.372| 207.615)
Voltage 3 inputLine 0
CreateNob 4 inputLine( 593.353| 214.375)
Voltage 4 inputLine 0
CreateNob 7 inputLine( 712.402| 211.121)
Voltage 7 inputLine 0
CreateNob 8 inputLine( 831.451| 216.817)
Voltage 8 inputLine 0
CreateNob 11 inputLine( 1389.19| 204.388)
Voltage 11 inputLine 0
CreateNob 12 inputLine( 1270.14| 198.693)
Voltage 12 inputLine 0
CreateNob 15 inputLine( 1151.09| 201.946)
Voltage 15 inputLine 0
CreateNob 16 inputLine( 1590.86| 212.454)
Voltage 16 inputLine 0
CreateNob 19 inputLine( 1709.91| 209.201)
Voltage 19 inputLine 0
CreateNob 20 inputLine( 1812.57| 198.513)
Voltage 20 inputLine 0
CreateNob 23 inputLine( 2206.47| 192.638)
Voltage 23 inputLine 0
CreateNob 24 inputLine( 2103.8| 203.326)
Voltage 24 inputLine 0
CreateNob 27 inputLine( 1984.75| 206.579)
Voltage 27 inputLine 0
CreateNob 28 outputLine( 1592.21| 1511.77)
Voltage 28 outputLine 0
CreateNob 31 outputLine( 1688.33| 1560.95)
Voltage 31 outputLine 0
CreateNob 32 outputLine( 1797.54| 1586.3)
Voltage 32 outputLine 0
CreateNob 34 neuron( 2048.63| 1101.69)
Voltage 34 neuron 0
CreateNob 35 neuron( 1948.83| 1107.96)
Voltage 35 neuron 0
CreateNob 36 neuron( 1849.03| 1114.24)
Voltage 36 neuron 0
CreateNob 37 knot( 2156.96| 902.594)
Voltage 37 knot 0
CreateNob 38 knot( 2057.59| 881.291)
Voltage 38 knot 0
CreateNob 39 knot( 1961.48| 884.544)
Voltage 39 knot 0
CreateNob 43 knot( 1582.86| 1836.58)
Voltage 43 knot 0
CreateNob 44 knot( 1463.81| 1824.33)
Voltage 44 knot 0
CreateNob 45 knot( 1341.48| 1807.92)
Voltage 45 knot 0
CreateNob 46 knot( 1783.89| 1616.82)
Voltage 46 knot 0
CreateNob 47 knot( 1677.54| 1593.61)
Voltage 47 knot 0
CreateNob 48 knot( 1581.01| 1537.62)
Voltage 48 knot 0
CreateNob 58 knot( 1430.89| 2756.25)
Voltage 58 knot 0
CreateNob 59 knot( 1295.23| 2767.17)
Voltage 59 knot 0
CreateNob 60 knot( 1145.93| 2690.33)
Voltage 60 knot 0
CreateNob 61 knot( 1025.08| 874.069)
Voltage 61 knot 0
CreateNob 62 knot( 921.577| 1010.86)
Voltage 62 knot 0
CreateNob 63 neuron( 1733.17| 2412.36)
Voltage 63 neuron 0
CreateNob 64 neuron( 1633.26| 2416.63)
Voltage 64 neuron 0
CreateNob 65 neuron( 1533.35| 2420.89)
Voltage 65 neuron 0
CreateNob 69 knot( 843.564| 1080.2)
Voltage 69 knot 0
CreateNob 70 inputLine( 1279.38| -424.705)
Voltage 70 inputLine 0
CreateNob 79 inputLine( -709.169| 187.935)
Voltage 79 inputLine 0
CreateNob 80 knot( -23.3981| 1104.82)
Voltage 80 knot 0
CreateNob 81 knot( 8.56981| 989.445)
Voltage 81 knot 0
CreateNob 82 knot( 158.114| 898.691)
Voltage 82 knot 0
CreateNob 83 knot( 1061.77| 3331.3)
Voltage 83 knot 287.6
CreateNob 84 knot( 1214.34| 3305.24)
Voltage 84 knot 0
CreateNob 85 knot( 1393.91| 3261.55)
Voltage 85 knot 0
CreateNob 87 inputLine( -75.5114| 201.439)
Voltage 87 inputLine 0
CreateNob 88 inputLine( -194.56| 195.743)
Voltage 88 inputLine 0
CreateNob 91 inputLine( -313.609| 198.997)
Voltage 91 inputLine 0
CreateNob 92 inputLine( -444.59| 192.238)
Voltage 92 inputLine 0
CreateNob 95 inputLine( -570.24| 212.739)
Voltage 95 inputLine 0
CreateNob 96 knot( 1155.95| 3687.48)
Voltage 96 knot 0
CreateNob 98 knot( 1131.36| 3672.44)
Voltage 98 knot 0
CreateNob 105 knot( -85.8982| 3605)
Voltage 105 knot 9.6
CreateNob 106 knot( -265.468| 3648.69)
Voltage 106 knot 0
CreateNob 107 knot( -418.038| 3674.75)
Voltage 107 knot 0
CreateNob 111 neuron( 339.971| 1878.67)
Voltage 111 neuron 0
CreateNob 112 neuron( 565.512| 1837.33)
Voltage 112 neuron 0
CreateNob 113 neuron( 670.024| 1893.12)
Voltage 113 neuron 0
CreateNob 114 knot( -333.878| 3033.78)
Voltage 114 knot 0
CreateNob 115 knot( -184.578| 3110.61)
Voltage 115 knot 0
CreateNob 116 knot( -48.9182| 3099.7)
Voltage 116 knot 9.6
CreateNob 123 inputLine( -1388.69| 191.06)
Voltage 123 inputLine 0
CreateNob 124 inputLine( -1549.57| 206.382)
Voltage 124 inputLine 0
CreateNob 125 inputLine( -1227.82| 160.417)
Voltage 125 inputLine 0
CreateNob 128 knot( -1441.98| 1152.69)
Voltage 128 knot 0
CreateNob 129 knot( -542.532| 560.168)
Voltage 129 knot 0
CreateNob 133 knot( -1406.24| 742.804)
Voltage 133 knot 0
CreateNob 134 knot( -428.57| 404.421)
Voltage 134 knot 0
CreateNob 138 knot( -1240.38| 532.069)
Voltage 138 knot 0
CreateNob 139 knot( -290.906| 356.283)
Voltage 139 knot 0
CreateNob 146 neuron( -634.699| 2158.12)
Voltage 146 neuron 0
CreateNob 147 neuron( -795.895| 2193.03)
Voltage 147 neuron 0
CreateNob 148 neuron( -964.751| 2143.67)
Voltage 148 neuron 0
CreateNob 149 knot( -1339.11| 1523.89)
Voltage 149 knot 0
CreateNob 151 knot( -1161.21| 1513.42)
Voltage 151 knot 0
CreateNob 154 knot( -1014.7| 1487.26)
Voltage 154 knot 7.5
CreateNob 157 knot( 75.491| 2853.45)
Voltage 157 knot 0
CreateNob 160 knot( -58.9769| 2902.53)
Voltage 160 knot 0
CreateNob 163 knot( -229.919| 2877.2)
Voltage 163 knot 0
CreateNob 167 knot( -2991.33| 6828.94)
Voltage 167 knot 0
CreateNob 170 knot( -2890.35| 6617.23)
Voltage 170 knot 0
CreateNob 173 knot( -2730.18| 6433.2)
Voltage 173 knot 268.7
CreateNob 175 knot( -841.746| 4102.39)
Voltage 175 knot 0
CreateNob 177 knot( -979.41| 4150.53)
Voltage 177 knot 0
CreateNob 179 knot( -1093.37| 4306.27)
Voltage 179 knot 0
CreateNob 183 neuron( 139.184| 5659.23)
Voltage 183 neuron 0
CreateNob 184 neuron( 34.6718| 5603.44)
Voltage 184 neuron 0
CreateNob 185 neuron( -190.869| 5644.77)
Voltage 185 neuron 0
CreateNob 189 inputLine( -2136.85| 232.267)
Voltage 189 inputLine 0
CreateNob 192 inputLine( -2011.2| 211.766)
Voltage 192 inputLine 0
CreateNob 196 knot( 863.07| 7027.65)
Voltage 196 knot 8.4
CreateNob 197 knot( 683.5| 7071.35)
Voltage 197 knot 0
CreateNob 198 knot( 530.93| 7109.3)
Voltage 198 knot 0
CreateNob 199 knot( -372.726| 4664.8)
Voltage 199 knot 3.6
CreateNob 200 knot( -522.27| 4755.55)
Voltage 200 knot 0
CreateNob 201 knot( -484.607| 5123.34)
Voltage 201 knot 0
CreateNob 202 inputLine( -2275.78| 207.463)
Voltage 202 inputLine 0
CreateNob 211 knot( 312.724| 4846.31)
Voltage 211 knot 0
CreateNob 215 neuron( 982.51| 6167)
Voltage 215 neuron 0
CreateNob 216 neuron( 1082.42| 6162.73)
Voltage 216 neuron 0
CreateNob 217 neuron( 1182.33| 6158.47)
Voltage 217 neuron 0
CreateNob 218 knot( 390.737| 4776.96)
Voltage 218 knot 0
CreateNob 219 knot( 494.24| 4640.17)
Voltage 219 knot 8.4
CreateNob 220 knot( 615.09| 6468.33)
Voltage 220 knot 0
CreateNob 221 knot( 764.39| 6533.27)
Voltage 221 knot 0
CreateNob 222 knot( 900.05| 6522.35)
Voltage 222 knot 15.1
CreateNob 232 knot( 1010.17| 5263.73)
Voltage 232 knot 0
CreateNob 233 knot( 1106.7| 5319.71)
Voltage 233 knot 0
CreateNob 234 knot( 1213.05| 5342.92)
Voltage 234 knot 0
CreateNob 235 knot( 810.64| 5574.02)
Voltage 235 knot 0
CreateNob 236 knot( 932.97| 5590.44)
Voltage 236 knot 0
CreateNob 237 knot( 1052.02| 5602.69)
Voltage 237 knot 298.7
CreateNob 241 knot( 1430.64| 4650.65)
Voltage 241 knot 9.1
CreateNob 242 knot( 1526.75| 4647.4)
Voltage 242 knot 0
CreateNob 243 knot( 1626.12| 4668.7)
Voltage 243 knot 0
CreateNob 244 neuron( 1298.19| 4860.35)
Voltage 244 neuron 10
CreateNob 245 neuron( 1397.99| 4854.06)
Voltage 245 neuron 0
CreateNob 246 neuron( 1497.79| 4847.79)
Voltage 246 neuron 0
CreateNob 248 outputLine( 1226.7| 5312.41)
Voltage 248 outputLine 0
CreateNob 249 outputLine( 1117.49| 5287.05)
Voltage 249 outputLine 0
CreateNob 252 outputLine( 1021.37| 5237.88)
Voltage 252 outputLine 0
CreateNob 267 neuron( 1624.57| 3919.44)
Voltage 267 neuron 0
CreateNob 268 neuron( 1524.77| 3925.71)
Voltage 268 neuron 0
CreateNob 269 neuron( 1424.97| 3931.99)
Voltage 269 neuron 0
CreateNob 270 neuron( 241.557| 3942.73)
Voltage 270 neuron 0
CreateNob 271 neuron( 141.562| 3941.7)
Voltage 271 neuron 0
CreateNob 272 neuron( 41.5671| 3940.68)
Voltage 272 neuron 0
CreateNob 273 neuron( -1269.84| 2413.01)
Voltage 273 neuron 0
CreateNob 274 neuron( -1427.65| 2475.64)
Voltage 274 neuron 0
CreateNob 275 neuron( -1527.64| 2474.02)
Voltage 275 neuron 0
CreateNob 276 knot( -1343.94| 1532.75)
Voltage 276 knot 0
CreateNob 278 knot( -1251.26| 1202.08)
Voltage 278 knot 0
CreateNob 279 knot( -1248.95| 1219.34)
Voltage 279 knot 0
CreateNob 281 knot( -1113.68| 1234.69)
Voltage 281 knot 7.5
CreateNob 282 knot( -1099.31| 1235.46)
Voltage 282 knot 5.1
CreateNob 285 knot( -833.7| 4110.41)
Voltage 285 knot 0
CreateNob 287 knot( -799.34| 3406.66)
Voltage 287 knot 0
CreateNob 289 knot( -114.832| 3335.1)
Voltage 289 knot 9.6
CreateNob 291 knot( 944.462| 3079.36)
Voltage 291 knot 302.6
CreateNob 293 knot( 884.863| 2576.47)
Voltage 293 knot 0
CreateNob 295 knot( 825.26| 2212.21)
Voltage 295 knot 0
CreateNob 297 knot( -283.563| 2986.56)
Voltage 297 knot 0
CreateNob 299 knot( -132.326| 3026.11)
Voltage 299 knot 0
CreateNob 301 knot( 24.1746| 2939.38)
Voltage 301 knot 0
CreateNob 303 knot( 43.3253| 1246.78)
Voltage 303 knot 0
CreateNob 304 knot( 546.643| 6941.7)
Voltage 304 knot 0
CreateNob 311 knot( -3126.32| 6715.44)
Voltage 311 knot 0
CreateNob 313 knot( -3101.73| 6730.47)
Voltage 313 knot 0
CreateNob 314 inputLine( -4827.92| 3255.73)
Voltage 314 inputLine 0
CreateNob 317 inputLine( -4702.27| 3235.23)
Voltage 317 inputLine 0
CreateNob 318 inputLine( -4571.29| 3241.99)
Voltage 318 inputLine 0
CreateNob 321 inputLine( -4452.24| 3238.74)
Voltage 321 inputLine 0
CreateNob 322 inputLine( -4333.2| 3244.43)
Voltage 322 inputLine 0
CreateNob 324 knot( -2756.52| 6220.27)
Voltage 324 knot 246.1
CreateNob 325 knot( -2936.09| 6263.96)
Voltage 325 knot 0
CreateNob 326 knot( -3088.66| 6290.02)
Voltage 326 knot 0
CreateNob 327 knot( -4099.57| 3941.68)
Voltage 327 knot 0
CreateNob 328 knot( -4203.07| 4078.48)
Voltage 328 knot 0
CreateNob 329 knot( -4281.08| 4147.81)
Voltage 329 knot 0
CreateNob 330 inputLine( -4966.85| 3230.93)
Voltage 330 inputLine 0
CreateNob 339 inputLine( -4059.89| 3246.3)
Voltage 339 inputLine 0
CreateNob 340 knot( -3414.12| 4123.19)
Voltage 340 knot 0
CreateNob 344 neuron( -2617.08| 5379.61)
Voltage 344 neuron 0
CreateNob 345 neuron( -2517.17| 5375.35)
Voltage 345 neuron 0
CreateNob 346 neuron( -2417.26| 5371.08)
Voltage 346 neuron 0
CreateNob 347 knot( -3336.11| 4053.85)
Voltage 347 knot 0
CreateNob 348 knot( -3232.6| 3917.06)
Voltage 348 knot 9.1
CreateNob 349 knot( -3004.5| 5649.05)
Voltage 349 knot 0
CreateNob 350 knot( -2855.2| 5725.89)
Voltage 350 knot 0
CreateNob 351 knot( -2719.54| 5714.97)
Voltage 351 knot 6.4
CreateNob 361 knot( -2676.67| 4580.61)
Voltage 361 knot 0
CreateNob 362 knot( -2580.14| 4636.6)
Voltage 362 knot 0
CreateNob 363 knot( -2473.79| 4659.81)
Voltage 363 knot 0
CreateNob 364 knot( -2916.2| 4850.91)
Voltage 364 knot 0
CreateNob 365 knot( -2793.87| 4867.32)
Voltage 365 knot 0
CreateNob 366 knot( -2674.82| 4879.57)
Voltage 366 knot 0
CreateNob 370 knot( -2296.2| 3927.54)
Voltage 370 knot 0
CreateNob 371 knot( -2200.09| 3924.28)
Voltage 371 knot 0
CreateNob 372 knot( -2100.72| 3945.59)
Voltage 372 knot 0
CreateNob 373 neuron( -2408.65| 4157.23)
Voltage 373 neuron 0
CreateNob 374 neuron( -2308.85| 4150.95)
Voltage 374 neuron 0
CreateNob 375 neuron( -2209.05| 4144.68)
Voltage 375 neuron 0
CreateNob 377 outputLine( -2460.14| 4629.29)
Voltage 377 outputLine 0
CreateNob 378 outputLine( -2569.35| 4603.94)
Voltage 378 outputLine 0
CreateNob 381 outputLine( -2665.47| 4554.76)
Voltage 381 outputLine 0
CreateNob 388 inputLine( -3106.59| 3244.94)
Voltage 388 inputLine 0
CreateNob 391 inputLine( -2987.54| 3241.69)
Voltage 391 inputLine 0
CreateNob 392 inputLine( -2868.49| 3337.35)
Voltage 392 inputLine 0
CreateNob 395 inputLine( -3426.23| 3259.81)
Voltage 395 inputLine 0
CreateNob 396 inputLine( -3545.28| 3254.11)
Voltage 396 inputLine 0
CreateNob 399 inputLine( -3664.33| 3257.37)
Voltage 399 inputLine 0
CreateNob 400 inputLine( -3795.31| 3250.61)
Voltage 400 inputLine 0
CreateNob 403 inputLine( -3920.96| 3271.11)
Voltage 403 inputLine 0
CreateNob 404 knot( -853.483| 2278.55)
Voltage 404 knot 0
CreateNob 405 knot( -682.989| 2287.47)
Voltage 405 knot 0
CreateNob 406 knot( -515.872| 2275.1)
Voltage 406 knot 0
CreateNob 410 knot( -1524.87| 1836.56)
Voltage 410 knot 0
CreateNob 411 knot( -1627.33| 1918.33)
Voltage 411 knot 0
CreateNob 412 knot( -1752.56| 1999.17)
Voltage 412 knot 0
CreateNob 416 knot( -2838.69| 3527.51)
Voltage 416 knot 0
CreateNob 417 knot( -2970.05| 3716.17)
Voltage 417 knot 0
CreateNob 423 knot( -389.789| 261.387)
Voltage 423 knot 0
CreateNob 424 knot( -3022.9| 6671.44)
Voltage 424 knot 0
CreateNob 428 knot( -2850.75| 3424.66)
Voltage 428 knot 0
CreateNob 429 knot( 878.128| 6763.17)
Voltage 429 knot 9.1
CreateNob 432 knot( -1214.58| 351.091)
Voltage 432 knot 9.9
CreateNob 433 knot( 827.047| 7083.08)
Voltage 433 knot 17.5
CreateNob 437 knot( -3281.43| 3735.27)
Voltage 437 knot 6.4
CreateNob 438 knot( 874.35| 6659.45)
Voltage 438 knot 15
CreateNob 442 knot( -143.438| 369.466)
Voltage 442 knot 0
CreateNob 443 knot( -3077.58| 6341.56)
Voltage 443 knot 0
CreateNob 447 knot( 5164.62| 4840.19)
Voltage 447 knot 0
CreateNob 448 knot( -3043.29| 6480.42)
Voltage 448 knot 0
CreateNob 452 knot( 636.113| 2374.52)
Voltage 452 knot 350.8
CreateNob 453 knot( -2705.34| 6619.81)
Voltage 453 knot 283.5
CreateNob 457 knot( -1770.13| 1058.66)
Voltage 457 knot 7.5
CreateNob 458 knot( -3087.33| 5959.64)
Voltage 458 knot 0
CreateNob 462 knot( -3940.59| 4215.49)
Voltage 462 knot 290.4
CreateNob 463 knot( 1111.84| 5875.44)
Voltage 463 knot 277.1
CreateNob 468 knot( -755.063| 8358.39)
Voltage 468 knot 6.4
CreateNob 470 knot( 98.1973| 10102.5)
Voltage 470 knot 0
CreateNob 472 knot( 498.417| 10914.6)
Voltage 472 knot 7.5
CreateNob 474 knot( 117.177| 10564.9)
Voltage 474 knot 0
CreateNob 476 knot( 107.947| 10484.5)
Voltage 476 knot 0
CreateNob 478 knot( -95.9026| 7878.17)
Voltage 478 knot 5.1
CreateNob 480 knot( 334.778| 7567.56)
Voltage 480 knot 0
CreateNob 482 knot( 162.627| 10814.3)
Voltage 482 knot 0
CreateNob 488 knot( 235.477| 7879.07)
Voltage 488 knot 0
CreateNob 489 knot( 366.838| 7690.41)
Voltage 489 knot 0
CreateNob 500 outputLine( 500.058| 8677.66)
Voltage 500 outputLine 0
CreateNob 503 outputLine( 596.177| 8726.84)
Voltage 503 outputLine 0
CreateNob 504 outputLine( 705.388| 8752.19)
Voltage 504 outputLine 0
CreateNob 506 neuron( 956.478| 8267.58)
Voltage 506 neuron 5.1
CreateNob 507 neuron( 876.677| 8293.85)
Voltage 507 neuron 0
CreateNob 508 neuron( 776.878| 8300.13)
Voltage 508 neuron 0
CreateNob 509 knot( 999.98| 6270.73)
Voltage 509 knot 0
CreateNob 510 knot( 889.066| 6234.51)
Voltage 510 knot 0
CreateNob 511 knot( 530.708| 9042.47)
Voltage 511 knot 0
CreateNob 512 knot( 411.657| 9030.22)
Voltage 512 knot 0
CreateNob 513 knot( 289.327| 9013.81)
Voltage 513 knot 0
CreateNob 514 knot( 691.738| 8782.71)
Voltage 514 knot 0
CreateNob 515 knot( 585.388| 8759.5)
Voltage 515 knot 0
CreateNob 516 knot( 488.858| 8703.51)
Voltage 516 knot 0
CreateNob 517 outputLine( 186.887| 10999.2)
Voltage 517 outputLine 0
CreateNob 518 outputLine( 339.687| 11005.9)
Voltage 518 outputLine 253.5
CreateNob 519 outputLine( 488.228| 10995.6)
Voltage 519 outputLine 9.1
CreateNob 529 knot( 485.988| 9877.87)
Voltage 529 knot 0
CreateNob 530 knot( 350.328| 9888.79)
Voltage 530 knot 248.2
CreateNob 531 knot( 201.027| 9811.95)
Voltage 531 knot 0
CreateNob 532 knot( -27.0728| 8079.96)
Voltage 532 knot 9.6
CreateNob 533 knot( -130.583| 8216.75)
Voltage 533 knot 256.9
CreateNob 534 neuron( 768.268| 9513.98)
Voltage 534 neuron 0
CreateNob 535 neuron( 668.358| 9518.25)
Voltage 535 neuron 0
CreateNob 536 neuron( 568.448| 9522.51)
Voltage 536 neuron 0
CreateNob 540 knot( -208.593| 8286.09)
Voltage 540 knot 0
CreateNob 549 knot( -1075.55| 8310.71)
Voltage 549 knot 0
CreateNob 550 knot( -997.542| 8241.38)
Voltage 550 knot 0
CreateNob 551 knot( -894.042| 8104.58)
Voltage 551 knot 9.6
CreateNob 552 knot( 116.867| 10452.9)
Voltage 552 knot 0
CreateNob 553 knot( 269.437| 10426.9)
Voltage 553 knot 196.8
CreateNob 554 knot( 449.008| 10383.2)
Voltage 554 knot 8.4
CreateNob 560 knot( 63.7974| 10853.4)
Voltage 560 knot 0
CreateNob 562 knot( 39.2073| 10838.3)
Voltage 562 knot 0
CreateNob 566 knot( 475.348| 10596.1)
Voltage 566 knot 0
CreateNob 568 knot( 315.177| 10780.1)
Voltage 568 knot 281.5
CreateNob 570 knot( 194.197| 10971.8)
Voltage 570 knot 0
CreateNob 571 neuron( -1569.05| 7357.6)
Voltage 571 neuron 0
CreateNob 572 neuron( -1669.03| 7355.98)
Voltage 572 neuron 0
CreateNob 573 neuron( -1769.02| 7354.35)
Voltage 573 neuron 0
CreateNob 574 neuron( 295.554| 7391.47)
Voltage 574 neuron 9.6
CreateNob 575 neuron( 196.049| 7381.53)
Voltage 575 neuron 0
CreateNob 576 neuron( 96.5441| 7371.59)
Voltage 576 neuron 0
CreateNob 577 knot( -2019.24| 5528.47)
Voltage 577 knot 0
CreateNob 578 knot( -2317.17| 4969.27)
Voltage 578 knot 0
CreateNob 579 knot( -2414.43| 5001.77)
Voltage 579 knot 0
CreateNob 585 knot( 1090.31| 6284.59)
Voltage 585 knot 0
CreateNob 588 knot( -2009.94| 5688.87)
Voltage 588 knot 0
CreateNob 589 knot( -1878.64| 5619.91)
Voltage 589 knot 0
CreateNob 590 knot( 794.75| 4805.58)
Voltage 590 knot 0
CreateNob 591 knot( 1027.17| 5038.77)
Voltage 591 knot 288.1
CreateNob 592 knot( 919.424| 4758.89)
Voltage 592 knot 0
CreateNob 593 knot( -2139.09| 5703.42)
Voltage 593 knot 0
CreateNob 600 knot( 2318.04| 7605.9)
Voltage 600 knot 0
CreateNob 602 knot( 3187.7| 7133.07)
Voltage 602 knot 0
CreateNob 604 knot( 1255.77| 7311.72)
Voltage 604 knot 0
CreateNob 1 pipe(0->62)
Voltage 1 pipe(98: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 2 pipe(3->61)
Voltage 2 pipe(87: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 5 pipe(69->60)
Voltage 5 pipe(164: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 6 pipe(62->59)
Voltage 6 pipe(180: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 9 pipe(61->58)
Voltage 9 pipe(193: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 10 pipe(11->43)
Voltage 10 pipe(164: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 13 pipe(12->44)
Voltage 13 pipe(164: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 14 pipe(15->45)
Voltage 14 pipe(162: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 17 pipe(16->39)
Voltage 17 pipe(77: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 18 pipe(19->38)
Voltage 18 pipe(76: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 21 pipe(20->37)
Voltage 21 pipe(78: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 22 pipe(37->34)
Voltage 22 pipe(23: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 25 pipe(38->35)
Voltage 25 pipe(25: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 26 pipe(39->36)
Voltage 26 pipe(26: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 29 pipe(36->28)
Voltage 29 pipe(47: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 30 pipe(35->31)
Voltage 30 pipe(52: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 33 pipe(34->32)
Voltage 33 pipe(55: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 40 pipe(23->37)
Voltage 40 pipe(71: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 41 pipe(24->38)
Voltage 41 pipe(68: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 42 pipe(27->39)
Voltage 42 pipe(68: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 49 pipe(43->63)
Voltage 49 pipe(60: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 50 pipe(44->64)
Voltage 50 pipe(62: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 51 pipe(45->65)
Voltage 51 pipe(64: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 52 pipe(83->269)
Voltage 52 pipe(70: 259.8, 278.5, 278.6, 278.7, 279, 283.5, 274.2, 258.1, 248.2, 239.1, 232.9, 229.4, 236.8, 227.8, 237.5, 229.7, 239, 232.4, 243.5, 257.1, 280.9, 290.7, 288.6, 302.1, 307, 310.7, 306.3, 315.8, 318.5, 306.3, 302.3, 299.2, 297.2, 290.6, 283.6, 280.2, 276.4, 268.2, 261.9, 257.9, 262.1, 264.2, 262.1, 259.6, 256.5, 255.1, 261.7, 268.3, 276.8, 281.1, 283.3, 283.6, 284.1, 282.5, 286.2, 291, 293.1, 292.5, 290.9, 287.7, 291.3, 287.1, 279.2, 275.1, 268.9, 258.7, 252.7, 249, 255.4, 259.7)
CreateNob 53 pipe(84->268)
Voltage 53 pipe(69: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 54 pipe(85->267)
Voltage 54 pipe(70: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 55 pipe(46->43)
Voltage 55 pipe(30: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 56 pipe(47->44)
Voltage 56 pipe(31: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 57 pipe(48->45)
Voltage 57 pipe(36: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 66 pipe(63->58)
Voltage 66 pipe(46: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 67 pipe(64->59)
Voltage 67 pipe(49: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 68 pipe(65->60)
Voltage 68 pipe(47: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 71 pipe(447->69)
Voltage 71 pipe(573: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 11.5, 13.5, 15.1, 16.3, 17.1, 17.5, 17.5, 17.1, 16.3, 15.1, 13.5, 11.5, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 72 pipe(8->61)
Voltage 72 pipe(69: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 73 pipe(7->62)
Voltage 73 pipe(83: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 74 pipe(4->69)
Voltage 74 pipe(90: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 75 pipe(91->80)
Voltage 75 pipe(95: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 76 pipe(442->81)
Voltage 76 pipe(64: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 77 pipe(87->82)
Voltage 77 pipe(74: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 78 pipe(129->80)
Voltage 78 pipe(75: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 86 pipe(295->85)
Voltage 86 pipe(119: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 89 pipe(293->84)
Voltage 89 pipe(80: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 90 pipe(291->83)
Voltage 90 pipe(28: 283.7, 305.6, 309.5, 310.5, 308.8, 304.4, 297.1, 290.9, 286, 287.1, 271.2, 273.9, 272.3, 271.9, 266, 258.8, 258.3, 251.5, 250.4, 253, 256, 250.3, 262.3, 266.2, 268.2, 267.1, 267.6, 270.1)
CreateNob 93 pipe(139->82)
Voltage 93 pipe(70: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 94 pipe(134->81)
Voltage 94 pipe(73: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 97 pipe(96->98)
Voltage 97 pipe(3: 0, 0, 0)
CreateNob 99 pipe(58->85)
Voltage 99 pipe(51: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 100 pipe(59->84)
Voltage 100 pipe(54: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 101 pipe(60->83)
Voltage 101 pipe(65: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 102 pipe(114->107)
Voltage 102 pipe(65: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 103 pipe(115->106)
Voltage 103 pipe(54: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 104 pipe(289->105)
Voltage 104 pipe(27: 9.1, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4)
CreateNob 108 pipe(297->114)
Voltage 108 pipe(7: 0, 0, 0, 0, 0, 0, 0)
CreateNob 109 pipe(299->115)
Voltage 109 pipe(10: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 110 pipe(301->116)
Voltage 110 pipe(18: 9.9, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10)
CreateNob 117 pipe(105->270)
Voltage 117 pipe(47: 0, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0)
CreateNob 118 pipe(106->271)
Voltage 118 pipe(50: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 119 pipe(107->272)
Voltage 119 pipe(53: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 120 pipe(303->111)
Voltage 120 pipe(70: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 121 pipe(81->112)
Voltage 121 pipe(101: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 122 pipe(82->113)
Voltage 122 pipe(112: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 126 pipe(278->279)
Voltage 126 pipe(2: 0, 0)
CreateNob 127 pipe(281->282)
Voltage 127 pipe(1: 6.4)
CreateNob 130 pipe(129->128)
Voltage 130 pipe(108: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 131 pipe(79->129)
Voltage 131 pipe(41: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 132 pipe(124->128)
Voltage 132 pipe(95: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 135 pipe(134->133)
Voltage 135 pipe(103: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 136 pipe(95->134)
Voltage 136 pipe(24: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 137 pipe(123->133)
Voltage 137 pipe(55: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 140 pipe(139->138)
Voltage 140 pipe(97: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 141 pipe(423->139)
Voltage 141 pipe(14: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 142 pipe(432->138)
Voltage 142 pipe(18: 0, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0)
CreateNob 143 pipe(154->146)
Voltage 143 pipe(77: 19.5, 6.4, 5.1, 3.6, 3.8, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 11.8, 13.2, 14.2, 14.8, 15, 14.8, 14.2, 13.2, 11.8, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 11.8, 13.6, 15, 16, 16.6, 16.8, 16.6, 16, 15, 13.6, 11.8, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 5.5, 8.7, 11.5, 13.9, 15.9, 17.5, 18.7)
CreateNob 144 pipe(151->147)
Voltage 144 pipe(77: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 145 pipe(149->148)
Voltage 145 pipe(72: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 150 pipe(276->275)
Voltage 150 pipe(96: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 152 pipe(151->274)
Voltage 152 pipe(100: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 153 pipe(278->151)
Voltage 153 pipe(32: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 155 pipe(154->273)
Voltage 155 pipe(96: 0, 7.5, 6.4, 5.1, 3.6, 3.8, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 11.8, 13.2, 14.2, 14.8, 15, 14.8, 14.2, 13.2, 11.8, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 11.8, 13.6, 15, 16, 16.6, 16.8, 16.6, 16, 15, 13.6, 11.8, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 5.5, 8.7, 11.5, 13.9, 15.9, 17.5, 18.7, 19.5, 19.9, 19.9, 19.5, 18.7, 17.5, 15.9, 13.9, 11.5, 8.7, 5.5, 1.9, 0, 0, 0, 0, 0, 0)
CreateNob 156 pipe(281->154)
Voltage 156 pipe(27: 8.4, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1)
CreateNob 158 pipe(406->157)
Voltage 158 pipe(83: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 159 pipe(113->157)
Voltage 159 pipe(113: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 161 pipe(405->160)
Voltage 161 pipe(88: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 162 pipe(112->160)
Voltage 162 pipe(123: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 164 pipe(404->163)
Voltage 164 pipe(86: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 165 pipe(111->163)
Voltage 165 pipe(115: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 166 pipe(185->167)
Voltage 166 pipe(304: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 168 pipe(184->170)
Voltage 168 pipe(310: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 169 pipe(417->170)
Voltage 169 pipe(290: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 171 pipe(183->173)
Voltage 171 pipe(297: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 172 pipe(416->173)
Voltage 172 pipe(291: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 174 pipe(410->175)
Voltage 174 pipe(237: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 176 pipe(411->177)
Voltage 176 pipe(232: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 178 pipe(412->179)
Voltage 178 pipe(240: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 180 pipe(199->183)
Voltage 180 pipe(112: 0, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 181 pipe(200->184)
Voltage 181 pipe(101: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 182 pipe(201->185)
Voltage 182 pipe(60: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 186 pipe(304->198)
Voltage 186 pipe(17: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 187 pipe(221->197)
Voltage 187 pipe(54: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 188 pipe(429->196)
Voltage 188 pipe(26: 7.5, 11.5, 13.5, 15.1, 16.3, 17.1, 17.5, 17.5, 17.1, 16.3, 15.1, 13.5, 11.5, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 1.9, 3.6, 5.1, 6.4)
CreateNob 190 pipe(177->200)
Voltage 190 pipe(76: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 191 pipe(285->199)
Voltage 191 pipe(72: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 193 pipe(185->198)
Voltage 193 pipe(163: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 194 pipe(184->197)
Voltage 194 pipe(160: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 195 pipe(183->196)
Voltage 195 pipe(155: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 203 pipe(179->201)
Voltage 203 pipe(102: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 204 pipe(289->199)
Voltage 204 pipe(135: 1.9, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 205 pipe(287->200)
Voltage 205 pipe(138: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 206 pipe(285->201)
Voltage 206 pipe(107: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 207 pipe(272->211)
Voltage 207 pipe(95: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 208 pipe(271->218)
Voltage 208 pipe(87: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 209 pipe(270->219)
Voltage 209 pipe(74: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 210 pipe(287->211)
Voltage 210 pipe(182: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 212 pipe(510->220)
Voltage 212 pipe(36: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 213 pipe(509->221)
Voltage 213 pipe(35: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 214 pipe(585->222)
Voltage 214 pipe(30: 3.6, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9)
CreateNob 223 pipe(232->235)
Voltage 223 pipe(37: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 224 pipe(233->236)
Voltage 224 pipe(32: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 225 pipe(234->237)
Voltage 225 pipe(31: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 226 pipe(433->574)
Voltage 226 pipe(61: 9.9, 17.5, 17.1, 16.3, 15.1, 13.5, 11.5, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 8.3, 11.1, 13.5, 15.5, 17.1, 18.3, 19.1, 19.5, 19.5, 19.1, 18.3, 17.1, 15.5, 13.5, 11.1, 8.3, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10)
CreateNob 227 pipe(197->575)
Voltage 227 pipe(58: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 228 pipe(198->576)
Voltage 228 pipe(51: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 229 pipe(235->215)
Voltage 229 pipe(62: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 230 pipe(236->216)
Voltage 230 pipe(59: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 231 pipe(463->217)
Voltage 231 pipe(29: 282.2, 280.2, 279.5, 276.9, 272.4, 262.2, 254.3, 248.7, 241.6, 235.1, 229.4, 224.7, 224.8, 223.6, 223.2, 225.5, 226.5, 230.4, 233.4, 235.5, 240.7, 245, 252, 261.3, 266.8, 270.2, 273.4, 276.6, 279.6)
CreateNob 238 pipe(269->241)
Voltage 238 pipe(72: 9.6, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9)
CreateNob 239 pipe(268->242)
Voltage 239 pipe(72: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 240 pipe(267->243)
Voltage 240 pipe(75: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 247 pipe(246->248)
Voltage 247 pipe(54: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 250 pipe(245->249)
Voltage 250 pipe(52: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 251 pipe(244->252)
Voltage 251 pipe(47: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 253 pipe(241->244)
Voltage 253 pipe(25: 9.9, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6)
CreateNob 254 pipe(242->245)
Voltage 254 pipe(24: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 255 pipe(243->246)
Voltage 255 pipe(22: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 256 pipe(301->243)
Voltage 256 pipe(236: 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 257 pipe(299->242)
Voltage 257 pipe(232: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 258 pipe(297->241)
Voltage 258 pipe(239: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 259 pipe(590->235)
Voltage 259 pipe(77: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 260 pipe(592->236)
Voltage 260 pipe(83: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 261 pipe(591->237)
Voltage 261 pipe(56: 301, 284.3, 281.8, 274.7, 271, 269, 263, 257.2, 249.7, 246.8, 240.9, 236.2, 227, 219.2, 212.6, 211, 212.1, 215.7, 223.9, 238.4, 248.7, 259, 271.4, 281.7, 293.9, 302.1, 310.1, 315.6, 320.5, 320.8, 318.4, 313.1, 300.7, 291.1, 276.5, 265.3, 255.8, 244.4, 231.5, 231.4, 228.3, 226, 224.5, 224, 226.4, 227.5, 229.4, 238, 248.9, 257.9, 266.9, 275.9, 282.8, 289.7, 294.5, 298.9)
CreateNob 262 pipe(219->222)
Voltage 262 pipe(193: 9.9, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6)
CreateNob 263 pipe(218->221)
Voltage 263 pipe(180: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 264 pipe(211->220)
Voltage 264 pipe(165: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 265 pipe(273->219)
Voltage 265 pipe(284: 9.1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6)
CreateNob 266 pipe(274->218)
Voltage 266 pipe(293: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 277 pipe(128->276)
Voltage 277 pipe(39: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 280 pipe(133->278)
Voltage 280 pipe(48: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 283 pipe(138->281)
Voltage 283 pipe(71: 8.4, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 11.9, 13.5, 14.7, 15.5, 15.9, 15.9, 15.5, 14.7, 13.5, 11.9, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1)
CreateNob 284 pipe(149->276)
Voltage 284 pipe(1: 0)
CreateNob 286 pipe(175->285)
Voltage 286 pipe(1: 0)
CreateNob 288 pipe(275->287)
Voltage 288 pipe(118: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 290 pipe(116->289)
Voltage 290 pipe(24: 9.1, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4)
CreateNob 292 pipe(452->291)
Voltage 292 pipe(77: 294.6, 347.4, 342.8, 331.1, 314.4, 296.9, 278.8, 264.1, 250.9, 235.4, 219.7, 208, 202.8, 206.6, 213.5, 223.5, 230.7, 237, 240.3, 246.7, 256, 265.7, 283.3, 285.5, 288.9, 291.5, 290.1, 290.8, 293.5, 289.2, 278.3, 269.8, 261.5, 253.6, 248.4, 244, 236.4, 227.7, 216, 205.3, 201.7, 207.1, 209.3, 218.4, 226.4, 237.1, 244.4, 252.5, 263.3, 274.3, 285.3, 292.1, 298.5, 300.5, 302.1, 303.3, 298.2, 288.9, 279.2, 265.1, 254.8, 250.6, 244.7, 239.2, 232.4, 230.4, 229, 227.8, 228.9, 234.6, 242.8, 253.3, 262.1, 269.2, 276.5, 282.1, 287.7)
CreateNob 294 pipe(112->293)
Voltage 294 pipe(81: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 296 pipe(113->295)
Voltage 296 pipe(35: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 298 pipe(163->297)
Voltage 298 pipe(12: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 300 pipe(160->299)
Voltage 300 pipe(14: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 302 pipe(157->301)
Voltage 302 pipe(10: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 305 pipe(304->303)
Voltage 305 pipe(572: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 306 pipe(220->304)
Voltage 306 pipe(48: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 307 pipe(80->303)
Voltage 307 pipe(16: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 308 pipe(458->326)
Voltage 308 pipe(33: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 309 pipe(350->325)
Voltage 309 pipe(54: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 310 pipe(351->324)
Voltage 310 pipe(51: 0, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0)
CreateNob 312 pipe(313->311)
Voltage 312 pipe(3: 0, 0, 0)
CreateNob 315 pipe(314->328)
Voltage 315 pipe(103: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 316 pipe(317->327)
Voltage 316 pipe(93: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 319 pipe(329->326)
Voltage 319 pipe(245: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 320 pipe(328->325)
Voltage 320 pipe(253: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 323 pipe(462->324)
Voltage 323 pipe(233: 244.1, 286.3, 281.2, 277.4, 275.5, 272.1, 269.5, 269.4, 271.4, 269.4, 263.4, 255.5, 253.5, 251.1, 248.3, 243.2, 250.1, 257.2, 260.5, 264.2, 268.5, 271.3, 274.5, 274.1, 279.6, 280.3, 278.1, 279.3, 282.2, 280.9, 277.3, 271.2, 270.8, 266.6, 262.8, 261.1, 263.4, 264, 264.8, 261.6, 266, 284.562, 276, 277.4, 284.1, 290.6, 292.9, 295, 295, 292.9, 288.7, 284.5, 280.3, 271.9, 261.2, 254.5, 249.9, 241.3, 232.9, 220.9, 219.6, 217.6, 219.1, 222, 224.2, 227.6, 230.1, 229.8, 228.8, 227.1, 222.8, 219.9, 218.4, 222.1, 224.7, 228.1, 234.4, 243.4, 256.4, 266.9, 278.9, 284.4, 289.5, 294.4, 301, 307.2, 313, 312.3, 309.1, 303.4, 296.9, 291.3, 284.5, 282.6, 281.6, 275.6, 266.9, 259.9, 259.2, 257.2, 256, 253.5, 249.5, 244, 243.1, 246.6, 256, 262.9, 267.1, 270.5, 275.2, 281.4, 285.3, 291.1, 295.2, 297.6, 296, 292.3, 290.3, 285.8, 282.6, 276.7, 270.2, 257.2, 244, 233.1, 231, 228, 224.3, 222, 221.1, 221.6, 219.5, 218.8, 222.9, 231.2, 239.5, 251.4, 262.9, 274.2, 287.4, 298.5, 307.5, 312.3, 312.7, 308.7, 304.5, 298.2, 287.7, 275.1, 264.4, 251.6, 234.8, 218, 209.6, 202.2, 196.2, 194.1, 190.2, 186.6, 185.4, 188.5, 191.7, 198.8, 205.6, 210, 215.8, 222.8, 230.8, 235.6, 243.3, 255.6, 264.1, 269, 272.6, 276.8, 279.3, 281.8, 283.9, 279.5, 272.8, 268, 267, 265.8, 266.5, 263, 261.4, 259.8, 260.3, 258.9, 257.5, 264.3, 271.3, 272.2, 272.7, 276.6, 278, 279.2, 280.6, 278.2, 271.8, 263.5, 259.6, 256.1, 256.8, 259.4, 263.5, 265.1, 268.2, 268.8, 267.1, 267.3, 271.5, 273.6, 277.6, 281.8, 284.3, 284.9, 283.4, 285.5, 284.9, 279.5, 273.3, 266.5, 259.5, 254.8, 250.9, 249.7, 252.9, 250.6, 246.8, 243.4, 242.5)
CreateNob 331 pipe(330->329)
Voltage 331 pipe(114: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 332 pipe(322->327)
Voltage 332 pipe(74: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 333 pipe(321->328)
Voltage 333 pipe(88: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 334 pipe(318->329)
Voltage 334 pipe(95: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 335 pipe(399->340)
Voltage 335 pipe(90: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 336 pipe(396->347)
Voltage 336 pipe(83: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 337 pipe(437->348)
Voltage 337 pipe(19: 9.6, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 11.5, 12.7, 13.5, 13.9, 13.9, 13.5, 12.7, 11.5, 9.9, 10, 9.9)
CreateNob 338 pipe(339->340)
Voltage 338 pipe(109: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 341 pipe(344->349)
Voltage 341 pipe(47: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 342 pipe(345->350)
Voltage 342 pipe(49: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 343 pipe(346->351)
Voltage 343 pipe(46: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 352 pipe(361->364)
Voltage 352 pipe(36: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 353 pipe(362->365)
Voltage 353 pipe(31: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 354 pipe(363->366)
Voltage 354 pipe(30: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 355 pipe(453->571)
Voltage 355 pipe(135: 239.1, 291.4, 294.3, 294.2, 293.3, 289.5, 291, 294, 292.6, 294.6, 296, 295.1, 291.9, 286.2, 279.9, 273, 273.6, 269.4, 262.9, 260.4, 261.2, 254.6, 254, 251.7, 251.7, 248.1, 252.7, 264.9, 276.1, 292.2, 305.2, 314.9, 321.3, 324.6, 326.9, 328.2, 328.5, 329.7, 325.7, 316.7, 309, 296.7, 283.8, 268.4, 252.6, 232.4, 218.1, 212.4, 211.7, 212.2, 216, 221, 223.2, 222.6, 221.3, 223.3, 226.5, 233, 236.9, 238, 238.2, 235.6, 236.1, 239.3, 242.9, 242.9, 243.3, 244.1, 249.3, 250.9, 253.1, 256.1, 259.7, 261.6, 263.5, 265.4, 267.5, 267.9, 266.6, 265.5, 260.6, 253.8, 247.2, 247.1, 251.4, 255.7, 257.9, 258.2, 260.8, 261.7, 258.8, 254.2, 250.2, 252.7, 257.5, 256.8, 256.7, 255.1, 252, 251.4, 251, 250.6, 248.1, 245.6, 245.6, 250.4, 253.9, 259.7, 265.5, 269.4, 271.4, 273.2, 274.4, 281.1, 289.5, 291.4, 290.8, 289.8, 284.4, 278.8, 271.5, 268.6, 264, 255.8, 245.9, 238.3, 229.2, 224.9, 227.3, 226.1, 225.3, 225.1, 227.8, 229.6, 234.7)
CreateNob 356 pipe(170->572)
Voltage 356 pipe(143: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 357 pipe(167->573)
Voltage 357 pipe(133: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 358 pipe(364->344)
Voltage 358 pipe(61: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 359 pipe(365->345)
Voltage 359 pipe(58: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 360 pipe(366->346)
Voltage 360 pipe(55: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 367 pipe(404->370)
Voltage 367 pipe(219: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 368 pipe(405->371)
Voltage 368 pipe(223: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 369 pipe(406->372)
Voltage 369 pipe(230: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 376 pipe(375->377)
Voltage 376 pipe(55: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 379 pipe(374->378)
Voltage 379 pipe(52: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 380 pipe(373->381)
Voltage 380 pipe(47: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 382 pipe(370->373)
Voltage 382 pipe(26: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 383 pipe(371->374)
Voltage 383 pipe(25: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 384 pipe(372->375)
Voltage 384 pipe(23: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 385 pipe(410->372)
Voltage 385 pipe(219: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 386 pipe(411->371)
Voltage 386 pipe(209: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 387 pipe(412->370)
Voltage 387 pipe(200: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 389 pipe(388->364)
Voltage 389 pipe(162: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 390 pipe(417->365)
Voltage 390 pipe(116: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 393 pipe(416->366)
Voltage 393 pipe(136: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 394 pipe(348->351)
Voltage 394 pipe(187: 5.1, 9.1, 8.4, 7.5, 6.4, 5.1, 5.5, 5.5, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 11.5, 12.7, 13.5, 13.9, 13.9, 13.5, 12.7, 11.5, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 11, 13.2, 15, 16.4, 17.4, 18, 18.2, 18, 17.4, 16.4, 15, 13.2, 11, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 11.9, 13.5, 14.7, 15.5, 15.9, 15.9, 15.5, 14.7, 13.5, 11.9, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 11.9, 13.5, 14.7, 15.5, 15.9, 15.9, 15.5, 14.7, 13.5, 11.9, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6)
CreateNob 397 pipe(347->350)
Voltage 397 pipe(174: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 398 pipe(340->349)
Voltage 398 pipe(158: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 401 pipe(400->348)
Voltage 401 pipe(87: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 402 pipe(403->347)
Voltage 402 pipe(98: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 407 pipe(146->406)
Voltage 407 pipe(17: 0, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 408 pipe(147->405)
Voltage 408 pipe(15: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 409 pipe(148->404)
Voltage 409 pipe(17: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 413 pipe(457->410)
Voltage 413 pipe(82: 0, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 414 pipe(189->411)
Voltage 414 pipe(176: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 415 pipe(202->412)
Voltage 415 pipe(187: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 418 pipe(428->416)
Voltage 418 pipe(10: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 419 pipe(391->417)
Voltage 419 pipe(47: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 420 pipe(424->167)
Voltage 420 pipe(16: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 421 pipe(325->170)
Voltage 421 pipe(36: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 422 pipe(324->173)
Voltage 422 pipe(21: 267.6, 246.1, 244.5, 241.4, 237, 231.5, 231.2, 234.2, 236.3, 237.5, 237.8, 235.1, 235.1, 235.3, 243.5, 253.8, 258.2, 262.6, 265.1, 265.9, 267.3)
CreateNob 425 pipe(578->423)
Voltage 425 pipe(509: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 11.5, 13.5, 15.1, 16.3, 17.1, 17.5, 17.5, 17.1, 16.3, 15.1, 13.5, 11.5, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 426 pipe(448->424)
Voltage 426 pipe(19: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 427 pipe(92->423)
Voltage 427 pipe(9: 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 430 pipe(438->429)
Voltage 430 pipe(10: 10.3, 13.6, 11.8, 9.6, 9.1, 10.3, 11.1, 11.5, 11.5, 11.1)
CreateNob 431 pipe(392->428)
Voltage 431 pipe(9: 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 434 pipe(433->432)
Voltage 434 pipe(703: 9.6, 17.5, 17.5, 17.1, 16.3, 15.1, 13.5, 11.5, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 8.3, 11.1, 13.5, 15.5, 17.1, 18.3, 19.1, 19.5, 19.5, 19.1, 18.3, 17.1, 15.5, 13.5, 11.1, 8.3, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 10.3, 12.7, 14.7, 16.3, 17.5, 18.3, 18.7, 18.7, 18.3, 17.5, 16.3, 14.7, 12.7, 10.3, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 11.9, 13.5, 14.7, 15.5, 15.9, 15.9, 15.5, 14.7, 13.5, 11.9, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 5.5, 5.5, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 11.5, 12.7, 13.5, 13.9, 13.9, 13.5, 12.7, 11.5, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 5.5, 5.5, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 11.5, 12.7, 13.5, 13.9, 13.9, 13.5, 12.7, 11.5, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 5.5, 5.5, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 11.5, 12.7, 13.5, 13.9, 13.9, 13.5, 12.7, 11.5, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 5.5, 5.5, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 11.5, 12.7, 13.5, 13.9, 13.9, 13.5, 12.7, 11.5, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 11, 13.2, 15, 16.4, 17.4, 18, 18.2, 18, 17.4, 16.4, 15, 13.2, 11, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 11.9, 13.5, 14.7, 15.5, 15.9, 15.9, 15.5, 14.7, 13.5, 11.9, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 11.9, 13.5, 14.7, 15.5, 15.9, 15.9, 15.5, 14.7, 13.5, 11.9, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1)
CreateNob 435 pipe(196->433)
Voltage 435 pipe(7: 17.1, 8.4, 9.1, 11.5, 13.5, 15.1, 16.3)
CreateNob 436 pipe(125->432)
Voltage 436 pipe(19: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 439 pipe(438->437)
Voltage 439 pipe(508: 5.1, 15, 13.6, 11.8, 9.6, 9.1, 10.3, 11.1, 11.5, 11.5, 11.1, 10.3, 9.1, 11.5, 13.5, 15.1, 16.3, 17.1, 17.5, 17.5, 17.1, 16.3, 15.1, 13.5, 11.5, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 11.5, 13.5, 15.1, 16.3, 17.1, 17.5, 17.5, 17.1, 16.3, 15.1, 13.5, 11.5, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 8.3, 11.1, 13.5, 15.5, 17.1, 18.3, 19.1, 19.5, 19.5, 19.1, 18.3, 17.1, 15.5, 13.5, 11.1, 8.3, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 10.3, 12.7, 14.7, 16.3, 17.5, 18.3, 18.7, 18.7, 18.3, 17.5, 16.3, 14.7, 12.7, 10.3, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 11.9, 13.5, 14.7, 15.5, 15.9, 15.9, 15.5, 14.7, 13.5, 11.9, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 5.5, 5.5, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 11.5, 12.7, 13.5, 13.9, 13.9, 13.5, 12.7, 11.5, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 5.5, 5.5, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 11.5, 12.7, 13.5, 13.9, 13.9, 13.5, 12.7, 11.5, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 5.5, 5.5)
CreateNob 440 pipe(222->438)
Voltage 440 pipe(14: 16, 15.1, 16.3, 17.1, 17.5, 17.5, 17.1, 18.2, 18.7, 18.6, 17.9, 16.6, 16.8, 16.6)
CreateNob 441 pipe(395->437)
Voltage 441 pipe(50: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 444 pipe(579->442)
Voltage 444 pipe(516: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 11.5, 13.5, 15.1, 16.3, 17.1, 17.5, 17.5, 17.1, 16.3, 15.1, 13.5, 11.5, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 445 pipe(326->443)
Voltage 445 pipe(5: 0, 0, 0, 0, 0)
CreateNob 446 pipe(88->442)
Voltage 446 pipe(18: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 449 pipe(602->447)
Voltage 449 pipe(303: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 450 pipe(443->448)
Voltage 450 pipe(14: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 451 pipe(70->447)
Voltage 451 pipe(654: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 454 pipe(453->452)
Voltage 454 pipe(540: 350.9, 283.5, 291.4, 294.3, 294.2, 293.3, 289.5, 291, 294, 292.6, 294.6, 296, 295.1, 291.9, 286.2, 279.9, 273, 273.6, 269.4, 262.9, 260.4, 261.2, 254.6, 254, 251.7, 251.7, 248.1, 252.7, 264.9, 276.1, 292.2, 305.2, 314.9, 321.3, 324.6, 326.9, 328.2, 328.5, 329.7, 325.7, 316.7, 309, 296.7, 283.8, 268.4, 252.6, 232.4, 218.1, 212.4, 211.7, 212.2, 216, 221, 223.2, 222.6, 221.3, 223.3, 226.5, 233, 236.9, 238, 238.2, 235.6, 236.1, 239.3, 242.9, 242.9, 243.3, 244.1, 249.3, 250.9, 253.1, 256.1, 259.7, 261.6, 263.5, 265.4, 267.5, 267.9, 266.6, 265.5, 260.6, 253.8, 247.2, 247.1, 251.4, 255.7, 257.9, 258.2, 260.8, 261.7, 258.8, 254.2, 250.2, 252.7, 257.5, 256.8, 256.7, 255.1, 252, 251.4, 251, 250.6, 248.1, 245.6, 245.6, 250.4, 253.9, 259.7, 265.5, 269.4, 271.4, 273.2, 274.4, 281.1, 289.5, 291.4, 290.8, 289.8, 284.4, 278.8, 271.5, 268.6, 264, 255.8, 245.9, 238.3, 229.2, 224.9, 227.3, 226.1, 225.3, 225.1, 227.8, 229.6, 234.7, 239.1, 242.6, 247.1, 250.5, 252.6, 256.8, 264.8, 268.8, 270.7, 272.4, 275.8, 274.8, 271.5, 272.4, 271.8, 273.5, 269.5, 265.9, 258.7, 250, 240, 232.7, 227.9, 225.4, 221.2, 223.7, 227.2, 229.6, 232.8, 236.8, 243.7, 251.4, 257.6, 264, 268.5, 275.3, 276.4, 275.8, 275.4, 272.9, 266.2, 263.3, 260.4, 261.7, 261.1, 260.5, 258, 257.6, 255.3, 257, 256.6, 258.3, 260.2, 264.4, 266.9, 269.6, 270.4, 271, 275.2, 280.9, 283.9, 290.1, 293.4, 295.9, 295.9, 297.4, 296.2, 296.3, 291.8, 289, 284.1, 279, 275.6, 271.8, 267.6, 261.1, 252.5, 247.9, 243.5, 241.6, 240.3, 239.8, 242.2, 243.3, 241, 241.4, 244.5, 250.1, 254, 257.9, 259.7, 259.6, 259.9, 258.9, 258.7, 257.2, 252.3, 246.3, 237.5, 231.8, 227.1, 221.3, 220.3, 222, 222.2, 220.9, 222.3, 228.5, 235.5, 245.4, 252.3, 258.1, 262.6, 265.6, 267.1, 269.2, 273.6, 275.7, 277, 277.5, 275.5, 273.1, 272.4, 273.6, 276.7, 275.6, 272.4, 269.4, 264.9, 264.8, 260.9, 257.2, 253.7, 250.4, 245.2, 241.9, 242.6, 245.6, 249.2, 251.5, 250.4, 248, 246.4, 243.7, 244.1, 243.6, 242, 241, 238.3, 241.9, 243.8, 245.9, 246.3, 245.2, 242.8, 249, 251.8, 257.3, 259.4, 264, 266.9, 266, 263.4, 257.2, 253.3, 249.6, 246.1, 244.9, 246, 247.5, 253.4, 257.6, 260.1, 262.6, 260.9, 263.4, 262.3, 263.5, 263, 264.8, 267, 267.5, 266.1, 260.7, 255.5, 252.6, 248, 243.8, 240.2, 235.3, 233.3, 230.4, 230.4, 235.2, 236.8, 237.1, 238, 239.5, 241.6, 246.2, 249.3, 258.7, 270, 277.1, 280, 284.8, 287.5, 288.1, 286.6, 283, 277.3, 269.5, 263.8, 260.4, 253.4, 246.8, 240.6, 234.8, 229.4, 226.5, 222.1, 226.3, 233.4, 239.4, 247.9, 258.5, 271, 283.1, 294.6, 305.3, 316.9, 323.3, 326.6, 327, 326.4, 322.7, 318, 310.4, 302, 292.8, 279, 266.9, 254.6, 244, 237.2, 232.5, 228.2, 228.5, 229.4, 233, 239.5, 246.8, 250.7, 256.9, 263.3, 266.1, 267.4, 269.1, 276.7, 283.9, 286.5, 288.3, 291.2, 295.2, 300.3, 302.5, 305.6, 309.4, 308.2, 306.2, 301.5, 298.3, 292.6, 288.6, 282.5, 274.1, 263.2, 253.8, 252.2, 252.5, 252.6, 250.6, 248.8, 247.4, 246.6, 246.4, 252.7, 257.7, 257.4, 259.6, 260.1, 262.7, 269.1, 271.1, 272.7, 279.6, 281.7, 281.3, 280.7, 282.2, 287.7, 289, 293.7, 299.3, 299.7, 302.9, 305.1, 304.4, 298.7, 294.1, 286.8, 281, 279, 276.8, 276.1, 281.1, 281.9, 278.3, 270.3, 267.6, 270, 269.5, 274.3, 282.7, 284.8, 286.9, 285.2, 285.6, 283.9, 280.1, 278, 273.4, 266.5, 263.4, 262.2, 262.9, 263.4, 259.7, 253.7, 251.7, 252.2, 255.2, 260.5, 266.2, 266.4, 263, 257.9, 257, 258.2, 257.3, 258.5, 261.8, 259, 258.3, 255.9, 257.9, 260.3, 259.1, 256.4, 252.2, 248.4, 245.2, 246.8, 245.2, 244.2, 239.6, 237.3, 235.4, 236.2, 243.5, 250.8, 262.1, 269.4, 274.6, 275.8, 277.2, 280.9, 282.7, 280.5, 280.2, 277.8, 271.4, 265.2, 257.3, 251.7, 242.5, 235.8, 227.6, 217.9, 220.7, 222.3, 225.1, 231, 237.7, 242.9, 246.4, 256, 271.3, 289.6, 308.6, 326.2, 338.2, 344.6, 347.5)
CreateNob 455 pipe(173->453)
Voltage 455 pipe(19: 276.9, 268.7, 268.3, 262.4, 253.1, 244.4, 242.2, 242.3, 238.6, 235.1, 231.6, 232.1, 233, 238.7, 245.4, 251, 257.4, 264.4, 269.7)
CreateNob 456 pipe(111->452)
Voltage 456 pipe(58: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 459 pipe(458->457)
Voltage 459 pipe(507: 8.4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1)
CreateNob 460 pipe(349->458)
Voltage 460 pipe(32: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 461 pipe(192->457)
Voltage 461 pipe(88: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 464 pipe(463->462)
Voltage 464 pipe(532: 291.4, 277.1, 280.2, 279.5, 276.9, 272.4, 262.2, 254.3, 248.7, 241.6, 235.1, 229.4, 224.7, 224.8, 223.6, 223.2, 225.5, 226.5, 230.4, 233.4, 235.5, 240.7, 245, 252, 261.3, 266.8, 270.2, 273.4, 276.6, 279.6, 282.2, 282.3, 283.7, 290.4, 292.5, 295.9, 300.6, 302.6, 301.9, 296.6, 288.8, 282.5, 273.7, 270.6, 269.4, 264.2, 261.1, 254.2, 247.5, 245, 242.7, 240.6, 240.8, 243.5, 248.5, 255.6, 264.8, 275.9, 284.7, 291, 292.7, 295.9, 292.8, 291.6, 288.5, 283.5, 276.6, 265.9, 253.5, 245.5, 237.9, 228.8, 220.3, 212.4, 209.3, 209.1, 211.8, 219.5, 228.2, 236, 246.7, 258, 269.5, 278.9, 288.1, 296.9, 305.1, 308.5, 309, 312.5, 313.1, 308.9, 305.8, 299.8, 291.1, 282, 268.9, 261.9, 253, 242.2, 237.5, 231.1, 229.3, 226.2, 223.9, 222.6, 224.4, 225.3, 227.2, 234.1, 241.8, 249.9, 262.2, 272.6, 282.8, 286.7, 286.2, 289.3, 290.1, 288.6, 290.9, 290.9, 288.6, 288, 281.1, 275.9, 272.2, 267.9, 261.1, 261.7, 263.6, 270.8, 277.4, 285.5, 295.1, 298.2, 298.8, 297.1, 295.2, 291, 290.4, 287.5, 284.2, 278.6, 268.8, 262.6, 263.8, 264.2, 259.8, 260.9, 261.8, 268.6, 271.4, 278.2, 283.1, 288, 288.7, 285.2, 283.4, 281.2, 278.8, 272.4, 266.2, 254.3, 242.6, 233.2, 232.4, 232.4, 231.1, 230.4, 230.3, 229.1, 227, 229.9, 233.6, 238.1, 241.5, 245.5, 253.9, 264.6, 271.5, 278.4, 283, 285.1, 286.6, 285.6, 288.2, 290.4, 290.3, 290, 287.6, 281, 272.1, 263.2, 254.7, 242.8, 229.6, 221.2, 213.8, 205.9, 199.6, 194.9, 190.1, 189.2, 190.1, 192.6, 196.7, 210.2, 226.4, 246.8, 265.1, 285.1, 300.7, 313.8, 324.2, 331.7, 334.2, 339.7, 340.2, 333.6, 326, 315.5, 300, 285.6, 270.4, 254.6, 236.3, 223.9, 218, 216.9, 214.9, 214.3, 213.2, 213.5, 213.3, 214.7, 215.6, 223.8, 229.2, 233.7, 240.9, 252.3, 263.5, 276.2, 288.3, 295.8, 302.7, 305, 308.8, 312.2, 311.2, 305.8, 298.1, 288.3, 274.5, 256.9, 239.7, 227.1, 209.4, 190.8, 179.5, 175.7, 173.7, 175.8, 180.1, 186.4, 192.6, 196.8, 210.6, 227.5, 245.2, 263.3, 277.6, 288.1, 294.8, 295.6, 300.2, 304.4, 302.1, 299, 298.9, 299.9, 296.1, 287.7, 278.9, 273.7, 264.1, 252, 245.8, 240.2, 235.6, 234.3, 232.3, 231.3, 230.9, 226.9, 227.5, 230.8, 232.6, 236.9, 242, 250, 256.7, 259.8, 261.2, 263.2, 262, 263.5, 261.6, 258.2, 253.1, 246.3, 241.8, 237.7, 236.3, 231.7, 226, 221.5, 218.4, 216.7, 216.2, 222.6, 227.9, 236.1, 241.1, 246.7, 250.6, 256.8, 261.3, 264.1, 267.3, 266.9, 269, 267.7, 265.1, 261.2, 255.8, 248.7, 241.8, 235.1, 234.5, 236.2, 234.5, 233.6, 229.7, 230.8, 231, 232.4, 235, 238.6, 243, 246.1, 247.9, 250.3, 251, 250, 249.4, 251.1, 254.9, 256.8, 263.1, 267.9, 271, 272.2, 271.3, 276.1, 276.5, 276.5, 278, 279.1, 276.2, 275.4, 270.6, 267.9, 267.3, 264.6, 259.8, 259, 262.2, 265.2, 265.9, 264.5, 261.2, 258.1, 253.3, 253.1, 253.5, 254.3, 257.6, 261.3, 259.1, 259, 255.1, 253.3, 251.7, 250.5, 245.7, 243.4, 240, 239.9, 241, 241, 239.7, 240.9, 240.4, 243.9, 249.3, 254.7, 258.2, 262.1, 264.5, 265.4, 264.8, 266.7, 269, 269.6, 268.5, 267.6, 262.7, 255.7, 250.8, 246.1, 245.4, 244.7, 242.3, 244.5, 243.7, 244.1, 245.5, 245.6, 248.2, 251, 251.9, 256.8, 261.7, 264.7, 265.8, 267.1, 270.5, 271.8, 272.9, 271.9, 273.2, 271.1, 265.6, 260.7, 250.3, 238.4, 231.1, 222.5, 216.8, 216.1, 216.2, 221.1, 221.1, 224.2, 230, 236.2, 243, 248.7, 255.4, 264.8, 276.3, 287.4, 295.8, 301.5, 304.5, 302.7, 302.2, 299, 293.1, 286.8, 282.4, 276.1, 263.9, 251.9, 244.5, 237.9, 228.1, 215.1, 204.8, 199.3, 195.2, 195, 196.8, 200.4, 203.7, 206.7, 209.4, 217.7, 227.2, 235.6, 242.9, 247.2, 250.4, 252.3, 254.8, 259.8, 261.2, 260.9, 261, 267.4, 267.7, 265.9, 264.1, 264.4, 260.9, 259.5, 256, 254.6, 251.9, 250.2, 249.3, 246.9, 244.9, 243.3, 245.9, 254.4, 262.5, 270, 272.9, 281.5, 286.1, 286.7, 287.3, 287.9, 286.4, 286.8, 287.2, 293.1, 293.8, 291.2)
CreateNob 465 pipe(237->463)
Voltage 465 pipe(28: 272.1, 298.7, 293.7, 290.2, 282.5, 276.7, 265, 255.4, 249.8, 240.4, 229.3, 222.8, 217.5, 213.6, 209.2, 206.4, 207.1, 207.1, 210.4, 217, 222.7, 229.4, 237.1, 243.7, 249, 254.9, 259.1, 265.4)
CreateNob 466 pipe(327->462)
Voltage 466 pipe(32: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 467 pipe(551->468)
Voltage 467 pipe(29: 7.5, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 5.5, 8.7, 11.5, 13.9, 15.9, 17.5, 20.6, 23.1, 25, 26.3, 27, 27.1, 26.6, 25.5, 23.8, 21.5, 18.6, 15.1, 11, 8.4)
CreateNob 469 pipe(531->470)
Voltage 469 pipe(31: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 471 pipe(566->472)
Voltage 471 pipe(32: 6.4, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1)
CreateNob 473 pipe(476->474)
Voltage 473 pipe(8: 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 475 pipe(552->476)
Voltage 475 pipe(3: 0, 0, 0)
CreateNob 477 pipe(592->478)
Voltage 477 pipe(328: 6.4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5)
CreateNob 479 pipe(574->480)
Voltage 479 pipe(18: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 481 pipe(474->482)
Voltage 481 pipe(25: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 483 pipe(554->566)
Voltage 483 pipe(21: 0, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0)
CreateNob 484 pipe(553->568)
Voltage 484 pipe(36: 272.9, 200.4, 203.7, 206.7, 209.4, 217.7, 227.2, 235.6, 242.9, 247.2, 250.4, 252.3, 254.8, 259.8, 261.2, 260.9, 261, 267.4, 267.7, 265.9, 264.1, 264.4, 260.9, 259.5, 256, 254.6, 251.9, 250.2, 249.3, 246.9, 244.9, 243.3, 245.9, 254.4, 262.5, 270)
CreateNob 485 pipe(482->570)
Voltage 485 pipe(16: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 486 pipe(575->488)
Voltage 486 pipe(50: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 487 pipe(480->489)
Voltage 487 pipe(13: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 490 pipe(588->533)
Voltage 490 pipe(315: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 11.5, 13.5, 15.1, 16.3, 17.1, 17.5, 17.5, 17.1, 16.3, 15.1, 13.5, 11.5, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 491 pipe(589->532)
Voltage 491 pipe(308: 9.1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 11.5, 13.5, 15.1, 16.3, 17.1, 17.5, 17.5, 17.1, 16.3, 15.1, 13.5, 11.5, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4)
CreateNob 492 pipe(540->531)
Voltage 492 pipe(158: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 493 pipe(533->530)
Voltage 493 pipe(174: 245.6, 239.7, 227.1, 209.4, 190.8, 179.5, 175.7, 173.7, 175.8, 180.1, 186.4, 192.6, 196.8, 210.6, 227.5, 245.2, 263.3, 277.6, 288.1, 294.8, 295.6, 300.2, 304.4, 302.1, 299, 298.9, 299.9, 296.1, 287.7, 278.9, 273.7, 264.1, 252, 245.8, 240.2, 235.6, 234.3, 232.3, 231.3, 230.9, 226.9, 227.5, 230.8, 232.6, 236.9, 242, 250, 256.7, 259.8, 261.2, 263.2, 262, 263.5, 261.6, 258.2, 253.1, 246.3, 241.8, 237.7, 236.3, 231.7, 226, 221.5, 218.4, 216.7, 216.2, 222.6, 227.9, 236.1, 241.1, 246.7, 250.6, 256.8, 261.3, 264.1, 267.3, 266.9, 269, 267.7, 265.1, 261.2, 255.8, 248.7, 241.8, 235.1, 234.5, 236.2, 234.5, 233.6, 229.7, 230.8, 231, 232.4, 235, 238.6, 243, 246.1, 247.9, 250.3, 251, 250, 249.4, 251.1, 254.9, 256.8, 263.1, 267.9, 271, 272.2, 271.3, 276.1, 276.5, 276.5, 278, 279.1, 276.2, 275.4, 270.6, 267.9, 267.3, 264.6, 259.8, 259, 262.2, 265.2, 265.9, 264.5, 261.2, 258.1, 253.3, 253.1, 253.5, 254.3, 257.6, 261.3, 259.1, 259, 255.1, 253.3, 251.7, 250.5, 245.7, 243.4, 240, 239.9, 241, 241, 239.7, 240.9, 240.4, 243.9, 249.3, 254.7, 258.2, 262.1, 264.5, 265.4, 264.8, 266.7, 269, 269.6, 268.5, 267.6, 262.7, 255.7, 250.8, 246.1, 245.4, 244.7, 242.3, 244.5, 243.7, 244.1, 245.5)
CreateNob 494 pipe(532->529)
Voltage 494 pipe(187: 0, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 495 pipe(489->511)
Voltage 495 pipe(136: 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 496 pipe(488->512)
Voltage 496 pipe(116: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 497 pipe(576->513)
Voltage 497 pipe(165: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 498 pipe(509->507)
Voltage 498 pipe(203: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 499 pipe(510->508)
Voltage 499 pipe(207: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 501 pipe(508->500)
Voltage 501 pipe(47: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 502 pipe(507->503)
Voltage 502 pipe(52: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 505 pipe(506->504)
Voltage 505 pipe(55: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 520 pipe(511->534)
Voltage 520 pipe(53: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 521 pipe(512->535)
Voltage 521 pipe(55: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 522 pipe(513->536)
Voltage 522 pipe(58: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 523 pipe(570->517)
Voltage 523 pipe(3: 0, 0, 0)
CreateNob 524 pipe(568->518)
Voltage 524 pipe(23: 255.5, 286.1, 286.7, 287.3, 287.9, 286.4, 286.8, 287.2, 293.1, 293.8, 291.2, 291.4, 290.4, 286.3, 281.2, 277.4, 275.5, 272.1, 269.5, 269.4, 271.4, 269.4, 263.4)
CreateNob 525 pipe(472->519)
Voltage 525 pipe(8: 9.6, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9)
CreateNob 526 pipe(514->511)
Voltage 526 pipe(31: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 527 pipe(515->512)
Voltage 527 pipe(32: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 528 pipe(516->513)
Voltage 528 pipe(37: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 537 pipe(534->529)
Voltage 537 pipe(46: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 538 pipe(535->530)
Voltage 538 pipe(49: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 539 pipe(536->531)
Voltage 539 pipe(47: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 541 pipe(593->540)
Voltage 541 pipe(322: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 11.5, 13.5, 15.1, 16.3, 17.1, 17.5, 17.5, 17.1, 16.3, 15.1, 13.5, 11.5, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 542 pipe(478->532)
Voltage 542 pipe(21: 0, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 543 pipe(591->533)
Voltage 543 pipe(338: 274.5, 284.3, 281.8, 274.7, 271, 269, 263, 257.2, 249.7, 246.8, 240.9, 236.2, 227, 219.2, 212.6, 211, 212.1, 215.7, 223.9, 238.4, 248.7, 259, 271.4, 281.7, 293.9, 302.1, 310.1, 315.6, 320.5, 320.8, 318.4, 313.1, 300.7, 291.1, 276.5, 265.3, 255.8, 244.4, 231.5, 231.4, 228.3, 226, 224.5, 224, 226.4, 227.5, 229.4, 238, 248.9, 257.9, 266.9, 275.9, 282.8, 289.7, 294.5, 298.9, 301, 298.7, 293.7, 290.2, 282.5, 276.7, 265, 255.4, 249.8, 240.4, 229.3, 222.8, 217.5, 213.6, 209.2, 206.4, 207.1, 207.1, 210.4, 217, 222.7, 229.4, 237.1, 243.7, 249, 254.9, 259.1, 265.4, 272.1, 277.1, 280.2, 279.5, 276.9, 272.4, 262.2, 254.3, 248.7, 241.6, 235.1, 229.4, 224.7, 224.8, 223.6, 223.2, 225.5, 226.5, 230.4, 233.4, 235.5, 240.7, 245, 252, 261.3, 266.8, 270.2, 273.4, 276.6, 279.6, 282.2, 282.3, 283.7, 290.4, 292.5, 295.9, 300.6, 302.6, 301.9, 296.6, 288.8, 282.5, 273.7, 270.6, 269.4, 264.2, 261.1, 254.2, 247.5, 245, 242.7, 240.6, 240.8, 243.5, 248.5, 255.6, 264.8, 275.9, 284.7, 291, 292.7, 295.9, 292.8, 291.6, 288.5, 283.5, 276.6, 265.9, 253.5, 245.5, 237.9, 228.8, 220.3, 212.4, 209.3, 209.1, 211.8, 219.5, 228.2, 236, 246.7, 258, 269.5, 278.9, 288.1, 296.9, 305.1, 308.5, 309, 312.5, 313.1, 308.9, 305.8, 299.8, 291.1, 282, 268.9, 261.9, 253, 242.2, 237.5, 231.1, 229.3, 226.2, 223.9, 222.6, 224.4, 225.3, 227.2, 234.1, 241.8, 249.9, 262.2, 272.6, 282.8, 286.7, 286.2, 289.3, 290.1, 288.6, 290.9, 290.9, 288.6, 288, 281.1, 275.9, 272.2, 267.9, 261.1, 261.7, 263.6, 270.8, 277.4, 285.5, 295.1, 298.2, 298.8, 297.1, 295.2, 291, 290.4, 287.5, 284.2, 278.6, 268.8, 262.6, 263.8, 264.2, 259.8, 260.9, 261.8, 268.6, 271.4, 278.2, 283.1, 288, 288.7, 285.2, 283.4, 281.2, 278.8, 272.4, 266.2, 254.3, 242.6, 233.2, 232.4, 232.4, 231.1, 230.4, 230.3, 229.1, 227, 229.9, 233.6, 238.1, 241.5, 245.5, 253.9, 264.6, 271.5, 278.4, 283, 285.1, 286.6, 285.6, 288.2, 290.4, 290.3, 290, 287.6, 281, 272.1, 263.2, 254.7, 242.8, 229.6, 221.2, 213.8, 205.9, 199.6, 194.9, 190.1, 189.2, 190.1, 192.6, 196.7, 210.2, 226.4, 246.8, 265.1, 285.1, 300.7, 313.8, 324.2, 331.7, 334.2, 339.7, 340.2, 333.6, 326, 315.5, 300, 285.6, 270.4, 254.6, 236.3, 223.9, 218, 216.9, 214.9, 214.3, 213.2, 213.5, 213.3, 214.7, 215.6, 223.8, 229.2, 233.7, 240.9, 252.3, 263.5, 276.2, 288.3, 295.8, 302.7, 305, 308.8, 312.2, 311.2, 305.8, 298.1, 288.3)
CreateNob 544 pipe(590->540)
Voltage 544 pipe(362: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 545 pipe(593->549)
Voltage 545 pipe(282: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 11.5, 13.5, 15.1, 16.3, 17.1, 17.5, 17.5, 17.1, 16.3, 15.1, 13.5, 11.5, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 546 pipe(588->550)
Voltage 546 pipe(275: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 11.5, 13.5, 15.1, 16.3, 17.1, 17.5, 17.5, 17.1, 16.3, 15.1, 13.5, 11.5, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 547 pipe(589->551)
Voltage 547 pipe(267: 9.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10)
CreateNob 548 pipe(573->549)
Voltage 548 pipe(118: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 555 pipe(468->554)
Voltage 555 pipe(236: 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 8.3, 11.1, 13.5, 15.5, 17.1, 18.3, 19.1, 19.5, 19.5, 19.1, 18.3, 17.1, 15.5, 13.5, 11.1, 8.3, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4)
CreateNob 556 pipe(550->553)
Voltage 556 pipe(253: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 557 pipe(549->552)
Voltage 557 pipe(245: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 558 pipe(571->551)
Voltage 558 pipe(101: 1.9, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6)
CreateNob 559 pipe(572->550)
Voltage 559 pipe(111: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 561 pipe(560->562)
Voltage 561 pipe(3: 0, 0, 0)
CreateNob 563 pipe(529->554)
Voltage 563 pipe(51: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 564 pipe(530->553)
Voltage 564 pipe(54: 195, 248.2, 251, 251.9, 256.8, 261.7, 264.7, 265.8, 267.1, 270.5, 271.8, 272.9, 271.9, 273.2, 271.1, 265.6, 260.7, 250.3, 238.4, 231.1, 222.5, 216.8, 216.1, 216.2, 221.1, 221.1, 224.2, 230, 236.2, 243, 248.7, 255.4, 264.8, 276.3, 287.4, 295.8, 301.5, 304.5, 302.7, 302.2, 299, 293.1, 286.8, 282.4, 276.1, 263.9, 251.9, 244.5, 237.9, 228.1, 215.1, 204.8, 199.3, 195.2)
CreateNob 565 pipe(470->552)
Voltage 565 pipe(35: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 567 pipe(489->566)
Voltage 567 pipe(291: 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 569 pipe(488->568)
Voltage 569 pipe(290: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 580 pipe(443->579)
Voltage 580 pipe(149: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 581 pipe(424->578)
Voltage 581 pipe(184: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 582 pipe(448->577)
Voltage 582 pipe(140: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 583 pipe(215->510)
Voltage 583 pipe(12: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 584 pipe(216->509)
Voltage 584 pipe(14: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 586 pipe(585->506)
Voltage 586 pipe(199: 3.6, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9)
CreateNob 587 pipe(217->585)
Voltage 587 pipe(16: 0, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0)
CreateNob 594 pipe(579->593)
Voltage 594 pipe(75: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 595 pipe(578->588)
Voltage 595 pipe(78: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 596 pipe(577->589)
Voltage 596 pipe(17: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 597 pipe(295->590)
Voltage 597 pipe(259: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 598 pipe(293->592)
Voltage 598 pipe(218: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 599 pipe(291->591)
Voltage 599 pipe(196: 291.3, 302.6, 305.6, 309.5, 310.5, 308.8, 304.4, 297.1, 290.9, 286, 287.1, 271.2, 273.9, 272.3, 271.9, 266, 258.8, 258.3, 251.5, 250.4, 253, 256, 250.3, 262.3, 266.2, 268.2, 267.1, 267.6, 270.1, 283.7, 287.6, 278.5, 278.6, 278.7, 279, 283.5, 274.2, 258.1, 248.2, 239.1, 232.9, 229.4, 236.8, 227.8, 237.5, 229.7, 239, 232.4, 243.5, 257.1, 280.9, 290.7, 288.6, 302.1, 307, 310.7, 306.3, 315.8, 318.5, 306.3, 302.3, 299.2, 297.2, 290.6, 283.6, 280.2, 276.4, 268.2, 261.9, 257.9, 262.1, 264.2, 262.1, 259.6, 256.5, 255.1, 261.7, 268.3, 276.8, 281.1, 283.3, 283.6, 284.1, 282.5, 286.2, 291, 293.1, 292.5, 290.9, 287.7, 291.3, 287.1, 279.2, 275.1, 268.9, 258.7, 252.7, 249, 255.4, 259.7, 259.8, 257.6, 253.1, 248.6, 252.5, 258.9, 263.6, 268.5, 271.7, 273.2, 273, 274.9, 276.4, 279.4, 279.9, 277.7, 272.8, 273.2, 281, 284.6, 284.2, 283.8, 279.4, 271, 264.7, 262.6, 262.6, 262.6, 260.5, 256.3, 250, 245.8, 246, 246.6, 253.3, 261.7, 265.7, 273.5, 283.4, 291.2, 296.5, 299.3, 301.5, 302.7, 300.8, 298.1, 298.8, 298.9, 296.5, 291.6, 286.1, 277.9, 269.1, 261.8, 260.2, 260.7, 259.3, 256, 251, 250.4, 250.2, 248.3, 246.8, 248, 247.9, 244.4, 243.6, 245.5, 253.4, 255.1, 257, 259.1, 261.2, 267.1, 272.8, 274.5, 274.5, 272.8, 267.3, 266, 263, 262.3, 263.5, 262.4, 265.1, 263.6, 267.6, 271, 275.9, 278.3, 278.4, 278.3, 280.1, 284, 288.1, 292)
CreateNob 601 pipe(604->600)
Voltage 601 pipe(110: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 603 pipe(600->602)
Voltage 603 pipe(99: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
CreateNob 605 pipe(577->604)
Voltage 605 pipe(373: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 11.5, 13.5, 15.1, 16.3, 17.1, 17.5, 17.5, 17.1, 16.3, 15.1, 13.5, 11.5, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 1.9, 3.6, 5.1, 6.4, 7.5, 8.4, 9.1, 9.6, 9.9, 10, 9.9, 9.6, 9.1, 8.4, 7.5, 6.4, 5.1, 3.6, 1.9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)

SignalGenerator "Standard"      10  50      10  20   500000
ActiveSigGen  0


NrOfTracks 1
Signal  track  0 source 101(( -46.8858| 3865.39),442.89)
