//tank.h 

#include <iostream>
#include <cstdint>
#include <memory>
#include <functional>
#include <map>

#include <iocsh.h>
#include <epicsExport.h>
#include <epicsThread.h>
#include <epicsEvent.h>
#include <epicsTimer.h>
#include <epicsTypes.h>
#include <asynPortDriver.h>

using namespace std;

#define P_TankString "SCOPE_WATER_LEVEL"
#define P_TankInflowString "SCOPE_WATER_INFLOW"
#define P_TankOutflowString "SCOPE_WATER_OUTFLOW"

class WaterDriver : public asynPortDriver {
  public:
    WaterDriver(const char *portName);
    void waterTask(void);
  protected:
    int P_WaterLevel;
    int P_WaterInflow;
    int P_WaterOutflow;
};
