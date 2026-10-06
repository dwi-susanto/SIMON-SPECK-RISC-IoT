# Hardware-in-the-Loop Benchmarking: SIMON and SPECK on RISC Microcontrollers

This repository contains the source code and raw experimental dataset for the paper evaluating the empirical performance of SIMON and SPECK lightweight cryptography (64-bit block / 128-bit key) on RISC architecture.

## Repository Contents
- **/src/**: Contains the C/C++ implementation of SIMON and SPECK optimized for 8-bit AVR (ATmega328P) and 32-bit Xtensa (ESP32) microcontrollers. Compiled using GCC toolchain with `-Os` flag.
- **/dataset/**: Contains the raw CSV logs of 1,000 test iterations. This raw data includes execution latency ($\mu s$) and power profiling measurements used for standard deviation and statistical significance (Independent Sample t-test) analysis in the paper.

## Hardware Setup
- **8-bit Target:** Arduino Uno (ATmega328P, 16 MHz)
- **32-bit Target:** ESP32 Development Board (240 MHz)
- **Measurement:** Digital Oscilloscope (100 ksps) with a $10 \, \Omega$ precision shunt resistor.

## Reproducibility
The data provided in the `/dataset/` folder can be verified using any standard statistical software (SPSS, R, or Python SciPy) to reproduce the $p-value < 0.001$ significance reported in the manuscript.
