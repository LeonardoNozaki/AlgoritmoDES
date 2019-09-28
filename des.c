#include <stdio.h>

//mask de bit serve para pegar um bit especifico
//pegar o antipenultimo bit da direita, aplicar and bit a bit com o valor e a maskBit[61]
//onde a maskBit[61] é = ...000100
unsigned long long int maskBit[64];

//mask de bloco serve para pegar um bloco de 8 bits especifico
//pegar o penultimo bloco, aplicar and bit a bit com o valor e a maskBloco[1]
//onde a maskBloco[6] é = ...0001111111100000000
unsigned long long int maskBloco[8];

//Faz a inicializacao da maskBit
void inicializacaoMaskBit(){
  maskBit[63] = 1;
  for(int i = 62; i >= 0; i--){
    maskBit[i] = maskBit[i+1] * 2;
  }
}

//Faz a inicializacao da maskBloco
void inicializacaoMaskBloco(){
  maskBloco[7] = 255;
  for(int i = 6; i >=0; i--){
    maskBloco[i] = maskBloco[i+1] * 256;
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
    unsigned long long int resultado = valor & maskBloco[i];
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
  inicializacaoMaskBloco();

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

  printf("\nEXPANCAO\n");
  int right;
  unsigned long long int exp = 0;
  exp = expancao(right);
  printf("%ul\n", exp);
  printLongLongToHEX(exp, 64);

}
