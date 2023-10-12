
#include <Wire.h> // Biblioteca para la comunicación I2C
#include <LiquidCrystal_I2C.h> // Biblioteca para el LCD I2C
#include "HX711.h" // Biblioteca del sensor de carga
#include <Servo.h> // Biblioteca del micro servo

LiquidCrystal_I2C lcd(0x27, 16, 2); // Dirección I2C del LCD y número de columnas y filas // si no te sale con esta direccion  puedes usar (0x3f,16,2) || (0x27,16,2)  ||(0x20,16,2) 

HX711 scale;

int manualPin = 2; // Botón para apertura manual
int estadoBoton = 0; // Variable para estado del botón
int servoPin = 4; // Variable del servo motor continuo
Servo Servo1;

const float pesoControl = 500.0; // Peso objetivo en gramos
bool alimentar = false; // Variable para controlar la alimentación en curso

void setup()
{
  lcd.noBlink();
  lcd.init();
  lcd.backlight();
  
  scale.begin(A1, A0);// pines DT y SCK del módulo HX711

  pinMode(manualPin, INPUT_PULLUP);
  
  Servo1.attach(servoPin);
  pinMode(servoPin, OUTPUT); // Servomotor nombrado como salida
  
  lcd.begin(16, 2); // Inicializar el LCD I2C
  
  lcd.setCursor(2, 0);
  lcd.print("Benvenidos a");
  lcd.setCursor(4, 1);
  lcd.print("GammFeed");
  delay(3000);
  lcd.clear();
  
  scale.set_scale(); // Configurar la escala del HX711
  
  lcd.print("Realizando tara...");
  scale.tare(); // Realizar la tara inicial de la balanza
  
  lcd.setCursor(0, 0);
  lcd.println("Listo!!!");
  delay(3000);
  lcd.clear();
}

void loop()
{
  estadoBoton = digitalRead(manualPin); // Leer el estado del botón de apertura manual
  
  // Verificar si se presionó el botón de apertura manual
  if (estadoBoton == LOW)
  {
    lcd.noBlink();
    lcd.print("Sirviendo...");
    alimentar = true; // Iniciar la alimentación manual
    Servo1.write(360); // Girar el servo motor continuo en una dirección
    delay(1500);
    Servo1.write(0); // Detener el servo motor continuo
    lcd.clear();
    lcd.print("Finalizado");
    delay(2000);
    lcd.clear();
    alimentar = false; // Finalizar la alimentación manual
  }
  
  // Verificar si se alcanzó el peso objetivo
  if (scale.get_units() >= pesoControl && !alimentar)
  {
    lcd.noBlink();
    lcd.print("Sirviendo...");
    alimentar = true; // Iniciar la alimentación automática
    Servo1.write(180); // Girar el servo motor continuo en una dirección
    delay(1500);
    Servo1.write(0); // Detener el servo motor continuo
    lcd.clear();
    lcd.print("Finalizado");
    delay(2000);
    lcd.clear();
    alimentar = false; // Finalizar la alimentación automática
  }
  
  // Mostrar el mensaje de bienvenida en el LCD
  lcd.noBlink();
  lcd.setCursor(2, 0);
  lcd.print("Benvenidos a");
  lcd.setCursor(4, 1);
  lcd.print("GammFeed");
  
  delay(3000); // Esperar un segundo antes de actualizar el peso nuevamente
  lcd.clear();
}
