//Getter.cpp

#include "getter.h"
#include <unistd.h>

void getterTask(void *driverPointer);

GetterDriver::GetterDriver(const char *portName): asynPortDriver(
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
  createParam(P_GetterString, asynParamFloat64, &P_Getter);
  printf("P_Getter index is %d\n", P_Getter);
  asynStatus status;
  //status = (asynStatus)(epicsThreadCreate("LujkoGetterTask", epicsThreadPriorityMedium, epicsThreadGetStackSize(epicsThreadStackMedium), (EPICSTHREADFUNC)::getterTask, this) == NULL);
  //status = 1;
  if (false)
  {
    printf("Thread shot itself for some reason");
    return;
  }
}

void getterTask(void *driverPointer)
{
  GetterDriver *pPvt = (GetterDriver *) driverPointer;
  printf("Hasn't Segfaulted getterTask\n");
  pPvt->getterTask();
}

void GetterDriver::getterTask(void)
{
  printf("Thread Lol");
  /**
  sleep(30);
  printf("Stopped Sleeping");
  double variable;
  for(int x = 0; x <= 100; x++)
  {
    sleep(1);
    getDoubleParam(P_Getter, &variable);
    printf("Retrieved variable is %f\n", variable);
  }
  **/ 
}

extern "C" {
  int GetterDriverConfigure(const char* portName) {
    new GetterDriver(portName);
    return asynSuccess;
  }
  static const iocshArg initArg0 = {"portName", iocshArgString };
  static const iocshArg * const initArgs[] = {&initArg0};
  static const iocshFuncDef initFuncDef = {"GetterDriverConfigure", 1, initArgs};
  static void initCallFunc(const iocshArgBuf *args)
  {
    GetterDriverConfigure(args[0].sval);
  }
  void GetterDriverRegister(void) {
    iocshRegister(&initFuncDef, initCallFunc);
  }
  epicsExportRegistrar(GetterDriverRegister);
}
