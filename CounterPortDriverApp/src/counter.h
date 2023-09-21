//Counter.h 
#include <epicsString.h>
#include <iocsh.h>
#include <iostream>
#include <epicsExport.h>
#include <epicsThread.h>
#include <epicsTypes.h>
#include <asynPortDriver.h>

#define COUNTER_STRING "COUNTER"
#define SCALAR_STRING "SCALAR"

static const char *driverName = "Counter";

class CounterDriver : public asynPortDriver {
  public:
    CounterDriver(const char *portName);
    void counterTask(void);
    virtual asynStatus readFloat64(asynUser *pasynUser, epicsFloat64 *value);

  private:
    int _counter;
    int _scalar;
    double counter;
    double scalar;
};

