#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define GATE1_PIN 34
#define GATE2_PIN 33
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const float GATE_SPACING_M = 0.23f;  // distance between gates in meters

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

volatile bool g1_triggered = false;
volatile bool g2_triggered = false;
volatile uint32_t t_start = 0;
volatile uint32_t t_end = 0;

// Gate 1 broken — start timer
void IRAM_ATTR isr_gate1() {
  if (!g1_triggered) {
    t_start = micros();
    g1_triggered = true;
  }
}

// Gate 2 broken — stop timer (only after gate 1 has fired)
void IRAM_ATTR isr_gate2() {
  if (g1_triggered && !g2_triggered) {
    t_end = micros();
    g2_triggered = true;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(GATE1_PIN, INPUT);
  pinMode(GATE2_PIN, INPUT);

  // RISING because LM393 output goes HIGH when beam is broken
  attachInterrupt(digitalPinToInterrupt(GATE1_PIN), isr_gate1, RISING);
  attachInterrupt(digitalPinToInterrupt(GATE2_PIN), isr_gate2, RISING);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.println("Ready...");
  display.display();
}

void loop() {
  if (g1_triggered && g2_triggered) {
    uint32_t elapsed_us = t_end - t_start;
    float elapsed_s = elapsed_us / 1000000.0f;
    float velocity_ms = GATE_SPACING_M / elapsed_s;
    float velocity_fps = velocity_ms * 3.28084f;

    Serial.print("Time: ");
    Serial.print(elapsed_us);
    Serial.print(" us  |  ");
    Serial.print(velocity_ms);
    Serial.print(" m/s  |  ");
    Serial.print(velocity_fps);
    Serial.println(" fps");

    // Show velocity on OLED
    display.clearDisplay();
    display.setTextSize(3);
    display.setCursor(0, 0);
    display.print(velocity_ms, 2);
    display.setTextSize(2);
    display.setCursor(0, 40);
    display.print("m/s");
    display.display();

    g1_triggered = false;
    g2_triggered = false;
  }
}
