#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "/usr/include/xmlrpc-c/base.h"
#include "/usr/include/xmlrpc-c/client.h"
#include "emf_to_temperature.h"
#include "dieIfFaultOccured.h"
#include "get_thermocouple_signal_mV.h"
#include "constants.h"
/* Choose here which server would you like to use: local or remote */




void init_xmlrpc(xmlrpc_env* env) {
    /* Initialize our error-handling environment. */
    xmlrpc_env_init(env);

    /* Create the global XML-RPC client object. */
    xmlrpc_client_init2(env, XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION, NULL, 0);
    dieIfFaultOccurred(env);
}


void deinit_xmlrpc(xmlrpc_env* env) {
    /* Clean up our error-handling environment. */
    xmlrpc_env_clean(env);

    /* Shutdown our XML-RPC client library. */
    xmlrpc_client_cleanup();
}


void run_experiment(xmlrpc_env* env) {
    /*
     * Run this function to start clean simulation
     * This function requires initialized XML-RPC environment
     */
    xmlrpc_value* resultP;

    resultP = xmlrpc_client_call(env, SERVER_URI, "run_experiment", "(s)", EXPERIMENT_TOKEN);
    dieIfFaultOccurred(env);
    xmlrpc_DECREF(resultP);
}


void set_current(xmlrpc_env* env, double current) {

    xmlrpc_value* resultP;

    resultP = xmlrpc_client_call(env, SERVER_URI, "set_current", "(sd)", EXPERIMENT_TOKEN, current); // set current in Amperes
    dieIfFaultOccurred(env); // check for errors
}

int main(int const argc, const char** const argv) {
    xmlrpc_env env;

    /* Initialize XML-RPC environment in the very beginning */
    init_xmlrpc(&env);

    /* (Re)Start new simulation */
    run_experiment(&env);

    /* Variables for calculation */

    /*
    double peltier_current - variable for storage of needed peltier current, 
    double thermocouple_voltage_mV - variable for storage actual thermocouple signal
    double temperature - variable for storage actual temperature(we will calculate it from thermocouple signal)
    */

    double peltier_current = 0; 
    double thermocouple_voltage_mV;
    double temperature;

    /* 
    We use PID controller for temperature control
    u(t)=Kp*e(t)+Ki*∫e(τ)dτ+Kd*de/dt
           ↓         ↓           ↓
        Current  Accumulated  Prediction
        reaction  past errors  of future 
             
    So, 
    double mismatch = temperature - T
    double previous_mismatch = mismatch(on previous step)
    double integral_mismatch = ∑err*dt
    double differential_mismatch = de/dt([mismatch - previous_mismatch] / DELTA_TAU)

    Information about PID controller:
    https://en.wikipedia.org/wiki/PID_controller
    */

    double mismatch = 0; 
    double previous_mismatch = 0; 
    double integral_mismatch = 0; 
    double differential_mismatch = 0;

    while(1){

        thermocouple_voltage_mV = get_thermocouple_signal_mV(&env); // Read thermocouple signal
        temperature = emf_to_temperature(thermocouple_voltage_mV); // Convert mV to Celsius
        
        // Calculate PID
        mismatch = temperature - T; 
        integral_mismatch += (temperature - T) * DELTA_TAU; 
        differential_mismatch = (mismatch - previous_mismatch) / DELTA_TAU; 

        peltier_current = mismatch * K_P + differential_mismatch* K_D + integral_mismatch * K_I; // Calculate needed peltier current
        previous_mismatch = mismatch; // Update previous mismatch

        if(peltier_current > MAX_CURRENT) { /* Avoid overcurrent */
            peltier_current = MAX_CURRENT;
        }
        else if (peltier_current < -MAX_CURRENT) {
            peltier_current = -MAX_CURRENT;
        }

        set_current(&env, peltier_current); // Set prefered peltier current

        // Printing data, uncomment if needed
        // printf("Thermocouple signal: %lf mV\n", thermocouple_voltage_mV);
        // printf("Temperature: %lf C\n", temperature);
        // printf("Peltier current: %lf A\n", peltier_current);
        // printf("<---------------PID CONTROLLER--------------->\n");
        // printf("Proportional mismatch: %lf\n", mismatch);
        // printf("Integral mismatch: %lf\n", integral_mismatch);
        // printf("Differential mismatch: %lf\n", differential_mismatch);
        // printf("<=======================================================>")
        // printf("\n");
        

        sleep(DELTA_TAU); // Wait
    }

    /* Clear XML-RPC environment before exiting */
    deinit_xmlrpc(&env);
    return 0;
}
