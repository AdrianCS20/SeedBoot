#include <Arduino.h>
#include <ESP32Servo.h>
#include <SPI.h>
#include <MFRC522.h>

// Pines para los motores
#define motor1_pin1 12  // Conectado al positivo del motor 1
#define motor1_pin2 14  // Conectado al negativo del motor 1
#define motor2_pin1 13  // Conectado al positivo del motor 2
#define motor2_pin2 15  // Conectado al negativo del motor 2

// Pines para el RFID-RC522
const int SS_PIN = 21;
const int RST_PIN = 22;

MFRC522 rfid(SS_PIN, RST_PIN);  // Crear un objeto MFRC522

// Configuración del servo
Servo myServo;
const int servoPin = 4;
int currentServoAngle = 0;  // Variable para rastrear el ángulo actual del servo

void setup() {
  // Configuración de los pines de los motores como salida
  pinMode(motor1_pin1, OUTPUT);
  pinMode(motor1_pin2, OUTPUT);
  pinMode(motor2_pin1, OUTPUT);
  pinMode(motor2_pin2, OUTPUT);

  // Configuración del servo
  myServo.attach(servoPin);  // Vincular el pin GPIO al servo
  myServo.write(currentServoAngle);  // Asegurar que el servo esté en la posición inicial (0 grados)

  // Iniciar comunicación SPI
  SPI.begin();
  // Iniciar el sensor RFID
  rfid.PCD_Init();
  Serial.begin(9600);  

  Serial.println("Sistema iniciado. Esperando tarjeta RFID...");
}

void loop() {
  // Revisar si se ha detectado una tarjeta RFID
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    Serial.println("Tarjeta detectada. Deteniendo motores y servo.");
    
    // Detener los motores
    analogWrite(motor1_pin1, 0);
    analogWrite(motor1_pin2, 0);
    analogWrite(motor2_pin1, 0);
    analogWrite(motor2_pin2, 0);
    
    // Detener el servo
    myServo.detach();
    
    // Parar la lectura de la tarjeta
    rfid.PICC_HaltA();
    
    // Mantener todo detenido
    while (true) {
      delay(100); 
    }
  } else {
    // Configurar la velocidad de los motores
    int velocidad = 100; //Velocidad

    // Controlar los motores a velocidad reducida
    analogWrite(motor1_pin1, velocidad);
    analogWrite(motor1_pin2, 0);
    analogWrite(motor2_pin1, velocidad);
    analogWrite(motor2_pin2, 0);

    // Continuar moviendo el servo si no se detecta tarjeta
    currentServoAngle += 30;
    if (currentServoAngle >= 360) {
      currentServoAngle = 0;  
    }
    myServo.write(currentServoAngle);  // Mover el servo al nuevo ángulo
    delay(1000);  // Esperar 1 segundo antes de repetir el ciclo
  }
}
