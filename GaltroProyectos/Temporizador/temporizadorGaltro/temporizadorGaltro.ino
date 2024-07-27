#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte filas = 4; // Número de filas del teclado
const byte columnas = 4; // Número de columnas del teclado
char teclas[filas][columnas] = {
  {'1','2','A','3'},
  {'4','5','B','6'},
  {'7','8','C','9'},
  {'*','0','D','#'}
};

byte pinesFilas[columnas] = {5, 4, 3, 2}; // Conecta las filas del teclado a los pines 5, 4, 3 y 2 del Arduino
byte pinesColumnas[filas] = {9, 8, 7, 6}; // Conecta las columnas del teclado a los pines 9, 8, 7 y 6 del Arduino

Keypad teclado = Keypad(makeKeymap(teclas), pinesFilas, pinesColumnas, filas, columnas);

LiquidCrystal_I2C lcd(0x27, 16, 2); // Dirección I2C de la pantalla LCD, 16 columnas y 2 filas

const int buzzerPin = 10;
const int ledPin = 11;
const int startButtonPin = 12;
const int resetButtonPin = 13;

int minutos = 0;
int segundos = 0;
bool temporizadorActivo = false;

void setup() {
  lcd.init(); 
  lcd.backlight();
  
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(startButtonPin, INPUT_PULLUP);
  pinMode(resetButtonPin, INPUT_PULLUP);
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
}

void loop() {
  char tecla = teclado.getKey();
  
  if (tecla) {
    if (tecla >= '0' && tecla <= '9') {
      minutos = minutos * 10 + (tecla - '0');
      lcd.setCursor(0, 0);
      lcd.print("Minutos: ");
      lcd.print(minutos);
    } else if (tecla == '*') {
      minutos = 0;
      segundos = 0;
      temporizadorActivo = false;
      digitalWrite(ledPin, LOW);
      digitalWrite(buzzerPin, LOW);
      lcd.clear();
    } else if (tecla == '#') {
      temporizadorActivo = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temporizador:");
    }
  }
  
  if (digitalRead(startButtonPin) == LOW) {
    temporizadorActivo = true;
  }
  
  if (digitalRead(resetButtonPin) == LOW) {
    minutos = 0;
    segundos = 0;
    temporizadorActivo = false;
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
    lcd.clear();
  }
  
  if (temporizadorActivo) {
    if (segundos == 0 && minutos == 0) {
      digitalWrite(ledPin, HIGH);
      digitalWrite(buzzerPin, HIGH);
      delay(3000);
      digitalWrite(buzzerPin, LOW);
      delay(1000);
    } else {
      delay(1000);
      if (segundos == 0) {
        minutos--;
        segundos = 59;
      } else {
        segundos--;
      }
      lcd.setCursor(0, 1);
      lcd.print(minutos);
      lcd.print(":");
      if (segundos < 10) {
        lcd.print("0");
      }
      lcd.print(segundos);
    }
  }
}
