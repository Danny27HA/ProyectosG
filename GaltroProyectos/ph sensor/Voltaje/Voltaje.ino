#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int PHPin = A0;  // Sensor conectado en el pin A0
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Dirección I2C y tamaño de la pantalla LCD

void setup() {
  Serial.begin(9600);
  lcd.init(); // Inicializa la pantalla LCD
  lcd.backlight(); // Enciende la retroiluminación de la pantalla LCD
}

void loop() {
  voltajepromedio();  // Función que calcula el ph promedio de 10 muestras
  delay(1000);
}

void impresion(LiquidCrystal_I2C &lcd, float v) {
  lcd.clear();  // Limpia la pantalla LCD
  lcd.setCursor(0, 0); // Establece el cursor en la primera fila
  lcd.print("Voltaje: ");
  lcd.print(v, 2); // Muestra el valor de pH con dos decimales
  Serial.print("Voltaje: ");
  Serial.println(v);
}

void voltajepromedio() {
  long muestras = 0;       // Variable para las 10 muestras
  float muestrasprom = 0;  // Variable para calcular el promedio de las muestras

  // 10 muestras, una cada 10ms
  for (int x = 0; x < 10; x++) {
    muestras += analogRead(PHPin);
    delay(10);
  }

  muestrasprom = muestras / 10.0;
  float Voltaje = muestrasprom * (5.0 / 1024.0);  // Se calcula el voltaje
  impresion(lcd, Voltaje);  // Se llama a la función de impresión
}
