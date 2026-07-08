/*
 * Teste ESTATICO parametrizado. Mostra UMA coisa fixa, sem animacao.
 * Catodo comum: DIGITO=LOW liga o digito, SEGMENTO=HIGH liga o traco.
 *
 * MODE 'A' (mapa posicao): acende TODOS os segmentos, so o comum digPin[IDX].
 * MODE 'B' (mapa letra):   acende TODOS os comuns, so o segmento segPin[IDX].
 *
 * >>> Ajustar MODE e IDX a cada passo e reenviar. <<<
 */
#define MODE 'B'   // 'A' = posicoes (comuns) | 'B' = letras (segmentos)
#define IDX  7     // A: 0=D10 1=D11 2=D12 3=D13 | B: 0=D2 ... 7=D9

const byte segPin[8] = {2, 3, 4, 5, 6, 7, 8, 9};
const byte digPin[4] = {10, 11, 12, 13};

void setup() {
  for (byte i = 0; i < 8; i++) pinMode(segPin[i], OUTPUT);
  for (byte i = 0; i < 4; i++) pinMode(digPin[i], OUTPUT);

  if (MODE == 'A') {
    for (byte i = 0; i < 8; i++) digitalWrite(segPin[i], HIGH);   // todos segmentos
    for (byte i = 0; i < 4; i++) digitalWrite(digPin[i], HIGH);   // todos digitos off
    digitalWrite(digPin[IDX], LOW);                               // liga so 1 comum
  } else { // MODE 'B'
    for (byte i = 0; i < 8; i++) digitalWrite(segPin[i], LOW);    // todos segmentos off
    digitalWrite(segPin[IDX], HIGH);                              // liga so 1 segmento
    for (byte i = 0; i < 4; i++) digitalWrite(digPin[i], LOW);    // todos digitos on
  }
}

void loop() { /* estatico */ }
