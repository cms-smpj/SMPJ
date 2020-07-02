#!/bin/bash

# 1: Choose 16/17/18
# 2: Era (DT only)
# 3: ak4/ak8, zb (DT only)
# 4: Choose dt/py/pylong/hw/nu/mg (MC only)
# 5: In case of pylong (0-14) or mg (0-8)

python crabConfig.py '17' 'B' 'ak4' 'dt'
python crabConfig.py '17' 'C' 'ak4' 'dt'
python crabConfig.py '17' 'D' 'ak4' 'dt'
python crabConfig.py '17' 'E' 'ak4' 'dt'
python crabConfig.py '17' 'F' 'ak4' 'dt'
python crabConfig.py '17' 'B' 'zb' 'dt'
python crabConfig.py '17' 'C' 'zb' 'dt'
python crabConfig.py '17' 'D' 'zb' 'dt'
python crabConfig.py '17' 'E' 'zb' 'dt'
python crabConfig.py '17' 'F' 'zb' 'dt'

#python crabConfig.py '18' 'A' 'ak4' 'dt'
#python crabConfig.py '18' 'B' 'ak4' 'dt'
#python crabConfig.py '18' 'C' 'ak4' 'dt'
#python crabConfig.py '18' 'D' 'ak4' 'dt'
#python crabConfig.py '18' 'A' 'zb' 'dt'
#python crabConfig.py '18' 'B' 'zb' 'dt'
#python crabConfig.py '18' 'C' 'zb' 'dt'
#python crabConfig.py '18' 'D' 'zb' 'dt'

#python crabConfig.py '17' 'X' 'ak4' 'nu'
#python crabConfig.py '17' 'X' 'ak4' 'py'
#python crabConfig.py '17' 'X' 'ak4' 'hw'

#for i in $(seq 0 8);
#do
#    python crabConfig.py '17' 'B' 'ak4' 'mg' $i
#done

#for i in $(seq 0 14);
#do
#    python crabConfig.py '17' 'B' 'ak4' 'pylong' $i
#done
