#include "RoboVariables.h"
#include "RoboFunciones.h"
#include <Adafruit_NeoPixel.h>

// Pines del sensor TCS3200
#define PIN_OUT 2     // Pin de salida del sensor de color
#define PIN_S0 4      // Control de frecuencia
#define PIN_S1 5
#define PIN_S2 6      // Selección de filtro de color
#define PIN_S3 7

// Pines de la tira LED WS2812
#define PIN_LED_STRIP 8  // Pin de control de la tira de LEDs
#define NUM_LEDS 8       // Número de LEDs en la tira

Adafruit_NeoPixel strip(NUM_LEDS, PIN_LED_STRIP, NEO_GRB + NEO_KHZ800);

/**
 * Configuración inicial de pines y sensores.
 * Inicializa la comunicación serial, sensores ultrasónicos,
 * pinza del robot y la tira LED WS2812. Establece la frecuencia
 * de funcionamiento del sensor de color.
 */
void setup() {
    Serial.begin(9600);

    // Configuración de los sensores ultrasónicos
    pinMode(Trig1, OUTPUT);
    pinMode(Echo1, INPUT);
    pinMode(Trig2, OUTPUT);
    pinMode(Echo2, INPUT);
    pinMode(Trig3, OUTPUT);
    pinMode(Echo3, INPUT);
    pinMode(Trig4, OUTPUT);
    pinMode(Echo4, INPUT);

    // Configuración de la pinza
    gripperServo.attach(servoPin);

    // Inicializar la tira LED
    strip.begin();
    strip.show();

    // Configuración del sensor de color
    pinMode(PIN_S0, OUTPUT);
    pinMode(PIN_S1, OUTPUT);
    pinMode(PIN_S2, OUTPUT);
    pinMode(PIN_S3, OUTPUT);
    pinMode(PIN_OUT, INPUT);

    // Establecer frecuencia alta para el sensor de color
    digitalWrite(PIN_S0, HIGH);
    digitalWrite(PIN_S1, LOW);

    // Iniciar movimiento del robot
    MoverAdelante(NormalSpeed);
    delay(400);
}

/**
 * Mide la frecuencia de salida del sensor de color TCS3200 según
 * el filtro de color seleccionado por S2 y S3.
 * @param s2 Estado de S2.
 * @param s3 Estado de S3.
 * @return Frecuencia de salida medida.
 */
unsigned long medirColor(bool s2, bool s3) {
    digitalWrite(PIN_S2, s2);
    digitalWrite(PIN_S3, s3);
    return pulseIn(PIN_OUT, LOW, 100000);
}

/**
 * Enciende todos los LEDs de la tira con el color especificado.
 * @param color Color en formato RGB para la tira LED.
 */
void encenderLEDs(uint32_t color) {
    for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, color);
    }
    strip.show();
}

/**
 * Lógica de navegación en laberinto. El robot se mueve hacia adelante
 * si los sensores detectan que el camino está libre; si detecta obstáculos,
 * retrocede y gira en función de la distancia detectada por los sensores laterales.
 */
void Maze() {
    if (CalcularDistancia(Trig2, Echo2) > DisFront && 
        CalcularDistancia(Trig3, Echo3) > DisSides && 
        CalcularDistancia(Trig1, Echo1) > DisSides) {
        MoverAdelante(NormalSpeed);
    } else if (CalcularDistancia(Trig2, Echo2) < DisFront && 
               CalcularDistancia(Trig3, Echo3) < DisSides && 
               CalcularDistancia(Trig1, Echo1) < DisSides) {
        MoverAtras(NormalSpeed);
        delay(Wait + 200);
        if (CalcularDistancia(Trig1, Echo1) > CalcularDistancia(Trig3, Echo3)) {
            GirarIzquierda(NormalSpeed);
        } else {
            GirarDerecha(NormalSpeed);
        }
        delay(WaitG);
    } else if (CalcularDistancia(Trig2, Echo2) < DisFront && 
               CalcularDistancia(Trig3, Echo3) < DisSides) {
        GirarIzquierda(NormalSpeed);
        delay(WaitG);
    } else if (CalcularDistancia(Trig2, Echo2) < DisFront && 
               CalcularDistancia(Trig1, Echo1) < DisSides) {
        GirarDerecha(NormalSpeed);
        delay(WaitG);
    }
}

/**
 * Bucle principal que realiza las siguientes acciones:
 * 1. Detecta si el robot está cerca de una pelota y, si es así, la recoge con la pinza.
 * 2. Navega por un laberinto en función de las distancias detectadas.
 * 3. Mide el color dominante en el entorno (rojo, verde o azul) y enciende la tira LED
 *    con el color correspondiente.
 */
void loop() {  
    if (CalcularDistancia(Trig4, Echo4) <= DisPelota && flag == false) {
        DetenerMotores();
        AbrirPinza();
        MoverAdelante(NormalSpeed);
        delay(200);
        DetenerMotores();
        CerrarPinza();
        delay(200);
        flag = true;
    } else {
        Maze();
    }

    // Medición y detección del color dominante
    unsigned long rojo = medirColor(LOW, LOW);     // Filtro rojo
    unsigned long azul = medirColor(LOW, HIGH);    // Filtro azul
    unsigned long verde = medirColor(HIGH, HIGH);  // Filtro verde

    if (rojo == 0) rojo = 1;
    if (verde == 0) verde = 1;
    if (azul == 0) azul = 1;

    Serial.print("Rojo: "); Serial.print(rojo);
    Serial.print(" Verde: "); Serial.print(verde);
    Serial.print(" Azul: "); Serial.println(azul);

    if (rojo < verde && rojo < azul) {
        encenderLEDs(strip.Color(255, 0, 0));  // Rojo
    } else if (verde < rojo && verde < azul) {
        encenderLEDs(strip.Color(0, 255, 0));  // Verde
    } else {
        encenderLEDs(strip.Color(0, 0, 255));  // Azul
    }

    delay(500);  // Espera antes de la siguiente medición
}
