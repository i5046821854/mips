#ifndef REG_H
#define REG_H
#include <math.h>
#include <stdio.h>

int CH = 0;
int CM = 0;
int InstNum = 0;
int reg[32] = { 0, };
int bin[32] = { 0, };
long diff;
long diff2;
char* PC = 0x00000000;

int idx;
int tag;
int offset;

typedef struct {
	char LRU;
	char dirty1;
	char dirty2;
	char valid1;
	char valid2;
	int tag1;
	int tag2;
	char data1[64];
	char data2[64];
}Cache;
Cache set[8];

void Cload(char *  data, int base)
{

	long temp = (long)base + diff2;
	char* ptr = (char*)temp;
	for (int i = 0; i < 64; i++)
	{
		data[i] = *(ptr + i);
	}
}

void Mstore(char* data, int base)
{
	long temp = (long)base + diff2;
	char* ptr = (char*)temp;
	for (int i = 0; i < 64; i++)
	{
		*(ptr + i) = data[i];
	}
}

void load(int imme, int rs, int byte, int rt)
{
	int addr = reg[rs] + imme;
	char* ptr = (char*)(long)addr;
	int tag = addr >> 9 & 0x007fffff;
	int idx = addr >> 6 & 0x00000007;
	int offset = addr & 0x0000003f;
	int value = 0;
	Cache* setptr = set+idx;
	if (setptr->tag1 == tag) 
	{
		if (setptr->valid1 == 0) 
		{
			Cload(setptr->data1, addr & 0xffffffc0);
			setptr->valid1 = 1;
			CM++;
		}
		else
		{
			CH++;
		}
		value = setptr->data1[offset];
		if(value >= 128)
			value -= 256;
		for (int i = 1; i < byte; i++) 
		{
			if (setptr->data1[offset+i] < 0)
				value = (value * 256) + (setptr->data1[offset + i] + 256);
			else
				value = (value * 256) + (setptr->data1[offset + i]);
		}
		setptr->LRU = 2;
	}
	else if (setptr->tag2 == tag)  
	{
		if (setptr->valid2 == 0) 
		{
			Cload(setptr->data2, addr & 0xffffffc0);
			setptr->valid2 = 1;
			CM++;
		}
		else
		{
			CH++;
		}
		value = setptr->data2[offset];
		if(value >= 128)
			value -= 256;
		for (int i = 1; i < byte; i++) 
		{
			if (setptr->data2[offset + i] < 0)
				value = (value * 256) + (setptr->data2[offset + i] + 256);
			else
				value = (value * 256) + (setptr->data2[offset + i]);
		}
		setptr->LRU = 1;
	}
	else if (setptr->LRU == 0) 
	{
		
		CM++;
		Cload(setptr->data1, addr & 0xffffffc0);  
		value = setptr->data1[offset];
		if(value >= 128)
			value -= 256;
		for (int i = 1; i < byte; i++) 
		{
			if (setptr->data1[offset + i] < 0)
				value = (value * 256) + (setptr->data1[offset + i] + 256);
			else
				value = (value * 256) + (setptr->data1[offset + i]);
		}
		setptr->valid1 = 1;
		setptr->LRU = 2;
		setptr->tag1 = tag;
	}
	else if (setptr->LRU == 1) 
	{
		CM++;
		if (setptr->dirty1 == 1) 
		{
			int ogAddr = (setptr->tag1 << 9) + (idx << 6); 
			Mstore(setptr->data1, ogAddr); 
			setptr->dirty1 = 0;
		}
		Cload(setptr->data1, addr & 0xffffffc0);
		value = setptr->data1[offset];
		if(value >= 128)
			value -= 256;
		for (int i = 1; i < byte; i++)  
		{
			if (setptr->data1[offset + i] < 0)
				value = (value * 256) + (setptr->data1[offset + i] + 256);
			else
				value = (value * 256) + (setptr->data1[offset + i]);
		}
		setptr->valid1 = 1;
		setptr->tag1 = tag;
		setptr->LRU = 2;
	}
	else 
	{
		CM++;
		if (setptr->dirty2 == 1)
		{
			int ogAddr = (setptr->tag2 << 9) + (idx << 6); 
			Mstore(setptr->data2, ogAddr);
			setptr->dirty2 = 0;
		}
		Cload(setptr->data2, addr & 0xffffffc0);
		value = setptr->data2[offset];
		if(value >= 128)
			value -= 256;
		for (int i = 1; i < byte; i++) 
		{
			if (setptr->data2[offset + i] < 0)
				value = (value * 256) + (setptr->data2[offset + i] + 256);
			else
				value = (value * 256) + (setptr->data2[offset + i]);
		}
		setptr->valid2 = 1;
		setptr->tag2 = tag;
		setptr->LRU = 1;
	}
	reg[rt] = value;
}

void store(int imme, int rs, int byte, int rt)
{
	int addr = reg[rs] + imme;
	char* ptr = (char*)(long)addr;
	int tag = addr >> 9 & 0x007fffff;
	int idx = addr >> 6 & 0x00000007;
	int offset = addr & 0x0000003f;
	int value = 0;
	Cache* setptr = set + idx;
	if (setptr->tag1 == tag)
	{
		if (setptr->valid1 == 0)
		{
			CM++;
			Cload(setptr->data1, addr & 0xffffffc0);
			for (int i = 0; i < byte; i++)
			{
				setptr->data1[offset + i] = (reg[rt] >> ((byte-1) * 8 - (8 * i))) & 0x00FF;
			}
			setptr->dirty1 = 1;
			setptr->valid1 = 1;
		}
		else
		{
			CH++;
			for (int i = 0; i < byte; i++)
			{
				setptr->data1[offset + i] = (reg[rt] >> ((byte-1) * 8 - (8 * i))) & 0x00FF;
			}
		}
		setptr->dirty1 = 1;
		setptr->LRU = 2;
	}
	else if (setptr->tag2 == tag)
	{
		if (setptr->valid2 == 0)
		{
			CM++;
			Cload(setptr->data2, addr & 0xffffffc0);
			for (int i = 0; i < byte; i++)
			{
				setptr->data2[offset + i] = (reg[rt] >> ((byte-1) * 8 - (8 * i))) & 0x00FF;
			}
			setptr->dirty2 = 1;
			setptr->valid2 = 1;
		}
		else
		{
			CH++;
			for (int i = 0; i < byte; i++)
			{
				setptr->data2[offset + i] = (reg[rt] >> ((byte-1) * 8  - (8 * i))) & 0x00FF;
			}
		}
		setptr->dirty2 = 1;
		setptr->LRU = 1;
	}
	else if(setptr->LRU == 0)
	{
		CM++;
		Cload(setptr->data1, addr & 0xffffffc0);
		for (int i = 0; i < byte; i++)
		{
			setptr->data1[offset+i] = (reg[rt] >> ((byte-1) * 8  - (8 * i))) & 0x00FF;
		}
		setptr->dirty1 = 1; 
		setptr->valid1 = 1;
		setptr->LRU = 2;
		setptr->tag1 = tag;
	}
	else if (setptr->LRU ==  1)
	{
		CM++;
		if (setptr->dirty1 == 1)
		{
			int ogAddr = (setptr->tag1 << 9) + (idx << 6); 
			Mstore(setptr->data1, ogAddr);
			setptr->dirty1 = 0;
		}
		Cload(setptr->data1, addr & 0xffffffc0);
		for (int i = 0; i < byte; i++)
		{
			setptr->data1[offset+i] = (reg[rt] >> ((byte-1) * 8  - (8 * i))) & 0x00FF;
		}
		setptr->dirty1 = 1; 
		setptr->valid1 = 1;
		setptr->LRU = 2;
		setptr->tag1 = tag;
	}
	else
	{
		CM++;
		if (setptr->dirty2 == 1)
		{
			int ogAddr = (setptr->tag2 << 9) + (idx << 6); 
			Mstore(setptr->data2, ogAddr); 
			setptr->dirty2 = 0;
		}
		Cload(setptr->data2, addr & 0xffffffc0);
		for (int i = 0; i < byte; i++)
		{
			setptr->data2[offset+i] = (reg[rt] >> ((byte-1) * 8  - (8 * i))) & 0x00FF;
		}
		setptr->dirty2 = 1; 
		setptr->valid2 = 1;
		setptr->LRU = 1;
		setptr->tag2 = tag;
	}
}

void add(int rs, int rt, int rd)
{
	reg[rd] = reg[rs] + reg[rt];
}

void addu(int rs, int rt, int rd)
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

void sltu(int rs, int rt, int rd)
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
	if (reg[rt] < 0)
	{
		int cmp = (0xFFFFFFFF << (32 - sa));
		reg[rd] = (reg[rt] >> sa) ^ cmp;
	}
	else
		reg[rd] = reg[rt] >> sa;
}
void addi(int rs, int rt, int imme) {
	reg[rt] = reg[rs] + imme;
}
void addiu(int rs, int rt, int imme) {
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
	load(imme, rs, 4, rt);
}
void sw(int rs, int rt, int imme) {
	store(imme, rs, 4, rt);
}
void lh(int rs, int rt, int imme) {
	load(imme, rs, 2, rt);
}

void lhu(int rs, int rt, int imme) {
	load(imme, rs, 2, rt);
	reg[rt] = reg[rt] & 0x0000FFFF;
}
void sh(int rs, int rt, int imme) {
	store(imme, rs, 2, rt);
}
void lb(int rs, int rt, int imme) {
	load(imme, rs, 1, rt);
}

void lbu(int rs, int rt, int imme) {
	load(imme, rs, 1, rt);
	reg[rt] = reg[rt] & 0x000000FF;
}

void sb(int rs, int rt, int imme) {
	store(imme, rs, 1, rt);
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
	intPC = ((intPC + 4) & 0xF0000000) | (0x0fffffff & (imme << 2));
	PC = (char*)intPC;
}

void jal(int rs, int rt, int imme)
{
	reg[31] = (int)((long)PC + 4);
	long intPC = (long)PC;
	intPC = ((intPC + 4) & 0xF0000000) | (0x0fffffff & (imme << 2));
	PC = (char*)intPC;
}

void jr(int rs)
{
	long intPC = (long)reg[rs]-4;
	PC = (char*)intPC;
}

#endif 
