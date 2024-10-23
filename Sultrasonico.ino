#include <Servo.h>

// Pines del Puente H TB6612FNG
const int STBY = 10;
const int PWMA = 5;   
const int AIN1 = 7;
const int AIN2 = 6;
const int PWMB = 9;   
const int BIN1 = 8;
const int BIN2 = 11;

// Pines del Sensor Ultrasonico HC-SR04
const int trigPin = 3;
const int echoPin = 4;

// Servo para dirección adicional (opcional)
Servo servoDireccion;
const int pinServo = 12;

// Distancia mínima para detectar un obstáculo (en cm)
const int distanciaMinima = 15;

void setup() {
  // Configuración de pines del puente H
  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // Configuración del sensor ultrasónico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Inicialización del servo (opcional)
  servoDireccion.attach(pinServo);
  servoDireccion.write(90);  // Posición neutra

  Serial.begin(9600);  // Inicia el monitor serial
}

void loop() {
  long distancia = detectarObstaculo();  // Mide la distancia

  if (distancia < distanciaMinima) {
    detenerMotores();   // Detiene si hay pared
    girarDerecha();     // Gira para buscar espacio libre
    delay(500);         // Espera mientras gira
  } else {
    moverAdelante(255); // Continúa moviéndose hacia adelante
  }

  delay(100);  // Pequeña pausa antes de la siguiente lectura
}

// Función para mover el robot hacia adelante
void moverAdelante(int velocidad) {
  digitalWrite(STBY, HIGH);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, velocidad);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, velocidad);
}

// Función para detener los motores
void detenerMotores() {
  digitalWrite(STBY, LOW);
}

// Función para girar hacia la derecha
void girarDerecha() {
  digitalWrite(STBY, HIGH);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 150);

  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 150);
}

// Función para medir la distancia con el sensor ultrasónico
long detectarObstaculo() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH);
  long distancia = (duracion / 2) / 29.1;
  Serial.print("Distancia: ");
  Serial.println(distancia);
  return distancia;
}
