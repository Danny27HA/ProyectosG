#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
#define sensorPin A0          // Pin analógico donde está conectado el sensor de pH

void setup() {
  lcd.init(); // Inicializa la pantalla LCD
  lcd.backlight(); // Enciende la retroiluminación de la pantalla LCD
  lcd.begin(16, 2); // Inicializa la pantalla con 16 columnas y 2 filas
  lcd.setCursor(0, 0);
  lcd.print("Galtro de Mexico");
  lcd.setCursor(0, 1); // Establece el cursor en la segunda fila
  lcd.print("Medidor De pH");
  delay(2000); // Espera 2 segundos
  lcd.clear(); // Limpia la pantalla LCD
}

void loop() {
  float pHValue = analogRead(sensorPin); // Lee el valor analógico del sensor
  pHValue = map(pHValue, 0, 1023, 0, 14); // Mapea el valor a un rango de 0 a 14
  lcd.setCursor(0, 1); // Posición del cursor en la segunda línea
  lcd.print("pH: "); // Muestra "pH: " en la pantalla LCD
  lcd.print(pHValue, 2); // Muestra el valor de pH con 2 decimales
  delay(1000); // Espera 1 segundo antes de volver a leer el sensor
}
