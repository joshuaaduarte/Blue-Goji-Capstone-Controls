/*
 * Blue Goji Omnidirectional Treadmill Capstone
 * University of California Berkeley MEng 2023
 * Author: Billy Lin 
 * Last-Edit: 02-14-2023
 */

#ifndef PIN_DEFINITION_H
#define PIN_DEFINITION_H

// motor pin definitions
#define pwmA 6
#define pwmB 11
#define fwA 7 // equivalent to in1A, in2A is obtained by negating this signal
#define fwB 8
#define enA 12 // only used in big motors
#define enB 13

// Pin definitions for load cells
#define clk 3
#define dat_lc1 14
#define dat_lc2 15
#define dat_lc3 16
#define dat_lc4 17

#define startButton 2  // Green
#define emergencyButton 4 // Yellow

#endif
