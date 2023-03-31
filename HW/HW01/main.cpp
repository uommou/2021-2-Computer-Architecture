/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Homework #01
 *
 *        Version:  1.0
 *        Created:  2021-09-04
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
#include <stdlib.h>



void calculation(int size[], int* a[], int* b[], int* c[]) {
	int i, j, k;
	int result;
	for (i = 0; i < size[0]; ++i) {
		for (j = 0; j < size[2]; ++j) {
			result = 0;
			for (k = 0; k < size[1]; ++k)
				result += a[i][k] * b[k][j];
			c[i][j] = result;
		}
	}
}

int main() {
	FILE* inputfile;
	int count, i, j, k;
	int** arrA, ** arrB, ** arrC;
	int* ptr;
	int temp[3];
	int calcount;

	inputfile = fopen("input.txt", "r");

	//MK: YOUR NAME with Student ID
	//output 파일 열기
	printf("Hwang, Chaiwon (1976435)\n");
	FILE* fp = fopen("hw01_output.txt", "w");

	//연산횟수 저장
	fscanf(inputfile, "%d", &calcount);

	//여기서부터 연산 반복
	for (int count = 0; count < calcount; count++) {

		//1차원 배열에 행렬의 크기 저장
		for (i = 0; i < 3; i++) {
			fscanf(inputfile, "%d", &temp[i]);
			//printf("%d\n", temp[i]);
		}

		//배열의 메모리 할당
		arrA = (int**)malloc(temp[0] * sizeof(int*));
		arrB = (int**)malloc(temp[1] * sizeof(int*));
		arrC = (int**)malloc(temp[0] * sizeof(int*));

		//배열 arrA 메모리 할당
		ptr = (int*)malloc(temp[0] * temp[1] * sizeof(int));
		for (i = 0; i < temp[0]; ++i) {
			arrA[i] = (int*)(ptr + i * temp[1]);
		}
		//배열 arrB 메모리 할당
		ptr = (int*)malloc(temp[1] * temp[2] * sizeof(int));
		for (i = 0; i < temp[1]; ++i) {
			arrB[i] = (int*)(ptr + i * temp[2]);
		}
		//배열 arrC 메모리 할당
		ptr = (int*)malloc(temp[0] * temp[2] * sizeof(int));
		for (i = 0; i < temp[0]; ++i) {
			arrC[i] = (int*)(ptr + i * temp[2]);
		}

		//행렬 A 입력
		for (i = 0; i < temp[0]; ++i) {
			for (j = 0; j < temp[1]; ++j) {
				fscanf(inputfile, "%d", &arrA[i][j]);
				//printf("%d ", arrA[i][j]);
			}
			//printf("\n");
		}
		//printf("\n");

		//행렬 B 입력
		for (i = 0; i < temp[1]; ++i) {
			for (j = 0; j < temp[2]; ++j) {
				fscanf(inputfile, "%d", &arrB[i][j]);
				//printf("%d ", arrB[i][j]);
			}
			//printf("\n");
		}
		//printf("\n");

		calculation(temp, arrA, arrB, arrC);

		//행렬C output 파일에 입력
		for (i = 0; i < temp[0]; ++i) {
			for (j = 0; j < temp[2]; ++j) {
				fprintf(fp, "%d ", arrC[i][j]);
			}
			fprintf(fp, "\n");
		}


		
		free(arrA);
		free(arrB);
		free(arrC);
		free(ptr);
	}
	fclose(inputfile);
	fclose(fp);

	return 0;

}