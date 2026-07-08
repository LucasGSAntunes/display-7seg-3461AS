/*
 * Teste de fiacao: display 4 digitos 7 segmentos 3461AS-1
 * Placa: Arduino Uno
 * Display: 3461AS-1, CATODO COMUM, 12 pinos, sem driver
 * Objetivo: mostrar "1234" fixo pra validar a fiacao (segmentos + digitos)
 *
 * Biblioteca: SevSeg (Dean Reading) -> instalar com:
 *   arduino-cli lib install "SevSeg"
 *   (ou Library Manager da IDE: procurar "SevSeg")
 *
 * Mapeamento Arduino (lado do micro):
 *   Segmentos: A=D2  B=D3  C=D4  D=D5  E=D6  F=D7  G=D8  DP=D9
 *   Digitos:   D1=D10 D2=D11 D3=D12 D4=D13
 *
 * Pinout REAL do display (verificado na bancada, ver README.md):
 *   pinos display -> A=7 B=11 C=4 D=2 E=1 F=10 G=5 DP=3
 *                    Dig1=12 Dig2=9 Dig3=8 Dig4=6
 *   (a tabela "pesquisada" do 3461AS-1 estava errada na fileira de cima)
 */

#include "SevSeg.h"

SevSeg sevseg;

void setup() {
  const byte numDigits = 4;

  // Pinos dos digitos (comum de cada digito), na ordem D1..D4 (esq -> dir)
  byte digitPins[numDigits] = {10, 11, 12, 13};

  // Pinos dos segmentos, na ordem que a SevSeg espera: A, B, C, D, E, F, G, DP
  byte segmentPins[8] = {2, 3, 4, 5, 6, 7, 8, 9};

  // Sem resistor nos digitos -> resistores (se houver) ficariam nos segmentos.
  // Para este teste sem resistor nenhum, mantemos 'true' (config padrao de fiacao).
  bool resistorsOnSegments = true;

  bool updateWithDelays   = false;  // multiplexacao sem delay bloqueante
  bool leadingZeros       = true;   // garante "1234" preenchendo os 4 digitos
  bool disableDecPoint    = true;   // nao usamos o ponto decimal neste teste

  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins,
               resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);

  sevseg.setBrightness(90);

  // Numero fixo pro teste de fiacao
  sevseg.setNumber(1234, 0);  // 0 casas decimais -> ponto decimal apagado
}

void loop() {
  // Multiplexacao: precisa ser chamada constantemente, sem delay, senao pisca
  sevseg.refreshDisplay();
}
