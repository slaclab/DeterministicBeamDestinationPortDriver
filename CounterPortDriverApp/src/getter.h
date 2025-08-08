//Getter.h 

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

#define P_GetterString "SCOPE_GETTER"

class GetterDriver : public asynPortDriver {
  public:
    GetterDriver(const char *portName);
    void getterTask(void);
  protected:
    int P_Getter;

    int hxr_state_idx;

    int shutter_idx;
    int bcs_fault_idx;
    int gun_off_idx;
    int gun_rate_idx;

    int hxr_permit_idx;
    int hard_injrate_idx;

    int spectrometer_state_idx;
    int td_11_in_idx;
    int d2_in_1_idx;
    int d2_in_2_idx;
    int bykik_idx;
    int tdund_in_idx;

};
