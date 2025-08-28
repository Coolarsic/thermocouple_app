#include "/usr/include/xmlrpc-c/base.h"
#include "/usr/include/xmlrpc-c/client.h"
#include "dieIfFaultOccured.h"
#include <stdio.h>
#include <stdlib.h>
/* XML-RPC Error handling */
void dieIfFaultOccurred(xmlrpc_env* const envP) {
    if (envP->fault_occurred) {
        fprintf(stderr, "ERROR: %s (%d)\n",
            envP->fault_string, envP->fault_code);
        exit(1);
    }
}