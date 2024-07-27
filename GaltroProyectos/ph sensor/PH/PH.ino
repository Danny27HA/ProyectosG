#include <Wire.h>
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x27,16,2);
const int PHPin = A0;  //Sensor conectado en PIN A0

float b;
float m;
float vsol1; //voltaje de 6.86
float vsol2; //voltaje de4.01

void setup() {
  vsol1= 3.65;
  vsol2= 4.25;
  m= (4.01 - 6.86)/(vsol2 - vsol1);
  b= (m * -vsol1) + 6.86;

  Serial.begin(9600);
  lcd.init(); // Inicializa la pantalla LCD
  lcd.backlight(); // Enciende la retroiluminación de la pantalla LCD
  lcd.begin(16, 2); // Inicializa la pantalla con 16 columnas y 2 filas
  lcd.setCursor(0, 0);
  lcd.print("Galtro de Mexico");
  lcd.setCursor(0, 1); // Establece el cursor en la segunda fila
  lcd.print("Medidor De pH");
  delay(2000); // Espera 2 segundos
  lcd.clear(); // Limpia la pantalla LCD

  Serial.print("Valor de m: ");
  Serial.println(m);
  Serial.print("Valor de b: ");
  Serial.println(b);

}


void loop() {

  PHpromedio();  //función que calcula el ph promedio de 10 muestras
  delay(2000);
}


void PHpromedio() {
  long muestras;       //Variable para las 10 muestras
  float muestrasprom;  //Variable para calcular el promedio de las muestras

  muestras = 0;
  muestrasprom = 0;


  //100 muestras, una cada 100ms
  for (int x = 0; x < 100; x++) {
    muestras += analogRead(PHPin);
    delay(10);
  }

  muestrasprom = muestras / 100.0;

  float phVoltaje = muestrasprom * (5.0 / 1024.0);  //se calcula el voltaje

  float pHValor = phVoltaje * m + b;  //Aplicando la ecuación se calcula el PH

  impresion(pHValor, phVoltaje);
}


void impresion(float ph, float v) {
  //float promph;
  //promph= ph+0.4;
  Serial.print("Ph: ");
  Serial.print(ph);
  Serial.print("  Voltaje: ");
  Serial.println(v);
  lcd.setCursor(0, 0); // Posición del cursor en la segunda línea
  lcd.print("Voltaje: "); // Muestra "pH: " en la pantalla LCD
  lcd.print(v, 2); // Muestra el valor de pH con 2 decimales
  lcd.setCursor(0, 1); // Posición del cursor en la segunda línea
  lcd.print("pH: "); // Muestra "pH: " en la pantalla LCD
  lcd.print(ph, 2); // Muestra el valor de pH con 2 decimales
}
