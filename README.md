# Automatic Rain Detection with Temperature Control for Automobiles

## Overview
Developed an embedded system for automobiles that automatically detects rain and controls window operation, wiper activation, and temperature-based actions using ATmega32 microcontroller.

## Features
- Automatic rain detection using sensor input
- Automatic window closing when rain is detected
- Wiper activation for rain conditions
- Temperature monitoring and AC control suggestion
- Manual and automatic operation modes
- LCD display for real-time system status

## System Functionality
- In automatic mode:
  - Detects rain and closes window automatically
  - Activates wiper motor
  - Monitors temperature and suggests AC adjustment
- In manual mode:
  - User-controlled window and wiper operation
- Displays system status (Engine ON/OFF, Rain detected, Window status, Temperature)

## Hardware Components
- ATmega32 Microcontroller
- Rain Sensor
- Temperature Sensor (Comparator-based)
- DC Motors (Window + Wiper)
- LCD Display (16x2)
- Switches for manual control

## Software Details
- Developed using Embedded C
- Timer-based delays for motor control
- GPIO-based control for actuators
- Interrupt-free polling-based system design

## Tools Used
- Atmel Studio
- SimulIDE (for circuit simulation)

## Key Highlights
- Automotive use-case implementation
- Multi-mode system (manual + automatic)
- Integration of multiple subsystems (rain, temperature, motor control)
