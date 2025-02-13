#pragma once

#include <Arduino.h>

// --- Hardware ---
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_ADDR 0x3C

// --- Device Configuration ---
#define DEVICE_ORIENTATION 0  // 0 = Default, 1 = Rotated 180°

// Configure settings based on orientation
#if DEVICE_ORIENTATION == 1
    #define OLED_ROTATION 2  // 180° rotation
    #define ENCODER_A_PIN 25  // Swapped encoder pins
    #define ENCODER_B_PIN 27
    #define ENCODER_REVERSE_VALUE true  // Reverse encoder for timer adjustment
#else
    #define OLED_ROTATION 0  // Normal orientation
    #define ENCODER_A_PIN 27  // Normal encoder pins
    #define ENCODER_B_PIN 25
    #define ENCODER_REVERSE_VALUE false  // Normal encoder direction
#endif

#define LED_PIN 15
#define NUM_LEDS 16
#define LED_BRIGHTNESS 100

#define BUTTON_PIN 26  // A0 on QT Py ESP32

// --- LED Colors ---
#define BLUE        0x0000FF
#define AMBER       0xFFBF00
#define RED         0xFF0000
#define GREEN       0x00FF00
#define YELLOW      0xFFFF00 
#define MAGENTA     0xFF00FF 
#define TEAL        0x008080  

// --- Defaults ---
#define DEFAULT_TIMER   25  // min - Default to 25 minutes if no value in NVS
#define MIN_TIMER       5   // min - Minimum timer
#define MAX_TIMER       240 // min - Maximum timer (4 hours)

#define SPLASH_DURATION 2   // sec - 2 seconds splash state
#define CHANGE_TIMEOUT  5   // sec - 5 seconds adjust timeout
#define SLEEP_TIMOUT    5   // min - 5 minutes to transition to sleep
#define PAUSE_TIMEOUT   10  // min - 10 minutes to cancel the timer if stayed paused