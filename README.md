# Arduino Button Simulator
Sample Arduino app for automatize Can Car Alarm settings.

## Control from Serial port (USB, serial monitor)
It can be controlled from serial port with the following commands:
- Reset Program: "r"
- Reset the Alarm: "i"
- Playback buttons: "p"
- Configure numbers: [Num]

## Control from IR Remot
It can be controlled from IR remote controller with the following commands:
- Reset Program: "*"
- Reset the Alarm: "#"
- Playback buttons: "OK" 
- Configure numbers: [Num]


## Usage
1. Configure the two car specific numbers (maximum two digits per number) 
   * Serial commands: "2\n" (first led on) and "12\n" (second led on)
   * IR Commands: "02" (first led on) and "12" (second led on)
2. Start playback: (Main output blinks the pre codes (8, 7, 6, _, 1) then the confiured car specific numbers (2, 12))
   * Serial commands: "p" 
   * IR Commands: "OK"

## Hardware
1. Arduino nano 2
2. IR receiver and remote controller
3. 3 led (with resistors (200 Ohm to 1k Ohm))
   * D3 is a Main Output
   * D4 represents a first number (D4 --> Resistor --> LED+ --> GND)
   * D5 represents a second number
   * D11 is an IR Receiver data pin (+ should be connected to 3v3 and - should be connected to GND pins)
   
## Software prerequisites
This code uses the IRremote library (https://github.com/shirriff/Arduino-IRremote). Copy to /Arduino/libraries/IRremote folder (Default IR library should be removed)
   

