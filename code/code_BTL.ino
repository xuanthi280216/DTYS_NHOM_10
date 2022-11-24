#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>
Adafruit_SSD1306 display(128, 64, &Wire, -1);
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS     1000
PulseOximeter pox;

uint32_t tsLastReport = 0;
void onBeatDetected(){
    Serial.println("Beat!");
}

void setup(){
    Serial.begin(115200);
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      // thiết lập màn hình với địa chỉ 0x3C
      Serial.println("SSD1306 allocation failed");
      for(;;);
    }
    display.setFont(&FreeSerif9pt7b);   // cài đặt phông chữ
    display.clearDisplay();             // xóa toàn màn hình
    display.setTextSize(1);             // setTextSize(s): chỉnh size chữ, mặc định không dùng lệnh này thì s = 1             
    display.setTextColor(WHITE);        // setTextColor(c): c là màu chữ. WHITE(1) hoặc BLACK(0)
    display.setCursor(20,15);           // setCursor(x,y): đưa điểm bắt đầu vẽ lên màn hình từ tọa độ x,y  
    display.println("Welcom to");       // hiển thị lên màn hình 
    display.setCursor(20,40);             
    display.println("NHOM 10");
    display.display();
    display.setTextSize(1);
    delay(2000); 
    Serial.print("Initializing pulse oximeter..");
    if (!pox.begin()) {
        // bắt đầu khởi chạy với cảm biến
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }
    pox.setOnBeatDetectedCallback(onBeatDetected);
}
void loop(){
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("bpm / SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");
        display.clearDisplay(); 
        display.setCursor(10,12); 
        display.print("Pulse Oximeter");     
        display.setCursor(0,35); 
        display.print("HeartR:");
        display.setCursor(62,35);
        display.print(pox.getHeartRate(),0); 
        display.println(" bpm");
        display.setCursor(0,59);
        display.print("SpO2  : ");
        display.setCursor(62,59);
        display.print(pox.getSpO2());
        display.println(" %");
        display.display();
        tsLastReport = millis();
    }
}
