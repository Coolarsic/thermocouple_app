#ifndef CONSTANTS_H
#define CONSTANTS_H

// Эксперимент и сервер
extern const char* const EXPERIMENT_TOKEN;
extern const char* const SERVER_URI;

// Информация о клиенте
extern const char* const NAME;
extern const char* const VERSION;

// Настройки PID-регулятора
extern const double DELTA_TAU;       // For integral mismatch
extern const double T;               // Target temperature
extern const double K_P;             // Proportional constant
extern const double K_I;             // Integral constant
extern const double K_D;             // Differential constant
extern const double MAX_CURRENT;     // Maximal safe current(peak 6A)

// Коэффициенты для расчета температуры из напряжения
// Взято из: https://meganorm.ru/Data2/1/4294815/4294815957.pdf

// Диапазон от -200 °C до 0 °C (-5.891 до 0 mV)
extern const double C0_NEG;
extern const double C1_NEG;
extern const double C2_NEG;
extern const double C3_NEG;
extern const double C4_NEG;
extern const double C5_NEG;
extern const double C6_NEG;
extern const double C7_NEG;
extern const double C8_NEG;

// Диапазон от 0 °C до 500 °C (0 до 20.644 mV)
extern const double C0_LOW;
extern const double C1_LOW;
extern const double C2_LOW;
extern const double C3_LOW;
extern const double C4_LOW;
extern const double C5_LOW;
extern const double C6_LOW;
extern const double C7_LOW;
extern const double C8_LOW;
extern const double C9_LOW;

// Диапазон от 500 °C до 1372 °C (20.644 до 54.886 mV)
extern const double C0_HIGH;
extern const double C1_HIGH;
extern const double C2_HIGH;
extern const double C3_HIGH;
extern const double C4_HIGH;
extern const double C5_HIGH;
extern const double C6_HIGH;

#endif // CONSTANTS_H