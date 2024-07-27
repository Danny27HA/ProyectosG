#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI(); // Inicializa la pantalla TFT

#define BTN_RESET 33  // Botón de reinicio, Pin GPIO D33
#define BTN_NEXT  26   // Botón de siguiente cama, Pin GPIO D35
#define BTN_SEND  25  // Botón de envío de datos, Pin GPIO D32

#define SENSOR_PIN 27  // Pin GPIO D27 para el sensor inductivo

volatile int countPerBed = 0; // Contador por cama
volatile int totalCount = 0;  // Contador total
bool metalDetected = false;   // Variable para evitar contar múltiples veces por un solo objeto metálico

void setup() {
  Serial.begin(9600);

  pinMode(BTN_RESET, INPUT_PULLUP);
  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_SEND, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BTN_RESET), resetCounter, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_NEXT), nextBed, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_SEND), sendData, FALLING);

  Wire.begin();
  tft.init();
  updateScreen(); // Actualiza la pantalla

}

void loop() {
  // Lógica del sensor inductivo
  int sensorState = digitalRead(SENSOR_PIN);
  if (sensorState == HIGH && !metalDetected) {
    // Si el sensor detecta metal y no se ha detectado previamente, incrementa los contadores
    countPerBed++;
    totalCount++;
    metalDetected = true; // Establece la detección de metal a verdadero para evitar contar múltiples veces
    updateScreen(); // Actualiza la pantalla cuando se detecta un metal
  } else if (sensorState == LOW) {
    // Si el sensor no detecta metal, restablecer la variable de detección de metal
    metalDetected = false;
  }
}

// Función de reinicio del contador de cama actual
void resetCounter() {
  countPerBed = 0; // Reinicia el contador de cama actual
  totalCount = 0;  // Reinicia el contador total
  updateScreen(); // Actualiza la pantalla
}

// Función de incremento de cama
void nextBed() {
  countPerBed = 0; // Reinicia el contador de cama actual
  updateScreen(); // Actualiza la pantalla
}

// Función de envío de datos
void sendData() {
  // Aquí agregarías el código para enviar los datos por Telegram
}

// Función para actualizar la pantalla
void updateScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2); // Aumentar el tamaño del texto
  tft.print("Cama : ");
  tft.println(countPerBed);
  tft.print("Total: ");
  tft.println(totalCount);
}
