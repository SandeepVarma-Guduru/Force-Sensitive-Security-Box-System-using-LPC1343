# Force-Sensitive Security Box System using LPC1343

## Overview

This project implements a **Force-Sensitive Security Box System** using the **NXP LPC1343 ARM Cortex-M3** microcontroller. The system continuously monitors the force applied to a protected object using an analog sensor (simulated with a potentiometer). The analog signal is converted using the on-chip ADC, processed by the microcontroller, and displayed on a 16×2 LCD.

When the sensed force exceeds a predefined threshold, the system enters **Alert Mode**, displays an alert message on the LCD, and turns ON an LED. When the force falls below the secure threshold, the system returns to **Secure Mode**.

A hysteresis-based decision algorithm is implemented to eliminate false triggering caused by sensor noise.

---

# Features

- Embedded C firmware for LPC1343
- ARM Cortex-M3 based implementation
- On-chip ADC configuration
- 16×2 LCD interfacing (4-bit mode)
- GPIO programming
- LED indication
- Sensor monitoring using ADC
- Hysteresis-based threshold detection
- Noise reduction using ADC sample averaging
- Continuous real-time monitoring

---

# Hardware Used

- NXP LPC1343 (ARM Cortex-M3)
- 16×2 LCD (HD44780 Compatible)
- Potentiometer (used as Force Sensor Simulation)
- LED
- Power Supply

---

# Software Used

- Embedded C
- Keil µVision
- Proteus
- CMSIS (Cortex Microcontroller Software Interface Standard)

---

# Project Working

1. Initialize GPIO pins.
2. Configure LCD interface.
3. Initialize the on-chip ADC.
4. Read the analog sensor value.
5. Average 10 ADC samples to reduce noise.
6. Compare the ADC value with predefined thresholds.
7. If the value exceeds the Alert Threshold:
   - Display **"!! ALERT !!"**
   - Turn ON the LED.
8. If the value falls below the Secure Threshold:
   - Display **"SECURE"**
   - Turn OFF the LED.
9. Repeat continuously.

---

# Pin Configuration

| LPC1343 Pin | Function |
|-------------|----------|
| P0.11 | ADC Input (Sensor) |
| P0.7 | LED Output |
| P1.0 | LCD RS |
| P1.1 | LCD RW |
| P1.2 | LCD EN |
| P1.4 | LCD D4 |
| P1.5 | LCD D5 |
| P1.6 | LCD D6 |
| P1.7 | LCD D7 |

---

# Software Architecture

```
System Start
      │
      ▼
GPIO Initialization
      │
      ▼
LCD Initialization
      │
      ▼
ADC Initialization
      │
      ▼
Read ADC Value
      │
      ▼
Average 10 Samples
      │
      ▼
Compare Thresholds
      │
 ┌────┴─────┐
 │          │
 ▼          ▼
Secure    Alert
 │          │
 ▼          ▼
LCD       LCD
LED OFF   LED ON
 │
 ▼
Repeat Forever
```

---

# Threshold Values

| State | ADC Value |
|---------|-----------|
| Secure | Below 1630 |
| Alert | Above 2460 |

The gap between these two values implements **hysteresis**, preventing rapid switching caused by sensor fluctuations.

---

# Code Modules

### delay_ms()

Generates software delays required for LCD timing and ADC stabilization.

### lcd_init()

Initializes the LCD in 4-bit communication mode.

### adc_init()

Configures the LPC1343 on-chip ADC for analog input acquisition.

### read_adc()

- Performs ADC conversion
- Reads 10 samples
- Computes the average value
- Returns the filtered ADC reading

### show_secure()

Displays:

```
SECURE
NO INTRUDERS
```

Turns OFF the LED.

### show_alert()

Displays:

```
!! ALERT !!
INTRUDERS DETECT
```

Turns ON the LED.

### main()

Initializes all peripherals and continuously monitors the sensor to update the system status.

---

# Skills Demonstrated

- Embedded C Programming
- ARM Cortex-M3 Programming
- LPC1343 Microcontroller
- Bare-Metal Programming
- Register-Level Programming
- CMSIS
- GPIO Configuration
- ADC Programming
- LCD Driver Development
- Embedded Firmware Development
- Peripheral Initialization
- Sensor Interfacing
- Real-Time Embedded Systems
- State Machine Design
- Polling-Based Programming
- Embedded System Simulation

---

# Applications

- Smart Security Systems
- Locker Monitoring
- Asset Protection
- Anti-Tamper Systems
- Smart Cabinets
- Laboratory Equipment Security
- IoT Security Devices

---

# Future Improvements

- Replace the potentiometer with a Force Sensitive Resistor (FSR).
- Add GSM/Wi-Fi notifications.
- Store alert logs using external EEPROM or SD card.
- Add keypad-based authentication.
- Integrate a Real-Time Clock (RTC).
- Extend to multiple force sensors.

---

# Repository Structure

```
Force-Sensitive-Security-Box-LPC1343
│
├── Source
│   └── main.c
│
├── Proteus
│   ├── Project.pdsprj
│   └── Simulation Files
│
├── HEX
│   └── firmware.hex
│
├── Images
│   ├── Circuit.png
│   ├── Secure.png
│   └── Alert.png
│
├── Report
│   └── Embedded_Systems_Project.pdf
│
└── README.md
```

---

# Author

**Guduru Sandeep Varma**

B.Tech Electronics and Communication Engineering

Indian Institute of Information Technology Kottayam

---

## License

This project is developed for educational and learning purposes.
