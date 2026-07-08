/*
 * Scanner BRUTO de teclado de membrana 4x4 (8 pinos no conector).
 * Descobre empiricamente qual PAR de pinos cada tecla conecta,
 * sem assumir quem e linha e quem e coluna.
 *
 * Ligacao (temporaria, para o bring-up do teclado, display desconectado):
 *   Conector do teclado (pino 1..8 da esquerda) -> Arduino D2..D9
 *     conector 1 -> D2
 *     conector 2 -> D3
 *     ...
 *     conector 8 -> D9
 *
 * Como funciona: um pino por vez vira saida LOW, os outros ficam INPUT_PULLUP.
 * Se apertar uma tecla que liga o pino X ao pino Y, o Y (pullup) e puxado a LOW.
 * Ao detectar, imprime o par no Serial (9600 baud).
 *
 * Uso: abrir Serial Monitor a 9600, apertar cada tecla e anotar o par.
 */

const byte kp[8] = {2, 3, 4, 5, 6, 7, 8, 9};   // conector 1..8 -> D2..D9

void soltarTodos() {
  for (byte i = 0; i < 8; i++) pinMode(kp[i], INPUT_PULLUP);
}

int lastA = -1, lastB = -1;

void setup() {
  Serial.begin(9600);
  soltarTodos();
  Serial.println("Scanner de teclado pronto. Aperte uma tecla por vez.");
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

  // normaliza par (menor, maior) em numero de conector 1..8
  if (a >= 0) {
    int lo = min(a, b) + 1;
    int hi = max(a, b) + 1;
    if (lo != lastA || hi != lastB) {   // so imprime quando muda
      Serial.print("TECLA -> conector ");
      Serial.print(lo);
      Serial.print(" + ");
      Serial.println(hi);
      lastA = lo; lastB = hi;
    }
  } else {
    lastA = lastB = -1;                  // soltou: pronto pra proxima
  }

  delay(20);   // debounce simples
}
