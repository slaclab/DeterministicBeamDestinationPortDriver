//Getter.cpp

#include "getter.h"
#include <unistd.h>

void getterTask(void *driverPointer);

GetterDriver::GetterDriver(const char *portName): asynPortDriver(
    portName,
    1,
    asynDrvUserMask | asynInt32ArrayMask | asynInt16ArrayMask | asynUInt32DigitalMask | asynInt32Mask | asynFloat64Mask | asynFloat64ArrayMask,
    asynInt32ArrayMask | asynInt16ArrayMask | asynInt32Mask | asynUInt32DigitalMask | asynFloat64Mask | asynFloat64ArrayMask,
    0,
    1,
    0,
    0
    )
{
  createParam("HXR_STATE", asynParamInt32, &hxr_state_idx);

  createParam("SHUTTER", asynParamInt32, &shutter_idx);
  createParam("BCS_FAULT", asynParamInt32, &bcs_fault_idx);
  createParam("GUN_OFF", asynParamInt32, &gun_off_idx);
  createParam("GUN_RATE", asynParamFloat64, &gun_rate_idx);

  createParam("HXR_PERMIT", asynParamFloat64, &hxr_permit_idx);
  createParam("HARD_INJRATE", asynParamFloat64, &hard_injrate_idx);

  createParam("SPECTROMETER_STATE", asynParamInt32, &spectrometer_state_idx);
  createParam("TD_11_IN", asynParamInt32, &td_11_in_idx);

  createParam("D2_IN_A", asynParamInt32, &d2_in_1_idx);
  createParam("D2_IN_B", asynParamInt32, &d2_in_2_idx);
  createParam("BYKIK", asynParamInt32, &bykik_idx);
  createParam("TDUND_IN", asynParamInt32, &tdund_in_idx);

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
  int state = 0;
  int shutter;
  int bcs_fault;
  int gun_off;
  double gun_rate;
  double hxr_permit;
  double hard_injrate;

  getIntegerParam(shutter_idx, &shutter);
  getIntegerParam(bcs_fault_idx, &bcs_fault);
  getIntegerParam(gun_off_idx, &gun_off);
  getDoubleParam(gun_rate_idx, &gun_rate);
  getDoubleParam(hxr_permit_idx, &hxr_permit);
  getDoubleParam(hard_injrate_idx, &hard_injrate);
  
  if (not(shutter == 1 or bcs_fault == 0 or gun_off == 1 or gun_rate == 0 or hxr_permit == 1 or hard_injrate == 0))
  {
    int spectrometer_state;
    int td_11_in;
    int d2_in_1;
    int d2_in_2;
    int bykik;
    int tdund_in;

    getIntegerParam(spectrometer_state_idx, &spectrometer_state);
    getIntegerParam(td_11_in_idx, &td_11_in);
    getIntegerParam(d2_in_1_idx, &d2_in_1);
    getIntegerParam(d2_in_2_idx, &d2_in_2);
    getIntegerParam(bykik_idx, &bykik);
    getIntegerParam(tdund_in_idx, &tdund_in);

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
  setIntegerParam(hxr_state_idx, state);
  callParamCallbacks();
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

