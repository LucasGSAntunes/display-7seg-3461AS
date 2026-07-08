/*
 * DIAGNOSTICO v3 (empirico): descobre a fiacao real, SEM confiar em pinout.
 * Catodo comum: acende com DIGITO=LOW e SEGMENTO=HIGH.
 *
 * O que ja sabemos pela presenca de resistor:
 *   Segmentos (com resistor) -> Arduino D2,D3,D4,D5,D6,D7,D8,D9
 *   Comuns  (sem resistor)   -> Arduino D10,D11,D12,D13
 * O que NAO sabemos: qual D10..D13 = qual POSICAO fisica,
 *                    e qual D2..D9 = qual LETRA de segmento.
 *
 * PARTE 1 - POSICAO DOS COMUNS (2.5s por passo, blank 1s):
 *   Acende TODOS os segmentos (8) e habilita UM comum por vez.
 *   Passo 1=D10 | 2=D11 | 3=D12 | 4=D13
 *   Anote: em cada passo, qual POSICAO fisica (esq->dir = 1,2,3,4) mostra
 *          o "8" completo. Se acender em 2 posicoes ou com fantasma, anote.
 *
 * PARTE 2 - LETRA DOS SEGMENTOS (2.5s por passo):
 *   Liga os 4 comuns e acende UM segmento por vez (D2..D9 em ordem).
 *   Anote qual BARRA fisica acende: cima / meio / baixo /
 *   canto sup-esq / sup-dir / inf-esq / inf-dir / ponto.
 *
 * (blank 2.5s = reinicia)
 */

const byte segPin[8] = {2, 3, 4, 5, 6, 7, 8, 9};
const byte digPin[4] = {10, 11, 12, 13};

void allSeg(bool on) { for (byte i = 0; i < 8; i++) digitalWrite(segPin[i], on ? HIGH : LOW); }
void allDigOff()     { for (byte i = 0; i < 4; i++) digitalWrite(digPin[i], HIGH); }
void allDigOn()      { for (byte i = 0; i < 4; i++) digitalWrite(digPin[i], LOW); }
void blank()         { allSeg(false); allDigOff(); }

void setup() {
  Serial.begin(9600);
  for (byte i = 0; i < 8; i++) pinMode(segPin[i], OUTPUT);
  for (byte i = 0; i < 4; i++) pinMode(digPin[i], OUTPUT);
  blank();
}

void loop() {
  Serial.println("== PARTE 1: posicao dos COMUNS (8 aceso, 1 comum por vez) ==");
  allSeg(true);
  for (byte i = 0; i < 4; i++) {
    allDigOff();
    digitalWrite(digPin[i], LOW);
    Serial.print("  passo "); Serial.print(i + 1);
    Serial.print(" -> comum em D"); Serial.println(digPin[i]);
    delay(2500);
    blank();
    delay(1000);
  }

  blank();
  delay(1500);

  Serial.println("== PARTE 2: letra dos SEGMENTOS (4 comuns ON, 1 seg por vez) ==");
  allDigOn();
  for (byte j = 0; j < 8; j++) {
    allSeg(false);
    digitalWrite(segPin[j], HIGH);
    Serial.print("  passo "); Serial.print(j + 1);
    Serial.print(" -> segmento em D"); Serial.println(segPin[j]);
    delay(2500);
  }

  blank();
  delay(2500);
}
