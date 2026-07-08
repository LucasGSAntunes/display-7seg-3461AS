/*
 * IDLE: mantem o display 3461AS-1 apagado (catodo comum).
 * Digitos em HIGH (desligados) e segmentos em LOW -> nada conduz.
 * Uso: parquear a placa em seguranca enquanto nao ha resistores no circuito.
 */

const byte seg[8] = {2, 3, 4, 5, 6, 7, 8, 9};
const byte dig[4] = {10, 11, 12, 13};

void setup() {
  for (byte i = 0; i < 8; i++) { pinMode(seg[i], OUTPUT); digitalWrite(seg[i], LOW);  }
  for (byte i = 0; i < 4; i++) { pinMode(dig[i], OUTPUT); digitalWrite(dig[i], HIGH); }
}

void loop() {
  // nada: display permanece apagado
}
