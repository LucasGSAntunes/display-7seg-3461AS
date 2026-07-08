/*
 * Scanner de teclado 4x4 com FEEDBACK NO DISPLAY (sem Serial).
 * Display + teclado juntos no Uno (usa todos os 20 GPIO).
 *
 * Display 3461AS-1 (catodo comum) - pinos Arduino (ver README):
 *   Segmentos A..DP -> D2..D9 ; Digitos 1..4 -> D10..D13
 *
 * Teclado 4x4 (8 pinos do conector, esq->dir) -> pinos que sobraram:
 *   conector 1 -> A0
 *   conector 2 -> A1
 *   conector 3 -> A2
 *   conector 4 -> A3
 *   conector 5 -> A4
 *   conector 6 -> A5
 *   conector 7 -> D0
 *   conector 8 -> D1
 *
 * O que faz: descobre qual PAR de pinos do conector cada tecla fecha e mostra
 * no display o par como dois digitos (ex.: tecla que liga conector 3 e 7 -> "37").
 * Sem tecla apertada: display apagado.
 *
 * NOTA: com fio em D0/D1, se o upload falhar, tire esses 2 fios, suba, replugue.
 */

#include "SevSeg.h"

SevSeg sevseg;

const byte kp[8] = {A0, A1, A2, A3, A4, A5, 0, 1};   // conector 1..8

void soltarTodos() {
  for (byte i = 0; i < 8; i++) pinMode(kp[i], INPUT_PULLUP);
}

void setup() {
  byte digitPins[]   = {10, 11, 12, 13};
  byte segmentPins[] = {2, 3, 4, 5, 6, 7, 8, 9};   // A B C D E F G DP
  sevseg.begin(COMMON_CATHODE, 4, digitPins, segmentPins,
               true /*resistorsOnSegments*/, false, false /*leadingZeros*/, true);
  sevseg.setBrightness(90);
  soltarTodos();
  sevseg.blank();
}

void loop() {
  int a = -1, b = -1;

  for (byte i = 0; i < 8 && a < 0; i++) {
    pinMode(kp[i], OUTPUT);
    digitalWrite(kp[i], LOW);
    for (byte j = 0; j < 8; j++) {
      if (j == i) continue;
      if (digitalRead(kp[j]) == LOW) { a = i; b = j; break; }
    }
    pinMode(kp[i], INPUT_PULLUP);
  }

  if (a >= 0) {
    int lo = min(a, b) + 1;
    int hi = max(a, b) + 1;
    sevseg.setNumber(lo * 10 + hi, 0);   // ex.: conector 3 e 7 -> "37"
  } else {
    sevseg.blank();
  }

  sevseg.refreshDisplay();
}
