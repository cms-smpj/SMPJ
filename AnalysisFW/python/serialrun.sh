#!/bin/bash

# 1: Choose 16/17/18
# 2: Era (DT only)
# 3: ak4/ak8, zb (DT only)
# 4: Choose dt/py/pylong/hw/nu/mg (MC only)
# 5: In case of pylong (0-14) or mg (0-8)

# For data samples: choose ak4/zb (ak8 possible, but not used)
MODE='ak4'
#MODE='zb'

# Note: Be (B-ver1) is completely rejected by gtag
##python crabConfig.py '16' 'Be' '$MODE' 'dt'
#python crabConfig.py '16' 'Bl' $MODE 'dt'
#python crabConfig.py '16' 'C' $MODE 'dt'
#python crabConfig.py '16' 'D' $MODE 'dt'
#python crabConfig.py '16' 'E' $MODE 'dt'
#python crabConfig.py '16' 'Fe' $MODE 'dt'
#python crabConfig.py '16' 'Fl' $MODE 'dt'
#python crabConfig.py '16' 'G' $MODE 'dt'
#python crabConfig.py '16' 'H' $MODE 'dt'

#python crabConfig.py '16' 'e' 'ak4' 'hw'
#python crabConfig.py '16' 'l' 'ak4' 'hw'
#python crabConfig.py '16' 'e' 'ak4' 'nu'
#python crabConfig.py '16' 'l' 'ak4' 'nu'
#python crabConfig.py '16' 'e' 'ak4' 'py'
#python crabConfig.py '16' 'l' 'ak4' 'py'
#python crabConfig.py '16' 'eext' 'ak4' 'py'
#python crabConfig.py '16' 'lext' 'ak4' 'py'

for i in $(seq 0 8);
do
    python crabConfig.py '16' 'e' 'ak4' 'mg' $i
#    python crabConfig.py '16' 'l' 'ak4' 'mg' $i
done

#python crabConfig.py '17' 'B' $MODE 'dt' 0 'qq' 
#python crabConfig.py '17' 'C' $MODE 'dt' 0 'qq'
#python crabConfig.py '17' 'D' $MODE 'dt' 0 'qq'
#python crabConfig.py '17' 'E' $MODE 'dt' 0 'qq'
#python crabConfig.py '17' 'F' $MODE 'dt' 0 'qq'

#python crabConfig.py '17' 'X' 'ak4' 'hw'
#python crabConfig.py '17' 'X' 'ak4' 'nu'
#python crabConfig.py '17' 'X' 'ak4' 'py'
#python crabConfig.py '17' 'ext' 'ak4' 'py'

#for i in $(seq 0 8);
#do
#    python crabConfig.py '17' 'B' 'ak4' 'mg' $i
#done

#for i in $(seq 0 14);
#do
#    python crabConfig.py '17' 'B' 'ak4' 'pylong' $i
#done

#python crabConfig.py '18' 'A' $MODE 'dt'
#python crabConfig.py '18' 'B' $MODE 'dt'
#python crabConfig.py '18' 'C' $MODE 'dt'
#python crabConfig.py '18' 'D' $MODE 'dt'

#python crabConfig.py '18' 'X' 'ak4' 'hw'
#python crabConfig.py '18' 'X' 'ak4' 'nu'
#python crabConfig.py '18' 'X' 'ak4' 'py'
#python crabConfig.py '18' 'ext' 'ak4' 'py'

#for i in $(seq 0 8);
#do
#    python crabConfig.py '18' 'B' 'ak4' 'mg' $i
#done

