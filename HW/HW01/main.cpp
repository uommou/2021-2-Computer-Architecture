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
	//output ���� ����
	printf("Hwang, Chaiwon (1976435)\n");
	FILE* fp = fopen("hw01_output.txt", "w");

	//����Ƚ�� ����
	fscanf(inputfile, "%d", &calcount);

	//���⼭���� ���� �ݺ�
	for (int count = 0; count < calcount; count++) {

		//1���� �迭�� ����� ũ�� ����
		for (i = 0; i < 3; i++) {
			fscanf(inputfile, "%d", &temp[i]);
			//printf("%d\n", temp[i]);
		}

		//�迭�� �޸� �Ҵ�
		arrA = (int**)malloc(temp[0] * sizeof(int*));
		arrB = (int**)malloc(temp[1] * sizeof(int*));
		arrC = (int**)malloc(temp[0] * sizeof(int*));

		//�迭 arrA �޸� �Ҵ�
		ptr = (int*)malloc(temp[0] * temp[1] * sizeof(int));
		for (i = 0; i < temp[0]; ++i) {
			arrA[i] = (int*)(ptr + i * temp[1]);
		}
		//�迭 arrB �޸� �Ҵ�
		ptr = (int*)malloc(temp[1] * temp[2] * sizeof(int));
		for (i = 0; i < temp[1]; ++i) {
			arrB[i] = (int*)(ptr + i * temp[2]);
		}
		//�迭 arrC �޸� �Ҵ�
		ptr = (int*)malloc(temp[0] * temp[2] * sizeof(int));
		for (i = 0; i < temp[0]; ++i) {
			arrC[i] = (int*)(ptr + i * temp[2]);
		}

		//��� A �Է�
		for (i = 0; i < temp[0]; ++i) {
			for (j = 0; j < temp[1]; ++j) {
				fscanf(inputfile, "%d", &arrA[i][j]);
				//printf("%d ", arrA[i][j]);
			}
			//printf("\n");
		}
		//printf("\n");

		//��� B �Է�
		for (i = 0; i < temp[1]; ++i) {
			for (j = 0; j < temp[2]; ++j) {
				fscanf(inputfile, "%d", &arrB[i][j]);
				//printf("%d ", arrB[i][j]);
			}
			//printf("\n");
		}
		//printf("\n");

		calculation(temp, arrA, arrB, arrC);

		//���C output ���Ͽ� �Է�
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