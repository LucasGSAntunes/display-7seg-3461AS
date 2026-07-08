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

## Arquivos

- `teste_display_1234/` — sketch final. Usa SevSeg, mostra "1234" fixo.
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
