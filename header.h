void dst(int rd, int rs, int rt) {
	printf(" $%d, $%d, $%d\n", rd, rs, rt);
}
void st(int rs, int rt){
	printf(" $%d, $%d\n", rs, rt);
}
void ds(int rd, int rs) {
	printf(" $%d, $%d\n", rd, rs);
}
void rss(int rs) {
	printf(" $%d\n", rs);
}
void rdd(int rd) {
	printf(" $%d\n", rd);
}
void dta(int rd, int rt, int sa) {
	printf(" $%d, $%d, %d\n", rd, rt, sa);
}
void dts(int rd, int rt, int rs) {
	printf(" $%d, $%d, $%d\n", rd, rt, rs);
}
void tsi(int rt, int rs, int  imme) {
	printf(" $%d, $%d, %d\n", rt, rs, imme);
}
void stl(int rs, int  rt, int  imme) {
	printf(" $%d, $%d, %d\n", rs, rt, imme);
}
void tis(int rt, int  imme, int rs) {
	printf(" $%d, %d($%d)\n", rt, imme, rs);
}
void ti(int rt, int imme) {
	printf(" $%d, %d\n", rt, imme);
}

void PrintFunct(int num, int rs, int rt, int rd, int sa){
	if (num == 32)
	{
		printf("add");
		dst(rd, rs, rt);
	}
	else if (num == 33)
	{
		printf("addu");
		dst(rd, rs, rt);
	}
	else if (num == 36)
	{
		printf("and");
		dst(rd, rs, rt);
	}
	else if (num == 26)
	{
		printf("div");
		st(rs, rt);
	}
	else if (num == 27)
	{
		printf("divu");
		st(rs, rt);
	}
	else if (num == 9)
	{
		printf("jalr");
		ds(rd, rs);
	}
	else if (num == 8)
	{
		printf("jr");
		rss(rs);
	}
	else if (num == 16)
	{
		printf("mfhi");
		rdd(rd);
	}
	else if (num == 18)
	{
		printf("mflo");
		rdd(rd);
	}
	else if (num == 17)
	{
		printf("mthi");
		rss(rs);
	}
	else if (num == 19)
	{
		printf("mtlo");
		rss(rs);
	}
	else if (num == 24)
	{
		printf("mult");
		st(rs, rt);
	}
	else if (num == 25)
	{
		printf("multu");
		st(rs, rt);
	}
	else if (num == 39)
	{
		printf("nor");
		dst(rd, rs, rt);
	}
	else if (num == 37)
	{
		printf("or");
		dst(rd, rs, rt);
	}
	else if (num == 0)
	{
		printf("sll");
		dta(rd, rt, sa);
	}
	else if (num == 4)
	{
		printf("sllv");
		dts(rd, rt, rs);
	}
	else if (num == 42)
	{
		printf("slt");
		dst(rd, rs, rt);
	}
	else if (num == 43)
	{
		printf("sltu");
		dst(rd, rs, rt);
	}
	else if (num == 3)
	{
		printf("sra");
		dta(rd, rt, sa);
	}
	else if (num == 7)
	{
		printf("srav");
		dts(rd, rt, rs);
	}
	else if (num == 2)
	{
		printf("srl");
		dta(rd, rt, sa);
	}
	else if (num == 6)
	{
		printf("srlv");
		dts(rd, rt, rs);
	}
	else if (num == 34)
	{
		printf("sub");
		dst(rd, rs, rt);
	}
	else if (num == 35)
	{
		printf("subu");
		dst(rd, rs, rt);
	}
	else if (num == 12)
	{
		printf("syscall\n");
	}
	else if (num == 38)
	{
		printf("xor");
		dst(rd, rs, rt);
	}
	else
		printf("unknown instruction\n");
}

void PrintOp(int num, int rs, int rt, int imme)
{
	if (num == 8)
	{
		printf("addi");
		tsi(rt, rs, imme);
	}
	else if (num == 9)
	{
		printf("addiu");
		tsi(rt, rs, imme);
	}
	else if (num == 12)
	{
		printf("andi");
		tsi(rt, rs, imme);
	}
	else if (num == 4)
	{
		printf("beq");
		stl(rs, rt, imme);
	}
	else if (num == 5)
	{
		printf("bne");
		stl(rs, rt, imme);
	}
	else if (num == 32)
	{
		printf("lb");
		tis(rt, imme, rs);
	}
	else if (num == 36)
	{
		printf("lbu");
		tis(rt, imme, rs);
	}
	else if (num == 33)
	{
		printf("lh");
		tis(rt, imme, rs);
	}
	else if (num == 37)
	{
		printf("lhu");
		tis(rt, imme, rs);
	}
	else if (num == 15)
	{
		printf("lui");
		ti(rt, imme);
	}
	else if (num == 35)
	{
		printf("lw");
		tis(rt, imme, rs);
	}
	else if (num == 13)
	{
		printf("ori");
		tsi(rt, rs, imme);
	}
	else if (num == 40)
	{
		printf("sb");
		tis(rt, imme, rs);
	}
	else if (num == 10)
	{
		printf("slti");
		tsi(rt, rs, imme);
	}
	else if (num == 11)
	{
		printf("sltiu");
		tsi(rt, rs, imme);
	}
	else if (num == 41)
	{
		printf("sh");
		tis(rt, imme, rs);
	}
	else if (num == 43)
	{
		printf("sw");
		tis(rt, imme, rs);
	}
	else if (num == 14)
	{
		printf("xori");
		tsi(rt, rs, imme);
	}
	else if (num == 2)
	{
		printf("j %d\n", imme);
	}
	else if (num == 3)
	{
		printf("jal %d\n", imme);
	}
	else
		printf("unknown instruction\n");

}

