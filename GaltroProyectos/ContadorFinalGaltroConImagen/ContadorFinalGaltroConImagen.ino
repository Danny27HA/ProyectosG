#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <Esp32WifiManager.h> // Para ESP32
#include <Ticker.h> // Librería para temporizadores

const char* ssid = "GALTRO";
const char* password = "GALTRO12";

const char* BOT_TOKEN = "7086997891:AAEIMi4tqPMMOu-Tlisb90Yid3QNPeON1k8"; // Inserta tu token de bot de Telegram
const char* CHAT_ID = "-1002084319155"; // Inserta tu ID de chat de Telegram

TFT_eSPI tft = TFT_eSPI();

#define BTN_RESET 33
#define BTN_NEXT  26
#define BTN_SEND  25
#define SENSOR_PIN 27

volatile int countPerBed = 0;
volatile int totalCount = 0;
bool metalDetected = false;
volatile bool sendDataFlag = false;
volatile bool resetButtonPressed = false; // Variable para controlar el estado del botón de reinicio
bool isConnected = false; // Variable para indicar si el dispositivo está conectado a WiFi

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

void setup() {
  Serial.begin(9600);

  pinMode(BTN_RESET, INPUT_PULLUP);
  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_SEND, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BTN_RESET), resetCounter, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_NEXT), nextBed, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_SEND), triggerSendData, FALLING);

  Wire.begin();
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  tft.init();

  connectToWiFi();
  updateScreen();
}

void loop() {
  if (sendDataFlag) {
    sendData();
    sendDataFlag = false;
  }

  int sensorState = digitalRead(SENSOR_PIN);
  if (sensorState == HIGH && !metalDetected) {
    countPerBed++;
    totalCount++;
    metalDetected = true;
    updateScreen();
  } else if (sensorState == LOW) {
    metalDetected = false;
  }

  if (resetButtonPressed) {
    resetCounter();
    resetButtonPressed = false; // Restablece el estado del botón de reinicio
  }
}

void resetCounter() {
  countPerBed = 0;
  totalCount = 0;
  sendDataFlag = false; // 
  updateScreen();
}

void nextBed() {
  countPerBed = 0;
  updateScreen();
}


void triggerSendData() {
  sendDataFlag = true;
}

void sendData() {
  static bool sendDataTriggered = false; // Variable local estática para rastrear si la función sendData() ha sido activada

  if (!sendDataTriggered) {
    String message = String(totalCount) + " Tubos contados en el rack ";
    
    Serial.println("Enviando mensaje a Telegram");
    if (bot.sendMessage(CHAT_ID, message, "")) {
      Serial.println("Mensaje enviado con éxito");
    } else {
      Serial.println("Error al enviar el mensaje");
    }
    delay(1000); // Agrega un pequeño retardo antes de actualizar la pantalla
    updateScreen();
    sendDataTriggered = true; // Marca que la función sendData() ha sido activada
  }

  sendDataTriggered = false; // Restablece la bandera para permitir el envío de nuevos datos la próxima vez
}

void updateScreen() {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(3);
  tft.println("WiFi: ");
  tft.println(isConnected ? "Conectado" : "Desconectado");
  tft.println();
  tft.println();
  tft.println();
  tft.print("Cama : ");
  tft.println(countPerBed);
  tft.println();
  tft.print("Total: ");
  tft.println(totalCount);
  tft.println();
  tft.println();
  tft.println();
  tft.println("Datos: ");
  tft.println(sendDataFlag ? "Enviados" : "Pendientes");
}

void connectToWiFi() {
  Serial.print("Conectando a la red wifi... ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt < 10) {
    Serial.print(".");
    delay(500);
    attempt++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado a WiFi");
    isConnected = true; // Establece isConnected como verdadero si está conectado a WiFi
  } else {
    Serial.println("\nError: No se pudo conectar a WiFi. Revise las credenciales o la disponibilidad de la red.");
  }
}
