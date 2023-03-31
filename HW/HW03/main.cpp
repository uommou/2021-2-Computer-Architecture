/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Homework #03
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

//null 때문에 1씩 추가함
const int MAX = 101;
const int RESULT_MAX = 201;

const char *inputFileName = "input.txt";
const char *outputFileName = "hw03_output.txt";

char numberA[MAX] = {};
char numberB[MAX] = {};
char resultArray[RESULT_MAX] = {};//덧셈 뺄셈
int result;//less equal greater
int mul_Array[RESULT_MAX] = { 0, };

int operation_count = 0;//연산 횟수 저장
char tempArray[203] = {'\0', };//한 줄 씩 임시저장
char operationType;//연산자
int start_numberB_from_here;//numberB 저장용 인덱스

int i = 0;
int j = 0;
int count_digits_A = 0;
int count_digits_B = 0;
char temp;
int value;
int carry = 0;

int print_index = 0;


//연산횟수를 읽는 함수(완)
void read_operation_count(FILE* inputfile) {
	fscanf(inputfile, "%d", &operation_count);
}

//less 함수(완)
void less_operation(char numberA[MAX], char numberB[MAX]) {
	for (i = 0; i < MAX; i++) {
		if (numberA[i] != numberB[i]) {
			if (numberA[i] < numberB[i]) {
				result = 1;
				break;
			}
			else {
				result = 0;
				break;
			}
		}
		else if ((i == MAX - 1) && (numberA[i] == numberB[i]))
			result = 0;
	}
}

//equal 함수(완)
void equal_operation(char numberA[MAX], char numberB[MAX]) {
	for (i = 0; i < MAX; i++) {
		if (numberA[i] != numberB[i]) {
			result=0;
			break;
		}
		else if (i == MAX - 1)
			result=1;
	}
}

//greater 함수(완)
void greater_operation(char numberA[MAX], char numberB[MAX]) {
	for (i = 0; i < MAX; i++) {
		if (numberA[i] != numberB[i]) {
			if (numberA[i] > numberB[i]) {
				result = 1;
				break;
			}
			else {
				result = 0;
				break;
			}
		}
		else if ((i == MAX - 1) && (numberA[i] == numberB[i]))
			result = 0;
	}
}

//더하기 함수(완)
void add_operation(char numberA[MAX], char numberB[MAX]) {
	//resultArray 초기화
	for (i = 0; i < RESULT_MAX - 1; i++) {
		resultArray[i] = '0';
	}
	carry = 0;
	//연산
	for (i = MAX - 2; i >= 0; i--) {
		value = (numberA[i] - 48) + (numberB[i] - 48) + carry;
		if (value >= 10) {
			carry = 1;
			value -= 10;
		}
		else
			carry = 0;

		resultArray[100 + i] += value;
		if (i == 0)
			resultArray[100 + i - 1] += carry;
	}
}

//빼기 함수(완)
void sub_operation(char numberA[MAX], char numberB[MAX]) {
	//resultArray 초기화
	for (i = 0; i < RESULT_MAX - 1; i++) {
		resultArray[i] = '0';
	}
	carry = 0;

	//less의 반환값이 1(true)이면 결과값 음수
	less_operation(numberA, numberB);
	//결과가 0또는 양수일 경우 : A-B
	if (result == 0) {
		for (i = MAX - 2; i >= 0; i--) {
			value = (numberA[i] - 48) - (numberB[i] - 48) + carry;
			if (value < 0) {
				carry = -1;
				value += 10;
			}
			else
				carry = 0;

			resultArray[100 + i] += value;
		}
	}
	//결과가 음수일 경우 : B-A
	else if (result == 1) {
		for (i = MAX - 2; i >= 0; i--) {
			value = (numberB[i] - 48) - (numberA[i] - 48) + carry;
			if (value < 0) {
				carry = -1;
				value += 10;
			}
			else
				carry = 0;

			resultArray[100 + i] += value;
		}
	}
}

//곱하기 함수
void multiply_operation(char numberA[MAX], char numberB[MAX]) {
	
	for (i = MAX - 2; i >= 0; i--) {
		//numberA[0~MAX-2]*numberB[i]
		for (j = MAX - 2; j >= 0; j--) {
			value = (numberA[j] - 48) * (numberB[i] - 48);
			mul_Array[i + j + 1] += value % 10;
			mul_Array[i + j] += value / 10;
			mul_Array[i + j] += mul_Array[i + j + 1] / 10;
			mul_Array[i + j + 1] = mul_Array[i + j + 1] % 10;
		}
	}
	for (i = 0; i < RESULT_MAX - 1; i++) {
		resultArray[i] = mul_Array[i] + 48;
	}
}



int main(){
	//MK: YOUR NAME with Student ID
	printf("Hwang, Chai Won (1976435)\n");

	FILE* inputfile = fopen(inputFileName , "r");
	FILE* outputfile = fopen(outputFileName, "w");

	//연산횟수 읽어오기
	read_operation_count(inputfile);
	
	//남은 /n처리
	fgets(tempArray, sizeof(tempArray), inputfile);


	//여기서부터 반복문에 넣어주기
	for (int count = 0; count < operation_count; count++) {

		//첫번째 줄 읽고 임시 배열에 저장
		fgets(tempArray, sizeof(tempArray), inputfile);

		//첫번째 피연산자 임시저장
		for (i = 0; ; i++) {
			if (tempArray[i] == '+' ||
				tempArray[i] == '-' ||
				tempArray[i] == '*' ||
				tempArray[i] == '>' ||
				tempArray[i] == '=' ||
				tempArray[i] == '<') {
				//연산자저장
				operationType = tempArray[i];
				start_numberB_from_here = i + 1;
				break;
			}
			numberA[i] = tempArray[i];
		}
		//printf("%s\n", numberA);
		//printf("%c\n", operationType);

		//두번째 피연산자 임시저장
		for (i = start_numberB_from_here; ; i++) {
			if (tempArray[i] == '\n')
				break;
			numberB[j] = tempArray[i];
			j++;
		}
		//printf("%s\n", numberB);

		//피연산자 digit 세고 맨 뒤로 밀어주기
		for (i = 0; i < MAX - 1; i++) {
			if (numberA[i] != '\0')
				count_digits_A++;
			else
				numberA[i] = '0';
		}
		//printf("%s\n", numberA);

		for (i = count_digits_A; i > 0; i--) {
			temp = numberA[(MAX - count_digits_A - 1) + (i - 1)];
			numberA[(MAX - count_digits_A - 1) + (i - 1)] = numberA[i - 1];
			numberA[i - 1] = temp;
		}
		//printf("%s\n", numberA);

		//초기화
		temp = 0;
		for (i = 0; i < MAX - 1; i++) {
			if (numberB[i] != '\0')
				count_digits_B++;
			else
				numberB[i] = '0';
		}
		//printf("%s\n", numberB);

		for (i = count_digits_B; i > 0; i--) {
			temp = numberB[(MAX - count_digits_B - 1) + (i - 1)];
			numberB[(MAX - count_digits_B - 1) + (i - 1)] = numberB[i - 1];
			numberB[i - 1] = temp;
		}
		//printf("%s\n", numberB);

		//연산수행
		if (operationType == '+') {
			//더하기 연산 함수
			add_operation(numberA, numberB);

			//resultArray의 0을 제거
			for (i = 0; i < RESULT_MAX; i++) {
				if (resultArray[i] == '0') {
					print_index++;
					continue;
				}
				else
					break;
			}
			//결과값이 0일 경우
			if (print_index == RESULT_MAX - 1) {
				fprintf(outputfile, "%c", '0');
			}
			//0 아닌 경우
			for (i = print_index; i < RESULT_MAX-1; i++) {
				fprintf(outputfile, "%c", resultArray[i]);
			}
			fprintf(outputfile, "\n");
		}
		else if (operationType == '-') {
			//빼기 연산 함수
			sub_operation(numberA, numberB);
			//resultArray의 0을 건너뛰기
			for (i = 0; i < RESULT_MAX; i++) {
				if (resultArray[i] == '0') {
					print_index++;
					continue;
				}
				else
					break;
			}
			//결과값이 0일 경우
			if (print_index == RESULT_MAX - 1) {
				fprintf(outputfile, "%c", '0');
			}
			//음수일 경우 부호 추가
			less_operation(numberA, numberB);
			if (result == 1) {
				resultArray[print_index - 1] = '-';
				for (i = print_index - 1; i < RESULT_MAX-1; i++) {
					fprintf(outputfile, "%c", resultArray[i]);
				}
			}
			//양수일 경우
			else {
				for (i = print_index; i < RESULT_MAX-1; i++) {
					fprintf(outputfile, "%c", resultArray[i]);
				}
			}
			fprintf(outputfile, "\n");
		}
		else if (operationType == '*') {
			//곱하기 연산 함수
			multiply_operation(numberA, numberB);
			//resultArray의 0을 제거하고 출력
			for (i = 0; i < RESULT_MAX; i++) {
				if (resultArray[i] == '0') {
					print_index++;
					continue;
				}
				else
					break;
			}
			//결과값이 0일 경우
			if (print_index == RESULT_MAX - 1) {
				fprintf(outputfile, "%c", '0');
			}
			//0 아닌 경우
			for (i = print_index; i < RESULT_MAX-1; i++) {
				fprintf(outputfile, "%c", resultArray[i]);
			}
			fprintf(outputfile, "\n");
		}
		else if (operationType == '<') {
			//less 연산 함수
			less_operation(numberA, numberB);
			fprintf(outputfile, "%d", result);
			fprintf(outputfile, "\n");
		}
		else if (operationType == '=') {
			//equal 연산 함수
			equal_operation(numberA, numberB);
			fprintf(outputfile, "%d", result);
			fprintf(outputfile, "\n");
		}
		else if (operationType == '>') {
			//greater 연산 함수
			greater_operation(numberA, numberB);
			fprintf(outputfile, "%d", result);
			fprintf(outputfile, "\n");
		}
		else {
			printf("operationType error!");
		}

		//초기화
		for (i = 0; i < 202; i++) {
			tempArray[i] = '\0';
		}
		for (i = 0; i < MAX; i++) {
			numberA[i] = '\0';
		}
		for (i = 0; i < MAX; i++) {
			numberB[i] = '\0';
		}
		for (i = 0; i < RESULT_MAX; i++) {
			resultArray[i] = '\0';
		}
		for (i = 0; i < RESULT_MAX; i++) {
			mul_Array[i] = 0;
		}
		i = 0;
		j = 0;

		result = 0;
		start_numberB_from_here = 0;
		count_digits_A = 0;
		count_digits_B = 0;
		value = 0;
		print_index = 0;
	}
	fclose(inputfile);
	fclose(outputfile);
	return 1;
}


