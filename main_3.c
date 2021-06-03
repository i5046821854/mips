#define _CRT_SECURE_NO_WARNINGS
#include "reg.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void instParse(char* CurInst)
{
	int elem = 0;
	for (int i = 0; i < 4; i++)
	{
		char value = *(CurInst++);
		for (int j = 7; j >= 0; j--)
		{
			bin[elem++] = (value >> j) & 1;
		}
	}
}

void regPrint(void)
{
	int i = 0;
	for (i = 0; i < 32; i++)
	{
		printf("$%d: 0x%.8x\n", i, reg[i]);
	}
	int pchar = (int)(long)PC;
	printf("PC: 0x%.8x\n", pchar);
}

void memPrint(long num, int repNum)
{
	int value;
	int i = 0;
	char* ptr = (char*)(num + diff2);
	for (int j = 0; j < repNum; j++)
	{
		value = 0;
		value = *(ptr++);
		if (value >= 128)
			value -= 256;
		for (int i = 0; i < 3; i++)
		{
			if (*(ptr) < 0)
				value = (value * 256) + (*(ptr++) + 256);
			else
				value = (value * 256) + *(ptr++);
		}
		printf("0x%.8x\n", value);
	}
}

int main(int argc, char* argv[]) {
	for (int i = 0; i < 8; i++)
	{
		memset(&set[i], 0, sizeof(Cache));
	}
	int instCnt = 0;
	int dataCnt = 0;
	int repNum = atoi(argv[2]);
	char inst[65536] = { 0, };
	char data[65536] = { 0, };
	memset(inst, -1, 65536);
	memset(data, -1, 65536);
	FILE* fp;
	int i = 0, num = 0;
	char cnum = 0;
	int op = 0, funct = 0, rs = 0, rt = 0, rd = 0, shamt = 0, imme = 0, memCnt = 0;
	if ((fp = fopen(argv[1], "rb")) == NULL)
	{
		printf("½ÇÆÐ");
		return 0;
	}
	while (1)
	{
		num = fgetc(fp);
		if (num == EOF)
			break;
		inst[instCnt++] = num;
	}
	diff = (long)(inst - PC);
	diff2 = (long)(data - 0x10000000);
	for (num = 0; num < repNum ; num++)
	{
		char* CurInst = PC + diff;
		if (*(CurInst) == -1)
		{
			printf("unknown instruction\n");
			PC = PC + 4;
			InstNum++;
			break;
		}
		InstNum++;
		instParse(CurInst);
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
			if (funct == 32)
				add(rs, rt, rd);
			else if (funct == 33)
				addu(rs, rt, rd);
			else if (funct == 34)
				sub(rs, rt, rd);
			else if (funct == 36)
				and (rs, rt, rd);
			else if (funct == 37)
				or (rs, rt, rd);
			else if (funct == 42)
				slt(rs, rt, rd);
			else if (funct == 43)
			{
				sltu(rs, rt, rd);
			}
			else if (funct == 0)
				sll(rd, rt, shamt);
			else if (funct == 2)
			{
				srl(rd, rt, shamt);
			}
			else if (funct == 8)
				jr(rs);
			else
			{
				printf("unknown instruction\n");
				break;
			}

		}
		else if (op == 2 || op == 3) {
			//jump inst
			for (int i = 6; i < 32; i++)
			{
				imme += bin[i] * pow(2, 31 - i);
			}
			if (op == 2)
			{
				jump(rs, rt, imme);
				continue;
			}
			else if (op == 3)
			{
				jal(rs, rt, imme);
				continue;
			}
			else 
			{
				printf("unknown instruction\n");
				break;
			}
		}
		else {
			//i-type
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
			if (op == 8)
				addi(rs, rt, imme);
			else if (op == 9)
				addiu(rs, rt, imme);
			else if (op == 12)
				andi(rs, rt, imme);
			else if (op == 13)
				ori(rs, rt, imme);
			else if (op == 15)
				lui(rt, imme);
			else if (op == 10)
				slti(rs, rt, imme);
			else if (op == 35)
				lw(rs, rt, imme);
			else if (op == 36)
				lbu(rs, rt, imme);
			else if (op == 37)
				lhu(rs, rt, imme);
			else if (op == 43)
				sw(rs, rt, imme);
			else if (op == 33)
				lh(rs, rt, imme);
			else if (op == 41)
				sh(rs, rt, imme);
			else if (op == 40)
				sb(rs, rt, imme);
			else if (op == 32)
				lb(rs, rt, imme);
			else if (op == 4)
			{
				//printf("%d %d %d", rs, rt, imme);
				beq(rs, rt, imme);
			}
			else if (op == 5)
				bne(rs, rt, imme);
			else
			{
				printf("unknown instruction\n");
				break;
			}
		}
		PC = PC + 4;
		reg[0] = 0;
	}
	printf("Instructions: %d\nHits: %d\nMisses: %d\nTotal: %d\n",InstNum, CH, CM, CH+CM);
	Cache* ptr = set;
	int ogAddr;
	for (int i = 0; i < 8; i++)
	{
			if ((ptr + i)->dirty1 == 1)
			{
				ogAddr = ((ptr + i)->tag1 << 9) + (i << 6);
				Mstore((ptr + i)->data1, ogAddr);
			}
			if ((ptr + i)->dirty2 == 1)
			{
				ogAddr = ((ptr + i)->tag2 << 9) + (i << 6);
				Mstore((ptr + i)->data2, ogAddr);
			}

	}
	int memAddr;
	int rep;
	if (argc >= 4)
	{
		if (strcmp(argv[3], "reg") == 0)
		{
			regPrint();
		}
		else if (strcmp(argv[3], "mem") == 0)
		{
			if (argc >= 6)
			{
				memAddr = strtol(argv[4], NULL, 16);
				rep = strtol(argv[5], NULL, 10);
				memPrint((long)memAddr, rep);
			}
		}
	}
	fclose(fp);
	return 0;

}
