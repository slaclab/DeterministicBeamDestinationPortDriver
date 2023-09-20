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
  createParam(P_CounterString, asynParamFloat64, &P_Count);
  createParam(P_CounterString2, asynParamFloat64, &P_Count2);
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
    setDoubleParam(P_Count, x);
    getDoubleParam(P_Count, &twice);
    callParamCallbacks();
    //printf("it grabbed this: %f\n", twice);
    setDoubleParam(P_Count2, 33);
    getDoubleParam(P_Count2, &thrice);
    printf("Retrieved vals: %f, %f", twice, thrice);
    callParamCallbacks();
  }
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
