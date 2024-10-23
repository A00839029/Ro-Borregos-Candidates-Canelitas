#include <Adafruit_NeoPixel.h>

#define PIN_WS2812 7     // Pin donde conectas los LEDs WS2812
#define NUMPIXELS 8      // Número de LEDs en la tira

// Pines del TCS3200
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define OUT 6

Adafruit_NeoPixel pixels(NUMPIXELS, PIN_WS2812, NEO_GRB + NEO_KHZ800);

int rojo = 0, verde = 0, azul = 0;

void setup() {
  // Configurar pines del TCS3200
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT, INPUT);

  // Configurar TCS3200
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Configurar WS2812
  pixels.begin();
  pixels.show();  // Apaga todos los LEDs
  Serial.begin(9600);
}

void loop() {
  detectarColor();  // Detecta el color actual

  // Muestra el color detectado en los LEDs
  for (int i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(rojo, verde, azul));
  }
  pixels.show();  // Actualiza los LEDs

  delay(1000);  // Pausa para el siguiente color
}

void detectarColor() {
  // Lee el color rojo
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  rojo = map(pulseIn(OUT, LOW), 0, 255, 0, 255);

  // Lee el color verde
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  verde = map(pulseIn(OUT, LOW), 0, 255, 0, 255);

  // Lee el color azul
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  azul = map(pulseIn(OUT, LOW), 0, 255, 0, 255);

  Serial.print("Rojo: "); Serial.print(rojo);
  Serial.print(" Verde: "); Serial.print(verde);
  Serial.print(" Azul: "); Serial.println(azul);
}
