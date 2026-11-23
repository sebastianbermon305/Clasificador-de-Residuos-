#include <ESP32Servo.h>

Servo servoAprovechable;
Servo servoOrganico;
Servo servoNoAprovechable;

// --- PINES ---
const int pinAprovechable = 27;
const int pinOrganico = 21;
const int pinNoAprovechable = 19;
const int pinTrig = 25;
const int pinEcho = 26;

// --- CONFIGURACIÓN DE MOVIMIENTO ---
const int anguloCerrado = 0;
// Ángulos de apertura independientes para calibrar cada servo físicamente
const int anguloAbiertoAprovechable = 50;   
const int anguloAbiertoOrganico = 50;       
const int anguloAbiertoNoAprovechable = 90; 

const int velocidadServo = 15;
const int tiempoEsperaAbierto = 2000;

// --- VARIABLES DE LÓGICA ---
char servoArmado = ' '; 
unsigned long tiempoInicioEspera = 0;
const unsigned long TIEMPO_MAXIMO_ESPERA = 10000; 
int lecturasConsecutivas = 0;

void setup() {
  Serial.begin(115200);
  
  servoAprovechable.attach(pinAprovechable);
  servoOrganico.attach(pinOrganico);
  servoNoAprovechable.attach(pinNoAprovechable);
  
  servoAprovechable.write(anguloCerrado);
  servoOrganico.write(anguloCerrado);
  servoNoAprovechable.write(anguloCerrado);

  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);
}

long obtenerDistancia() {
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);
  long duracion = pulseIn(pinEcho, HIGH, 30000);
  if (duracion == 0) return 999;
  return duracion * 0.034 / 2;
}

// La función ahora recibe el ángulo exacto al que debe llegar cada servo
void moverServoLento(Servo &miServo, int anguloDestino) {
  for (int pos = anguloCerrado; pos <= anguloDestino; pos += 1) {
    miServo.write(pos);
    delay(velocidadServo);
  }
  delay(tiempoEsperaAbierto);
  for (int pos = anguloDestino; pos >= anguloCerrado; pos -= 1) {
    miServo.write(pos);
    delay(velocidadServo);
  }
  while(Serial.available() > 0) { Serial.read(); } 
}

void loop() {
  if (Serial.available() > 0) {
    char comando = Serial.read();
    if (comando == 'A' || comando == 'O' || comando == 'N') {
      servoArmado = comando;
      tiempoInicioEspera = millis();
      lecturasConsecutivas = 0;
      Serial.println("SISTEMA ARMADO. Esperando mano...");
    }
  }

  if (servoArmado != ' ') {
    long distancia = obtenerDistancia();
    
    Serial.print("Distancia detectada: ");
    Serial.print(distancia);
    Serial.println(" cm");
    
    if (distancia >= 2 && distancia <= 15) {
      lecturasConsecutivas++;
      
      if (lecturasConsecutivas >= 3) {
        Serial.println("¡Mano detectada! Abriendo servo...");
        
        // Se envía el servo correspondiente con su ángulo específico
        if (servoArmado == 'A') moverServoLento(servoAprovechable, anguloAbiertoAprovechable);
        else if (servoArmado == 'O') moverServoLento(servoOrganico, anguloAbiertoOrganico);
        else if (servoArmado == 'N') moverServoLento(servoNoAprovechable, anguloAbiertoNoAprovechable);
        
        servoArmado = ' '; 
        Serial.println("LISTO"); 
      }
    } else {
      lecturasConsecutivas = 0;
      
      if (millis() - tiempoInicioEspera > TIEMPO_MAXIMO_ESPERA) {
        servoArmado = ' ';
        Serial.println("Tiempo agotado. Cancelando...");
        Serial.println("LISTO"); 
      }
    }
    delay(200); 
  }
}