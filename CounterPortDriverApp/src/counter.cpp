#include "counter.h"
#include <unistd.h>

void counterTask(void *driverPointer);

CounterDriver::CounterDriver(const char *portName) : asynPortDriver(
    portName,
    1,
    asynFloat64Mask | asynDrvUserMask,
    asynFloat64Mask,
    ASYN_MULTIDEVICE,
    1,
    0,
    0
    )
{
  counter = 0.;
  scalar = 0.;
  createParam(COUNTER_STRING, asynParamFloat64, &_counter);
  createParam(SCALAR_STRING, asynParamFloat64, &_scalar);
  asynStatus status;
  status = (asynStatus)(epicsThreadCreate("LujkoCounterTask", epicsThreadPriorityMedium, epicsThreadGetStackSize(epicsThreadStackMedium), (EPICSTHREADFUNC)::counterTask, this) == NULL);
  if (status)
  {
    printf("Thread didn't launch properly");
    return;
  }

}

void counterTask(void* driverPointer)
{
  CounterDriver *pPvt = (CounterDriver *) driverPointer;
  pPvt->counterTask();
}

void CounterDriver::counterTask(void)
{
  double twice, thrice;
  for(int x = 0; x <=100; x++)
  {
    sleep(4);
    counter = x * 1.0;
    scalar = 33.;
    setDoubleParam(_counter,counter);
    setDoubleParam(_scalar,scalar);
    callParamCallbacks();
  }
}

asynStatus CounterDriver::readFloat64(asynUser *pasynUser, epicsFloat64 *value) {
  static const char *functionName = "readFloat64Overload";
  const char * paramName;
  asynStatus status = asynSuccess;
  int addr;
  int function = pasynUser->reason;
  status = getParamName(function, &paramName);
  getAddress(pasynUser, &addr);
  epicsTimeStamp timeStamp; getTimeStamp(&timeStamp);
  status = (asynStatus) getDoubleParam(addr,function,value);
  pasynUser->timestamp = timeStamp;
  getParamAlarmStatus(addr, function, &pasynUser->alarmStatus);
  getParamAlarmSeverity(addr, function, &pasynUser->alarmSeverity);
  if (status == asynParamUndefined) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d, name=%s, value is undefined", 
                  driverName, functionName, status, function, paramName );
  else if (status) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d, name=%s, value=%f", 
                  driverName, functionName, status, function, paramName, *value);
  else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
              "%s:%s: function=%d, name=%s, value=%f\n", 
              driverName, functionName, function, paramName, *value);
  return status;
}



extern "C" {
  int CounterDriverConfigure(const char* portName) {
    new CounterDriver(portName);
    return asynSuccess;
  }
  static const iocshArg initArg0 = {"portName", iocshArgString};
  static const iocshArg * const initArgs[] = {&initArg0};
  static const iocshFuncDef initFuncDef = {"CounterDriverConfigure", 1, initArgs};
  static void initCallFunc(const iocshArgBuf *args)
  {
    CounterDriverConfigure(args[0].sval);
  }
  void CounterDriverRegister(void) {
    iocshRegister(&initFuncDef, initCallFunc);
  }
  epicsExportRegistrar(CounterDriverRegister);
}
