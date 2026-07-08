/*
 * Mini cofre: senha de 4 digitos no teclado 4x4, feedback no display 7 seg.
 * Senha PERSISTENTE na EEPROM (sobrevive a desligar). PROGRAMAVEL pelo teclado.
 *
 * Uso:
 *   - Digite 4 numeros -> checa no 4o digito:
 *       certo  -> "OPEn"
 *       errado -> "Err"
 *   - '*'  -> limpa a entrada atual
 *   - 'D'  -> apaga o ultimo digito (backspace)
 *   - '#'  -> liga/desliga stand-by (display apagado + teclado ignorado)
 *   - 'A'  -> trocar senha: "OLd" (digite a atual) -> se ok "SEt" (digite a nova)
 *             -> salva e mostra "donE". Atual errada -> "Err".
 *   - Senha de fabrica (1a vez): 1234.
 *
 * Display (catodo comum): Segmentos A..DP -> D2..D9 ; Digitos 1..4 -> D10..D13
 * Teclado 4x4: Linhas conector 1-4 -> A0..A3 ; Colunas conector 5,6,8,7 -> A4,A5,D1,D0
 * (D0/D1 = USB: se upload falhar, tire os 2 fios, suba, replugue.)
 */

#include <EEPROM.h>
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
byte rowPins[ROWS] = {A0, A1, A2, A3};
byte colPins[COLS] = {A4, A5, 1, 0};
Keypad teclado = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// EEPROM
const int  EE_MAGIC = 0;      // byte de "ja inicializado"
const int  EE_PASS  = 1;      // 4 bytes da senha
const byte MAGIC    = 0x42;

char senha[5];   // 4 digitos + '\0'
char buf[5];     // entrada atual
byte n = 0;

enum Estado { TRAVADO, VERIF_ANTIGA, NOVA };
Estado estado = TRAVADO;

bool standby = false;   // '#' liga/desliga (display apagado + input travado)

void carregarSenha() {
  if (EEPROM.read(EE_MAGIC) != MAGIC) {          // primeira vez: default 1234
    const char* def = "1234";
    for (byte i = 0; i < 4; i++) EEPROM.update(EE_PASS + i, def[i]);
    EEPROM.update(EE_MAGIC, MAGIC);
  }
  for (byte i = 0; i < 4; i++) senha[i] = EEPROM.read(EE_PASS + i);
  senha[4] = '\0';
}

void salvarSenha(const char* nova) {
  for (byte i = 0; i < 4; i++) { EEPROM.update(EE_PASS + i, nova[i]); senha[i] = nova[i]; }
  senha[4] = '\0';
}

// mostra um texto fixo por 'ms' mantendo a multiplexacao viva
void mostrarPor(const char* txt, unsigned long ms) {
  sevseg.setChars(txt);
  unsigned long t = millis();
  while (millis() - t < ms) sevseg.refreshDisplay();
}

void resetEntrada() { n = 0; }

void setup() {
  byte digitPins[]   = {10, 11, 12, 13};
  byte segmentPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
  sevseg.begin(COMMON_CATHODE, 4, digitPins, segmentPins, true, false, false, true);
  sevseg.setBrightness(90);
  carregarSenha();
}

void processar4() {
  buf[4] = '\0';
  bool ok = (strncmp(buf, senha, 4) == 0);

  switch (estado) {
    case TRAVADO:
      mostrarPor(ok ? "OPEn" : "Err ", 1500);
      estado = TRAVADO;
      break;

    case VERIF_ANTIGA:
      if (ok) { mostrarPor("SEt ", 1200); estado = NOVA; }
      else    { mostrarPor("Err ", 1500); estado = TRAVADO; }
      break;

    case NOVA:
      salvarSenha(buf);
      mostrarPor("donE", 1500);
      estado = TRAVADO;
      break;
  }
  resetEntrada();
}

void loop() {
  char k = teclado.getKey();

  if (k == '#') {                 // liga/desliga stand-by
    standby = !standby;
    resetEntrada();
    estado = TRAVADO;
    k = 0;
  }

  if (standby) {                  // dormindo: display apagado, input ignorado
    sevseg.blank();
    sevseg.refreshDisplay();
    return;
  }

  if (k) {
    if (k >= '0' && k <= '9') {
      if (n < 4) buf[n++] = k;
      if (n == 4) processar4();
    } else if (k == '*') {              // limpa / cancela
      resetEntrada();
      estado = TRAVADO;
    } else if (k == 'D') {              // apaga o ultimo digito (backspace)
      if (n > 0) n--;
    } else if (k == 'A') {              // iniciar troca de senha
      resetEntrada();
      estado = VERIF_ANTIGA;
      mostrarPor("OLd ", 1200);
    }
    // 'B', 'C': reservadas
  }

  // desenho da entrada: "----" com os digitos preenchendo da esquerda
  char disp[5] = "----";
  for (byte i = 0; i < n; i++) disp[i] = buf[i];
  disp[4] = '\0';
  sevseg.setChars(disp);
  sevseg.refreshDisplay();
}
