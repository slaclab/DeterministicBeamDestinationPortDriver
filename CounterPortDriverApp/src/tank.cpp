#include "tank.h"
#include <unistd.h>

void waterTask(void *driverPointer);

WaterDriver::WaterDriver(const char *portName) : asynPortDriver(
    portName,
    1,
    asynFloat64Mask | asynFloat64ArrayMask,
    asynFloat64Mask | asynFloat64ArrayMask,
    0,
    1,
    0,
    0
    )
{
  createParam(P_TankInflowString, asynParamFloat64, &P_WaterInflow);
  createParam(P_TankString, asynParamFloat64, &P_WaterLevel);
  createParam(P_TankOutflowString, asynParamFloat64, &P_WaterOutflow);
  setDoubleParam(P_WaterLevel, 7);
  setDoubleParam(P_WaterInflow, 5);
  setDoubleParam(P_WaterOutflow, 4);
  callParamCallbacks();
  printf("ParamListIndices: %d, %d, %d\n", P_WaterLevel, P_WaterInflow, P_WaterOutflow);

  asynStatus status;
  status = (asynStatus)(epicsThreadCreate("LujkoWaterTask", epicsThreadPriorityMedium, epicsThreadGetStackSize(epicsThreadStackMedium), (EPICSTHREADFUNC)::waterTask, this)== NULL);
  if(status)
  {
    printf("Thread didn't launch properly");
    return;
  }
}

void waterTask(void* driverPointer)
{
  WaterDriver *pPvt = (WaterDriver *) driverPointer;
  pPvt->waterTask();
}

void WaterDriver::waterTask(void)
{
  double waterLevel;
  getDoubleParam(P_WaterLevel, &waterLevel); //Initialize our waterLevel to whatever we set it to originally
  double inflow, outflow;
  for(int i = 0; i<= 100; i++)
  {
    sleep(5);
    getDoubleParam(P_WaterInflow, &inflow);
    printf("Retrieved inflow: %f\n", inflow);
    getDoubleParam(P_WaterOutflow, &outflow);
    printf("Retrieved outflow: %f\n", outflow);
    waterLevel += inflow;
    waterLevel -= outflow;
    printf("Current water level: %f\n", waterLevel);
    setDoubleParam(P_WaterLevel, waterLevel);
    callParamCallbacks();
  }
}

extern "C" {
  int WaterDriverConfigure(const char* portName) {
    new WaterDriver(portName);
    return asynSuccess;
  }
  static const iocshArg waterArg0 = {"portName", iocshArgString};
  static const iocshArg * const waterArgs[] = {&waterArg0};
  static const iocshFuncDef waterFuncDef = {"WaterDriverConfigure", 1, waterArgs};
  static void waterCallFunc(const iocshArgBuf *args)
  {
    WaterDriverConfigure(args[0].sval);
  }
  void WaterDriverRegister(void) {
    iocshRegister(&waterFuncDef, waterCallFunc);
  }
  epicsExportRegistrar(WaterDriverRegister);
}
