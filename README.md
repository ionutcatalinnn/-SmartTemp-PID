🔥 ThermoGuard – Arduino PID Temperature Controller
ThermoGuard is an Arduino-based temperature regulation system that uses a PID (Proportional–Integral–Derivative) control loop to maintain a target temperature. It reads real-time data from a DHT11 temperature sensor and adjusts a connected MOSFET-driven heating element accordingly.

✅ Features
📟 LCD menu interface with two modes:

Real-time temperature + clock display

Adjustable temperature setpoint configuration

🔁 PID control logic with adjustable Kp, Ki, Kd values

🔘 Button-based menu navigation: OK, Cancel, Next, Previous

🧠 Stores and applies a new temperature setpoint on confirmation

🔌 MOSFET output control for heaters or similar devices

🔧 Hardware Requirements
Arduino Uno/Nano

DHT11 Temperature Sensor

LiquidCrystal I2C 16x2 LCD

HW-042 MOSFET Module

4x Push buttons

📂 Code Highlights
Modular structure with a clean state machine for menu handling

PID logic implemented for precise thermal control

Simple and intuitive user interface via LCD and buttons
