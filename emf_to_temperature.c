#include "constants.h"

double emf_to_temperature(double emf) {
    /* Convert emf to temperature in degrees Celsius by Horner method*/
    /*
    Converting using polynomial equation
    See https://meganorm.ru/Data2/1/4294815/4294815957.pdf for more details
    */
    double temperature; // Temperature in degrees Celsius

    /* Calculate temperature by Horner method */
    if (emf >= -5.891 && emf < 0) { // Range from -5.891 mV to 0 mV -> -200°C to 0°C
        double massive[9] = {C8_NEG, C7_NEG, C6_NEG, C5_NEG, C4_NEG, C3_NEG, C2_NEG, C1_NEG, C0_NEG};
        for(int i = 0; i < 9; i++) { temperature = temperature * emf + massive[i]; } 
        
    } else if (emf >= 0.0 && emf <= 20.644) { // Range from 0 mV to 20.644 mV -> 0°C to 500°C
        double massive[10] = {C9_LOW, C8_LOW, C7_LOW, C6_LOW, C5_LOW, C4_LOW, C3_LOW, C2_LOW, C1_LOW, C0_LOW};
        for(int i = 0; i < 10; i++) { temperature = temperature * emf + massive[i]; }   
        
    } else if (emf > 20.644 && emf <= 54.886) { // Range from 20.644 mV to 54.886 mV -> 500°C to 1372°C
        double massive[10] = {C6_HIGH, C5_HIGH, C4_HIGH, C3_HIGH, C2_HIGH, C1_HIGH, C0_HIGH};
        for(int i = 0; i < 10; i++) { temperature = temperature * emf + massive[i]; }
    } else { // Voltage out of range
        return -1000; // Error
    }
    return temperature;
}