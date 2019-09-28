#include <stdio.h>
#include "des.h"

//mask de bit serve para pegar um bit especifico
//pegar o antipenultimo bit da direita, aplicar and bit a bit com o valor e a maskBit[61]
//onde a maskBit[61] é = ...000100
unsigned long long int maskBit[64];

//mask de bloco serve para pegar um bloco de 8 bits especifico
//pegar o penultimo bloco, aplicar and bit a bit com o valor e a maskBloco8[1]
//onde a maskBloco8[6] é = ...0001111111100000000
unsigned long long int maskBloco8[8];
unsigned long long int maskBloco6[8];

//Faz a inicializacao da maskBit
void inicializacaoMaskBit(){
  maskBit[63] = 1;
  for(int i = 62; i >= 0; i--){
    maskBit[i] = maskBit[i+1] * 2;
  }
}

//Faz a inicializacao da maskBloco8
void inicializacaoMaskBloco8(){
  maskBloco8[7] = 255;
  for(int i = 6; i >=0; i--){
    maskBloco8[i] = maskBloco8[i+1] * 256;
  }
}

//Faz a inicializacao da maskBloco6
void inicializacaoMaskBloco6(){
  maskBloco6[7] = 63;
  for(int i = 6; i >=0; i--){
    maskBloco6[i] = maskBloco6[i+1] * 64;
  }
}

//Converte um conjunto de valores em hex em uma unica variavel int long long
//Exemplo de entrada: vetor[8] = {69, 6E, 74, 72, 6F, 64, 75, 63}
//Retorno: 696E74726F647563 em uma unica variavel int long long
unsigned long long int toLongLongInt(int vetor[8]){
  unsigned long long int resultado = 0;
  unsigned long long int mult = 1;
  for(int i = 7; i >= 0; i--){
    resultado += vetor[i] * mult;
    mult = mult * 256;
  }
  return resultado;
}

//Printa todos os valores da entrada de forma 1 byte por vez em HEXA
//Exemplo de entrada: valor = 696E74726F647563 e size = 64
//Print: 69 6E 74 72 6F 64 75 63
void printLongLongToHEX(unsigned long long int valor, int size){
  int qnt = (size/8);
  for(int i = 0; i < qnt; i++){
    unsigned long long int resultado = valor & maskBloco8[i];
    int deslocamento = 7 - i;

    while(deslocamento > 0){
      resultado = resultado / 256;
      deslocamento--;
    }

    if(i == 0){
      printf("%02llX", resultado);
    }
    else{
      printf(" %02llX", resultado);
    }
  }
  printf("\n");
}

//Realiza a permutacao inicial
//Exemplo de entrada: entrada = 696E74726F647563
//Retorno: FF4C76D100FF139A em uma unica variavel int long long
unsigned long long int permutacaoInicial(unsigned long long int entrada){
  //Variaveis auxiliares
  unsigned long long int ip = 0;
  int map[] = {4, 0, 5, 1, 6, 2, 7, 3};
  unsigned long long int aux = 0, resultado = 0, posResultado = 0, pos, posLinha;

  //for para interar a coluna (da esquerda pra direita)
  //coluna mais a esquerda = 0
  //coluna mais a direita = 7
  for(int posMask = 0; posMask < 8; posMask++){
    resultado = 0;
    posLinha = map[posMask];
    posResultado = 0;

    //for para interar cada valor da entrada
    for(int i = 0; i < 8; i++){
      //pega um bit especifico da entrada dada a linha e coluna
      aux = entrada & maskBit[(i*8) + posMask];

      if(63-((i*8) + posMask) > posResultado){
        //Precisa deslocar para a direita
        pos = 63-((i*8) + posMask);
        while(pos > posResultado){
          aux = aux/2;
          pos--;
        }
        resultado += aux;
      }
      else if(63-((i*8) + posMask) < posResultado){
        //Precisa deslocar para a esquerda
        pos = 63-((i*8) + posMask);
        while(pos < posResultado){
          aux = aux * 2;
          pos++;
        }
        resultado += aux;
      }
      else{
        //Ja esta na posicao correta
        resultado += aux;
      }

      posResultado++;
    }

    //Coloca o resultado na posicao correta
    for(int pp = posLinha; pp < 7; pp++){
      resultado = resultado * 256;
    }
    //Soma o resultado parcial na variavel de retorno
    ip += resultado;
  }

  return ip;
}

//Realiza a permutacao inicial da chave
//Exemplo de entrada: entrada = 696E74726F647563
//Retorno: FF4C76D100FF139A em uma unica variavel int long long
unsigned long long int permutacaoChavePC1(unsigned long long int chave){
  int map[] ={57,49,41,33,25,17,9,1,
              58,50,42,34,26,18,10,2,
              59,51,43,35,27,19,11,3,
              60,52,44,36,63,55,47,39,
              31,23,15,7,62,54,46,38,
              30,22,14,6,61,53,45,37,
              29,21,13,5,28,20,12,4};

  unsigned long long int resultado = 0, aux, posMap;
  for(int i = 0; i < 56; i++){
    //Valor i é a posicao que deve colocar o resultado
    //Valor posMap é o bit que vai ser pego da entrada
    posMap = map[i];

    aux = chave & maskBit[posMap];
    if(posMap < i){
      while(posMap < i){
        aux = aux / 2;
        posMap++;
      }
      resultado += aux;
    }
    else if(posMap > i){
      while(posMap > i){
        aux = aux * 2;
        posMap--;
      }
      resultado += aux;
    }
    else{
      resultado += aux;
    }
  }
  return resultado;
}

unsigned long long int rotacaoChave(unsigned long long int chave, int round){
  unsigned long long int resultado = 0;
  unsigned long long int C0, D0;
  unsigned long long int maskC = 72057593769492480, maskD = 268435455;
  unsigned long long int aux, aux1;
  C0 = chave & maskC;
  D0 = chave & maskD;
  if(round == 1 || round == 2 || round == 9 || round == 16){
    aux = C0 & maskBit[55];
    C0 = C0 - aux;
    C0 = C0 * 2; //Rotacao pra esquerda
    if(aux != 0){
      C0 = C0 + 268435456;
    }
  }
  else{
    aux = C0 & maskBit[55];
    aux1 = C0 & maskBit[54];
    C0 = C0 - aux;
    C0 = C0 - aux1;
    C0 = C0 * 4; //Rotacao para a esquerda duas vezes
    if(aux != 0){
      C0 = C0 + 536870912;
    }
    if(aux1 != 0){
      C0 = C0 + 268435456;
    }
  }
  if(round == 1 || round == 2 || round == 9 || round == 16){
    aux = D0 & maskBit[27];
    D0 = D0 - aux;
    D0 = D0 * 2; //Rotacao pra esquerda
    if(aux != 0){
      D0 = D0 + 1;
    }
  }
  else{
    aux = D0 & maskBit[27];
    aux1 = D0 & maskBit[26];
    D0 = D0 - aux;
    D0 = D0 - aux1;
    D0 = D0 * 4; //Rotacao para a esquerda duas vezes
    if(aux != 0){
      D0 = D0 + 536870912;
    }
    if(aux1 != 0){
      D0 = D0 + 268435456;
    }
  }
  return C0 + D0;
}
unsigned long long int escalonamentoChavePC2(unsigned long long int chave, int round){


}

unsigned long long int expancao(int right){
  int map[] = {32,1,2,3,4,5, 4,5,6,7,8,9, 8,9,10,11,12,13, 12,13,14,15,16,17, 16,17,18,19,20,21, 20,21,22,23,24,25, 24,25,26,27,28,29, 28,29,30,31,32,1};
  unsigned long long int resultado = 0;
  int aux;
  int posMap, posInicial, posResultado;

  for(int i = 0; i < 48; i++){
    posMap = map[i];
    aux = right & maskBit[posMap+31];

    posInicial = 32-posMap; //posição do bit do aux
    posResultado = 47-i;  //a diferença entre posInicial/posResultado é a qtd de deslocamento do bit

    //shift right
    if(posInicial > posResultado){
      while(posInicial > posResultado){
        aux = aux/2;
        posInicial--;
      }
      resultado += aux;
    }//shift left
    else if(posInicial < posResultado){
      while(posInicial < posResultado){
        aux = aux * 2;
        posInicial++;
      }
      resultado += aux;
    }
    else{
      resultado += aux;
    }
  }
  return resultado;
}

int main(){
  inicializacaoMaskBit();
  inicializacaoMaskBloco8();

  //Precisa ser vetor para fazer a leitura da entrada
  int vetorEntrada[8], vetorChave[8];

  //Leitura da entrada do texto
  for(int i = 0; i < 8; i++){
    scanf("%x", &vetorEntrada[i]);
  }
  unsigned long long int entrada = toLongLongInt(vetorEntrada);

  //Leitura da entrada da chave
  for(int i = 0; i < 8; i++){
    scanf("%x", &vetorChave[i]);
  }
  unsigned long long int chave = toLongLongInt(vetorChave);

  //Escrita da entrada do texto
  printf("PLAIN TEXT\n");
  printLongLongToHEX(entrada, 64);

  //Escrita da entrada depois da Permutacao Inicial
  printf("\nIP\n");
  unsigned long long int ip = permutacaoInicial(entrada);
  printLongLongToHEX(ip, 64);

  //Escrita da entrada da chave
  printf("\nCHAVE\n");
  printLongLongToHEX(chave, 64);

	int round = 1;
  chave = permutacaoChavePC1(chave);
  printLongLongToHEX(chave, 56);
  printf("\nCHAVE\n");
  chave = rotacaoChave(chave, round);
  printLongLongToHEX(chave, 56);

  printf("\nEXPANCAO\n");
  int right;
  unsigned long long int exp = 0;
  exp = expancao(right);
  printf("%ul\n", exp);
  printLongLongToHEX(exp, 64);

}

int sBox(unsigned long long int entrada){
  int aux, linha, coluna, resultado;
  aux = linha = coluna = resultado = 0;
  for(int i=0; i < 8; i++){
    aux = entrada & maskBloco6[i]; //pega 6 bits da Si ex: 101001
    linha = aux & maskBit[63]; //pega o primeiro bit mais significativo ex: 000001
    linha += (aux & maskBit[58])/16; //pega o ultimo bit e desloca para segunda posição ex: 101001 -> 000010
    coluna += aux & 30; //30 = 011110

    switch(i){
      case 0:
        resultado += S1[linha][coluna];
        break;
      case 1:
        resultado += S2[linha][coluna];
        break;
      case 2:
        resultado += S3[linha][coluna];
        break;
      case 3:
        resultado += S4[linha][coluna];
        break;
      case 4:
        resultado += S5[linha][coluna];
        break;
      case 5:
        resultado += S6[linha][coluna];
        break;
      case 6:
        resultado += S7[linha][coluna];
        break;
      case 7:
        resultado += S8[linha][coluna];
        break;
    }
  }

  return resultado;
}
