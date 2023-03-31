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

int testCase;//가장 처음에 입력받는 연산 횟수(MAX 10)

int numBlocks;//블럭 수 (MAX 512) <- 인덱스로 전부 표현 가능해야함
int blockSize;//블럭 사이즈 (MAX 16)
/*
블럭 사이즈 주의할 점
블럭 사이즈로 입력받는 수는 
4-주소의 2바이트 빼기
8-주소의 3바이트 빼기
16-주소의 4바이트 빼기
*/
int numAddress;//cache simulator에 넣을 주소의 수(MAX 10000)

int tempAddress;//배열에 저장할 때 사용할 임시 변수
int binaryAddress[10000][16] = { 0, };//주소값을 이진수 배열로 저장

//블럭 사이즈 고려하여 빼줄 바이트
int numIgnore = 0;
int numIndexBit = 1;
int numTagBit = 16 - numIgnore - numIndexBit;

//주소의 태그, 인덱스
int addressTag[16] = { 0, };
int addressIndex;

//캐시에 저장된 태그
//행은 블럭의 수(인덱스), 열은 태그
int cacheTag[512][16] = { 0, };
//cacheState 배열은 전부 0에서 시작, index 일치하는 주소값이 나오면 1로 바뀐다.
int cacheState[512] = { 0, };


void init(){
	cacheHit = 0;
	cacheMiss = 0;
	numIgnore = 0;
	numIndexBit = 1;

	//캐시초기화
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 16; j++) {
			cacheTag[i][j] = 0;
		}
	}
	//cacheState 초기화
	for (int i = 0; i < 512; i++)
		cacheState[i] = 0;

	//addressTag 초기화
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

//16진수 주소값을 받아 2진수 배열로 바꿔주는 함수
//주의할 점 : 블럭 수 고려 > index읽기 > tag읽기 편의를 위해 이진수 배열이 거꾸로 저장되었다.
void make_binaryAddress(int currentAddress) {
	for (int i = 15; i >= 0; i--) {
		binaryAddress[currentAddress][i] = tempAddress / (int)pow(2, i);
		tempAddress = tempAddress % (int)pow(2, i);
	}
	//binaryAddress[currentAddress][0] = tempAddress;
}

//2진수 배열로 바꿔서 저장
void make_arrayAddress(FILE* inputfile) {
	for (int currentAddress = 0; currentAddress < numAddress; currentAddress++) {
		fscanf(inputfile, "%x", &tempAddress);
		make_binaryAddress(currentAddress);
	}
}

//배열 저장 test 나중에 지울거임
//void test_make_arrayAddress() {
//	for (int i = 0; i < numAddress; i++) {
//		//거꾸로 출력
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

//캐시에 태그를 저장하는 함수
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
	init();//초기화

	//test case 읽어오기
	read_testCase(inputfile);

	//test case 만큼 반복 시작
	for (int i = 0; i < testCase; i++) {

		init();

		//number of blocks 읽어오기
		read_numBlocks(inputfile);
		//block size 읽어오기
		read_blockSize(inputfile);
		//number of address 읽어오기
		read_numAddress(inputfile);

		//binaryAddress 만들기
		make_arrayAddress(inputfile);
		//배열 테스트 나중에 제거
		//test_make_arrayAddress();

		
		//블럭사이즈 고려하여 무시할 비트 계산
		count_numIgnore(blockSize);
		//numIgnore 테스트 나중에 제거
		//printf("%d\n", numIgnore);

		//인덱스 사이즈 계산
		count_numIndexBit(numBlocks);
		//numIndexBit 테스트 나중에 제거
		//printf("%d\n", numIndexBit);

		//주소의 개수만큼 반복하기
		for (int currentAddress = 0; currentAddress < numAddress; currentAddress++) {

			//인덱스 찾기
			addressIndex = find_Index(currentAddress);
			//printf("%d\n", addressIndex);

			//addressIndex의 Index State 확인
			/*
			No 이면 Miss, Yes로 바꿔주기
			캐시에 주소의 태그 저장
			Yes이면 태그 비교 후(일치하면 Hit, 불일치하면 Miss)
			캐시에 주소의 태그 저장
			*/
			if (cacheState[addressIndex] == 0) {
				cacheMiss+=1.0;//No이므로 Miss
				//printf("cachemiss\n");
				cacheState[addressIndex] = 1;//Yes로 바꿔주기

				//태그저장하기
				save_TagInCache(currentAddress);
			}
			else {
				//태그비교하기
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
					
				//태그저장하기
				save_TagInCache(currentAddress);
			}
		}


		ratio(outputfile);//outputfile에 쓰기

	unsigned long long total = cacheHit + cacheMiss;
	//printf("Hit: %3.2f\n", 100*(float)cacheHit/total);
	//printf("Miss: %3.2f\n", 100*(float)cacheMiss/total);	

	}






	fclose(inputfile);
	fclose(outputfile);

	return 1;
}


