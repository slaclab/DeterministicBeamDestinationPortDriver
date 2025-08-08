#!../../bin/rhel9-x86_64/CounterPortDriver

#- You may have to change CounterPortDriver to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/CounterPortDriver.dbd"
CounterPortDriver_registerRecordDeviceDriver pdbbase

##Driver Launches
GetterDriverConfigure("lujko1")

## Load record instances
#dbLoadRecords("db/xxx.db","user=lujko")
dbLoadRecords("db/getter.db", "USER=lujko,PORT=lujko1,ADDR=0,TIMEOUT=0")
cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncxxx,"user=lujko"
