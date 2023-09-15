#include "counter.h"
#include <unistd.h>

void counterTask(void *driverPointer);

CounterDriver::CounterDriver(const char *portName) : asynPortDriver(
    portName,
    1,
    asynFloat64Mask,
    asynFloat64Mask,
    0,
    1,
    0,
    0
    )
{
  //createParam(P_CounterString, asynParamFloat64, &P_Count);
  createParam(P_GetterString,asynParamFloat64, &P_Count);
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
  double twice;
  for(int x = 0; x <=100; x++)
  {
    sleep(4);
    setDoubleParam(P_Count, x);
    getDoubleParam(P_Count, &twice);
    printf("Shoot me it grabbed this: %f\n", twice);
    callParamCallbacks();
  }
}

//New again

//Old again

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
/**
  //New things
  int GetterDriverConfigure(const char* portName) {
    new GetterDriver(portName);
    return asynSuccess;
  }
  static const iocshArg initArg1 = {"portName", iocshArgString};
  static const iocshArg * const initArgs1[] = {&initArg1};
  static const iocshFuncDef getterInitFuncDef = {"GetterDriverConfigure", 1, initArgs};
  static void getterInitCallFunc(const iocshArgBuf *args)
  {
    GetterDriverConfigure(args[0].sval);
  }
  void GetterDriverRegister(void) {
    iocshRegister(&getterInitFuncDef, getterInitCallFunc);
  }
  epicsExportRegistrar(GetterDriverRegister);
  **/ 
}
