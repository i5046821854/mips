#ifndef REG_H
#define REG_H

int reg[32] = { 0, };
int bin[32] = { 0, };
long diff;
long diff2;
char* PC = 0x00000000;

void add(int rs, int rt, int rd)
{
	reg[rd] = reg[rs] + reg[rt];
}

void sub(int rs, int rt, int rd)
{
	reg[rd] = reg[rs] - reg[rt];
}

void and (int rs, int rt, int rd)
{
	reg[rd] = reg[rs] & reg[rt];
}

void or (int rs, int rt, int rd)
{
	reg[rd] = reg[rs] | reg[rt];
}

void slt(int rs, int rt, int rd)
{
	if (reg[rs] < reg[rt])
	{
		reg[rd] = 1;
	}
	else {
		reg[rd] = 0;
	}
}

void sll(int rd, int rt, int sa)
{
	reg[rd] = reg[rt] << sa;
}
void srl(int rd, int rt, int sa)
{
	int cmp = (0xFFFFFFFF << (32 - sa));
	reg[rd] = (reg[rt] >> sa) ^ cmp;
}
void addi(int rs, int rt, int imme) {
	reg[rt] = reg[rs] + imme;
}
void andi(int rs, int  rt, int imme) {
	imme = imme & 0x0000FFFF;
	reg[rt] = reg[rs] & imme;
}
void ori(int rs, int rt, int imme) {
	imme = imme & 0x0000FFFF;
	reg[rt] = reg[rs] | imme;
}
void lui(int rt, int imme) {
	reg[rt] = imme << 16;
}
void slti(int rs, int rt, int imme) {
	if (reg[rs] < imme)
		reg[rt] = 1;
	else
		reg[rt] = 0;
}
void lw(int rs, int rt, int imme) {
	char* ptr = (char*)(long)reg[rs];
	ptr = ptr + diff2 + imme;
	int value = 0;
	value = *(ptr++);
	if (value >= 128)
		value -= 256;
	for (int i = 0; i < 3; i++)
	{
		if(*(ptr) < 0)
			value = (value * 256) + (*(ptr++) + 256);
		else
			value = (value * 256) + *(ptr++);
	}
	reg[rt] = value;
}
void sw(int rs, int rt, int imme) {
	char* ptr = (char*)(long)reg[rs];
	ptr = ptr + diff2 + imme;
	*(ptr) = (reg[rt] >> 24) & 0x00FF;
	*(ptr + 1) = (reg[rt] >> 16) & 0x00FF;
	*(ptr + 2) = (reg[rt] >> 8) & 0x00FF;
	*(ptr + 3) = (reg[rt]) & 0x00FF;
}
void lh(int rs, int rt, int imme) {
	char* ptr = (char*)(long)reg[rs];
	ptr = ptr + diff2 + imme;
	int value;
	if (*(ptr+1) < 0)
		value = (*(ptr) * 256) + (*(ptr + 1)+256);
	else
		value = (*(ptr) * 256) + *(ptr + 1);
	
	reg[rt] = value;
}

void lhu(int rs, int rt, int imme) {
	char* ptr = (char*)(long)reg[rs];
	ptr = ptr + diff2 + imme;
	int value;
	if (*(ptr + 1) < 0)
		value = (*(ptr) * 256) + (*(ptr + 1) + 256);
	else
		value = (*(ptr) * 256) + *(ptr + 1);
	reg[rt] = value & 0x0000FFFF;
}
void sh(int rs, int rt, int imme) {
	char* ptr = (char*)(long)reg[rs];
	ptr = ptr + diff2 + imme;
	*(ptr) = (reg[rt] >> 8) & 0x00FF;
	*(ptr + 1) = (reg[rt]) & 0x00FF;
}
void lb(int rs, int rt, int imme) {
	char* ptr = (char*)(long)reg[rs];
	ptr = ptr + diff2 + imme;
	int value = *(ptr);
	reg[rt] = value;
}

void lbu(int rs, int rt, int imme) {
	char* ptr = (char*)(long)reg[rs];
	ptr = ptr + diff2 + imme;
	int value = *(ptr);
	value = value & 0x000000FF;
	reg[rt] = value;
}

void sb(int rs, int rt, int imme) {
	char* ptr = (char*)(long)reg[rs];
	ptr = ptr + diff2 + imme;
	*(ptr) = (reg[rt]) & 0x00FF;
}
void beq(int rs, int rt, int imme) {
	if (reg[rs] == reg[rt])
	{
		PC = PC + (imme * 4);
	}
	else
	{
		return;
	}
}
void bne(int rs, int rt, int imme) {
	if (reg[rs] != reg[rt])
		PC = PC + (imme * 4);
	else
		return;
}

void jump(int rs, int rt, int imme)
{
	long intPC = (long)PC;
	intPC = ((intPC + 4) & 0xF0000000) | (0x0fffffff &(imme << 2));
	PC = (char*)intPC;
}

#endif 
