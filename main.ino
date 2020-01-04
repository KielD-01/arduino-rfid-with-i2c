#include <SPI.h>
#include <Wire.h>
#include <RFID.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SDA_DIO 9
#define RESET_DIO 8

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

RFID RC522(SDA_DIO, RESET_DIO);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void initializeDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.display();
  delay(2000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE); // Draw white text
  display.cp437(true);
  display.display();
}

void displayText(bool cd = false, int line = 0, int pos = 0, String text = "Loading...", int d = 0) {
  if (cd) {
    display.clearDisplay();
  }
  display.setCursor(line, pos);
  display.println(text);
  display.display();

  if (d) {
    delay(d);
  }
}

String getCardSerial(RFID rfid) {
  rfid.readCardSerial();

  String cardNo = "";
  for (int i = 0; i < 5; i++) {
    cardNo.concat(rfid.serNum[i]);
  }

  return cardNo;
}

void setWelcomeText() {
  displayText(true, 0, 0, "Welcome to The RFID!");
  displayText(false, 0, 10, "Waiting for card...");
}

void setup()
{
  Serial.begin(9600);
  initializeDisplay();
  SPI.begin();

  RC522.init();

  setWelcomeText();
}


void loop()
{
  if (RC522.isCard())
  {
    displayText(true, 0, 0, "Card detected");
    displayText(false, 0, 10, getCardSerial(RC522), 3000);
  }

  delay(1000);
  setWelcomeText();
}
