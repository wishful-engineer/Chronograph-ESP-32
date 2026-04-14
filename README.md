ESP32 Chronograph
Two-gate IR chronograph built with bare photodiodes, LM393 comparator IC, and an ESP32. Measures projectile velocity in m/s and fps, displays results on an SSD1306 OLED. Built as a support tool for a coilgun project but works for any fast moving object.
Uses hardware interrupts for gate timing — no polling, no missed triggers.
Gate 1 cannot re-arm until gate 2 fires, preventing false readings.

Hardware: 

ESP32
LM393 dual comparator
2x IR photodiode + IR emitter pairs
SSD1306 0.96" OLED (I2C)
10kΩ resistors (x6), 220Ω resistors (x2)

Libraries

Adafruit SSD1306
Adafruit GFX
Wire (built in)
