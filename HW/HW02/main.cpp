/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Homework #02
 *
 *        Version:  1.0
 *        Created:  2021-09-27
 *       Revision:  none
 *       Compiler:  g++
 *
 * 	 Student Name: Hwang, Chaiwon
 * 		 Student ID: 1976435
 *   Organization:  Ewha Womans University
 *
 * =====================================================================================
 */

#include <stdio.h>

const int MAX = 10;
const char *outputFileName="hw02_output.txt";

int matrixOutput[MAX][MAX] = {0};
int matrixA[MAX][MAX] = { 0 };
int matrixB[MAX][MAX] = { 0 };
int dia_count = 0;

void print_output_to_file(FILE *fp, int ROW, int COL){
	for(int y = 0; y < ROW; y++){
		for(int x = 0; x < COL; x++){
			fprintf(fp, "%d ", matrixOutput[y][x]);
		}
		fprintf(fp, "\n");
	}
}

int main(){
	//MK: YOUR NAME with Student ID
	printf("Hwang, Chaiwon (1976435)\n");
	FILE *fp = fopen(outputFileName, "w");
	FILE* inputfile = fopen("input.txt", "r");

	//행렬 할당
	for (int y = 0; y < MAX; y++) {
		for (int x = 0; x < MAX; x++) {
			fscanf(inputfile, "%d", &matrixA[y][x]);
		}
	}
	for (int y = 0; y < MAX; y++) {
		for (int x = 0; x < MAX; x++) {
			fscanf(inputfile, "%d", &matrixB[y][x]);
		}
	}
	

	//cycle 0
	fprintf(fp, "Cycle %d\n", 0);
	print_output_to_file(fp, MAX, MAX);

	//cycle start
	for (int cycle = 0; cycle < 28; cycle++) {
		dia_count = 0;
		for (int dia = cycle; dia > cycle - 10 && dia <= cycle; dia--) {
			if (dia < 0 || dia > 20)
				continue;
			for (int i = 0; i <= dia; i++) {
				if (dia - i > 9)
					continue;
				matrixOutput[i][dia - i] += matrixA[i][9 - dia_count] * matrixB[9 - dia_count][dia - i];
			}
			dia_count++;
		}

		fprintf(fp, "Cycle %d\n", cycle+1);
		print_output_to_file(fp, MAX, MAX);
	}

	fclose(fp);
	fclose(inputfile);
	return 1;
}


