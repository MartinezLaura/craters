Requirements: gcc v6+; libgdal-dev v2.1+; multi-core CPU

Prior to running this, modify the number of cores in the line 2 of run.sh
A good idea is to give it a bit less than you actual number of cores
I use 7 for an 8 core machine, so I am sure the laptop is usable in the meanwhile.

type "run.sh" to process centre points from dataOut/2_grass_process/*.tif to dataOut/3_circles/*.csv
