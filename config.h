#ifndef BIER_CONFIG_H
#define BIER_CONFIG_H

#define PID_P 8000
#define PID_I 0
#define PID_D 350000
#define PID_WINDOW_SIZE 10000
#define PID_SAMPLE_TIME 9000

#define TIME_SERIAL 1000
#define LCD_REFRESH 1000 // in ms

struct phase {
    int temp; //in °C
    int duration; //in m
};

#define NR_PHASES 5

phase EINMAISCH = {45, 5};
phase EIWEISS = {50, 5};
phase BETA = {64, 30};
phase ALPHA = {74, 40};
phase LAEUTER = {78, 100};

phase phases[] = {EINMAISCH, EIWEISS, BETA, ALPHA, LAEUTER};

#define TOLERANCE 0.5

#define SwITCH_WINDOW_SIZE 1000000

#endif
