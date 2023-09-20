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
  asynStatus status;
  status = (asynStatus)(epicsThreadCreate("LujkoGetterTask", epicsThreadPriorityMedium, epicsThreadGetStackSize(epicsThreadStackMedium), (EPICSTHREADFUNC)::getterTask, this) == NULL);
  if (status)
  {
    printf("Thread shot itself for some reason");
    return;
  }
}

void getterTask(void *driverPointer)
{
  GetterDriver *pPvt = (GetterDriver *) driverPointer;
  pPvt->getterTask();
}

void GetterDriver::getterTask(void)
{
  double variable;
  for(int x = 0; x <= 100; x++)
  {
    sleep(1);
    //printf("Things");
    getDoubleParam(P_Getter, &variable);
    printf("Retrieved variable is %f\n", variable);
  }
}


extern "C" {
  int GetterDriverConfigure(const char* portName) {
    new GetterDriver(portName);
    return asynSuccess;
  }
  static const iocshArg getterArg0 ={"portName", iocshArgString};
  static const iocshArg * const getterArgs[] = {&getterArg0};
  static const iocshFuncDef getterFuncDef = {"GetterDriverConfigure", 1, getterArgs};
  static void getterCallFunc(const iocshArgBuf *args)
  {
    GetterDriverConfigure(args[0].sval);
  }
  void GetterDriverRegister(void) {
    iocshRegister(&getterFuncDef, getterCallFunc);
  }
  epicsExportRegistrar(GetterDriverRegister);
}

