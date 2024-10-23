#include <Servo.h>

// Pines del Puente H TB6612FNG
const int STBY = 10;  // Pin de standby
const int PWMA = 5;   // PWM del Motor A
const int AIN1 = 7;
const int AIN2 = 6;
const int PWMB = 9;   // PWM del Motor B
const int BIN1 = 8;
const int BIN2 = 11;

// Pines del Encoder LM393
const int pinEncoder = 2;
volatile int contadorPulsos = 0;

// Pines del Sensor Ultrasonico HC-SR04
const int trigPin = 3;
const int echoPin = 4;

// Servo SG90
Servo servoDireccion;
const int pinServo = 12;

void setup() {
  // Configuración de pines del puente H
  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // Inicialización del Servo
  servoDireccion.attach(pinServo);
  servoDireccion.write(90);

  // Configuración del Encoder LM393
  pinMode(pinEncoder, INPUT);
  attachInterrupt(digitalPinToInterrupt(pinEncoder), contarPulsos, RISING);

  // Configuración del sensor ultrasonico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  moverAdelante(255);  // Mover el robot hacia adelante
  delay(1000);          // Durante 1 segundo

  if (detectarObstaculo() < 10) {  // Si detecta un obstáculo cerca
    detenerMotores();
    servoDireccion.write(45);  // Gira el servo para cambiar dirección
    delay(500);
    moverAdelante(255);  // Continúa moviéndose
  }

  mostrarVelocidad();
  delay(1000);
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

// Función para medir la distancia con el sensor ultrasonico
long detectarObstaculo() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duracion = pulseIn(echoPin, HIGH);
  long distancia = (duracion / 2) / 29.1;
  return distancia;
}

// Función para contar los pulsos del encoder
void contarPulsos() {
  contadorPulsos++;
}

// Función para mostrar la velocidad
void mostrarVelocidad() {
  int rpm = (contadorPulsos / 20) * 60;
  contadorPulsos = 0;
  Serial.print("Velocidad: ");
  Serial.print(rpm);
  Serial.println(" RPM");
}
