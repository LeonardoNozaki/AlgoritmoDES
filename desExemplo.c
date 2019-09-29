/* 
	Data Encryption Standard (DES) Implementation
	
	Version: 03-12-2006
	Authors:
					Paulo Caeiro - ei3737
					 Nuno Rosado - ei3730
				 Hugo Laibaças - ei4444
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gmp.h>

void convertHexadecimalToBinary(char list[]);
void convertDecimalToBinary(char list[]);
void generateCD();
void sendBinaryToF(int x, int blockNumber);
void invert_key_order();
 
char charKey64[64];
int binKey64[64];
int binKey56[56];
int binKey48[48];

int binBlock[64];

int pc1[56] = {56, 48, 40, 32, 24, 16, 8,
               0, 57, 49, 41, 33, 25, 17,
              9, 1, 58, 50, 42, 34, 26,
              18, 10, 2, 59, 51, 43, 35,
              62, 54, 46, 38, 30, 22, 14,
               6, 61, 53, 45, 37, 29, 21,
              13, 5, 60, 52, 44, 36, 28,
              20, 12, 4, 27, 19, 11, 3};
             
int pc2[48] = {13, 16, 10, 23, 0, 4,
               2, 27, 14, 5, 20, 9,
               22, 18, 11, 3, 25, 7,
               15, 6, 26, 19, 12, 1,
               40, 51, 30, 36, 46, 54,
               29, 39, 50, 44, 32, 47,
               43, 48, 38, 55, 33, 52,
				 			 45, 41, 49, 35, 28, 31};

int ip[64] = {57, 49, 41, 33, 25, 17, 9, 1,
              59, 51, 43, 35, 27, 19, 11, 3,
            	61, 53, 45, 37, 29, 21, 13, 5,
            	63, 55, 47, 39, 31, 23, 15, 7,
            	56, 48, 40, 32, 24, 16, 8, 0,
            	58, 50, 42, 34, 26, 18, 10, 2,
            	60, 52, 44, 36, 28, 20, 12, 4,
          		62, 54, 46, 38, 30, 22, 14, 6};
          		
int eBitSelection[48] = {31, 0, 1, 2, 3, 4,
                  			3, 4, 5, 6, 7, 8,
                  			7, 8, 9, 10, 11, 12,
                 				11, 12, 13, 14, 15, 16,
                 				15, 16, 17, 18, 19, 20,
                 				19, 20, 21, 22, 23, 24,
                 				23, 24, 25, 26, 27, 28,
                				27, 28, 29, 30, 31, 0};

int S1[64] = {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
      				0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
      				4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
     					15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13};

int S2[64] = {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
      				3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
      				0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
     					13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9};
     					
int S3[64] = {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
     					13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
     					13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
      				1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12};
      				
int S4[64] = {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
     					13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
     					10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
      				3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14};
      				
int S5[64] = {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
     					14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
      				4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
     					11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3};
     					
int S6[64] = {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
     					10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
      				9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
      				4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13};
      				
int S7[64] = {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
     					13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
      				1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
      				6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12};
      				
int S8[64] = {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
      				1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
      				7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
      				2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11};

int P[32] = {15, 6, 19, 20,
             28, 11, 27, 16,
             0, 14, 22, 25,
             4, 17, 30, 9,
             1, 7, 23, 13,
             31, 26, 2, 8,
             18, 12, 29, 5,
             21, 10, 3, 24};
             
int ip_1[64] = {39, 7, 47, 15, 55, 23, 63, 31,
            		38, 6, 46, 14, 54, 22, 62, 30,
            		37, 5, 45, 13, 53, 21, 61, 29,
            		36, 4, 44, 12, 52, 20, 60, 28,
            		35, 3, 43, 11, 51, 19, 59, 27,
            		34, 2, 42, 10, 50, 18, 58, 26,
            		33, 1, 41, 9, 49, 17, 57, 25,
            		32, 0, 40, 8, 48, 16, 56, 24};

int ipM[64];

int C1D1[56];
int C2D2[56];
int C3D3[56];
int C4D4[56];
int C5D5[56];
int C6D6[56];
int C7D7[56];
int C8D8[56];
int C9D9[56];
int C10D10[56];
int C11D11[56];
int C12D12[56];
int C13D13[56];
int C14D14[56];
int C15D15[56];
int C16D16[56];

int k1[48];
int k2[48];
int k3[48];
int k4[48];
int k5[48];
int k6[48];
int k7[48];
int k8[48];
int k9[48];
int k10[48];
int k11[48];
int k12[48];
int k13[48];
int k14[48];
int k15[48];
int k16[48];

int L1[32], R1[32];
int L2[32], R2[32];
int L3[32], R3[32];
int L4[32], R4[32];
int L5[32], R5[32];
int L6[32], R6[32];
int L7[32], R7[32];
int L8[32], R8[32];
int L9[32], R9[32];
int L10[32], R10[32];
int L11[32], R11[32];
int L12[32], R12[32];
int L13[32], R13[32];
int L14[32], R14[32];
int L15[32], R15[32];
int L16[32], R16[32];

int S[32];
int cipherBin[64];
int cipherMessageAscii[8];
char cipherMessageChar[8];

main (int argc, char *argv[]) {
  
 printf("\n\n::::::::::::::::::::::::::::.. DES - Data Enctyption Standard ..:::::::::::::::::::::::::::::\n", "");
 
 int v, cv, i;
 int row, column, subBlockStart;
 char temp_block[64], temp_key[16];
 int expand_PrevR[48], XOR[48], F[32];

 FILE *f, *f1, *enc;

 for (v = 0; v < 64; v++)
 {	
	temp_key[v] = '\0';
	temp_block[v] = '\0';
 }

  for (v = 0; v < 64; v++)
 {	
	charKey64[v] = '\0';
	binKey64[v] = '\0';
	binBlock[v] = '\0';
 }

// if some of the not given, the program shows the right sintax
if(argv[1] == NULL | argv[2] == NULL |argv[3] == NULL | argv[4] == NULL)
	{
		printf("\nsintax: des [{enc|dec}|encryption or decryption] [message_file] [key_file] [destination_file]\n","");
		printf("\nexample: des enc message.txt key.txt output.txt\n","");
		exit(1);
	}
	
// Testa a Existência dos ficheiros fornecidos pelo utiliozador na linhas de comandos
 if ((f = fopen(argv[2], "rb"))==NULL)
 {
	 printf("Impossível abrir o ficheiro da mensagem: %s.\n", argv[2]);
	 exit(1);
 }

 if ((f1 = fopen(argv[3], "rb"))==NULL)
 {
	 printf("Impossível abrir o ficheiro da chave: %s.\n", argv[3]);
	 exit(1);
 }

// Cria um novo ficheiro onde será guardado o ficheiro encriptado
enc = fopen(argv[4], "w"); // Create file to store the encrypted message
fclose(enc);
enc = fopen(argv[4], "a"); // open the file in append mode

// Leitura da chave contida no ficheiro
printf("\n> Reading and processing key from '%s' file...\n\n", argv[3]);
fread(temp_key, sizeof(char), 16, f1);
convertHexadecimalToBinary(temp_key);

//Applying pc-1 permutation table to 64 bit key
printf("\n...  Applying PC-1 permutation table...", "");
printf("\n...    56 bit binary key: ", "");
for(i = 0; i < 56; i++)
{
	binKey56[i] = binKey64[(pc1[i])];
	printf("%d", binKey56[i]);
	if(fmod(i+1, 7)==0)
		printf(" ","");
}

generateCD();

// *************** Applying pc-2 permutation table to 56 bit key **********************************
printf("\n...  Applying PC-2 permutation table...", "");
printf("\n...          48 bit - k1: ", "");
for(i = 0; i < 48; i++)
{
	k1[i] = C1D1[pc2[i]];
	printf("%d", k1[i]);
	if(fmod(i+1, 6)==0)
		printf(" ","");
}

printf("\n...          48 bit - k2: ", "");
for(i = 0; i < 48; i++)
{
	k2[i] = C2D2[pc2[i]];
	printf("%d", k2[i]);
	if(fmod(i+1, 6)==0)
		printf(" ","");
}


printf("\n...          48 bit - k3: ", "");
for(i = 0; i < 48; i++)
{
	k3[i] = C3D3[pc2[i]];
	printf("%d", k3[i]);
	if(fmod(i+1, 6)==0)
		printf(" ","");
}

printf("\n...          48 bit - k4: ", "");
for(i = 0; i < 48; i++)
{
	k4[i] = C4D4[pc2[i]];
	printf("%d", k4[i]);
	if(fmod(i+1, 6)==0)
		printf(" ","");
}

printf("\n...          48 bit - k5: ", "");
for(i = 0; i < 48; i++)
{
	k5[i] = C5D5[pc2[i]];
	printf("%d", k5[i]);
	if(fmod(i+1, 6)==0)
		printf(" ","");
}

printf("\n...          48 bit - k6: ", "");
for(i = 0; i < 48; i++)
{
	k6[i] = C6D6[pc2[i]];
	printf("%d", k6[i]);
	if(fmod(i+1, 6)==0)
		printf(" ","");
}

printf("\n...          48 bit - k7: ", "");
for(i = 0; i < 48; i++)
{
	k7[i] = C7D7[pc2[i]];
	printf("%d", k7[i]);
	if(fmod(i+1, 6)==0)
		printf(" ","");
}

printf("\n...          48 bit - k8: ", "");
for(i = 0; i < 48; i++)
{
	k8[i] = C8D8[pc2[i]];
	printf("%d", k8[i]);
	if(fmod(i+1, 6)==0)
		printf(" ","");
}

printf("\n...          48 bit - k9: ", "");
for(i = 0; i < 48; i++)
{
	k9[i] = C9D9[pc2[i]];
	printf("%d", k9[i]);
	if(fmod(i+1, 6)==0)
		printf(" ","");
}

printf("\n...         48 bit - k10: ", "");
for(i = 0; i < 48; i++)
{
	k10[i] = C10D10[pc2[i]];
	printf("%d", k10[i]);
	if(fmod(i+1, 6)==0)
		printf(" ","");
}

printf("\n...         48 bit - k11: ", "");
for(i = 0; i < 48; i++)
{
	k11[i] = C11D11[pc2[i]];
	printf("%d", k11[i]);
	if(fmod(i+1, 6)==0)
		printf(" ","");
}

printf("\n...         48 bit - k12: ", "");
for(i = 0; i < 48; i++)
{
	k12[i] = C12D12[pc2[i]];
	printf("%d", k12[i]);
	if(fmod(i+1, 6)==0)
		printf(" ","");
}

printf("\n...         48 bit - k13: ", "");
for(i = 0; i < 48; i++)
{
	k13[i] = C13D13[pc2[i]];
	printf("%d", k13[i]);
	if(fmod(i+1, 6)==0)
		printf(" ","");
}

printf("\n...         48 bit - k14: ", "");
for(i = 0; i < 48; i++)
{
	k14[i] = C14D14[pc2[i]];
	printf("%d", k14[i]);
	if(fmod(i+1, 6)==0)
		printf(" ","");
}

printf("\n...         48 bit - k15: ", "");
for(i = 0; i < 48; i++)
{
	k15[i] = C15D15[pc2[i]];
	printf("%d", k15[i]);
	if(fmod(i+1, 6)==0)
		printf(" ","");
}

printf("\n...         48 bit - k16: ", "");
for(i = 0; i < 48; i++)
{
	k16[i] = C16D16[pc2[i]];
	printf("%d", k16[i]);
	if(fmod(i+1, 6)==0)
		printf(" ","");
}

// Invert the subkeys order, if the user had choosed the decryption mode
if(strncmp(argv[1],"dec", 3)==0)
	invert_key_order();

// Leitura de blocos de 8 bytes (64 bits da mensagem)
printf("\n\n> Reading and processing message from '%s' file...", argv[2]);
while (fread(temp_block, sizeof(char), 8, f) > 0)
 {
	 convertDecimalToBinary(temp_block);
	 
	 // Applying IP table and send the result to ipM array
	 for(v = 0; v < 64; v++)
	 	ipM[v] = binBlock[ip[v]];

	// Print the Initial Permutation (IP) result
	printf("\n...             64 bit IP: %s", "");
	for(v = 0; v < 64; v++)
	{
		printf("%d", ipM[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}
	
	// Print the L0 array
	printf("\n...                    L0: ", "");
	for(v = 0; v < 32; v++)
	{
		printf("%d", ipM[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}
	
	// Print the R0 array
	printf("\n...                    R0: ", "");
	for(v = 32; v < 64; v++)
	{
		printf("%d", ipM[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}
	
	// ******************************************** L1 R1 calculation *********************************************
	printf("\n\n...        * Round 1...", "");
	// Set L1 from R0 aray
	printf("\n...                    L1: ", "");
	for(v = 32; v < 64; v++)
	{
		L1[v-32] = ipM[v];
		printf("%d", L1[v-32]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	printf("\n...        Calculating R1 = ( L0 + f(E(R0),K1) )... (+)", "");

	// Expanding R0
	printf("\n...           Applying E-bit selection table...", "");
	printf("\n...                              Expanded R0: ", "");
	for(v = 0; v < 48; v++)
	{
		expand_PrevR[v] = L1[eBitSelection[v]];
		printf("%d", expand_PrevR[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	// Calculating expanded R0 XOR K1
	printf("\n...                       Expanded_R0 XOR K1: ", "");
	for(v = 0; v < 48; v++)
	{
		if(expand_PrevR[v] == k1[v])
			XOR[v] = 0;
		else
			XOR[v] = 1;
		printf("%d", XOR[v]);
		if(fmod(v+1, 6)==0)
		printf(" ","");
	}
	

	// Processing the expanded R0 XOR K1, in order to transform each 6 bit in 4 bit (using de S-Boxes)
	
	// First 6-bit block
	printf("\n...                            Sub-block 1...","");
	subBlockStart = 0;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S1 Value: %d ", S1[(row*16)+column]);
	sendBinaryToF(S1[(row*16)+column], 1); // Send the first 4-bit to S array

	// Second 6-bit block
	printf("\n...                            Sub-block 2...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S2 Value: %d ", S2[(row*16)+column]);
	sendBinaryToF(S2[(row*16)+column], 2); // Send the second 4-bit block to S array
	
	// Third 6-bit block
	printf("\n...                            Sub-block 3...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S3 Value: %d ", S3[(row*16)+column]);
	sendBinaryToF(S3[(row*16)+column], 3); // Send the third 4-bit block to S array
	
	// Fourth 6-bit block
	printf("\n...                            Sub-block 4...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S4 Value: %d ", S4[(row*16)+column]);
	sendBinaryToF(S4[(row*16)+column], 4); // Send the fourth 4-bit block to S array

	// Fifth 6-bit block
	printf("\n...                            Sub-block 5...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S5 Value: %d ", S5[(row*16)+column]);
	sendBinaryToF(S5[(row*16)+column], 5); // Send the fifth 4-bit block to S array

	// Sixth 6-bit block
	printf("\n...                            Sub-block 6...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S6 Value: %d ", S6[(row*16)+column]);
	sendBinaryToF(S6[(row*16)+column], 6); // Send the sixth 4-bit block to S array
	
	// Seventh 6-bit block
	printf("\n...                            Sub-block 7...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S7 Value: %d ", S7[(row*16)+column]);
	sendBinaryToF(S7[(row*16)+column], 7); // Send the seventh 4-bit block to S array

	// Eighth 6-bit block
	printf("\n...                            Sub-block 8...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S8 Value: %d ", S8[(row*16)+column]);
	sendBinaryToF(S8[(row*16)+column], 8); // Send the eighth 4-bit block to S array

	// Printing the result of applying the all 8 S-boxes (completing f function result)
	printf("\n...                                        S: ", ""); 
	for(v = 0; v < 32; v++)
	{
		printf("%d", S[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// Applying P permutation table to S array, to obtain the final result of F function, and print it
	printf("\n...          Applying P permutation table...", "");
	printf("\n...                      Function result - F: ", ""); 
	for(v = 0; v < 32; v++)
	{
		F[v] = S[P[v]];
		printf("%d", F[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// XOR L0 and F, to obtain R1
	printf("\n...    (+) L0 XOR F - R1: ", "");
	for(v = 0; v < 32; v++)
	{
		if(ipM[v] == F[v])
			R1[v] = 0;
		else
			R1[v] = 1;
		printf("%d", R1[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}
	// ------------------------------------------------ END (L1 R1) ---------------------------------------------------
	

	// ******************************************** L2 R2 calculation *********************************************
	printf("\n\n...        * Round 2...", "");
	// Set L2 from R1 aray
	printf("\n...                    L2: ", "");
	for(v = 0; v < 32; v++)
	{
		L2[v] = R1[v];
		printf("%d", L2[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	printf("\n...        Calculating R2 = ( L1 + f(E(R1),K2) )... (+)", "");

	// Expanding R1
	printf("\n...           Applying E-bit selection table...", "");
	printf("\n...                              Expanded R1: ", "");
	for(v = 0; v < 48; v++)
	{
		expand_PrevR[v] = R1[eBitSelection[v]];
		printf("%d", expand_PrevR[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	// Calculating expanded R1 XOR K2
	printf("\n...                       Expanded_R1 XOR K2: ", "");
	for(v = 0; v < 48; v++)
	{
		if(expand_PrevR[v] == k2[v])
			XOR[v] = 0;
		else
			XOR[v] = 1;
		printf("%d", XOR[v]);
		if(fmod(v+1, 6)==0)
		printf(" ","");
	}
	
	// Processing the expanded R1 XOR K2, in order to transform each 6 bit in 4 bit (using de S-Boxes)
	
	// First 6-bit block
	printf("\n...                            Sub-block 1...","");
	subBlockStart = 0;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S1 Value: %d ", S1[(row*16)+column]);
	sendBinaryToF(S1[(row*16)+column], 1); // Send the first 4-bit to S array

	// Second 6-bit block
	printf("\n...                            Sub-block 2...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S2 Value: %d ", S2[(row*16)+column]);
	sendBinaryToF(S2[(row*16)+column], 2); // Send the second 4-bit block to S array
	
	// Third 6-bit block
	printf("\n...                            Sub-block 3...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S3 Value: %d ", S3[(row*16)+column]);
	sendBinaryToF(S3[(row*16)+column], 3); // Send the third 4-bit block to S array
	
	// Fourth 6-bit block
	printf("\n...                            Sub-block 4...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S4 Value: %d ", S4[(row*16)+column]);
	sendBinaryToF(S4[(row*16)+column], 4); // Send the fourth 4-bit block to S array

	// Fifth 6-bit block
	printf("\n...                            Sub-block 5...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S5 Value: %d ", S5[(row*16)+column]);
	sendBinaryToF(S5[(row*16)+column], 5); // Send the fifth 4-bit block to S array

	// Sixth 6-bit block
	printf("\n...                            Sub-block 6...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S6 Value: %d ", S6[(row*16)+column]);
	sendBinaryToF(S6[(row*16)+column], 6); // Send the sixth 4-bit block to S array
	
	// Seventh 6-bit block
	printf("\n...                            Sub-block 7...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S7 Value: %d ", S7[(row*16)+column]);
	sendBinaryToF(S7[(row*16)+column], 7); // Send the seventh 4-bit block to S array

	// Eighth 6-bit block
	printf("\n...                            Sub-block 8...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S8 Value: %d ", S8[(row*16)+column]);
	sendBinaryToF(S8[(row*16)+column], 8); // Send the eighth 4-bit block to S array

	// Printing the result of applying the all 8 S-boxes (completing f function result)
	printf("\n...                                        S: ", ""); 
	for(v = 0; v < 32; v++)
	{
		printf("%d", S[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// Applying P permutation table to S array, to obtain the final result of F function, and print it
	printf("\n...          Applying P permutation table...", "");
	printf("\n...                      Function result - F: ", ""); 
	for(v = 0; v < 32; v++)
	{
		F[v] = S[P[v]];
		printf("%d", F[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// XOR L1 and F, to obtain R2
	printf("\n...   (+) L1 XOR F - R2: ", "");
	for(v = 0; v < 32; v++)
	{
		if(L1[v] == F[v])
			R2[v] = 0;
		else
			R2[v] = 1;
		printf("%d", R2[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}
	// ---------------------------------------------- END (L2 R2) -----------------------------------------------------

	// ******************************************** L3 R3 calculation *********************************************
	printf("\n\n...        * Round 3...", "");
	// Set L3 from R2 aray
	printf("\n...                    L3: ", "");
	for(v = 0; v < 32; v++)
	{
		L3[v] = R2[v];
		printf("%d", L3[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	printf("\n...        Calculating R3 = L2 + f(E(R2),K3)...", "");

	// Expanding R2
	printf("\n...           Applying E-bit selection table...", "");
	printf("\n...                              Expanded R2: ", "");
	for(v = 0; v < 48; v++)
	{
		expand_PrevR[v] = R2[eBitSelection[v]];
		printf("%d", expand_PrevR[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	// Calculating expanded R2 XOR K3
	printf("\n...                       Expanded_R2 XOR K3: ", "");
	for(v = 0; v < 48; v++)
	{
		if(expand_PrevR[v] == k3[v])
			XOR[v] = 0;
		else
			XOR[v] = 1;
		printf("%d", XOR[v]);
		if(fmod(v+1, 6)==0)
		printf(" ","");
	}
	
	// Processing the expanded R2 XOR K3, in order to transform each 6 bit in 4 bit (using de S-Boxes)
	
	// First 6-bit block
	printf("\n...                            Sub-block 1...","");
	subBlockStart = 0;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S1 Value: %d ", S1[(row*16)+column]);
	sendBinaryToF(S1[(row*16)+column], 1); // Send the first 4-bit to S array

	// Second 6-bit block
	printf("\n...                            Sub-block 2...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S2 Value: %d ", S2[(row*16)+column]);
	sendBinaryToF(S2[(row*16)+column], 2); // Send the second 4-bit block to S array
	
	// Third 6-bit block
	printf("\n...                            Sub-block 3...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S3 Value: %d ", S3[(row*16)+column]);
	sendBinaryToF(S3[(row*16)+column], 3); // Send the third 4-bit block to S array
	
	// Fourth 6-bit block
	printf("\n...                            Sub-block 4...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S4 Value: %d ", S4[(row*16)+column]);
	sendBinaryToF(S4[(row*16)+column], 4); // Send the fourth 4-bit block to S array

	// Fifth 6-bit block
	printf("\n...                            Sub-block 5...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S5 Value: %d ", S5[(row*16)+column]);
	sendBinaryToF(S5[(row*16)+column], 5); // Send the fifth 4-bit block to S array

	// Sixth 6-bit block
	printf("\n...                            Sub-block 6...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S6 Value: %d ", S6[(row*16)+column]);
	sendBinaryToF(S6[(row*16)+column], 6); // Send the sixth 4-bit block to S array
	
	// Seventh 6-bit block
	printf("\n...                            Sub-block 7...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S7 Value: %d ", S7[(row*16)+column]);
	sendBinaryToF(S7[(row*16)+column], 7); // Send the seventh 4-bit block to S array

	// Eighth 6-bit block
	printf("\n...                            Sub-block 8...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S8 Value: %d ", S8[(row*16)+column]);
	sendBinaryToF(S8[(row*16)+column], 8); // Send the eighth 4-bit block to S array

	// Printing the result of applying the all 8 S-boxes (completing f function result)
	printf("\n...                                        S: ", ""); 
	for(v = 0; v < 32; v++)
	{
		printf("%d", S[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// Applying P permutation table to S array, to obtain the final result of F function, and print it
	printf("\n...          Applying P permutation table...", "");
	printf("\n...                      Function result - F: ", ""); 
	for(v = 0; v < 32; v++)
	{
		F[v] = S[P[v]];
		printf("%d", F[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// XOR L2 and F, to obtain R3
	printf("\n...         L2 XOR F - R3: ", "");
	for(v = 0; v < 32; v++)
	{
		if(L2[v] == F[v])
			R3[v] = 0;
		else
			R3[v] = 1;
		printf("%d", R3[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}
	// ---------------------------------------------- END (L3 R3) -----------------------------------------------------

	// ******************************************** L4 R4 calculation *********************************************
	printf("\n\n...        * Round 4...", "");
	// Set L4 from R3 aray
	printf("\n...                    L4: ", "");
	for(v = 0; v < 32; v++)
	{
		L4[v] = R3[v];
		printf("%d", L4[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	printf("\n...        Calculating R4 = L3 + f(E(R3),K4)... (+)", "");

	// Expanding R3
	printf("\n...           Applying E-bit selection table...", "");
	printf("\n...                              Expanded R3: ", "");
	for(v = 0; v < 48; v++)
	{
		expand_PrevR[v] = R3[eBitSelection[v]];
		printf("%d", expand_PrevR[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	// Calculating expanded R3 XOR K4
	printf("\n...                       Expanded_R3 XOR K4: ", "");
	for(v = 0; v < 48; v++)
	{
		if(expand_PrevR[v] == k4[v])
			XOR[v] = 0;
		else
			XOR[v] = 1;
		printf("%d", XOR[v]);
		if(fmod(v+1, 6)==0)
		printf(" ","");
	}
	
	// Processing the expanded R3 XOR K4, in order to transform each 6 bit in 4 bit (using de S-Boxes)
	
	// First 6-bit block
	printf("\n...                            Sub-block 1...","");
	subBlockStart = 0;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S1 Value: %d ", S1[(row*16)+column]);
	sendBinaryToF(S1[(row*16)+column], 1); // Send the first 4-bit to S array

	// Second 6-bit block
	printf("\n...                            Sub-block 2...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S2 Value: %d ", S2[(row*16)+column]);
	sendBinaryToF(S2[(row*16)+column], 2); // Send the second 4-bit block to S array
	
	// Third 6-bit block
	printf("\n...                            Sub-block 3...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S3 Value: %d ", S3[(row*16)+column]);
	sendBinaryToF(S3[(row*16)+column], 3); // Send the third 4-bit block to S array
	
	// Fourth 6-bit block
	printf("\n...                            Sub-block 4...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S4 Value: %d ", S4[(row*16)+column]);
	sendBinaryToF(S4[(row*16)+column], 4); // Send the fourth 4-bit block to S array

	// Fifth 6-bit block
	printf("\n...                            Sub-block 5...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S5 Value: %d ", S5[(row*16)+column]);
	sendBinaryToF(S5[(row*16)+column], 5); // Send the fifth 4-bit block to S array

	// Sixth 6-bit block
	printf("\n...                            Sub-block 6...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S6 Value: %d ", S6[(row*16)+column]);
	sendBinaryToF(S6[(row*16)+column], 6); // Send the sixth 4-bit block to S array
	
	// Seventh 6-bit block
	printf("\n...                            Sub-block 7...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S7 Value: %d ", S7[(row*16)+column]);
	sendBinaryToF(S7[(row*16)+column], 7); // Send the seventh 4-bit block to S array

	// Eighth 6-bit block
	printf("\n...                            Sub-block 8...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S8 Value: %d ", S8[(row*16)+column]);
	sendBinaryToF(S8[(row*16)+column], 8); // Send the eighth 4-bit block to S array

	// Printing the result of applying the all 8 S-boxes (completing f function result)
	printf("\n...                                        S: ", ""); 
	for(v = 0; v < 32; v++)
	{
		printf("%d", S[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// Applying P permutation table to S array, to obtain the final result of F function, and print it
	printf("\n...          Applying P permutation table...", "");
	printf("\n...                      Function result - F: ", ""); 
	for(v = 0; v < 32; v++)
	{
		F[v] = S[P[v]];
		printf("%d", F[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// XOR L3 and F, to obtain R4
	printf("\n...     (+) L3 XOR F - R4: ", "");
	for(v = 0; v < 32; v++)
	{
		if(L3[v] == F[v])
			R4[v] = 0;
		else
			R4[v] = 1;
		printf("%d", R4[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}
	// ---------------------------------------------- END (L4 R4) -----------------------------------------------------

	// ******************************************** L5 R5 calculation *********************************************
	printf("\n\n...        * Round 5...", "");
	// Set L5 from R4 aray
	printf("\n...                    L5: ", "");
	for(v = 0; v < 32; v++)
	{
		L5[v] = R4[v];
		printf("%d", L5[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	printf("\n...        Calculating R5 = L4 + f(E(R4),K5)... (+)", "");

	// Expanding R4
	printf("\n...           Applying E-bit selection table...", "");
	printf("\n...                              Expanded R4: ", "");
	for(v = 0; v < 48; v++)
	{
		expand_PrevR[v] = R4[eBitSelection[v]];
		printf("%d", expand_PrevR[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	// Calculating expanded R4 XOR K5
	printf("\n...                       Expanded_R4 XOR K5: ", "");
	for(v = 0; v < 48; v++)
	{
		if(expand_PrevR[v] == k5[v])
			XOR[v] = 0;
		else
			XOR[v] = 1;
		printf("%d", XOR[v]);
		if(fmod(v+1, 6)==0)
		printf(" ","");
	}
	
	// Processing the expanded R4 XOR K5, in order to transform each 6 bit in 4 bit (using de S-Boxes)
	
	// First 6-bit block
	printf("\n...                            Sub-block 1...","");
	subBlockStart = 0;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S1 Value: %d ", S1[(row*16)+column]);
	sendBinaryToF(S1[(row*16)+column], 1); // Send the first 4-bit to S array

	// Second 6-bit block
	printf("\n...                            Sub-block 2...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S2 Value: %d ", S2[(row*16)+column]);
	sendBinaryToF(S2[(row*16)+column], 2); // Send the second 4-bit block to S array
	
	// Third 6-bit block
	printf("\n...                            Sub-block 3...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S3 Value: %d ", S3[(row*16)+column]);
	sendBinaryToF(S3[(row*16)+column], 3); // Send the third 4-bit block to S array
	
	// Fourth 6-bit block
	printf("\n...                            Sub-block 4...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S4 Value: %d ", S4[(row*16)+column]);
	sendBinaryToF(S4[(row*16)+column], 4); // Send the fourth 4-bit block to S array

	// Fifth 6-bit block
	printf("\n...                            Sub-block 5...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S5 Value: %d ", S5[(row*16)+column]);
	sendBinaryToF(S5[(row*16)+column], 5); // Send the fifth 4-bit block to S array

	// Sixth 6-bit block
	printf("\n...                            Sub-block 6...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S6 Value: %d ", S6[(row*16)+column]);
	sendBinaryToF(S6[(row*16)+column], 6); // Send the sixth 4-bit block to S array
	
	// Seventh 6-bit block
	printf("\n...                            Sub-block 7...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S7 Value: %d ", S7[(row*16)+column]);
	sendBinaryToF(S7[(row*16)+column], 7); // Send the seventh 4-bit block to S array

	// Eighth 6-bit block
	printf("\n...                            Sub-block 8...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S8 Value: %d ", S8[(row*16)+column]);
	sendBinaryToF(S8[(row*16)+column], 8); // Send the eighth 4-bit block to S array

	// Printing the result of applying the all 8 S-boxes (completing f function result)
	printf("\n...                                        S: ", ""); 
	for(v = 0; v < 32; v++)
	{
		printf("%d", S[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// Applying P permutation table to S array, to obtain the final result of F function, and print it
	printf("\n...          Applying P permutation table...", "");
	printf("\n...                      Function result - F: ", ""); 
	for(v = 0; v < 32; v++)
	{
		F[v] = S[P[v]];
		printf("%d", F[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// XOR L4 and F, to obtain R5
	printf("\n...     (+) L4 XOR F - R5: ", "");
	for(v = 0; v < 32; v++)
	{
		if(L4[v] == F[v])
			R5[v] = 0;
		else
			R5[v] = 1;
		printf("%d", R5[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}
	// ---------------------------------------------- END (L5 R5) -----------------------------------------------------

	// ******************************************** L6 R6 calculation *********************************************
	printf("\n\n...        * Round 6...", "");
	// Set L6 from R5 aray
	printf("\n...                    L6: ", "");
	for(v = 0; v < 32; v++)
	{
		L6[v] = R5[v];
		printf("%d", L6[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	printf("\n...        Calculating R6 = L5 + f(E(R5),K6)... (+)", "");

	// Expanding R5
	printf("\n...           Applying E-bit selection table...", "");
	printf("\n...                              Expanded R5: ", "");
	for(v = 0; v < 48; v++)
	{
		expand_PrevR[v] = R5[eBitSelection[v]];
		printf("%d", expand_PrevR[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	// Calculating expanded R5 XOR K6
	printf("\n...                       Expanded_R5 XOR K6: ", "");
	for(v = 0; v < 48; v++)
	{
		if(expand_PrevR[v] == k6[v])
			XOR[v] = 0;
		else
			XOR[v] = 1;
		printf("%d", XOR[v]);
		if(fmod(v+1, 6)==0)
		printf(" ","");
	}
	
	// Processing the expanded R5 XOR K6, in order to transform each 6 bit in 4 bit (using de S-Boxes)
	
	// First 6-bit block
	printf("\n...                            Sub-block 1...","");
	subBlockStart = 0;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S1 Value: %d ", S1[(row*16)+column]);
	sendBinaryToF(S1[(row*16)+column], 1); // Send the first 4-bit to S array

	// Second 6-bit block
	printf("\n...                            Sub-block 2...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S2 Value: %d ", S2[(row*16)+column]);
	sendBinaryToF(S2[(row*16)+column], 2); // Send the second 4-bit block to S array
	
	// Third 6-bit block
	printf("\n...                            Sub-block 3...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S3 Value: %d ", S3[(row*16)+column]);
	sendBinaryToF(S3[(row*16)+column], 3); // Send the third 4-bit block to S array
	
	// Fourth 6-bit block
	printf("\n...                            Sub-block 4...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S4 Value: %d ", S4[(row*16)+column]);
	sendBinaryToF(S4[(row*16)+column], 4); // Send the fourth 4-bit block to S array

	// Fifth 6-bit block
	printf("\n...                            Sub-block 5...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S5 Value: %d ", S5[(row*16)+column]);
	sendBinaryToF(S5[(row*16)+column], 5); // Send the fifth 4-bit block to S array

	// Sixth 6-bit block
	printf("\n...                            Sub-block 6...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S6 Value: %d ", S6[(row*16)+column]);
	sendBinaryToF(S6[(row*16)+column], 6); // Send the sixth 4-bit block to S array
	
	// Seventh 6-bit block
	printf("\n...                            Sub-block 7...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S7 Value: %d ", S7[(row*16)+column]);
	sendBinaryToF(S7[(row*16)+column], 7); // Send the seventh 4-bit block to S array

	// Eighth 6-bit block
	printf("\n...                            Sub-block 8...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S8 Value: %d ", S8[(row*16)+column]);
	sendBinaryToF(S8[(row*16)+column], 8); // Send the eighth 4-bit block to S array

	// Printing the result of applying the all 8 S-boxes (completing f function result)
	printf("\n...                                        S: ", ""); 
	for(v = 0; v < 32; v++)
	{
		printf("%d", S[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// Applying P permutation table to S array, to obtain the final result of F function, and print it
	printf("\n...          Applying P permutation table...", "");
	printf("\n...                      Function result - F: ", ""); 
	for(v = 0; v < 32; v++)
	{
		F[v] = S[P[v]];
		printf("%d", F[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// XOR L5 and F, to obtain R6
	printf("\n...     (+) L5 XOR F - R6: ", "");
	for(v = 0; v < 32; v++)
	{
		if(L5[v] == F[v])
			R6[v] = 0;
		else
			R6[v] = 1;
		printf("%d", R6[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}
	// ---------------------------------------------- END (L6 R6) -----------------------------------------------------

	// ******************************************** L7 R7 calculation *********************************************
	printf("\n\n...        * Round 7...", "");
	// Set L7 from R6 aray
	printf("\n...                    L7: ", "");
	for(v = 0; v < 32; v++)
	{
		L7[v] = R6[v];
		printf("%d", L7[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	printf("\n...        Calculating R7 = L6 + f(E(R6),K7)... (+)", "");

	// Expanding R6
	printf("\n...           Applying E-bit selection table...", "");
	printf("\n...                              Expanded R6: ", "");
	for(v = 0; v < 48; v++)
	{
		expand_PrevR[v] = R6[eBitSelection[v]];
		printf("%d", expand_PrevR[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	// Calculating expanded R6 XOR K7
	printf("\n...                       Expanded_R6 XOR K7: ", "");
	for(v = 0; v < 48; v++)
	{
		if(expand_PrevR[v] == k7[v])
			XOR[v] = 0;
		else
			XOR[v] = 1;
		printf("%d", XOR[v]);
		if(fmod(v+1, 6)==0)
		printf(" ","");
	}
	
	// Processing the expanded R6 XOR K7, in order to transform each 6 bit in 4 bit (using de S-Boxes)
	
	// First 6-bit block
	printf("\n...                            Sub-block 1...","");
	subBlockStart = 0;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S1 Value: %d ", S1[(row*16)+column]);
	sendBinaryToF(S1[(row*16)+column], 1); // Send the first 4-bit to S array

	// Second 6-bit block
	printf("\n...                            Sub-block 2...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S2 Value: %d ", S2[(row*16)+column]);
	sendBinaryToF(S2[(row*16)+column], 2); // Send the second 4-bit block to S array
	
	// Third 6-bit block
	printf("\n...                            Sub-block 3...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S3 Value: %d ", S3[(row*16)+column]);
	sendBinaryToF(S3[(row*16)+column], 3); // Send the third 4-bit block to S array
	
	// Fourth 6-bit block
	printf("\n...                            Sub-block 4...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S4 Value: %d ", S4[(row*16)+column]);
	sendBinaryToF(S4[(row*16)+column], 4); // Send the fourth 4-bit block to S array

	// Fifth 6-bit block
	printf("\n...                            Sub-block 5...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S5 Value: %d ", S5[(row*16)+column]);
	sendBinaryToF(S5[(row*16)+column], 5); // Send the fifth 4-bit block to S array

	// Sixth 6-bit block
	printf("\n...                            Sub-block 6...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S6 Value: %d ", S6[(row*16)+column]);
	sendBinaryToF(S6[(row*16)+column], 6); // Send the sixth 4-bit block to S array
	
	// Seventh 6-bit block
	printf("\n...                            Sub-block 7...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S7 Value: %d ", S7[(row*16)+column]);
	sendBinaryToF(S7[(row*16)+column], 7); // Send the seventh 4-bit block to S array

	// Eighth 6-bit block
	printf("\n...                            Sub-block 8...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S8 Value: %d ", S8[(row*16)+column]);
	sendBinaryToF(S8[(row*16)+column], 8); // Send the eighth 4-bit block to S array

	// Printing the result of applying the all 8 S-boxes (completing f function result)
	printf("\n...                                        S: ", ""); 
	for(v = 0; v < 32; v++)
	{
		printf("%d", S[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// Applying P permutation table to S array, to obtain the final result of F function, and print it
	printf("\n...          Applying P permutation table...", "");
	printf("\n...                      Function result - F: ", ""); 
	for(v = 0; v < 32; v++)
	{
		F[v] = S[P[v]];
		printf("%d", F[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// XOR L6 and F, to obtain R7
	printf("\n...     (+) L6 XOR F - R7: ", "");
	for(v = 0; v < 32; v++)
	{
		if(L6[v] == F[v])
			R7[v] = 0;
		else
			R7[v] = 1;
		printf("%d", R7[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}
	// ---------------------------------------------- END (L7 R7) -----------------------------------------------------

	// ******************************************** L8 R8 calculation *********************************************
	printf("\n\n...        * Round 8...", "");
	// Set L8 from R7 aray
	printf("\n...                    L8: ", "");
	for(v = 0; v < 32; v++)
	{
		L8[v] = R7[v];
		printf("%d", L8[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	printf("\n...        Calculating R8 = L7 + f(E(R7),K8)... (+)", "");

	// Expanding R7
	printf("\n...           Applying E-bit selection table...", "");
	printf("\n...                              Expanded R7: ", "");
	for(v = 0; v < 48; v++)
	{
		expand_PrevR[v] = R7[eBitSelection[v]];
		printf("%d", expand_PrevR[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	// Calculating expanded R7 XOR K8
	printf("\n...                       Expanded_R7 XOR K8: ", "");
	for(v = 0; v < 48; v++)
	{
		if(expand_PrevR[v] == k8[v])
			XOR[v] = 0;
		else
			XOR[v] = 1;
		printf("%d", XOR[v]);
		if(fmod(v+1, 6)==0)
		printf(" ","");
	}
	
	// Processing the expanded R7 XOR K8, in order to transform each 6 bit in 4 bit (using de S-Boxes)
	
	// First 6-bit block
	printf("\n...                            Sub-block 1...","");
	subBlockStart = 0;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S1 Value: %d ", S1[(row*16)+column]);
	sendBinaryToF(S1[(row*16)+column], 1); // Send the first 4-bit to S array

	// Second 6-bit block
	printf("\n...                            Sub-block 2...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S2 Value: %d ", S2[(row*16)+column]);
	sendBinaryToF(S2[(row*16)+column], 2); // Send the second 4-bit block to S array
	
	// Third 6-bit block
	printf("\n...                            Sub-block 3...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S3 Value: %d ", S3[(row*16)+column]);
	sendBinaryToF(S3[(row*16)+column], 3); // Send the third 4-bit block to S array
	
	// Fourth 6-bit block
	printf("\n...                            Sub-block 4...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S4 Value: %d ", S4[(row*16)+column]);
	sendBinaryToF(S4[(row*16)+column], 4); // Send the fourth 4-bit block to S array

	// Fifth 6-bit block
	printf("\n...                            Sub-block 5...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S5 Value: %d ", S5[(row*16)+column]);
	sendBinaryToF(S5[(row*16)+column], 5); // Send the fifth 4-bit block to S array

	// Sixth 6-bit block
	printf("\n...                            Sub-block 6...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S6 Value: %d ", S6[(row*16)+column]);
	sendBinaryToF(S6[(row*16)+column], 6); // Send the sixth 4-bit block to S array
	
	// Seventh 6-bit block
	printf("\n...                            Sub-block 7...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S7 Value: %d ", S7[(row*16)+column]);
	sendBinaryToF(S7[(row*16)+column], 7); // Send the seventh 4-bit block to S array

	// Eighth 6-bit block
	printf("\n...                            Sub-block 8...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S8 Value: %d ", S8[(row*16)+column]);
	sendBinaryToF(S8[(row*16)+column], 8); // Send the eighth 4-bit block to S array

	// Printing the result of applying the all 8 S-boxes (completing f function result)
	printf("\n...                                        S: ", ""); 
	for(v = 0; v < 32; v++)
	{
		printf("%d", S[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// Applying P permutation table to S array, to obtain the final result of F function, and print it
	printf("\n...          Applying P permutation table...", "");
	printf("\n...                      Function result - F: ", ""); 
	for(v = 0; v < 32; v++)
	{
		F[v] = S[P[v]];
		printf("%d", F[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// XOR L7 and F, to obtain R8
	printf("\n...     (+) L7 XOR F - R8: ", "");
	for(v = 0; v < 32; v++)
	{
		if(L7[v] == F[v])
			R8[v] = 0;
		else
			R8[v] = 1;
		printf("%d", R8[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}
	// ---------------------------------------------- END (L8 R8) -----------------------------------------------------

	// ******************************************** L9 R9 calculation *********************************************
	printf("\n\n...        * Round 9...", "");
	// Set L9 from R8 aray
	printf("\n...                    L9: ", "");
	for(v = 0; v < 32; v++)
	{
		L9[v] = R8[v];
		printf("%d", L9[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	printf("\n...        Calculating R9 = L8 + f(E(R8),K9)... (+)", "");

	// Expanding R8
	printf("\n...           Applying E-bit selection table...", "");
	printf("\n...                              Expanded R8: ", "");
	for(v = 0; v < 48; v++)
	{
		expand_PrevR[v] = R8[eBitSelection[v]];
		printf("%d", expand_PrevR[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	// Calculating expanded R8 XOR K9
	printf("\n...                       Expanded_R8 XOR K9: ", "");
	for(v = 0; v < 48; v++)
	{
		if(expand_PrevR[v] == k9[v])
			XOR[v] = 0;
		else
			XOR[v] = 1;
		printf("%d", XOR[v]);
		if(fmod(v+1, 6)==0)
		printf(" ","");
	}
	
	// Processing the expanded R8 XOR K9, in order to transform each 6 bit in 4 bit (using de S-Boxes)
	
	// First 6-bit block
	printf("\n...                            Sub-block 1...","");
	subBlockStart = 0;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S1 Value: %d ", S1[(row*16)+column]);
	sendBinaryToF(S1[(row*16)+column], 1); // Send the first 4-bit to S array

	// Second 6-bit block
	printf("\n...                            Sub-block 2...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S2 Value: %d ", S2[(row*16)+column]);
	sendBinaryToF(S2[(row*16)+column], 2); // Send the second 4-bit block to S array
	
	// Third 6-bit block
	printf("\n...                            Sub-block 3...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S3 Value: %d ", S3[(row*16)+column]);
	sendBinaryToF(S3[(row*16)+column], 3); // Send the third 4-bit block to S array
	
	// Fourth 6-bit block
	printf("\n...                            Sub-block 4...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S4 Value: %d ", S4[(row*16)+column]);
	sendBinaryToF(S4[(row*16)+column], 4); // Send the fourth 4-bit block to S array

	// Fifth 6-bit block
	printf("\n...                            Sub-block 5...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S5 Value: %d ", S5[(row*16)+column]);
	sendBinaryToF(S5[(row*16)+column], 5); // Send the fifth 4-bit block to S array

	// Sixth 6-bit block
	printf("\n...                            Sub-block 6...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S6 Value: %d ", S6[(row*16)+column]);
	sendBinaryToF(S6[(row*16)+column], 6); // Send the sixth 4-bit block to S array
	
	// Seventh 6-bit block
	printf("\n...                            Sub-block 7...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S7 Value: %d ", S7[(row*16)+column]);
	sendBinaryToF(S7[(row*16)+column], 7); // Send the seventh 4-bit block to S array

	// Eighth 6-bit block
	printf("\n...                            Sub-block 8...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S8 Value: %d ", S8[(row*16)+column]);
	sendBinaryToF(S8[(row*16)+column], 8); // Send the eighth 4-bit block to S array

	// Printing the result of applying the all 8 S-boxes (completing f function result)
	printf("\n...                                        S: ", ""); 
	for(v = 0; v < 32; v++)
	{
		printf("%d", S[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// Applying P permutation table to S array, to obtain the final result of F function, and print it
	printf("\n...          Applying P permutation table...", "");
	printf("\n...                      Function result - F: ", ""); 
	for(v = 0; v < 32; v++)
	{
		F[v] = S[P[v]];
		printf("%d", F[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// XOR L8 and F, to obtain R9
	printf("\n...     (+) L8 XOR F - R9: ", "");
	for(v = 0; v < 32; v++)
	{
		if(L8[v] == F[v])
			R9[v] = 0;
		else
			R9[v] = 1;
		printf("%d", R9[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}
	// ---------------------------------------------- END (L9 R9) -----------------------------------------------------

	// ******************************************** L10 R10 calculation *********************************************
	printf("\n\n...        * Round 10...", "");
	// Set L10 from R9 aray
	printf("\n...                   L10: ", "");
	for(v = 0; v < 32; v++)
	{
		L10[v] = R9[v];
		printf("%d", L10[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	printf("\n...        Calculating R10 = L9 + f(E(R9),K10)... (+)", "");

	// Expanding R9
	printf("\n...           Applying E-bit selection table...", "");
	printf("\n...                              Expanded R9: ", "");
	for(v = 0; v < 48; v++)
	{
		expand_PrevR[v] = R9[eBitSelection[v]];
		printf("%d", expand_PrevR[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	// Calculating expanded R9 XOR K10
	printf("\n...                      Expanded_R9 XOR K10: ", "");
	for(v = 0; v < 48; v++)
	{
		if(expand_PrevR[v] == k10[v])
			XOR[v] = 0;
		else
			XOR[v] = 1;
		printf("%d", XOR[v]);
		if(fmod(v+1, 6)==0)
		printf(" ","");
	}
	
	// Processing the expanded R9 XOR K10, in order to transform each 6 bit in 4 bit (using de S-Boxes)
	
	// First 6-bit block
	printf("\n...                            Sub-block 1...","");
	subBlockStart = 0;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S1 Value: %d ", S1[(row*16)+column]);
	sendBinaryToF(S1[(row*16)+column], 1); // Send the first 4-bit to S array

	// Second 6-bit block
	printf("\n...                            Sub-block 2...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S2 Value: %d ", S2[(row*16)+column]);
	sendBinaryToF(S2[(row*16)+column], 2); // Send the second 4-bit block to S array
	
	// Third 6-bit block
	printf("\n...                            Sub-block 3...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S3 Value: %d ", S3[(row*16)+column]);
	sendBinaryToF(S3[(row*16)+column], 3); // Send the third 4-bit block to S array
	
	// Fourth 6-bit block
	printf("\n...                            Sub-block 4...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S4 Value: %d ", S4[(row*16)+column]);
	sendBinaryToF(S4[(row*16)+column], 4); // Send the fourth 4-bit block to S array

	// Fifth 6-bit block
	printf("\n...                            Sub-block 5...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S5 Value: %d ", S5[(row*16)+column]);
	sendBinaryToF(S5[(row*16)+column], 5); // Send the fifth 4-bit block to S array

	// Sixth 6-bit block
	printf("\n...                            Sub-block 6...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S6 Value: %d ", S6[(row*16)+column]);
	sendBinaryToF(S6[(row*16)+column], 6); // Send the sixth 4-bit block to S array
	
	// Seventh 6-bit block
	printf("\n...                            Sub-block 7...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S7 Value: %d ", S7[(row*16)+column]);
	sendBinaryToF(S7[(row*16)+column], 7); // Send the seventh 4-bit block to S array

	// Eighth 6-bit block
	printf("\n...                            Sub-block 8...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S8 Value: %d ", S8[(row*16)+column]);
	sendBinaryToF(S8[(row*16)+column], 8); // Send the eighth 4-bit block to S array

	// Printing the result of applying the all 8 S-boxes (completing f function result)
	printf("\n...                                        S: ", ""); 
	for(v = 0; v < 32; v++)
	{
		printf("%d", S[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// Applying P permutation table to S array, to obtain the final result of F function, and print it
	printf("\n...          Applying P permutation table...", "");
	printf("\n...                      Function result - F: ", ""); 
	for(v = 0; v < 32; v++)
	{
		F[v] = S[P[v]];
		printf("%d", F[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// XOR L9 and F, to obtain R10
	printf("\n...    (+) L9 XOR F - R10: ", "");
	for(v = 0; v < 32; v++)
	{
		if(L9[v] == F[v])
			R10[v] = 0;
		else
			R10[v] = 1;
		printf("%d", R10[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}
	// ---------------------------------------------- END (L10 R10) -----------------------------------------------------

	// ******************************************** L11 R11 calculation *********************************************
	printf("\n\n...        * Round 11...", "");
	// Set L11 from R10 aray
	printf("\n...                   L11: ", "");
	for(v = 0; v < 32; v++)
	{
		L11[v] = R10[v];
		printf("%d", L11[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	printf("\n...        Calculating R11 = L10 + f(E(R10),K11)... (+)", "");

	// Expanding R10
	printf("\n...           Applying E-bit selection table...", "");
	printf("\n...                             Expanded R10: ", "");
	for(v = 0; v < 48; v++)
	{
		expand_PrevR[v] = R10[eBitSelection[v]];
		printf("%d", expand_PrevR[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	// Calculating expanded R10 XOR K11
	printf("\n...                     Expanded_R10 XOR K11: ", "");
	for(v = 0; v < 48; v++)
	{
		if(expand_PrevR[v] == k11[v])
			XOR[v] = 0;
		else
			XOR[v] = 1;
		printf("%d", XOR[v]);
		if(fmod(v+1, 6)==0)
		printf(" ","");
	}
	
	// Processing the expanded R10 XOR K11, in order to transform each 6 bit in 4 bit (using de S-Boxes)
	
	// First 6-bit block
	printf("\n...                            Sub-block 1...","");
	subBlockStart = 0;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S1 Value: %d ", S1[(row*16)+column]);
	sendBinaryToF(S1[(row*16)+column], 1); // Send the first 4-bit to S array

	// Second 6-bit block
	printf("\n...                            Sub-block 2...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S2 Value: %d ", S2[(row*16)+column]);
	sendBinaryToF(S2[(row*16)+column], 2); // Send the second 4-bit block to S array
	
	// Third 6-bit block
	printf("\n...                            Sub-block 3...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S3 Value: %d ", S3[(row*16)+column]);
	sendBinaryToF(S3[(row*16)+column], 3); // Send the third 4-bit block to S array
	
	// Fourth 6-bit block
	printf("\n...                            Sub-block 4...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S4 Value: %d ", S4[(row*16)+column]);
	sendBinaryToF(S4[(row*16)+column], 4); // Send the fourth 4-bit block to S array

	// Fifth 6-bit block
	printf("\n...                            Sub-block 5...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S5 Value: %d ", S5[(row*16)+column]);
	sendBinaryToF(S5[(row*16)+column], 5); // Send the fifth 4-bit block to S array

	// Sixth 6-bit block
	printf("\n...                            Sub-block 6...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S6 Value: %d ", S6[(row*16)+column]);
	sendBinaryToF(S6[(row*16)+column], 6); // Send the sixth 4-bit block to S array
	
	// Seventh 6-bit block
	printf("\n...                            Sub-block 7...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S7 Value: %d ", S7[(row*16)+column]);
	sendBinaryToF(S7[(row*16)+column], 7); // Send the seventh 4-bit block to S array

	// Eighth 6-bit block
	printf("\n...                            Sub-block 8...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S8 Value: %d ", S8[(row*16)+column]);
	sendBinaryToF(S8[(row*16)+column], 8); // Send the eighth 4-bit block to S array

	// Printing the result of applying the all 8 S-boxes (completing f function result)
	printf("\n...                                        S: ", ""); 
	for(v = 0; v < 32; v++)
	{
		printf("%d", S[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// Applying P permutation table to S array, to obtain the final result of F function, and print it
	printf("\n...          Applying P permutation table...", "");
	printf("\n...                      Function result - F: ", ""); 
	for(v = 0; v < 32; v++)
	{
		F[v] = S[P[v]];
		printf("%d", F[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// XOR L10 and F, to obtain R11
	printf("\n...    (+) L10 XOR F - R11: ", "");
	for(v = 0; v < 32; v++)
	{
		if(L10[v] == F[v])
			R11[v] = 0;
		else
			R11[v] = 1;
		printf("%d", R11[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}
	// ---------------------------------------------- END (L11 R11) -----------------------------------------------------

	// ******************************************** L12 R12 calculation *********************************************
	printf("\n\n...        * Round 12...", "");
	// Set L12 from R11 aray
	printf("\n...                   L12: ", "");
	for(v = 0; v < 32; v++)
	{
		L12[v] = R11[v];
		printf("%d", L12[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	printf("\n...        Calculating R12 = L11 + f(E(R11),K12)... (+)", "");

	// Expanding R11
	printf("\n...           Applying E-bit selection table...", "");
	printf("\n...                             Expanded R11: ", "");
	for(v = 0; v < 48; v++)
	{
		expand_PrevR[v] = R11[eBitSelection[v]];
		printf("%d", expand_PrevR[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	// Calculating expanded R11 XOR K12
	printf("\n...                     Expanded_R11 XOR K12: ", "");
	for(v = 0; v < 48; v++)
	{
		if(expand_PrevR[v] == k12[v])
			XOR[v] = 0;
		else
			XOR[v] = 1;
		printf("%d", XOR[v]);
		if(fmod(v+1, 6)==0)
		printf(" ","");
	}
	
	// Processing the expanded R11 XOR K12, in order to transform each 6 bit in 4 bit (using de S-Boxes)
	
	// First 6-bit block
	printf("\n...                            Sub-block 1...","");
	subBlockStart = 0;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S1 Value: %d ", S1[(row*16)+column]);
	sendBinaryToF(S1[(row*16)+column], 1); // Send the first 4-bit to S array

	// Second 6-bit block
	printf("\n...                            Sub-block 2...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S2 Value: %d ", S2[(row*16)+column]);
	sendBinaryToF(S2[(row*16)+column], 2); // Send the second 4-bit block to S array
	
	// Third 6-bit block
	printf("\n...                            Sub-block 3...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S3 Value: %d ", S3[(row*16)+column]);
	sendBinaryToF(S3[(row*16)+column], 3); // Send the third 4-bit block to S array
	
	// Fourth 6-bit block
	printf("\n...                            Sub-block 4...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S4 Value: %d ", S4[(row*16)+column]);
	sendBinaryToF(S4[(row*16)+column], 4); // Send the fourth 4-bit block to S array

	// Fifth 6-bit block
	printf("\n...                            Sub-block 5...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S5 Value: %d ", S5[(row*16)+column]);
	sendBinaryToF(S5[(row*16)+column], 5); // Send the fifth 4-bit block to S array

	// Sixth 6-bit block
	printf("\n...                            Sub-block 6...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S6 Value: %d ", S6[(row*16)+column]);
	sendBinaryToF(S6[(row*16)+column], 6); // Send the sixth 4-bit block to S array
	
	// Seventh 6-bit block
	printf("\n...                            Sub-block 7...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S7 Value: %d ", S7[(row*16)+column]);
	sendBinaryToF(S7[(row*16)+column], 7); // Send the seventh 4-bit block to S array

	// Eighth 6-bit block
	printf("\n...                            Sub-block 8...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S8 Value: %d ", S8[(row*16)+column]);
	sendBinaryToF(S8[(row*16)+column], 8); // Send the eighth 4-bit block to S array

	// Printing the result of applying the all 8 S-boxes (completing f function result)
	printf("\n...                                        S: ", ""); 
	for(v = 0; v < 32; v++)
	{
		printf("%d", S[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// Applying P permutation table to S array, to obtain the final result of F function, and print it
	printf("\n...          Applying P permutation table...", "");
	printf("\n...                      Function result - F: ", ""); 
	for(v = 0; v < 32; v++)
	{
		F[v] = S[P[v]];
		printf("%d", F[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// XOR L11 and F, to obtain R12
	printf("\n...    (+) L11 XOR F - R12: ", "");
	for(v = 0; v < 32; v++)
	{
		if(L11[v] == F[v])
			R12[v] = 0;
		else
			R12[v] = 1;
		printf("%d", R12[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}
	// ---------------------------------------------- END (L12 R12) -----------------------------------------------------

	// ******************************************** L13 R13 calculation *********************************************
	printf("\n\n...        * Round 13...", "");
	// Set L13 from R12 aray
	printf("\n...                   L13: ", "");
	for(v = 0; v < 32; v++)
	{
		L13[v] = R12[v];
		printf("%d", L13[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	printf("\n...        Calculating R13 = L12 + f(E(R12),K13)... (+)", "");

	// Expanding R12
	printf("\n...           Applying E-bit selection table...", "");
	printf("\n...                             Expanded R12: ", "");
	for(v = 0; v < 48; v++)
	{
		expand_PrevR[v] = R12[eBitSelection[v]];
		printf("%d", expand_PrevR[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	// Calculating expanded R12 XOR K13
	printf("\n...                     Expanded_R12 XOR K13: ", "");
	for(v = 0; v < 48; v++)
	{
		if(expand_PrevR[v] == k13[v])
			XOR[v] = 0;
		else
			XOR[v] = 1;
		printf("%d", XOR[v]);
		if(fmod(v+1, 6)==0)
		printf(" ","");
	}
	
	// Processing the expanded R12 XOR K13, in order to transform each 6 bit in 4 bit (using de S-Boxes)
	
	// First 6-bit block
	printf("\n...                            Sub-block 1...","");
	subBlockStart = 0;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S1 Value: %d ", S1[(row*16)+column]);
	sendBinaryToF(S1[(row*16)+column], 1); // Send the first 4-bit to S array

	// Second 6-bit block
	printf("\n...                            Sub-block 2...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S2 Value: %d ", S2[(row*16)+column]);
	sendBinaryToF(S2[(row*16)+column], 2); // Send the second 4-bit block to S array
	
	// Third 6-bit block
	printf("\n...                            Sub-block 3...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S3 Value: %d ", S3[(row*16)+column]);
	sendBinaryToF(S3[(row*16)+column], 3); // Send the third 4-bit block to S array
	
	// Fourth 6-bit block
	printf("\n...                            Sub-block 4...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S4 Value: %d ", S4[(row*16)+column]);
	sendBinaryToF(S4[(row*16)+column], 4); // Send the fourth 4-bit block to S array

	// Fifth 6-bit block
	printf("\n...                            Sub-block 5...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S5 Value: %d ", S5[(row*16)+column]);
	sendBinaryToF(S5[(row*16)+column], 5); // Send the fifth 4-bit block to S array

	// Sixth 6-bit block
	printf("\n...                            Sub-block 6...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S6 Value: %d ", S6[(row*16)+column]);
	sendBinaryToF(S6[(row*16)+column], 6); // Send the sixth 4-bit block to S array
	
	// Seventh 6-bit block
	printf("\n...                            Sub-block 7...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S7 Value: %d ", S7[(row*16)+column]);
	sendBinaryToF(S7[(row*16)+column], 7); // Send the seventh 4-bit block to S array

	// Eighth 6-bit block
	printf("\n...                            Sub-block 8...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S8 Value: %d ", S8[(row*16)+column]);
	sendBinaryToF(S8[(row*16)+column], 8); // Send the eighth 4-bit block to S array

	// Printing the result of applying the all 8 S-boxes (completing f function result)
	printf("\n...                                        S: ", ""); 
	for(v = 0; v < 32; v++)
	{
		printf("%d", S[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// Applying P permutation table to S array, to obtain the final result of F function, and print it
	printf("\n...          Applying P permutation table...", "");
	printf("\n...                      Function result - F: ", ""); 
	for(v = 0; v < 32; v++)
	{
		F[v] = S[P[v]];
		printf("%d", F[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// XOR L12 and F, to obtain R13
	printf("\n...    (+) L12 XOR F - R13: ", "");
	for(v = 0; v < 32; v++)
	{
		if(L12[v] == F[v])
			R13[v] = 0;
		else
			R13[v] = 1;
		printf("%d", R13[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}
	// ---------------------------------------------- END (L13 R13) -----------------------------------------------------

	// ******************************************** L14 R14 calculation *********************************************
	printf("\n\n...        * Round 14...", "");
	// Set L14 from R13 aray
	printf("\n...                   L14: ", "");
	for(v = 0; v < 32; v++)
	{
		L14[v] = R13[v];
		printf("%d", L14[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	printf("\n...        Calculating R14 = L13 + f(E(R13),K14)... (+)", "");

	// Expanding R13
	printf("\n...           Applying E-bit selection table...", "");
	printf("\n...                             Expanded R13: ", "");
	for(v = 0; v < 48; v++)
	{
		expand_PrevR[v] = R13[eBitSelection[v]];
		printf("%d", expand_PrevR[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	// Calculating expanded R13 XOR K14
	printf("\n...                     Expanded_R13 XOR K14: ", "");
	for(v = 0; v < 48; v++)
	{
		if(expand_PrevR[v] == k14[v])
			XOR[v] = 0;
		else
			XOR[v] = 1;
		printf("%d", XOR[v]);
		if(fmod(v+1, 6)==0)
		printf(" ","");
	}
	
	// Processing the expanded R13 XOR K14, in order to transform each 6 bit in 4 bit (using de S-Boxes)
	
	// First 6-bit block
	printf("\n...                            Sub-block 1...","");
	subBlockStart = 0;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S1 Value: %d ", S1[(row*16)+column]);
	sendBinaryToF(S1[(row*16)+column], 1); // Send the first 4-bit to S array

	// Second 6-bit block
	printf("\n...                            Sub-block 2...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S2 Value: %d ", S2[(row*16)+column]);
	sendBinaryToF(S2[(row*16)+column], 2); // Send the second 4-bit block to S array
	
	// Third 6-bit block
	printf("\n...                            Sub-block 3...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S3 Value: %d ", S3[(row*16)+column]);
	sendBinaryToF(S3[(row*16)+column], 3); // Send the third 4-bit block to S array
	
	// Fourth 6-bit block
	printf("\n...                            Sub-block 4...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S4 Value: %d ", S4[(row*16)+column]);
	sendBinaryToF(S4[(row*16)+column], 4); // Send the fourth 4-bit block to S array

	// Fifth 6-bit block
	printf("\n...                            Sub-block 5...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S5 Value: %d ", S5[(row*16)+column]);
	sendBinaryToF(S5[(row*16)+column], 5); // Send the fifth 4-bit block to S array

	// Sixth 6-bit block
	printf("\n...                            Sub-block 6...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S6 Value: %d ", S6[(row*16)+column]);
	sendBinaryToF(S6[(row*16)+column], 6); // Send the sixth 4-bit block to S array
	
	// Seventh 6-bit block
	printf("\n...                            Sub-block 7...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S7 Value: %d ", S7[(row*16)+column]);
	sendBinaryToF(S7[(row*16)+column], 7); // Send the seventh 4-bit block to S array

	// Eighth 6-bit block
	printf("\n...                            Sub-block 8...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S8 Value: %d ", S8[(row*16)+column]);
	sendBinaryToF(S8[(row*16)+column], 8); // Send the eighth 4-bit block to S array

	// Printing the result of applying the all 8 S-boxes (completing f function result)
	printf("\n...                                        S: ", ""); 
	for(v = 0; v < 32; v++)
	{
		printf("%d", S[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// Applying P permutation table to S array, to obtain the final result of F function, and print it
	printf("\n...          Applying P permutation table...", "");
	printf("\n...                      Function result - F: ", ""); 
	for(v = 0; v < 32; v++)
	{
		F[v] = S[P[v]];
		printf("%d", F[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// XOR L13 and F, to obtain R14
	printf("\n...    (+) L13 XOR F - R14: ", "");
	for(v = 0; v < 32; v++)
	{
		if(L13[v] == F[v])
			R14[v] = 0;
		else
			R14[v] = 1;
		printf("%d", R14[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}
	// ---------------------------------------------- END (L14 R14) -----------------------------------------------------

	// ******************************************** L15 R15 calculation *********************************************
	printf("\n\n...        * Round 15...", "");
	// Set L15 from R14 aray
	printf("\n...                   L15: ", "");
	for(v = 0; v < 32; v++)
	{
		L15[v] = R14[v];
		printf("%d", L15[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	printf("\n...        Calculating R15 = L14 + f(E(R14),K15)... (+)", "");

	// Expanding R14
	printf("\n...           Applying E-bit selection table...", "");
	printf("\n...                             Expanded R14: ", "");
	for(v = 0; v < 48; v++)
	{
		expand_PrevR[v] = R14[eBitSelection[v]];
		printf("%d", expand_PrevR[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	// Calculating expanded R14 XOR K15
	printf("\n...                     Expanded_R14 XOR K15: ", "");
	for(v = 0; v < 48; v++)
	{
		if(expand_PrevR[v] == k15[v])
			XOR[v] = 0;
		else
			XOR[v] = 1;
		printf("%d", XOR[v]);
		if(fmod(v+1, 6)==0)
		printf(" ","");
	}
	
	// Processing the expanded R14 XOR K15, in order to transform each 6 bit in 4 bit (using de S-Boxes)
	
	// First 6-bit block
	printf("\n...                            Sub-block 1...","");
	subBlockStart = 0;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S1 Value: %d ", S1[(row*16)+column]);
	sendBinaryToF(S1[(row*16)+column], 1); // Send the first 4-bit to S array

	// Second 6-bit block
	printf("\n...                            Sub-block 2...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S2 Value: %d ", S2[(row*16)+column]);
	sendBinaryToF(S2[(row*16)+column], 2); // Send the second 4-bit block to S array
	
	// Third 6-bit block
	printf("\n...                            Sub-block 3...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S3 Value: %d ", S3[(row*16)+column]);
	sendBinaryToF(S3[(row*16)+column], 3); // Send the third 4-bit block to S array
	
	// Fourth 6-bit block
	printf("\n...                            Sub-block 4...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S4 Value: %d ", S4[(row*16)+column]);
	sendBinaryToF(S4[(row*16)+column], 4); // Send the fourth 4-bit block to S array

	// Fifth 6-bit block
	printf("\n...                            Sub-block 5...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S5 Value: %d ", S5[(row*16)+column]);
	sendBinaryToF(S5[(row*16)+column], 5); // Send the fifth 4-bit block to S array

	// Sixth 6-bit block
	printf("\n...                            Sub-block 6...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S6 Value: %d ", S6[(row*16)+column]);
	sendBinaryToF(S6[(row*16)+column], 6); // Send the sixth 4-bit block to S array
	
	// Seventh 6-bit block
	printf("\n...                            Sub-block 7...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S7 Value: %d ", S7[(row*16)+column]);
	sendBinaryToF(S7[(row*16)+column], 7); // Send the seventh 4-bit block to S array

	// Eighth 6-bit block
	printf("\n...                            Sub-block 8...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S8 Value: %d ", S8[(row*16)+column]);
	sendBinaryToF(S8[(row*16)+column], 8); // Send the eighth 4-bit block to S array

	// Printing the result of applying the all 8 S-boxes (completing f function result)
	printf("\n...                                        S: ", ""); 
	for(v = 0; v < 32; v++)
	{
		printf("%d", S[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// Applying P permutation table to S array, to obtain the final result of F function, and print it
	printf("\n...          Applying P permutation table...", "");
	printf("\n...                      Function result - F: ", ""); 
	for(v = 0; v < 32; v++)
	{
		F[v] = S[P[v]];
		printf("%d", F[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// XOR L14 and F, to obtain R15
	printf("\n...    (+) L14 XOR F - R15: ", "");
	for(v = 0; v < 32; v++)
	{
		if(L14[v] == F[v])
			R15[v] = 0;
		else
			R15[v] = 1;
		printf("%d", R15[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}
	// ---------------------------------------------- END (L15 R15) -----------------------------------------------------

	// ******************************************** L16 R16 calculation *********************************************
	printf("\n\n...        * Round 16...", "");
	// Set L16 from R15 aray
	printf("\n...                   L16: ", "");
	for(v = 0; v < 32; v++)
	{
		L16[v] = R15[v];
		printf("%d", L16[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	printf("\n...        Calculating R16 = L15 + f(E(R15),K16)... (+)", "");

	// Expanding R15
	printf("\n...           Applying E-bit selection table...", "");
	printf("\n...                             Expanded R15: ", "");
	for(v = 0; v < 48; v++)
	{
		expand_PrevR[v] = R15[eBitSelection[v]];
		printf("%d", expand_PrevR[v]);
		if(fmod(v+1, 8)==0)
		printf(" ","");
	}

	// Calculating expanded R15 XOR K16
	printf("\n...                     Expanded_R15 XOR K16: ", "");
	for(v = 0; v < 48; v++)
	{
		if(expand_PrevR[v] == k16[v])
			XOR[v] = 0;
		else
			XOR[v] = 1;
		printf("%d", XOR[v]);
		if(fmod(v+1, 6)==0)
		printf(" ","");
	}
	
	// Processing the expanded R15 XOR K16, in order to transform each 6 bit in 4 bit (using de S-Boxes)
	
	// First 6-bit block
	printf("\n...                            Sub-block 1...","");
	subBlockStart = 0;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S1 Value: %d ", S1[(row*16)+column]);
	sendBinaryToF(S1[(row*16)+column], 1); // Send the first 4-bit to S array

	// Second 6-bit block
	printf("\n...                            Sub-block 2...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S2 Value: %d ", S2[(row*16)+column]);
	sendBinaryToF(S2[(row*16)+column], 2); // Send the second 4-bit block to S array
	
	// Third 6-bit block
	printf("\n...                            Sub-block 3...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S3 Value: %d ", S3[(row*16)+column]);
	sendBinaryToF(S3[(row*16)+column], 3); // Send the third 4-bit block to S array
	
	// Fourth 6-bit block
	printf("\n...                            Sub-block 4...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S4 Value: %d ", S4[(row*16)+column]);
	sendBinaryToF(S4[(row*16)+column], 4); // Send the fourth 4-bit block to S array

	// Fifth 6-bit block
	printf("\n...                            Sub-block 5...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S5 Value: %d ", S5[(row*16)+column]);
	sendBinaryToF(S5[(row*16)+column], 5); // Send the fifth 4-bit block to S array

	// Sixth 6-bit block
	printf("\n...                            Sub-block 6...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S6 Value: %d ", S6[(row*16)+column]);
	sendBinaryToF(S6[(row*16)+column], 6); // Send the sixth 4-bit block to S array
	
	// Seventh 6-bit block
	printf("\n...                            Sub-block 7...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S7 Value: %d ", S7[(row*16)+column]);
	sendBinaryToF(S7[(row*16)+column], 7); // Send the seventh 4-bit block to S array

	// Eighth 6-bit block
	printf("\n...                            Sub-block 8...","");
	subBlockStart += 6;
	row = XOR[subBlockStart+0]*pow(2,1) + XOR[subBlockStart+5]*pow(2,0);
	column = XOR[subBlockStart+1]*pow(2,3) + XOR[subBlockStart+2]*pow(2,2) + XOR[subBlockStart+3]*pow(2,1) + XOR[subBlockStart+4]*pow(2,0);
	printf("\n...                                 Row, Column: %d, %d ", row, column);
	printf("\n...                                    S8 Value: %d ", S8[(row*16)+column]);
	sendBinaryToF(S8[(row*16)+column], 8); // Send the eighth 4-bit block to S array

	// Printing the result of applying the all 8 S-boxes (completing f function result)
	printf("\n...                                        S: ", ""); 
	for(v = 0; v < 32; v++)
	{
		printf("%d", S[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// Applying P permutation table to S array, to obtain the final result of F function, and print it
	printf("\n...          Applying P permutation table...", "");
	printf("\n...                      Function result - F: ", ""); 
	for(v = 0; v < 32; v++)
	{
		F[v] = S[P[v]];
		printf("%d", F[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	// XOR L15 and F, to obtain R16
	printf("\n...    (+) L15 XOR F - R16: ", "");
	for(v = 0; v < 32; v++)
	{
		if(L15[v] == F[v])
			R16[v] = 0;
		else
			R16[v] = 1;
		printf("%d", R16[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}
	// ---------------------------------------------- END (L16 R16) -----------------------------------------------------

	// Reverse L16R16 to R16L16
	printf("\n...               Reversing L16R16 to R16L16... ", "");  
	for(v = 0; v < 32; v++)
	{
		temp_block[v+32] = L16[v];
		temp_block[v] = R16[v];
	}

	printf("\n...                                   R16L16: ", "");
	for(v = 0; v < 64; v++)
	{
		printf("%d", temp_block[v]);
		if(fmod(v+1, 4)==0)
		printf(" ","");
	}

	printf("\n\n...encrypted bynary block: ", "");
	for(v = 0; v < 64; v++)
	{
		cipherBin[v] = temp_block[ip_1[v]];
		printf("%d", cipherBin[v]);
		if(fmod(v+1, 8)==0)
			printf(" ","");
	}

	// Convert (each 8-bit of cipherBin sub-block) the cipher block to ascii
	printf("\n\n... encrypted ascii block: ", "");
	for(v = 0; v < 8; v++)
	{
		cipherMessageAscii[v] = cipherBin[v*8]*pow(2,7) + cipherBin[v*8+1]*pow(2,6) + cipherBin[v*8+2]*pow(2,5) + cipherBin[v*8+3]*pow(2,4) + cipherBin[v*8+4]*pow(2,3) + cipherBin[v*8+5]*pow(2,2) + cipherBin[v*8+6]*pow(2,1) + cipherBin[v*8+7]*pow(2,0);
		printf("%d ", cipherMessageAscii[v]);
	}	

	printf("\n... encrypted ascii block: ", "");
	for(v = 0; v < 8; v++)
	{
		cipherMessageChar[v] = (char)cipherMessageAscii[v];
		printf("%c", cipherMessageChar[v]);
	}
	
	//for(v = 0; v < 8; v++)
	//{
		//fwrite(&cipherMessageChar[v], sizeof(char), 1, enc);
		fwrite(cipherMessageChar, sizeof(char), 8, enc);
	//}
	
	 for(v = 0; v < 64; v++)
	 	temp_block[v] = '\0';
  }
  
  fclose(f);
  fclose(f1);
  fclose(enc);

  return 0;
}

// ************************************************** F u n c t i o n s **********************************************

// ********************** Convert each hexadecimal representation to a binary string **********************
void hexa2bin(char c)
{
	switch(tolower(c))
	{
	case '0': strcat((char *)charKey64, "0000"); break;
	case '1': strcat((char *)charKey64, "0001"); break;
	case '2': strcat((char *)charKey64, "0010"); break;
	case '3': strcat((char *)charKey64, "0011"); break;
	case '4': strcat((char *)charKey64, "0100"); break;
	case '5': strcat((char *)charKey64, "0101"); break;
	case '6': strcat((char *)charKey64, "0110"); break;
	case '7': strcat((char *)charKey64, "0111"); break;
	case '8': strcat((char *)charKey64, "1000"); break;
	case '9': strcat((char *)charKey64, "1001"); break;
	case 'a': strcat((char *)charKey64, "1010"); break;
	case 'b': strcat((char *)charKey64, "1011"); break;
	case 'c': strcat((char *)charKey64, "1100"); break;
	case 'd': strcat((char *)charKey64, "1101"); break;
	case 'e': strcat((char *)charKey64, "1110"); break;
	case 'f': strcat((char *)charKey64, "1111"); break;
	}
}


// ********************** Convert a hexadecimal number (key) to binary **********************
void convertHexadecimalToBinary(char list[])
{
	printf("> 64 bit hexadecimal key: %s\n", list);
	int i;

	for(i = 0; i < 16; i++)
	{
		hexa2bin(list[i]);
	}

	for(i = 0; i < 64; i++)
	{
		if(charKey64[i]=='0')
			binKey64[i]=0;
		else
			binKey64[i]=1;
	}
	
	printf("...    64 bit binary key: %s", "");
	for(i = 0; i < 64; i++)
	{
		printf("%d", binKey64[i]);
		if(fmod(i+1, 4)==0)
		printf(" ","");
	}
}


// ********************** Convert a decimal number to its binary form **********************
void convertDecimalToBinary(char list[])
{
	printf("\n\n> 64 bit plain text block: %s", list);
	int i, j, cv, pos, p;

	int power[8] = {1,2,4,8,16,32,64,128};
	int myByte[8];

	for(i = 0; i < 8; i++)
	 	myByte[i] = '\0';

	for(i = 0; i <= 7; i++)
	{
		cv = (int)list[i];

		for(j = 7; j >= 0; j--)
		{
			if(cv >= power[j])
			{
				myByte[7-j] = 1;
				cv = cv - power[j];
			}
			else
			{
				myByte[7-j] = 0;
			}
		}

		for(j = 0; j <= 7; j++)
		{
			binBlock[(i*8)+j]=myByte[j];
		}
	}

		printf("\n...   64 bit binary block: ", "");
		for(p=0; p <= 63; p++)
		{
			printf("%d", binBlock[p]);
			if(fmod(p+1, 8)==0)
				printf(" ","");
		}
}


// ********************** Generate the C and D pairs **********************
void generateCD()
{
	int i;
	
	// ** C0D0 **
	printf("\n...                 C0D0: ", "");
	for(i=0; i < 56; i++)
	{
		printf("%d", binKey56[i]);
		if(fmod(i+1, 7)==0)
			printf(" ","");
	}
	
	// ** C1D1 **
	for(i=0; i < 55; i++)
	{
		C1D1[i] = binKey56[i+1];
	}
	C1D1[27]=binKey56[0];
	C1D1[55]=binKey56[28];
	printf("\n...                 C1D1: ", "");
	for(i=0; i < 56; i++)
	{
		printf("%d", C1D1[i]);
		if(fmod(i+1, 7)==0)
			printf(" ","");
	}
	
	// ** C2D2 **
	for(i=0; i < 55; i++)
	{
		C2D2[i] = C1D1[i+1];
	}
	C2D2[27]=C1D1[0];
	C2D2[55]=C1D1[28];
	printf("\n                    C2D2: ", "");
	for(i=0; i < 56; i++)
	{
		printf("%d", C2D2[i]);
		if(fmod(i+1, 7)==0)
			printf(" ","");
	}
	
	// ** C3D3 **
	for(i=0; i < 54; i++)
	{
		C3D3[i] = C2D2[i+2];
	}
	C3D3[27]=C2D2[1];
	C3D3[26]=C2D2[0];
	C3D3[55]=C2D2[29];
	C3D3[54]=C2D2[28];
	printf("\n                    C3D3: ", "");
	for(i=0; i < 56; i++)
	{
		printf("%d", C3D3[i]);
		if(fmod(i+1, 7)==0)
			printf(" ","");
	}
	
	// ** C4D4 **
	for(i=0; i < 54; i++)
	{
		C4D4[i] = C3D3[i+2];
	}
	C4D4[27]=C3D3[1];
	C4D4[26]=C3D3[0];
	C4D4[55]=C3D3[29];
	C4D4[54]=C3D3[28];
	printf("\n                    C4D4: ", "");
	for(i=0; i < 56; i++)
	{
		printf("%d", C4D4[i]);
		if(fmod(i+1, 7)==0)
			printf(" ","");
	}

	// ** C5D5 **
	for(i=0; i < 54; i++)
	{
		C5D5[i] = C4D4[i+2];
	}
	C5D5[27]=C4D4[1];
	C5D5[26]=C4D4[0];
	C5D5[55]=C4D4[29];
	C5D5[54]=C4D4[28];
	printf("\n                    C5D5: ", "");
	for(i=0; i < 56; i++)
	{
		printf("%d", C5D5[i]);
		if(fmod(i+1, 7)==0)
			printf(" ","");
	}
	
	// ** C6D6 **
	for(i=0; i < 54; i++)
	{
		C6D6[i] = C5D5[i+2];
	}
	C6D6[27]=C5D5[1];
	C6D6[26]=C5D5[0];
	C6D6[55]=C5D5[29];
	C6D6[54]=C5D5[28];
	printf("\n                    C6D6: ", "");
	for(i=0; i < 56; i++)
	{
		printf("%d", C6D6[i]);
		if(fmod(i+1, 7)==0)
			printf(" ","");
	}

	// ** C7D7 **
	for(i=0; i < 54; i++)
	{
		C7D7[i] = C6D6[i+2];
	}
	C7D7[27]=C6D6[1];
	C7D7[26]=C6D6[0];
	C7D7[55]=C6D6[29];
	C7D7[54]=C6D6[28];
	printf("\n                    C7D7: ", "");
	for(i=0; i < 56; i++)
	{
		printf("%d", C7D7[i]);
		if(fmod(i+1, 7)==0)
			printf(" ","");
	}

	// ** C8D8 **
	for(i=0; i < 54; i++)
	{
		C8D8[i] = C7D7[i+2];
	}
	C8D8[27]=C7D7[1];
	C8D8[26]=C7D7[0];
	C8D8[55]=C7D7[29];
	C8D8[54]=C7D7[28];
	printf("\n                    C8D8: ", "");
	for(i=0; i < 56; i++)
	{
		printf("%d", C8D8[i]);
		if(fmod(i+1, 7)==0)
			printf(" ","");
	}

	// ** C9D9 **
	for(i=0; i < 55; i++)
	{
		C9D9[i] = C8D8[i+1];
	}
	C9D9[27]=C8D8[0];
	C9D9[55]=C8D8[28];
	printf("\n                    C9D9: ", "");
	for(i=0; i < 56; i++)
	{
		printf("%d", C9D9[i]);
		if(fmod(i+1, 7)==0)
			printf(" ","");
	}

	// ** C10D10 **
	for(i=0; i < 54; i++)
	{
		C10D10[i] = C9D9[i+2];
	}
	C10D10[27]=C9D9[1];
	C10D10[26]=C9D9[0];
	C10D10[55]=C9D9[29];
	C10D10[54]=C9D9[28];
	printf("\n                  C10D10: ", "");
	for(i=0; i < 56; i++)
	{
		printf("%d", C10D10[i]);
		if(fmod(i+1, 7)==0)
			printf(" ","");
	}

	// ** C11D11 **
	for(i=0; i < 54; i++)
	{
		C11D11[i] = C10D10[i+2];
	}
	C11D11[27]=C10D10[1];
	C11D11[26]=C10D10[0];
	C11D11[55]=C10D10[29];
	C11D11[54]=C10D10[28];
	printf("\n                  C11D11: ", "");
	for(i=0; i < 56; i++)
	{
		printf("%d", C11D11[i]);
		if(fmod(i+1, 7)==0)
			printf(" ","");
	}

	// ** C12D12 **
	for(i=0; i < 54; i++)
	{
		C12D12[i] = C11D11[i+2];
	}
	C12D12[27]=C11D11[1];
	C12D12[26]=C11D11[0];
	C12D12[55]=C11D11[29];
	C12D12[54]=C11D11[28];
	printf("\n                  C12D12: ", "");
	for(i=0; i < 56; i++)
	{
		printf("%d", C12D12[i]);
		if(fmod(i+1, 7)==0)
			printf(" ","");
	}

	// ** C13D13 **
	for(i=0; i < 54; i++)
	{
		C13D13[i] = C12D12[i+2];
	}
	C13D13[27]=C12D12[1];
	C13D13[26]=C12D12[0];
	C13D13[55]=C12D12[29];
	C13D13[54]=C12D12[28];
	printf("\n                  C13D13: ", "");
	for(i=0; i < 56; i++)
	{
		printf("%d", C13D13[i]);
		if(fmod(i+1, 7)==0)
			printf(" ","");
	}

	// ** C14D14 **
	for(i=0; i < 54; i++)
	{
		C14D14[i] = C13D13[i+2];
	}
	C14D14[27]=C13D13[1];
	C14D14[26]=C13D13[0];
	C14D14[55]=C13D13[29];
	C14D14[54]=C13D13[28];
	printf("\n                  C14D14: ", "");
	for(i=0; i < 56; i++)
	{
		printf("%d", C14D14[i]);
		if(fmod(i+1, 7)==0)
			printf(" ","");
	}

	// ** C15D15 **
	for(i=0; i < 54; i++)
	{
		C15D15[i] = C14D14[i+2];
	}
	C15D15[27]=C14D14[1];
	C15D15[26]=C14D14[0];
	C15D15[55]=C14D14[29];
	C15D15[54]=C14D14[28];
	printf("\n                  C15D15: ", "");
	for(i=0; i < 56; i++)
	{
		printf("%d", C15D15[i]);
		if(fmod(i+1, 7)==0)
			printf(" ","");
	}

	// ** C16D16 **
	for(i=0; i < 55; i++)
	{
		C16D16[i] = C15D15[i+1];
	}
	C16D16[27]=C15D15[0];;
	C16D16[55]=C15D15[28];
	printf("\n                  C16D16: ", "");
	for(i=0; i < 56; i++)
	{
		printf("%d", C16D16[i]);
		if(fmod(i+1, 7)==0)
			printf(" ","");
	}
	
}

// ********************** Convert a number x and send its binary representation to the array f (function) ********************** 
void sendBinaryToF(int x, int blockNumber)
{
	int i, j, cv, pos, p;

	int power[4] = {1,2,4,8};
	int myByte[4];

	for(i = 0; i < 4; i++)
	 	myByte[i] = '\0';

	cv = x;

		for(j = 3; j >= 0; j--)
		{
			if(cv >= power[j])
			{
				myByte[3-j] = 1;
				cv = cv - power[j];
			}
			else
			{
				myByte[3-j] = 0;
			}
		}

		for(i = 0; i < 4; i++)
	 	 S[((blockNumber-1)*4)+i] =  myByte[i];
}


// invert the 16 subkeys order, necessary to perform the decryption
void invert_key_order()
{
	int i;
	int tmp1[48], tmp2[48], tmp3[48], tmp4[48], tmp5[48], tmp6[48], tmp7[48], tmp8[48], tmp9[48], tmp10[48], tmp11[48], tmp12[48], tmp13[48], tmp14[48], tmp15[48], tmp16[48];
	
	for(i = 0; i < 48; i++)
	{
		tmp1[i] = k1[i];
		tmp2[i] = k2[i];
		tmp3[i] = k3[i];
		tmp4[i] = k4[i];
		tmp5[i] = k5[i];
		tmp6[i] = k6[i];
		tmp7[i] = k7[i];
		tmp8[i] = k8[i];
		tmp9[i] = k9[i];
		tmp10[i] = k10[i];
		tmp11[i] = k11[i];
		tmp12[i] = k12[i];
		tmp13[i] = k13[i];
		tmp14[i] = k14[i];
		tmp15[i] = k15[i];
		tmp16[i] = k16[i];
	}
	
	for(i = 0; i < 48; i++)
	{
		k1[i] = tmp16[i];
		k2[i] = tmp15[i];
		k3[i] = tmp14[i];
		k4[i] = tmp13[i];
		k5[i] = tmp12[i];
		k6[i] = tmp11[i];
		k7[i] = tmp10[i];
		k8[i] = tmp9[i];
		k9[i] = tmp8[i];
		k10[i] = tmp7[i];
		k11[i] = tmp6[i];
		k12[i] = tmp5[i];
		k13[i] = tmp4[i];
		k14[i] = tmp3[i];
		k15[i] = tmp2[i];
		k16[i] = tmp1[i];
	}
}

