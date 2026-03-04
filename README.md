# Dino-Game-ESP32
<p align="center"><img alt="Dino logo" src="https://github.com/user-attachments/assets/0f98c5f6-505d-43f1-86d9-b0fb97a669ac" width="600"></p>
<p align="center">
  <b> Dino running game using ESP32
    
<br><br>

# About
Offline Chrome Dino game built on ESP32 DevKit V1 with a 0.96" SSD1306 OLED display. Features pixel-accurate sprite animations, scrolling terrain, cacti, pterodactyls, score tracking, and progressive speed — all on a 128×64 screen. Hardware: ESP32 DevKit V1 · OLED 0.96" I2C Push Button. Libraries: Adafruit SSD1306 · Adafruit GFX

<br><br>

# Do It Yourself
<p align="center"><img alt="Dino logo" src="https://github.com/user-attachments/assets/33581f38-d575-4b3a-bab4-b958cb0591f4" width="400"></p>


# Connections
Make the following connections between your oled display and your ESP32 board.

| OLED    | ESP32  |
| ------- | ------ |
|  GND    | GND    |
|  VCC    | 3.3v   |
|  SCL    | GPIO 22|
|  SDA    | GPIO 21|

Connections between your Buttons and your ESP32 board.

| button for jump | ESP32  |
| -------         | ------ |
|        GND      | GPIO 14|

| button for Down | ESP32  |
| -------         | ------ |
|        GND      | GPIO 27|
-----

## Hardware
This project requires the following hardware in order to work:
- ESP32 Dev v1
- oled display 0.96"
- Jumper Wires
- Breadboard
- switch

<br>

<img width="460" height="1500" alt="components_of_dino" src="https://github.com/user-attachments/assets/dca4714b-95e7-4206-b040-cc16f55ec908" />

<br>

and for [connections table](connections.md)
