/*
 * Entrada por teclado 4x4 -> numero no display 7 seg 3461AS-1.
 * Digita nos numeros e aparece no display (max 4 digitos, rola pra esquerda).
 *   '*' = limpa (zera)
 *   '#' , 'A'..'D' = sem funcao por enquanto
 *
 * Display (catodo comum) - pinos Arduino (ver README):
 *   Segmentos A..DP -> D2..D9 ; Digitos 1..4 -> D10..D13
 *
 * Teclado 4x4 - mapeamento REAL levantado na bancada (scanner):
 *   Linhas   : conector 1,2,3,4 -> A0,A1,A2,A3
 *   Colunas  : conector 5,6,8,7 -> A4,A5,D1,D0   (coluna 3=conn8, coluna 4=conn7)
 *
 * NOTA: colunas usam D0/D1 (USB). Se o upload falhar, tire os fios de D0/D1,
 *       suba, e replugue.
 */

#include "SevSeg.h"
#include <Keypad.h>

SevSeg sevseg;

const byte ROWS = 4, COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {A0, A1, A2, A3};   // linha 1..4
byte colPins[COLS] = {A4, A5, 1, 0};     // coluna 1..4 (C3=D1, C4=D0)
Keypad teclado = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

long valor = 0;
bool vazio = true;

void setup() {
  byte digitPins[]   = {10, 11, 12, 13};
  byte segmentPins[] = {2, 3, 4, 5, 6, 7, 8, 9};   // A B C D E F G DP
  sevseg.begin(COMMON_CATHODE, 4, digitPins, segmentPins,
               true /*resistorsOnSegments*/, false, false /*leadingZeros*/, true);
  sevseg.setBrightness(90);
  sevseg.blank();
}

void loop() {
  char k = teclado.getKey();
  if (k) {
    if (k >= '0' && k <= '9') {
      valor = (valor * 10 + (k - '0')) % 10000;   // 4 digitos, rola pra esquerda
      vazio = false;
    } else if (k == '*') {
      valor = 0;
      vazio = true;
    }
    // '#', 'A'..'D': reservadas
  }

  if (vazio) sevseg.blank();
  else       sevseg.setNumber(valor, 0);

  sevseg.refreshDisplay();   // precisa rodar constantemente
}
