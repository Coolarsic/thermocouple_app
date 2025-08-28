#include "/usr/include/xmlrpc-c/base.h"
#include "/usr/include/xmlrpc-c/client.h"
#include "constants.h"
#include "get_thermocouple_signal_mV.h"
#include "dieIfFaultOccured.h"

double get_thermocouple_signal_mV(xmlrpc_env* env) {
    /*Read  thermocouple signal (in mV)*/
    xmlrpc_value* resultP;
    double signal;

    resultP = xmlrpc_client_call(env, SERVER_URI, "get_thermocouple_signal_mV", "(s)", EXPERIMENT_TOKEN); // get temperature in mV
    dieIfFaultOccurred(env); // error handling

    xmlrpc_read_double(env, resultP, &signal); // Get thermocouple signal and than return it
    dieIfFaultOccurred(env); // error handling

    xmlrpc_DECREF(resultP);
    return signal;
}