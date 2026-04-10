#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ─── Pin Definitions ─────────────────────────────────────────────
#define FLAME_DIGITAL  7   // KY-026 DO pin  → Arduino D7 (Button simulates in Wokwi)
#define FLAME_ANALOG   A0  // KY-026 AO pin  → Arduino A0 (Potentiometer in Wokwi)
#define BUZZER         8   // Passive buzzer → Arduino D8
#define LED_PIN        13  // LED (220Ω)     → Arduino D13

// ─── LCD: I2C address 0x27, 16 cols, 2 rows ──────────────────────
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ─── Setup ───────────────────────────────────────────────────────
void setup() {
  pinMode(FLAME_DIGITAL, INPUT_PULLUP);
  pinMode(BUZZER,        OUTPUT);
  pinMode(LED_PIN,       OUTPUT);

  // Initialise LCD and show splash screen
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("Fire Alert Sys  ");
  lcd.setCursor(0, 1); lcd.print("  Initialising..");
  delay(2000);
  lcd.clear();
}

// ─── Main Loop ────────────────────────────────────────────────────
void loop() {
  // KY-026 active-LOW: LOW = flame detected
  // In Wokwi: press the button to simulate flame detection
  int flameDigital = digitalRead(FLAME_DIGITAL);
  int flameAnalog  = analogRead(FLAME_ANALOG);

  // Map analog: lower ADC = stronger/closer flame = higher %
  int intensity = map(flameAnalog, 1023, 0, 0, 100);
  intensity = constrain(intensity, 0, 100);

  if (flameDigital == LOW) {
    // ── Flame Detected ────────────────────────────────────────────
    lcd.setCursor(0, 0); lcd.print("FIRE DETECTED!  ");
    lcd.setCursor(0, 1);
    lcd.print("Intensity: ");
    lcd.print(intensity);
    lcd.print("%   ");

    // Buzzer beep pattern (more urgent)
    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(BUZZER, LOW);
    delay(50);
    digitalWrite(BUZZER, HIGH);
    delay(100);
    digitalWrite(BUZZER, LOW);
    delay(50);

    digitalWrite(LED_PIN, HIGH);   // LED stays ON

  } else {
    // ── No Flame ─────────────────────────────────────────────────
    lcd.setCursor(0, 0); lcd.print("No Fire Detected");
    lcd.setCursor(0, 1); lcd.print("System: SAFE    ");

    digitalWrite(BUZZER,  LOW);    // Buzzer OFF
    digitalWrite(LED_PIN, LOW);    // LED OFF
  }

  delay(200);  // 200 ms poll — prevents LCD flicker
}
