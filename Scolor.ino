#include <Adafruit_NeoPixel.h>

// Pines del sensor TCS3200
#define PIN_OUT 2     // Pin de salida del sensor de color
#define PIN_S0 4      // Control de frecuencia
#define PIN_S1 5
#define PIN_S2 6      // Selección de filtro de color
#define PIN_S3 7

// Pines de la tira LED WS2812
#define PIN_LED_STRIP 8  // Pin de control de la tira de LEDs
#define NUM_LEDS 8  // Número de LEDs en la tira

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN_LED_STRIP, NEO_GRB + NEO_KHZ800);

void setup() {
  // Configurar los pines del sensor TCS3200
  pinMode(PIN_S0, OUTPUT);
  pinMode(PIN_S1, OUTPUT);
  pinMode(PIN_S2, OUTPUT);
  pinMode(PIN_S3, OUTPUT);
  pinMode(PIN_OUT, INPUT);

  // Establecer la frecuencia a alta
  digitalWrite(PIN_S0, HIGH);
  digitalWrite(PIN_S1, LOW);

  // Inicializar la tira LED y el monitor serial
  strip.begin();
  strip.show();  // Apagar todos los LEDs
  Serial.begin(9600);
}

// Función para medir la frecuencia de cada color
int medirColor(int s2, int s3) {
  digitalWrite(PIN_S2, s2);
  digitalWrite(PIN_S3, s3);
  return pulseIn(PIN_OUT, LOW);  // Mide el tiempo en que la señal está en LOW
}

void loop() {
  // Medición de los colores rojo, verde y azul
  int rojo = medirColor(LOW, LOW);     // Filtro rojo
  int azul = medirColor(LOW, HIGH);    // Filtro azul
  int verde = medirColor(HIGH, HIGH);  // Filtro verde

  // Mostrar las lecturas en el Monitor Serial
  Serial.print("Rojo: "); Serial.print(rojo);
  Serial.print(" Azul: "); Serial.print(azul);
  Serial.print(" Verde: "); Serial.println(verde);

  // Determinar el color dominante y encender la tira LED
  if (rojo < azul && rojo < verde) {
    encenderLEDs(strip.Color(255, 0, 0));  // Rojo
  } else if (verde < rojo && verde < azul) {
    encenderLEDs(strip.Color(0, 255, 0));  // Verde
  } else {
    encenderLEDs(strip.Color(0, 0, 255));  // Azul
  }

  delay(500);  // Esperar 500ms antes de la siguiente medición
}

// Función para encender los LEDs con un color específico
void encenderLEDs(uint32_t color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
