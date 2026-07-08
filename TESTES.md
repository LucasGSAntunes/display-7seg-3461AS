# Lista de testes - display 3461AS-1 + Arduino Uno

Regra: um teste por vez. Cada teste mostra UMA coisa fixa (sem animacao).
Voce da UMA resposta, eu anoto, seguimos. Nada de reportar duas coisas juntas.

Ja sabemos (pela presenca de resistor):
- Segmentos (com resistor) -> Arduino D2..D9
- Comuns  (sem resistor)   -> Arduino D10..D13
Falta descobrir: qual D10..D13 = qual POSICAO, e qual D2..D9 = qual LETRA.

## Bloco A - mapa das POSICOES (comuns). "8" fixo em 1 comum por vez.
- [ ] A1: so D10 ligado -> em qual posicao (1/2/3/4 da esq) acende o 8?
- [ ] A2: so D11 ligado -> qual posicao?
- [ ] A3: so D12 ligado -> qual posicao?
- [ ] A4: so D13 ligado -> qual posicao?

## Bloco B - mapa das LETRAS (segmentos). 1 segmento fixo em todos os digitos.
- [ ] B1: so D2 -> qual barra? (cima/meio/baixo/sup-esq/sup-dir/inf-esq/inf-dir/ponto)
- [ ] B2: so D3 -> qual barra?
- [ ] B3: so D4 -> qual barra?
- [ ] B4: so D5 -> qual barra?
- [ ] B5: so D6 -> qual barra?
- [ ] B6: so D7 -> qual barra?
- [ ] B7: so D8 -> qual barra?
- [ ] B8: so D9 -> qual barra?

## Bloco C - validacao final
- [ ] C1: montar arrays com os mapas A+B, subir "1234", conferir.

## Resultados (preenchendo conforme anda)
A1 D10 = pos1   OK
A2 D11 = NADA   <-- morto (comum digito 2)
A3 D12 = NADA   <-- morto (comum digito 3)
A4 D13 = pos4   OK
B1 D2  = NADA   <-- morto (segmento A)
B2 D3  = NADA   <-- morto (segmento B)
B3 D4  = C      OK
B4 D5  = D      OK
B5 D6  = E      OK
B6 D7  = F      OK
B7 D8  = G      OK
B8 D9  = ponto  OK (DP)

## PINOUT REAL CORRIGIDO (tabela do outro Claude estava ERRADA na fileira de cima)
Prova: tirar energia do pino 8 apagou digito 3; tirar do pino 9 apagou digito 2.
Fileira de baixo (1-6): CORRETA. Nao mexer.
  pino 1 = E   -> D6 (resistor)
  pino 2 = D   -> D5 (resistor)
  pino 3 = DP  -> D9 (resistor)
  pino 4 = C   -> D4 (resistor)
  pino 5 = G   -> D8 (resistor)
  pino 6 = Dig4-> D13 (direto)
Fileira de cima (7-12) REAL:
  pino 7  = Segmento A ou B -> D2 (COM resistor)   [era ligado direto no D12: ERRADO]
  pino 8  = Digito 3 comum  -> D12 (DIRETO)         [tinha resistor + no D3: ERRADO]
  pino 9  = Digito 2 comum  -> D11 (DIRETO)         [tinha resistor + no D2: ERRADO]
  pino 10 = Segmento F      -> D7 (resistor)  OK
  pino 11 = Segmento B ou A -> D3 (COM resistor)   [era ligado direto no D11: ERRADO]
  pino 12 = Digito 1 comum  -> D10 (direto)   OK
A/B entre pino7/pino11 a confirmar por teste; se trocado, inverter fios D2<->D3.

## DIAGNOSTICO FINAL (antigo, baseado na tabela errada - ver correcao acima)
Mapeamento (onde conecta) esta CORRETO e bate com a tabela.
O problema sao 4 conexoes ABERTAS (fio solto / mau contato):
  D2  -> resistor -> display pino 9  = Segmento A   MORTO
  D3  -> resistor -> display pino 8  = Segmento B   MORTO
  D11 -> direto   -> display pino 11 = Digito 2     MORTO
  D12 -> direto   -> display pino 7  = Digito 3     MORTO
Os 4 sao da fileira de cima do display (pinos 7,8,9,11).
Acao: reencaixar esses 4 (jumper nas duas pontas + resistor de A e B + perna
do display bem no furo). Depois rerodar A2, A3, B1, B2 e o 1234.
