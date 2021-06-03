#include <stdio.h>
#include <math.h>
#include "header.h"


int main(int argc, char* argv[]) {
	FILE* fp;
	int bin[32] = {0, };
	int i=0, num=0,inst = 0;
	int op = 0, funct = 0, rs = 0, rt = 0, rd = 0, shamt = 0, imme = 0;
	if ((fp = fopen(argv[1], "rb")) == NULL)
	{
		printf("실패");
		return 0;
	}
	while (1)
	{
		i = 0;
		while (i < 32)
		{
			num = fgetc(fp);
			if (num == EOF)
				return 0;
			if (i == 0)
				printf("inst %d: ", inst++);
			printf("%02x", num);
			for (int j = 7; j >= 0; j--)
			{
				bin[i++] = (num >> j) & 1;
			}
		}
		printf(" ");
		op = 0, funct = 0, rs = 0, rt = 0, rd = 0, shamt = 0, imme = 0;
		for (int i = 0; i < 6; i++)
		{
			op += bin[i] * pow(2, 5 - i);
		}
		if (op == 0)
		{
			for (int i = 6; i < 11; i++)
			{
				rs += bin[i] * pow(2, 10 - i);
			}
			for (int i = 11; i < 16; i++)
			{
				rt += bin[i] * pow(2, 15 - i);
			}
			for (int i = 16; i < 21; i++)
			{
				rd += bin[i] * pow(2, 20 - i);
			}
			for (int i = 21; i < 26; i++)
			{
				shamt += bin[i] * pow(2, 25 - i);
			}
			for (int i = 26; i < 32; i++)
			{
				funct += bin[i] * pow(2, 31 - i);
			}
			PrintFunct(funct, rs, rt, rd, shamt);
		}
		else if (op == 2 || op == 3) {
			for (int i = 6; i < 32; i++)
			{
				imme += bin[i] * pow(2, 31 - i);
			}
			PrintOp(op, rs, rt, imme);
		}
		else {
			for (int i = 6; i < 11; i++)
			{
				rs += bin[i] * pow(2, 10 - i);
			}
			for (int i = 11; i < 16; i++)
			{
				rt += bin[i] * pow(2, 15 - i);
			}
			imme = bin[16] * pow(2, 15) * -1;
			for (int i = 17; i < 32; i++)
			{
				imme += bin[i] * pow(2, 31 - i);
			}
			PrintOp(op, rs, rt, imme);
		}
	}
	fclose(fp);
	return 0;
}
