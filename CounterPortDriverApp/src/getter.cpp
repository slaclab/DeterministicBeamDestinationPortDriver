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
  int state = 0;
  int shutter;
  int bcs_fault;
  int gun_off;
  double gun_rate;
  double hxr_permit;
  double hard_injrate;
  
  if (not(shutter == 1 or bcs_fault == 0 or gun_off == 1 or gun_rate == 0 or hxr_permit == 1 or hard_injrate == 0))
  {
    int spectrometer_state;
    int td_11_in;
    int d2_in_1;
    int d2_in_2;
    int bykik;
    int tdund_in;

    if (spectrometer_state == 0)
    {
      state = 1;
    }
    else if (td_11_in == 2) 
    {
      state = 2;
    }
    else if (d2_in_1 == 0 or d2_in_2 == 0)
    {
      state = 3;
    }
    else if (bykik == 0)
    {
      state = 4;
    }
    else if (tdund_in == 2)
    {
      state = 5;
    }
    else 
    {
      state = 6;  
    }
    



  }


  //Set param of state



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

