#include "constants.h"

// Experiment and server
const char* const EXPERIMENT_TOKEN = "o0ypjZCbvw";
const char* const SERVER_URI = "http://5.35.70.189:8999/RPC2";

// Client and version information
const char* const NAME = "Xmlrpc-c Test Client";
const char* const VERSION = "1.0";

// PID settings
const double DELTA_TAU = 0.1;       // For integral mismatch
const double T = 20;                // Target temperature
const double K_P = 0.3;             // Proportional constant
const double K_I = 0.7;             // Integral constant
const double K_D = 0.008;           // Differential constant
const double MAX_CURRENT = 5;       // Maximal safe current(peak 6A)

// Coeefficients for calculating temperature from voltage
// Data taken from https://meganorm.ru/Data2/1/4294815/4294815957.pdf

// Range from -200 °C to 0 °C (-5.891 to 0 mV)
const double C0_NEG = 0.0;
const double C1_NEG = 2.5173462E+01;
const double C2_NEG = -1.1662878E+00;
const double C3_NEG = -1.0833638E+00;
const double C4_NEG = -8.9773540E-01;
const double C5_NEG = -3.7342377E-01;
const double C6_NEG = -8.6632643E-02;
const double C7_NEG = -1.0450598E-02;
const double C8_NEG = -5.1920577E-04;

// Range from 0 °C to 500 °C (0 mV to 20.644 mV)
const double C0_LOW = 0.0;
const double C1_LOW = 2.508355E+01;
const double C2_LOW = 7.860106E-02;
const double C3_LOW = -2.503131E-01;
const double C4_LOW = 8.315270E-02;
const double C5_LOW = -1.228034E-02;
const double C6_LOW = 9.804036E-04;
const double C7_LOW = -4.413030E-05;
const double C8_LOW = 1.057734E-06;
const double C9_LOW = -1.052755E-08;

// Range from 500 °C to 1372 °C (20.644 mV to 54.886 mV)
const double C0_HIGH = -1.318058E+02;
const double C1_HIGH = 4.830222E+01;
const double C2_HIGH = -1.646031E+00;
const double C3_HIGH = 5.464731E-02;
const double C4_HIGH = -9.650715E-04;
const double C5_HIGH = 8.802193E-06;
const double C6_HIGH = -3.110810E-08;