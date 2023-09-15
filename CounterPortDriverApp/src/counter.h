//Counter.h 

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

#define P_CounterString "SCOPE_COUNTER"

class CounterDriver : public asynPortDriver {
  public:
    CounterDriver(const char *portName);
    void counterTask(void);
  protected:
    int P_Count;
};

//New Stuff, should be able to comment out and reset?
#define P_GetterString "SCOPE_GETTER"
/**
class GetterDriver : public asynPortDriver {
  public:
    GetterDriver(const char *portName);
    void getterTask(void);
  protected:
    int P_Getter;
};
**/
