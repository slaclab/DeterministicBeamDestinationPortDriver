#!../../bin/rhel7-x86_64/CounterPortDriver

#- You may have to change CounterPortDriver to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/CounterPortDriver.dbd"
CounterPortDriver_registerRecordDeviceDriver pdbbase

##Driver Launches
CounterDriverConfigure("jeremy")
##GetterDriverConfigure("lujko1")
##WaterDriverConfigure("lujko2")

## Load record instances
dbLoadRecords("db/counter.db", "USER=jeremy,PORT=jeremy,ADDR=0,TIMEOUT=1")
cd "${TOP}/iocBoot/${IOC}"
iocInit()

## Start any sequence programs
#seq sncxxx,"user=lujko"