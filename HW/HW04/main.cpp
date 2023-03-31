/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Homework #04
 *
 *        Version:  1.0
 *        Created:  2021-10-30
 *       Revision:  none
 *       Compiler:  g++
 *
 * 	 Student Name: Chaiwon Hwang
 * 		 Student ID: 1976435
 *   Organization:  Ewha Womans University
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <math.h>

//const char* inputFileName = "input_case10000_1.txt";
//const char* inputFileName = "input_case10000_2.txt";
const char* inputFileName = "input.txt";
//const char* inputFileName = "input2.txt";
const char *outputFileName="hw04_output.txt";

unsigned long long cacheHit;
unsigned long long cacheMiss;

int testCase;//���� ó���� �Է¹޴� ���� Ƚ��(MAX 10)

int numBlocks;//�� �� (MAX 512) <- �ε����� ���� ǥ�� �����ؾ���
int blockSize;//�� ������ (MAX 16)
/*
�� ������ ������ ��
�� ������� �Է¹޴� ���� 
4-�ּ��� 2����Ʈ ����
8-�ּ��� 3����Ʈ ����
16-�ּ��� 4����Ʈ ����
*/
int numAddress;//cache simulator�� ���� �ּ��� ��(MAX 10000)

int tempAddress;//�迭�� ������ �� ����� �ӽ� ����
int binaryAddress[10000][16] = { 0, };//�ּҰ��� ������ �迭�� ����

//�� ������ ����Ͽ� ���� ����Ʈ
int numIgnore = 0;
int numIndexBit = 1;
int numTagBit = 16 - numIgnore - numIndexBit;

//�ּ��� �±�, �ε���
int addressTag[16] = { 0, };
int addressIndex;

//ĳ�ÿ� ����� �±�
//���� ���� ��(�ε���), ���� �±�
int cacheTag[512][16] = { 0, };
//cacheState �迭�� ���� 0���� ����, index ��ġ�ϴ� �ּҰ��� ������ 1�� �ٲ��.
int cacheState[512] = { 0, };


void init(){
	cacheHit = 0;
	cacheMiss = 0;
	numIgnore = 0;
	numIndexBit = 1;

	//ĳ���ʱ�ȭ
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 16; j++) {
			cacheTag[i][j] = 0;
		}
	}
	//cacheState �ʱ�ȭ
	for (int i = 0; i < 512; i++)
		cacheState[i] = 0;

	//addressTag �ʱ�ȭ
	for (int i = 0; i < 16; i++)
		addressTag[i] = 0;
}

void ratio(FILE* outputfile){
	unsigned long long total = cacheHit + cacheMiss;
	fprintf(outputfile, "Hit: %3.2f\n", 100*(float)cacheHit/total);
	fprintf(outputfile, "Miss: %3.2f\n", 100*(float)cacheMiss/total);
}

void read_testCase(FILE* inputfile) {
	fscanf(inputfile, "%d", &testCase);
}

void read_numBlocks(FILE* inputfile) {
	fscanf(inputfile, "%d", &numBlocks);
}

void read_blockSize(FILE* inputfile) {
	fscanf(inputfile, "%d", &blockSize);
}

void read_numAddress(FILE* inputfile) {
	fscanf(inputfile, "%d", &numAddress);
}

//16���� �ּҰ��� �޾� 2���� �迭�� �ٲ��ִ� �Լ�
//������ �� : �� �� ��� > index�б� > tag�б� ���Ǹ� ���� ������ �迭�� �Ųٷ� ����Ǿ���.
void make_binaryAddress(int currentAddress) {
	for (int i = 15; i >= 0; i--) {
		binaryAddress[currentAddress][i] = tempAddress / (int)pow(2, i);
		tempAddress = tempAddress % (int)pow(2, i);
	}
	//binaryAddress[currentAddress][0] = tempAddress;
}

//2���� �迭�� �ٲ㼭 ����
void make_arrayAddress(FILE* inputfile) {
	for (int currentAddress = 0; currentAddress < numAddress; currentAddress++) {
		fscanf(inputfile, "%x", &tempAddress);
		make_binaryAddress(currentAddress);
	}
}

//�迭 ���� test ���߿� �������
//void test_make_arrayAddress() {
//	for (int i = 0; i < numAddress; i++) {
//		//�Ųٷ� ���
//		for (int j = 15; j >= 0; j--)
//			printf("%d", binaryAddress[i][j]);
//		printf("\n");
//	}
//}

void count_numIgnore(int blockSize) {
	while (blockSize >= 2) {
		numIgnore++;
		blockSize = blockSize / 2;
	}
}

void count_numIndexBit(int numBlocks) {
	while (numBlocks > 2) {
		numIndexBit++;
		numBlocks= numBlocks / 2;
	}
}

int find_Index(int currentAddress) {
	int sum = 0;
	for (int i = numIgnore; i < (numIgnore + numIndexBit); i++) {
		sum += binaryAddress[currentAddress][i] * (int)pow(2, i - numIgnore);
	}
	return sum;
}

//ĳ�ÿ� �±׸� �����ϴ� �Լ�
void save_TagInCache(int currentAddress) {
	int cacheIndex = 0;
	for (int i = 16 - numTagBit; i < 16; i++) {
		cacheTag[addressIndex][cacheIndex] = binaryAddress[currentAddress][i];
		cacheIndex++;
	}
}

int main(){

	FILE* inputfile = fopen(inputFileName, "r");
	FILE* outputfile = fopen(outputFileName, "wb");

	//MK: YOUR NAME with Student ID
	printf("Hwang, Chai Won (1976435)\n");
	init();//�ʱ�ȭ

	//test case �о����
	read_testCase(inputfile);

	//test case ��ŭ �ݺ� ����
	for (int i = 0; i < testCase; i++) {

		init();

		//number of blocks �о����
		read_numBlocks(inputfile);
		//block size �о����
		read_blockSize(inputfile);
		//number of address �о����
		read_numAddress(inputfile);

		//binaryAddress �����
		make_arrayAddress(inputfile);
		//�迭 �׽�Ʈ ���߿� ����
		//test_make_arrayAddress();

		
		//�������� ����Ͽ� ������ ��Ʈ ���
		count_numIgnore(blockSize);
		//numIgnore �׽�Ʈ ���߿� ����
		//printf("%d\n", numIgnore);

		//�ε��� ������ ���
		count_numIndexBit(numBlocks);
		//numIndexBit �׽�Ʈ ���߿� ����
		//printf("%d\n", numIndexBit);

		//�ּ��� ������ŭ �ݺ��ϱ�
		for (int currentAddress = 0; currentAddress < numAddress; currentAddress++) {

			//�ε��� ã��
			addressIndex = find_Index(currentAddress);
			//printf("%d\n", addressIndex);

			//addressIndex�� Index State Ȯ��
			/*
			No �̸� Miss, Yes�� �ٲ��ֱ�
			ĳ�ÿ� �ּ��� �±� ����
			Yes�̸� �±� �� ��(��ġ�ϸ� Hit, ����ġ�ϸ� Miss)
			ĳ�ÿ� �ּ��� �±� ����
			*/
			if (cacheState[addressIndex] == 0) {
				cacheMiss+=1.0;//No�̹Ƿ� Miss
				//printf("cachemiss\n");
				cacheState[addressIndex] = 1;//Yes�� �ٲ��ֱ�

				//�±������ϱ�
				save_TagInCache(currentAddress);
			}
			else {
				//�±׺��ϱ�
				int cacheIndex = 0;
				for (int i = 16 - numTagBit; i < 16; i++) {
					if (cacheTag[addressIndex][cacheIndex] != binaryAddress[currentAddress][i]) {
						cacheMiss+=1.0;
						//printf("cachemiss\n");
						break;
					}
					cacheIndex++;
				}
				if (cacheIndex == numTagBit){
					cacheHit+=1.0;
					//printf("cachehit\n");
				}
					
				//�±������ϱ�
				save_TagInCache(currentAddress);
			}
		}


		ratio(outputfile);//outputfile�� ����

	unsigned long long total = cacheHit + cacheMiss;
	//printf("Hit: %3.2f\n", 100*(float)cacheHit/total);
	//printf("Miss: %3.2f\n", 100*(float)cacheMiss/total);	

	}






	fclose(inputfile);
	fclose(outputfile);

	return 1;
}


