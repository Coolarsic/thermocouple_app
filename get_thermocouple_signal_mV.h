#include "/usr/include/xmlrpc-c/base.h"
#include "/usr/include/xmlrpc-c/client.h"
#include "dieIfFaultOccured.h"
#include "constants.h"
#ifndef GET_THERMOCOUPLE_SIGNAL_MV_H
#define GET_THERMOCOUPLE_SIGNAL_MV_H

double get_thermocouple_signal_mV(xmlrpc_env* env);

#endif