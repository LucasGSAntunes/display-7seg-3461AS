/*
 * Contador 0 -> 9999 no display 3461AS-1 (catodo comum) + Arduino Uno.
 * Multiplexacao nao-bloqueante: refreshDisplay() roda sempre; o incremento
 * do contador usa millis() (sem delay), senao o display pisca.
 *
 * Pinout Arduino (lado do micro):
 *   Segmentos: A=D2 B=D3 C=D4 D=D5 E=D6 F=D7 G=D8 DP=D9
 *   Digitos:   D1=D10 D2=D11 D3=D12 D4=D13
 * Pinout real do display: ver README.md do projeto.
 */

#include "SevSeg.h"

SevSeg sevseg;

const unsigned long PASSO_MS = 100;   // intervalo entre incrementos (100ms = 10/s)
unsigned long ultimoPasso = 0;
int contador = 0;

void setup() {
  byte numDigits           = 4;
  byte digitPins[]         = {10, 11, 12, 13};
  byte segmentPins[]       = {2, 3, 4, 5, 6, 7, 8, 9};   // A B C D E F G DP
  bool resistorsOnSegments = true;
  bool updateWithDelays    = false;
  bool leadingZeros        = false;  // conta "42" e nao "0042"
  bool disableDecPoint     = true;

  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins,
               resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
  sevseg.setNumber(contador, 0);
}

void loop() {
  unsigned long agora = millis();
  if (agora - ultimoPasso >= PASSO_MS) {
    ultimoPasso = agora;
    contador++;
    if (contador > 9999) contador = 0;   // volta pro zero
    sevseg.setNumber(contador, 0);
  }
  sevseg.refreshDisplay();               // precisa rodar constantemente
}
