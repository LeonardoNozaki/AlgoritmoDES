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

//mascara para pegar os 32 bits da direita
unsigned long long int rightMask = 4294967295;

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
  for(int i = 8 - qnt; i < 8; i++){
    unsigned long long int resultado = valor & maskBloco8[i];
    int deslocamento = 7 - i;

    while(deslocamento > 0){
      resultado = resultado / 256;
      deslocamento--;
    }

    if(i == 8 - qnt){
      printf("%02llX ", resultado);
    }
    else{
      printf("%02llX ", resultado);
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
  int pc1[56] = {56, 48, 40, 32, 24, 16, 8,
               0, 57, 49, 41, 33, 25, 17,
              9, 1, 58, 50, 42, 34, 26,
              18, 10, 2, 59, 51, 43, 35,
              62, 54, 46, 38, 30, 22, 14,
               6, 61, 53, 45, 37, 29, 21,
              13, 5, 60, 52, 44, 36, 28,
              20, 12, 4, 27, 19, 11, 3};
             

  unsigned long long int resultado = 0, aux, posMap, pos;
  for(int i = 0; i < 56; i++){
    //Valor i é a posicao que deve colocar o resultado
    //Valor posMap é o bit que vai ser pego da entrada
    posMap = pc1[i];

    aux = chave & maskBit[posMap];
		pos = i + 8;
    if(posMap < pos){
      while(posMap < pos){
        aux = aux / 2;
        posMap++;
      }
      resultado += aux;
    }
    else if(posMap > pos){
      while(posMap > pos){
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
  unsigned long long int C0 = 0, D0 = 0;
  unsigned long long int maskC = 72057593769492480, maskD = 268435455;
  unsigned long long int aux, aux1;
  C0 = chave & maskC;
  D0 = chave & maskD;
  if(round == 1 || round == 2 || round == 9 || round == 16){
		aux = 0;    
		aux = C0 & maskBit[8];
    C0 = C0 - aux;
    C0 = C0 * 2; //Rotacao pra esquerda
    if(aux != 0){
      C0 = C0 + 268435456;
    }
  }
  else{
		aux = 0;
		aux1 = 0;
    aux = C0 & maskBit[8];
    aux1 = C0 & maskBit[9];
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
		aux = 0;	    
		aux = D0 & maskBit[36];
    D0 = D0 - aux;
    D0 = D0 * 2; //Rotacao pra esquerda
    if(aux != 0){
      D0 = D0 + 1;
    }
  }
  else{
		aux = 0;
		aux1 = 0;
    aux = D0 & maskBit[36];
    aux1 = D0 & maskBit[37];
    D0 = D0 - aux;
    D0 = D0 - aux1;
    D0 = D0 * 4; //Rotacao para a esquerda duas vezes
    if(aux != 0){
      D0 = D0 + 2;
    }
    if(aux1 != 0){
      D0 = D0 + 1;
    }
  }
  return C0 + D0;
}
unsigned long long int escalonamentoChavePC2(unsigned long long int chave){
	int pc2[48] = {13, 16, 10, 23, 0, 4,
               2, 27, 14, 5, 20, 9,
               22, 18, 11, 3, 25, 7,
               15, 6, 26, 19, 12, 1,
               40, 51, 30, 36, 46, 54,
               29, 39, 50, 44, 32, 47,
               43, 48, 38, 55, 33, 52,
				 			 45, 41, 49, 35, 28, 31};

  unsigned long long int resultado = 0, aux, posMap, pos;
  for(int i = 0; i < 48; i++){
    //Valor i é a posicao que deve colocar o resultado
    //Valor posMap é o bit que vai ser pego da entrada
    posMap = pc2[i];

    aux = chave & maskBit[posMap+8];
		pos = i + 8;
    if(posMap < pos){
      while(posMap < pos){
        aux = aux / 2;
        posMap++;
      }
      resultado += aux;
    }
    else if(posMap > pos){
      while(posMap > pos){
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

unsigned long long int expancao(int right){
  int map[48] = {31, 0, 1, 2, 3, 4,
							3, 4, 5, 6, 7, 8,
							7, 8, 9, 10, 11, 12,
							11, 12, 13, 14, 15, 16,
							15, 16, 17, 18, 19, 20,
							19, 20, 21, 22, 23, 24,
							23, 24, 25, 26, 27, 28,
							27, 28, 29, 30, 31, 0};


  unsigned long long int resultado = 0;
  unsigned long long int aux = 0;
  int posMap, posInicial, posResultado;

  for(int i = 0; i < 48; i++){
    posMap = map[i];
    aux = right & maskBit[posMap+32];

    posInicial = 31-posMap; //posição do bit do aux
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

int sBox(unsigned long long int entrada);
int permutacaoP(int value);

int main(){
  inicializacaoMaskBit();
  inicializacaoMaskBloco8();
  inicializacaoMaskBloco6();

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

  //L e R
  int right = ip & rightMask;
  int left = ip >> 32;
	printf("divisao %X \n%X\n", left, right);
  //Escrita da entrada da chave
  printf("\nCHAVE\n\n");
  printLongLongToHEX(chave, 64);


	unsigned long long int aux;

  chave = permutacaoChavePC1(chave);
	
	for(int round = 1; round <= 16; round++){
		chave = rotacaoChave(chave, round);
		aux = escalonamentoChavePC2(chave);
		printf("CHAVE DE ROUND %d\n", round);
		printLongLongToHEX(aux, 48);
	}

  printf("\nEXPANCAO\n");

  unsigned long long int exp = 0;
  exp = expancao(right);
  printLongLongToHEX(exp, 64);

  printf("\nXOR\n");
  unsigned long long int xorResult = 0;
  xorResult = exp ^ chave;
  printLongLongToHEX(xorResult, 64);

  //unsigned long long int steste = 211420430091726;
  printf("\nsBOX\n");
  int sboxResult = sBox(xorResult);
  printf("sbox: %X\n", sboxResult);

  printf("\nROUND 1\n");
  int result = permutacaoP(sboxResult);
  result = left ^ result;
  unsigned long long int round1 = right;
  round1 = round1 << 32;
  round1 = round1 | result;

  printLongLongToHEX(round1, 64);
  printf("%X\n", left);
  printf("%X\n", right);
}

int sBox(unsigned long long int entrada){
  int resultado = 0;
  unsigned long long int aux, linha, linhaAux1, linhaAux2, coluna, tableValue;
  aux = linha = linhaAux1 = linhaAux2 = coluna = tableValue = 0;
  unsigned long long int maskColuna = 30;
  for(int i=0; i < 8; i++){
    aux = entrada & maskBloco6[i]; //pega 6 bits da Si ex: 101001
    aux = aux >> (42-(i*6));

    linhaAux1 = aux & maskBit[63];
    linhaAux2 = aux & maskBit[58];
    linhaAux2 = linhaAux2 >> 4;
    linha = linhaAux1 | linhaAux2;

    //printf("linha: %llu\n", linha);
    coluna = aux & maskColuna; //maskcoluna = 30 = 011110
    coluna = coluna >> 1;
    //printf("coluna: %llu\n", coluna);

    switch(i){
      case 0:
        tableValue = S1[linha][coluna];
        break;
      case 1:
        tableValue = S2[linha][coluna];
        break;
      case 2:
        tableValue = S3[linha][coluna];
        break;
      case 3:
        tableValue = S4[linha][coluna];
        break;
      case 4:
        tableValue = S5[linha][coluna];
        break;
      case 5:
        tableValue = S6[linha][coluna];
        break;
      case 6:
        tableValue = S7[linha][coluna];
        break;
      case 7:
        tableValue = S8[linha][coluna];
        break;
    }

    for(int j=0; j<28-(i*4); j++){
      tableValue = tableValue*2;
    }
    resultado = resultado | tableValue;
  }
  return resultado;
}

int permutacaoP(int value){
  int pTable[32] = {16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25};
  int resultado, aux, posMap, posInicial, posResultado;
  resultado = aux = 0;
  for(int i=0; i < 32; i++){
    posMap = pTable[i];
    aux = value & maskBit[posMap+31];

    posInicial = 32-posMap;
    //shift left
    if(posInicial > i){
      while(posInicial > i){
        aux = aux*2;
        posInicial--;
      }
      resultado += aux;
    }//shift right
    else if(posInicial < i){
      while(posInicial < i){
        aux = aux/2;
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
