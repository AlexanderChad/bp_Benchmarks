#pragma once
#include <Arduino.h>

#define ION_Power_PIN PA2  // пин vcc
#define ION_ADC_PIN PA1    // пин АЦП
#define ION_Ground_PIN PA0 // пин gnd

#define const_mass_size 1000 // размер массива в ПЗУ
#define CLOCKS_PER_MCSEC 72  // тактов процессора за микросекунду

#define Benchmarks_tests 6 // количество тестов

#define Benchmarks_multiplier 5 // запусков бенчмарка после запуска (сброса)
uint8_t Benchmarks_int = 0;

uint64_t BResult[Benchmarks_multiplier][Benchmarks_tests][2];

// Обнуляем счетчик
#define StartTime DWT->CYCCNT = 0
// Печатаем время
#define PrintTime(test_num)                                                                                                   \
    uint32_t test##test_num##_time = DWT->CYCCNT;                                                                             \
    float test##test_num##_time_mcs = (float)test##test_num##_time / (float)CLOCKS_PER_MCSEC;                                 \
    sprintf(buff, "Test %d. Result: %ld pc, %d mcs.", test_num, (long)test##test_num##_time, (int)test##test_num##_time_mcs); \
    Serial.println(buff);                                                                                                     \
    BResult[Benchmarks_int][test_num - 1][0] = (uint64_t)test##test_num##_time;                                               \
    BResult[Benchmarks_int][test_num - 1][1] = (uint64_t)test##test_num##_time_mcs

#define const_mass_multiplier 1 // во сколько раз массив должен быть больше в ОЗУ
#define big_mass_size 1000      // const_mass_size *const_mass_multiplier // размер массива в RAM
#define ADC_INT_speed 50000     // количество интераций (замеров) в тесте speed
#define ADC_INT_jitter 1000     // количество интераций (замеров) в тесте jitter
#define GPIO_SPEED_PIN PC13     // пин для теста gpio_speed
#define GPIO_INT_speed 50000    // количество интераций в тесте gpio_speed
// парамертры фильтра ADC
#define Filtr_A 3
#define Filtr_B 1
#define Filtr_K 2

const char const_mass[const_mass_size] = "d-Q0bfdq7UqbUatqkdkhU-VJNtTg6FH1NBBNJPp24-oIoOGAtvpfCwZYu82GmwHKFbXR32wj27XmCmGopIjj88uBfALuxTYOQCqZuFQsibcYb5a1pfXV5nyfG_JwOAD04xFV3eMxopTiqpjEZJ0hrxn_M7zspIRXTH-xGPqA8aT3tCyWQd2ZzU3wkA3Asr2woxUKQpY67sIcZLLiZb8r29okopXaXZIID5dl4tCM0J6HjYJiGNPtTQ5NeQC4vGNoQXmIi2UBTz1JnA5tlzBF2TLvb0TpXd1d-zoUKWL6ZEFg0qb_YZvDvrEkftm8DersVPnGboYC1XZAr_LpmHUu1rjT5LPsrD97QrUB2dNULawGH2wwRGRr2gDpHltU4hwmu_RSdd3rkLPx2SONChbNuLR1wQt-EP4Ku_BjJln2md9RHHltq6ik47Qy8-lGzxE8x3JkEqHGQoaOJKvh2qkBZzb74fo4MBbGZVUBtl8mkRHaEqr9E1ZmEUYmPxNt6ZZjIcmX3n5YBD-m32RHrx4-cDApuf0QMOLeEt5d-AeYidyRp-dK3nvi9OqiCGDFpp8GtW0iOl6JEvlXIf74Ll6BGArGYhf4RGjFggg5hEAxf7UQ0l2DlMstW24kYhvHws6HGpW9k1sakNYU6GDdpAvcAYB4WqtSYl2wDiuhWciczhBf0iDHE_yzkt0m7wX_JhEt_ZKj0CwUhQS0T8xjvF1jYO0CSSwoCSuVAVwN6CWgypUT-mxs6Es03Z0vIF-ZTl5woNvIvpMGyasKEeqjZn6T_32PzaRwxrjqdmnF9mWdWbfOTD2NRD3xsF8SUX5T75ccv5EI2bU9C1yHKDS02VmXMcZDUGdsmJaltcvX39D_wmn-1gNJy31mG1lpVs_Ij8HM2AixB9gGYY7MIaRYS9cS05Rt0jPi92cZks0TtEqQ7YWWXleyK_bUTipCvRWB8edbRWcGcwIrFfxm6Nff3Sunb1Ri66PCW6jeQIUrvJU";