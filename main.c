#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "/usr/include/xmlrpc-c/base.h"
#include "/usr/include/xmlrpc-c/client.h"

/* Choose here which server would you like to use: local or remote */

#define EXPERIMENT_TOKEN "o0ypjZCbvw"
#define SERVER_URI "http://5.35.70.189:8999/RPC2"

#define NAME "Xmlrpc-c Test Client"
#define VERSION "1.0"

// Coefficients are taken from https://meganorm.ru/Data2/1/4294815/4294815957.pdf

/* Coefficients for calculation of temperature from voltage*/
// Range from -200 °C to 0 °C (-5.891 to 0 mV)
#define C0_NEG (0.0)
#define C1_NEG (2.5173462E+01)    
#define C2_NEG (-1.1662878E+00)
#define C3_NEG (-1.0833638E+00)
#define C4_NEG (-8.9773540E-01)
#define C5_NEG (-3.7342377E-01)
#define C6_NEG (-8.6632643E-02)
#define C7_NEG (-1.0450598E-02)
#define C8_NEG (-5.1920577E-04)

// Range from 0 °C to 500 °C (0 to 20.644 mV)
#define C0_LOW (0.0)
#define C1_LOW (2.508355E+01)      
#define C2_LOW (7.860106E-02)
#define C3_LOW (-2.503131E-01)
#define C4_LOW (8.315270E-02)
#define C5_LOW (-1.228034E-02)
#define C6_LOW (9.804036E-04)
#define C7_LOW (-4.413030E-05)
#define C8_LOW (1.057734E-06)
#define C9_LOW (-1.052755E-08)

// Range from 500 °C to 1372 °C (20.644 to 54.886 mV)
#define C0_HIGH (-1.318058E+02)    
#define C1_HIGH (4.830222E+01)     
#define C2_HIGH (-1.646031E+00)
#define C3_HIGH (5.464731E-02)
#define C4_HIGH (-9.650715E-04)
#define C5_HIGH (8.802193E-06)
#define C6_HIGH (-3.110810E-08)
/*End coeeficients to calculate temperature from voltage*/

#define DELTA_TAU 0.1 // For integral mismatch
#define T 20 // Target temperature
#define K_P 0.3 // Proportional constant
#define K_I 0.7 // Integral constant
#define K_D 0.0045 // Differential constant
#define MAX_CURRENT 5 // Maximal safe current(peak 6A)
 
/* XML-RPC Error handling */
static void dieIfFaultOccurred(xmlrpc_env* const envP) {
    if (envP->fault_occurred) {
        fprintf(stderr, "ERROR: %s (%d)\n",
            envP->fault_string, envP->fault_code);
        exit(1);
    }
}

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


double emf_to_temperature(double emf) {
    /* Convert emf to temperature in degrees Celsius by Horner method*/
    if (emf >= -5.891 && emf < 0) { // Range from -5.891 mV to 0 mV -> -200°C to 0°C
        
        return C0_NEG + emf * (C1_NEG + emf * 
            (C2_NEG + emf * (C3_NEG + emf * (C4_NEG + emf * (C5_NEG + emf * 
                (C6_NEG + emf * (C7_NEG + emf * C8_NEG)))))));
    
    } else if (emf >= 0.0 && emf <= 20.644) { // Range from 0 mV to 20.644 mV -> 0°C to 500°C
        return C0_LOW + emf * (C1_LOW + emf * (C2_LOW + emf * 
            (C3_LOW + emf * (C4_LOW + emf * (C5_LOW + emf * 
                (C6_LOW + emf * (C7_LOW + emf * (C8_LOW + emf * C9_LOW))))))));
        
    } else if (emf > 20.644 && emf <= 54.886) { // Range from 20.644 mV to 54.886 mV -> 500°C to 1372°C
        return C0_HIGH + emf * (C1_HIGH + emf * (C2_HIGH + emf * (C3_HIGH + emf * 
            (C4_HIGH + emf * (C5_HIGH + emf * C6_HIGH)))));
    } else {
        // Voltage out of range
        printf("Error: voltage out of range(must be from -5.891 mV to 54.886 mV). Current voltage: %.4lf\n", emf);
        return -1000; // Error
    }
}

int main(int const argc, const char** const argv) {
    xmlrpc_env env;
    double peltier_current = 0; // in A
    double thermocouple_voltage_mV;

    /* Initialize XML-RPC environment in the very beginning */
    init_xmlrpc(&env);

    /* (Re)Start new simulation */
    run_experiment(&env);

    double mismatch = 0; // mismatch between target temperature and current temperature
    double previous_mismatch = 0; // previous mismatch(for derivative calculation)
    double integral_mismatch = 0; // integral mismatch(for integral calculation)
    while(1){
        thermocouple_voltage_mV = get_thermocouple_signal_mV(&env); // Read thermocouple signal
        double temperature = emf_to_temperature(thermocouple_voltage_mV); // Convert mV to temperature
        mismatch = temperature - T; // Calculate mismatch
        integral_mismatch += (temperature - T) * DELTA_TAU; // Calculate integral mismatch
        peltier_current = mismatch * K_P + (mismatch - previous_mismatch) * K_D + integral_mismatch * K_I; // Calculate needed peltier current
        previous_mismatch = mismatch; // Update previous mismatch
        if(peltier_current > MAX_CURRENT) { /* Avoid overcurrent */
            peltier_current = MAX_CURRENT;
        }
        else if (peltier_current < -MAX_CURRENT) {
            peltier_current = -MAX_CURRENT;
        }
        set_current(&env, peltier_current); // Set prefered peltier current
        printf("Thermocouple signal: %lf mV\n", thermocouple_voltage_mV); /* data printing */
        printf("Temperature: %lf C\n", temperature);
        printf("Peltier current: %lf A\n", peltier_current);
        sleep(DELTA_TAU); // Wait
    }


    /* Clear XML-RPC environment before exiting */
    deinit_xmlrpc(&env);
    return 0;
}
