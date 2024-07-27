#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI(); // Inicializa la pantalla TFT
setup_t user; // The library defines the type "setup_t" as a struct
              // Calling tft.getSetup(user) populates it with the settin
void setup() {
  Serial.begin(9600);

  // Inicialización de la pantalla TFT
  tft.init();
  tft.setRotation(1); // Ajustar rotación si es necesario
  tft.fillScreen(TFT_BLACK); // Limpia la pantalla con color negro
}

void loop() {
  tft.setCursor(10, 10); // Establece la posición del texto
  tft.setTextColor(TFT_WHITE); // Color del texto
  tft.setTextSize(2); // Tamaño del texto
  tft.print("Hola Mundo"); // Imprime "Hola Mundo" en la pantalla
  
  delay(5000); // Espera 5 segundos antes de volver a mostrar el texto
  tft.fillScreen(TFT_BLACK); // Limpia la pantalla nuevamente
}
