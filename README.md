# Display 7 segmentos 4 dígitos 3461AS-1 + Arduino Uno

Sketch de teste e diagnóstico para o display **3461AS-1** (cátodo comum, 12 pinos,
4 dígitos, sem driver) ligado direto ao Arduino Uno com resistores nos segmentos.

Resultado: mostra **1234** fixo, multiplexado via biblioteca **SevSeg**.

## Pinout REAL verificado (importante)

O pinout que se acha "pesquisando" para o 3461AS-1 estava **errado na fileira de
cima**. Este mapeamento abaixo foi levantado **empiricamente** (segmento por
segmento, dígito por dígito) e validado na bancada. Use este.

Numeração do display (tipo CI): pino 1 embaixo à esquerda, conta 1→6 pela fileira
de baixo (esq→dir); pino 7 fica em cima do pino 6, conta 7→12 pela fileira de cima
da direita para a esquerda.

| Pino display | Função        | Arduino | Resistor 330Ω |
|:---:|:---|:---:|:---:|
| 1  | Segmento E     | D6  | sim |
| 2  | Segmento D     | D5  | sim |
| 3  | Ponto decimal (DP) | D9 | sim |
| 4  | Segmento C     | D4  | sim |
| 5  | Segmento G     | D8  | sim |
| 6  | Dígito 4 (comum) | D13 | não (direto) |
| 7  | Segmento A     | D2  | sim |
| 8  | Dígito 3 (comum) | D12 | não (direto) |
| 9  | Dígito 2 (comum) | D11 | não (direto) |
| 10 | Segmento F     | D7  | sim |
| 11 | Segmento B     | D3  | sim |
| 12 | Dígito 1 (comum) | D10 | não (direto) |

Regra de ouro: **segmento (A-G, DP) sempre passa por resistor; comum de dígito
(1-4) vai direto no Arduino.**

> Observação: a orientação A↔B entre os pinos 7 e 11 foi fechada pelo teste final
> (1234 saiu correto). Se algum dia o "1" sair esquisito, é só inverter os fios
> de D2 e D3.

## Teclado de membrana 4x4 (input)

Teclado 4x4 lido junto com o display. Como display (12 pinos) + teclado (8 pinos)
usam os 20 GPIO do Uno, o teclado ocupa `A0-A5 + D0 + D1`. **D0/D1 são o RX/TX do
USB**: se um upload falhar, tire os 2 fios de D0/D1, suba, e replugue. Sem Serial
disponível (por isso o feedback de diagnóstico foi feito no próprio display).

### Pinout REAL do teclado (levantado na bancada, não confie em foto)

Cada tecla fecha um par (linha, coluna). Descoberto com o scanner
(`teclado_scanner_display`). **Atenção: os conectores 7 e 8 estão invertidos**
em relação à ordem ingênua (coluna 3 = conector 8; coluna 4 = conector 7).

| Papel     | Conector | Arduino |
|:---|:---:|:---:|
| Linha 1   | 1 | A0 |
| Linha 2   | 2 | A1 |
| Linha 3   | 3 | A2 |
| Linha 4   | 4 | A3 |
| Coluna 1  | 5 | A4 |
| Coluna 2  | 6 | A5 |
| Coluna 3  | 8 | D1 |
| Coluna 4  | 7 | D0 |

Layout das teclas (padrão): linhas `1 2 3 A` / `4 5 6 B` / `7 8 9 C` / `* 0 # D`.

Biblioteca: **Keypad** 3.1.1 (`arduino-cli lib install "Keypad"`).

## Mini cofre (senha de 4 dígitos, programável, EEPROM)

`senha_teclado/` — trava com senha de 4 dígitos digitada no teclado 4x4, feedback
no display. Senha **persistente na EEPROM** (sobrevive a desligar) e **programável
pelo próprio teclado**. Senha de fábrica (primeira vez): **`1234`**.

Teclas:
- **`0-9`** — digita (checa automático no 4º dígito): certo → `OPEn`, errado → `Err`.
- **`*`** — limpa a entrada atual.
- **`D`** — apaga o último dígito (backspace).
- **`A`** — trocar senha: `OLd` (digite a atual) → se ok `SEt` (digite a nova) →
  salva e mostra `donE`. Atual errada → `Err`.
- **`#`** — liga/desliga stand-by (display apagado + teclado ignorado).

Notas de implementação:
- 7 segmentos não renderiza "OK" (o "K" fica ilegível), então o feedback usa
  palavras que saem limpas: `OPEn`, `Err`, `donE`, `SEt`, `OLd`.
- EEPROM: byte MAGIC (0x42) no endereço 0 detecta primeiro boot e grava o default;
  senha nos endereços 1-4. `EEPROM.update` só escreve se mudou (poupa desgaste).
- Requer libs **SevSeg** e **Keypad** (EEPROM já vem no core AVR).

## Arquivos

- `teste_display_1234/` — sketch final. Usa SevSeg, mostra "1234" fixo.
- `contador_0_9999/` — conta 0→9999 no display (millis, não-bloqueante).
- `senha_teclado/` — mini cofre: senha 4 dígitos programável + stand-by (ver acima).
- `teclado_display/` — digita número no teclado 4x4 e mostra no display (`*` limpa).
- `teclado_scanner_display/` — scanner do teclado com feedback no display (mapeia
  qual par de conectores cada tecla fecha, sem Serial).
- `teclado_scanner/` — versão do scanner que imprime no Serial (uso standalone,
  display desconectado).
- `diagnostico/diag_static/` — teste estático parametrizado (MODE A = mapa das
  posições/comuns; MODE B = mapa das letras/segmentos). Foi o que destravou o
  mapeamento: acende UMA coisa por vez, sem animação.
- `diagnostico/diag_display/` — varredura animada (versão anterior do diagnóstico).
- `diagnostico/display_off/` — mantém o display apagado (parquear a placa em
  segurança).
- `TESTES.md` — log do processo de diagnóstico, resultados pino a pino e o
  pinout corrigido.

## Como usar (arduino-cli)

```bash
# biblioteca (uma vez)
arduino-cli lib install "SevSeg"

# compilar + subir
arduino-cli compile --fqbn arduino:avr:uno teste_display_1234
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno teste_display_1234
```

Se der `Permission denied` na porta: `sudo chmod a+rw /dev/ttyACM0` (temporário)
ou entre no grupo `dialout` de vez: `sudo usermod -aG dialout $USER` (relogar).

## Lições do debug

1. Não confiar em pinout "pesquisado" para displays genéricos: **verificar na
   bancada**. A tabela inicial estava errada nos 4 pinos de cima e gerou horas de
   confusão (dígitos fantasma, "7" no lugar de "1").
2. Diagnóstico bom = **isolar uma variável por vez**, estático, sem animação.
3. A presença/ausência de resistor já denuncia o papel do pino (segmento vs comum).
4. Teste clássico de bancada: tirar energia de um pino e ver o que apaga revela se
   é comum de dígito (apaga o dígito inteiro) ou segmento (apaga só um traço).
