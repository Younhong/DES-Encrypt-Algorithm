#include <stdio.h>
#include <stdlib.h>

/* 
	Computer Security First Assignment
	21400022 ����ȫ
*/

int XOR(int a, int b);
void shiftOnce(int *A);
void shiftTwice(int *A);

unsigned _int64 getHexNum(int A[], int length);
void getBinNum(unsigned _int64 num, int *A);
void split(int text[], int *left, int *right, int length);
void combine(int* text, int left[], int right[], int length);

int main() {
	int plainTextArray[64]; // ó�� �Է¹޴� ���ڿ��� �����ϴ� �迭 // �Է¹���
	int cipherTextArray[64]; // ���� ������� 16���� ���ڿ��� ����Ǵ� �迭
	int keyTextArray[64]; // ó���� �Է¹���
	int initial_permutation[64] = { 58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8, 57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3, 61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7 };
	int parity_drop[56] = { 57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4 };
	int compression_table[48] = { 14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32 };
	int expansion_pbox[48] = { 32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17, 16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1 };
	int straight_pbox[32] = { 16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10, 2, 8, 24, 14, 32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25 };
	int final_permutation[64] = { 40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25 };
	int sbox[8][16][4] = {
		{14,0,4,15,4,15,1,12,13,7,14,8,1,4,8,2,2,14,13,4,15,2,6,9,11,13,2,1,8,1,11,7,3,10,15,5,10,6,12,11,6,12,9,3,12,11,7,14,5,9,3,10,9,5,10,0,0,3,5,6,7,8,0,13},
		{15,3,0,13,1,13,14,8,8,4,7,10,14,7,11,1,6,15,10,3,11,2,4,15,3,8,13,4,4,14,1,2,9,12,5,11,7,0,8,6,2,1,12,7,13,10,6,12,12,6,9,0,0,9,3,5,5,11,2,14,10,5,15,9},
		{10,13,13,1,0,7,6,10,9,0,4,13,14,9,9,0,6,3,8,6,3,4,15,9,15,6,3,8,5,10,0,7,1,2,11,4,13,8,1,15,12,5,2,14,7,14,12,3,11,12,5,11,4,11,10,5,2,15,14,2,8,1,7,12},
		{7,13,10,3,13,8,6,15,14,11,9,0,3,5,0,6,0,6,12,10,6,15,11,1,9,0,7,13,10,3,13,8,1,4,15,9,2,7,1,4,8,2,3,5,5,12,14,11,11,1,5,12,12,10,2,7,4,14,8,2,15,9,4,14},
		{2,14,4,11,12,11,2,8,4,2,1,12,1,12,11,7,7,4,10,1,10,7,13,14,11,13,7,2,6,1,8,13,8,5,15,6,5,0,9,15,3,15,12,0,15,10,5,9,13,3,6,10,0,9,3,4,14,8,0,5,9,6,14,3},
		{12,10,9,4,1,15,14,3,10,4,15,2,15,2,5,12,9,7,2,9,2,12,8,5,6,9,12,15,8,5,3,10,0,6,7,11,13,1,0,14,3,13,4,1,4,14,10,7,14,0,1,6,7,11,13,0,5,3,11,8,11,8,6,13},
		{4,13,1,6,11,0,4,11,2,11,11,13,14,7,13,8,15,4,12,1,0,9,3,4,8,1,7,10,13,10,14,7,3,14,10,9,12,3,15,5,9,5,6,0,7,12,8,15,5,2,0,14,10,15,5,2,6,8,9,3,1,6,2,12},
		{13,1,7,2,2,15,11,1,8,13,4,14,4,8,1,7,6,10,9,4,15,3,12,10,11,7,14,8,1,4,2,13,10,12,0,15,9,5,6,12,3,6,10,9,14,11,13,0,5,0,15,3,0,14,3,5,12,9,5,6,7,2,8,11}
	};
	int pText[64]; // permutation result �����ϴ� �迭
	int leftText[32]; // pText�� left half �迭
	int rightText[32]; // pText�� right half �迭

	int expandedRightText[48]; // rightText expand �����ϴ� �迭
	int keyGenText[56]; // ��ȣŰ�� 56bit�� ���� ���� �����ϴ� �迭
	int roundKeyText[48]; // ���� Ű �����ϴ� �迭
	int frontHalf[28]; // shift �����ϱ� ���� ���� ���� �迭
	int backHalf[28]; // shift �����ϱ� ���� ���� ������ �迭
	int pBoxResult[32]; // pbox ����� �����ϴ� �迭
	int sBoxResult[32]; // sbox ����� �����ϴ� �迭
	int temp[32]; // swap�� �� data loss ������ �迭

	unsigned _int64 plaintext, keytext; // ó�� �Է� �޴� plaintext �� keytext
	unsigned _int64 ciphertext = 0; // ���������� ����ϴ� ��ȣ�ؽ�Ʈ
	int sNum; // sbox�� ���� ���� ���� �ӽ÷� �����ϴ� ����
	
	// plaintext, keytext �Է� ����
	printf("Enter plaintext: ");
	scanf("%I64x", &plaintext);
	printf("Enter key text: ");
	scanf("%I64x", &keytext);

	// �Է¹��� �ؽ�Ʈ �޽��� ��� 
	printf("\nPlaintext Input: %.16I64x\n", plaintext);
	printf("Keytext Input: %.16I64x\n", keytext);
	
	// �Է¹��� plaintext�� 2�������� ��ȯ �� 64 size�� array�� ����
	getBinNum(plaintext, plainTextArray);
	getBinNum(keytext, keyTextArray);
	
	// initial permutation
	for (int i = 0; i < 64; i++) {
		pText[i] = plainTextArray[initial_permutation[i] - 1];
	}

	// permutation �Ϸ�� text -->  split left, right
	split(pText, leftText, rightText, 32);

	// round �����ϱ� �� L, R ���
	printf("\nRound �����ϱ� �� L: %.8I64x\n", getHexNum(leftText,8));
	printf("Round �����ϱ� �� R: %.8I64x\n", getHexNum(rightText, 8));

	// parity drop
	for (int i = 0; i < 56; i++)
		keyGenText[i] = keyTextArray[parity_drop[i] - 1];

	// 16���� round ����
	for (int round = 1; round <= 16; round++) {
		printf("\n%d��° Roound ����\n", round);

		// shift�� �����ϱ� ���� ������ ����
		split(keyGenText, frontHalf, backHalf, 28);

		// ���� ȹ���� ���� shift ����
		if ((round == 1) | (round == 2) | (round == 9) | (round == 16)) {
			shiftOnce(frontHalf);
			shiftOnce(backHalf);
		}
		else {
			shiftTwice(frontHalf);
			shiftTwice(backHalf);	
		}

		// shift ������ �Ŀ� �ٽ� ��ģ��.
		combine(keyGenText, frontHalf, backHalf, 28);

		// ���� Ű ����
		printf("%d round key: ", round);
		for (int i = 0; i < 48; i++) {
			roundKeyText[i] = keyGenText[compression_table[i] - 1];
		}

		// ���� Ű ���
		printf("%.12I64x\n", getHexNum(roundKeyText, 12));

		// right Text expansion
		// XOR
		for (int i = 0; i < 48; i++) {
			expandedRightText[i] = rightText[expansion_pbox[i] - 1];
			expandedRightText[i] = XOR(expandedRightText[i], roundKeyText[i]);
		}

		// S-box
		for (int i = 0; i < 8; i++) {
			int k = expandedRightText[i * 6] * 2 + expandedRightText[i * 6 + 5];
			int j = (expandedRightText[i * 6 + 1] * 8) + (expandedRightText[i * 6 + 2] * 4) +
				(expandedRightText[i * 6 + 3] * 2) + expandedRightText[i * 6 + 4];
			sNum = sbox[i][j][k];

			// sbox�� ���� ���� 2�������� ��ȯ
			for (j = 0; j < 4; j++) {
				sBoxResult[i * 4 + 3 - j] = sNum % 2;
				sNum = sNum / 2;
			}
		}

		// pBox ����
		for (int i = 0; i < 32; i++) {
			pBoxResult[i] = sBoxResult[straight_pbox[i] - 1];
		}
		
		// XOR and swap
		for (int i = 0; i < 32; i++) {
			temp[i] = XOR(pBoxResult[i], leftText[i]);
			leftText[i] = rightText[i];
			rightText[i] = temp[i];
		}

		// ������ round������ swap �ѹ� �� �Ѵ�.
		if (round == 16) {
			for (int i = 0; i < 32; i++) {
				temp[i] = leftText[i];
				leftText[i] = rightText[i];
				rightText[i] = temp[i];
			}
		}

		// Round Left ���
		printf("%d Round L: %.8I64x\n", round, getHexNum(leftText,8));

		// Round Right ���
		printf("%d Round R: %.8I64x\n", round, getHexNum(rightText, 8));
	}
	// 16���� round ����

	// ��� ���带 ���� ���� left�� right�� ��ģ��.
	combine(pText, leftText, rightText, 32);

	// final permutation
	for (int i = 0; i < 64; i++)
		cipherTextArray[i] = pText[final_permutation[i] - 1];

	// ��ȣ�ؽ�Ʈ ���
	printf("\nCipher Text Output: %.16I64x ", getHexNum(cipherTextArray, 16));

	return 0;
}
// 16������ 2�������� ��ȯ�ϴ� �Լ�
void getBinNum(unsigned _int64 num, int *A) {
	for (int i = 0; i < 64; i++) {
		A[63 - i] = num % 2;
		num = num / 2;
	}

	return;
}

// 2������ 16�������� ��ȯ�ϰ� ������� return���ִ� �Լ�
unsigned _int64 getHexNum(int A[], int length) {
	unsigned _int64 textNum = 0;
	for (int i = 0; i < length; i++) {
		textNum = textNum * 16;
		textNum += ((unsigned _int64) A[4 * i] * 8) + ((unsigned _int64)A[4 * i + 1] * 4) + ((unsigned _int64)A[4 * i + 2] * 2) + (unsigned _int64)A[4 * i + 3];
	}

	return textNum;
}

// XOR ������� ����ϴ� �Լ�
int XOR(int a, int b) {
	if (a == b)
		return 0;
	else
		return 1;
}

// �������� 1ĭ shift
void shiftOnce(int *A) {
	int temp = A[0];
	for (int i = 1; i < 28; i++) {
		A[i - 1] = A[i];
	}
	A[27] = temp;

	return;
}

// �������� 2ĭ shift
void shiftTwice(int *A) {
	int temp1 = A[0];
	int temp2 = A[1];

	for (int i = 2; i < 28; i++) {
		A[i - 2] = A[i];
	}

	A[26] = temp1;
	A[27] = temp2;

	return;
}

// functions to split text to half
void split(int text[], int *left, int *right, int length) {
	for (int i = 0; i < length; i++) {
		left[i] = text[i];
		right[i] = text[i + length];
	}

	return;
}

// functions to combine splitted text into one
void combine(int *text, int left[], int right[], int length) {
	for (int i = 0; i < length; i++) {
		text[i] = left[i];
		text[i + length] = right[i];
	}

	return;
}
