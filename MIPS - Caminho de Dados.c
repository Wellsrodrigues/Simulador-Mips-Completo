#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "windows.h"


/*
 -------------------------------------------------------------------------------------------------------
  Author: Wellison Rodrigues
 
  Trabalho Apresentado a Arquitetura de Computadores
 
  Prof. Ramon Napomuceno
 -------------------------------------------------------------------------------------------------------
*/


//------------------------------------ CENTRALIZAR TEXT ----------------------------------------

void gotoxy(int x, int y){
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int stringToInt(char text[3]){
	char number[3];
	
	number[0] = text[1];
	number[1] = text[2]; 
	
	int cont = atoi(number);
	return cont;
}

//---------------------------------------- CONTROLE ------------------------------------------------

bool RegDst, Jump, Branch, MemRead, MentoReg, MemWrite, ALUsrc, RegWrite, ALUzero; // 1 bit
int ALUop, ALUcontrol, function; // 2 bits

void setControl(bool a, bool b, bool c, bool d, bool e, int f, bool g, bool h, bool i, bool j, int k){ // adiciona as saidas
    RegDst = a;
	Jump = b;
	Branch = c;
	MemRead = d;
	MentoReg = e;
	ALUop = f;
	MemWrite = g;
	ALUsrc = h;
	RegWrite = i;
	ALUzero = j;
	ALUcontrol = k;
}

void control(int opCode){ // define qual saida com base no opCode
	
	//RegDst, Jump, Branch, MemRead, MentoReg, ALUOp, MemWrite, ALUSrc, RegWrite;
	
	if(opCode == 000000){
		if(function == 100000){
			setControl(1,0,0,0,0,10,0,0,1,0,10); //add
		}
		
		if(function == 100010){
			setControl(1,0,0,0,0,10,0,0,1,0,110); //sub
		}
	}
	
	else if(opCode == 001000) { 
		 setControl(0,0,0,0,0,00,0,1,1,0,10); //addi
	}
		
	else if(opCode == 100011){ 
	 	setControl(0,0,0,1,1,00,0,1,1,0,10); //lw
	}
	    
	else if(opCode == 101011) {
		setControl(0,0,0,0,0,00,1,1,0,0,10); //sw
		gotoxy(93, 28);
		printf("(Don't care)");
		gotoxy(93, 32);
		printf("(Don't care)");
	}
		
	else if(opCode == 000100) {
		setControl(0,0,1,0,0,01,0,0,0,1,110); //beq
		gotoxy(93, 28);
		printf("(Don't care)");
		gotoxy(93, 32);
		printf("(Don't care)");
	}
		
	else if(opCode == 000010) {
		setControl(0,1,0,0,0,00,0,0,0,0,0); //j
		gotoxy(93, 28);
		printf("(Don't care)");
		gotoxy(93, 32);
		printf("(Don't care)");
		gotoxy(93, 33);
		printf("(Don't care)");
		gotoxy(93, 34);
		printf("(Don't care)");
	}
	
	else{
		gotoxy(0,40);
		printf("Instrucao Inexistente!");
		exit(1);
	}
}

void exibeControl(){
	int linha = 26;
	
	gotoxy(83, linha);
	printf("CONTROLE");
	gotoxy(80, linha+2);
	printf("REG_DST:   %d", RegDst);
	gotoxy(80, linha+3);
	printf("JUMP:      %d", Jump);
	gotoxy(80, linha+4);
	printf("BRANCH:    %d", Branch);
	gotoxy(80, linha+5);
	printf("MEM_READ:  %d", MemRead);
	gotoxy(80, linha+6);
	printf("MENTO_REG: %d", MentoReg);
	gotoxy(80, linha+7);
	printf("ALU_OP:    %02d", ALUop);
	gotoxy(80, linha+8);
	printf("MEM_WRITE: %d", MemWrite);
	gotoxy(80, linha+9);
	printf("ALU_SRC:   %d", ALUsrc);
	gotoxy(80, linha+10);
	printf("REG_WRITE: %d", RegWrite);
	gotoxy(80, linha+11);
	printf("ALU_ZERO:  %d", ALUzero);
	gotoxy(80, linha+12);
	printf("ALU_CONTROL: %04d", ALUcontrol);
}

//------------------------------------------ PROGRAM COUNTER --------------------------------------------------------

int PC = 0;

void pc(){
	PC += 4;	
}

void mostraPC(){
	gotoxy(65,25);
	printf("PC: %d  ", PC);
}

//---------------------------------------- REGISTRADORES ------------------------------------------------

int R[32];

bool nomeReg(char reg[4]){
	if(reg[0] == 'R' || reg[0] == 'r'){
		return 1;
	}else{
		return 0;
	}
}

void inicializaRegistradores(){
	
	for(int j=0; j<32; j++){
		R[j] = 0;
	}
}

void atualizarRegistradores(){
	
	char rg[4];
	int valor;
	
    gotoxy(35, 25);
	scanf("%s %d", &rg, &valor);
	
	if(nomeReg(rg) == 1){
		int cont = stringToInt(rg);
		for(int i=0; i<32; i++){
			if(cont == i){
				R[i] = valor;
			}
		}
	}
	
	else{
		gotoxy(25,40);
		printf("Nome do Registrador Invalido!");
	}
}

void mostrarRegistradores(){	
	int linha = 22;
	
	gotoxy(115, linha);
	printf("REGISTRADORES");
	
    for(int i=0; i<16; i++){
		gotoxy(109, linha+i+2);
		printf("R%d: %d\t\tR%d: %d",i, R[i], i+16, R[i+16]);
	}
}

int retornaReg(char reg[4]){
	int cont = stringToInt(reg);
	return R[cont];
}

int* RegDestino(char reg[4]){
	int *p;
	
	int cont = stringToInt(reg);
	p = &R[cont];
	return p;
}

//---------------------------------------- MEMORIA ------------------------------------------------

int M[1024];

bool nomeMem(char M[4]){
	if(M[0] == 'M' || M[0] == 'm'){
		return 1;
	}else{
		return 0;
	}
}

void inicializaMemoria(){
	for(int i=0; i<1024; i++){
		M[i] = 0;
	}
}

void atualizarMemoria(){
	
	char memo[4];
	int valor;

	gotoxy(35, 25);
	scanf("%s %d", &memo, &valor);
	
	if(nomeMem(memo) == 1){
		int cont = stringToInt(memo);
	
		for(int i=0; i<1024; i++){
			if(cont == i){
				M[i] = valor;
			}
		}
	}
	
	else{
		gotoxy(25,40);
		printf("Instrucao de Memoria Inexistente!");
	}
	
}

void mostrarMemoria(){
	int linha = 2;
	gotoxy(119, linha);
	printf("MEMORIA");
	
	for(int i=0; i<16; i++){
		gotoxy(100, linha+i+2);
		printf("M%d: %d\t\tM%d: %d\t\tM%d: %d",i, M[i],i+16, M[i+16],i+32, M[i+32]);
	}
}

//---------------------------------------- INSTRUCOES ------------------------------------------------

void aritmetica(int opCode,char rd[4], char rs[4], char rt[4]){ //aritmetica
	
	if(nomeReg(rd) == 1 && nomeReg(rs) == 1 && nomeReg(rt) == 1){
		int *destino;
	
		if(opCode == 000000){
			
			if(function == 100010){
				destino = RegDestino(rd);
				*destino = retornaReg(rs) - retornaReg(rt);
				pc(); 
				control(opCode);
			}
			
			if(function == 100000){
				destino = RegDestino(rd);
				*destino = retornaReg(rs) + retornaReg(rt);
				pc();
				control(opCode);
			}
		}
	}
	
	else{
		gotoxy(25,40);
		printf("Nome do Registrador Invalido!");
	}
	
}

void memoria(int opCode,char rs[4], char rt[4], int imediato){ //transferencia de dados
	
	if(nomeReg(rs) == 1 && nomeReg(rt) == 1){
	
			int *destino;
			
			if(opCode == 100011){ //lw
			    destino = RegDestino(rt);
				*destino = M[(imediato * 4) + retornaReg(rs)];
				pc();
			    control(opCode);	
			}
			
			else if(opCode == 101011){ //sw
				
				 M[(imediato * 4) + retornaReg(rs)] = retornaReg(rt);
				 pc();
				 control(opCode);
			}
			
			else{//addi
				destino = RegDestino(rt);
				*destino = retornaReg(rs) + imediato;
				pc();
				control(opCode);
			}
	}
	
	else{
		gotoxy(25,40);
		printf("Nome do Registrador Invalido!");
	}
}

void jump(int opCode, int salto){ //j
	PC = salto * 4;
	control(opCode);
}


void compara(int opCode, char rs[4], char rt[4], int imediato){ //beq
	
	if(nomeReg(rs) == 1 && nomeReg(rt) == 1){
	
			if(opCode == 000100){
				
					if(retornaReg(rs) == retornaReg(rt)){
						PC = imediato*4+PC+4;
						control(opCode);
					}
					else{
						setControl(0,0,1,0,0,01,0,0,0,0,110); //nao sao iguais
						pc();
					}
			}
	}
	else{
		gotoxy(25,40);
		printf("Nome do Registrador Invalido!");
	}
}

void inserir(){

char instrucao[5],  rd[4], rs[4], rt[4];
int opCode, imediato;
	
	gotoxy(25, 22);
	printf("| ADD | SUB | ADDi | LW | SW | BEQ | J |");
	
	gotoxy(35, 25); 
	scanf("%s", &instrucao);
	
	
	if(strcmp(instrucao, "ADD") == 0 || strcmp(instrucao, "add") == 0){		
		opCode = 000000;
		function = 100000;
			
		scanf("%s", &rd);
		scanf("%s", &rs);
		scanf("%s", &rt);
		
		aritmetica(opCode,rd, rs, rt);
	}
	
	else if(strcmp(instrucao, "SUB") == 0 || strcmp(instrucao, "sub") == 0){		
		opCode = 000000;
		function = 100010;
			
		scanf("%s", &rd);
		scanf("%s", &rs);
		scanf("%s", &rt);
		
		aritmetica(opCode,rd, rs, rt);
	}
	
	else if(strcmp(instrucao, "LW" ) == 0 || strcmp(instrucao, "lw") == 0){
		opCode = 100011;
	
		scanf("%s", &rt);
		scanf("%d", &imediato);
		scanf("%s", &rs);
		
		memoria(opCode, rs, rt, imediato);
	}
	
	else if(strcmp(instrucao, "SW") == 0 || strcmp(instrucao, "sw") == 0){
		opCode = 101011;
			
		scanf("%s", &rt);
		scanf("%d", &imediato);
		scanf("%s", &rs);
		
		memoria(opCode, rs, rt, imediato);
	}
	
	else if(strcmp(instrucao, "ADDi") == 0 || strcmp(instrucao, "addi") == 0){
		opCode = 001000;
			
		scanf("%s", &rt);
		scanf("%s", &rs);
		scanf("%d", &imediato);
		
		memoria(opCode, rs, rt, imediato);
	}
	
	else if(strcmp(instrucao, "BEQ") == 0 || strcmp(instrucao, "beq") == 0){
		opCode = 000100;
		
		scanf("%s", &rt);
		scanf("%s", &rs);
		scanf("%d", &imediato);
		
		compara(opCode, rs, rt, imediato);
	}
	
	else if(strcmp(instrucao, "J") == 0 || strcmp(instrucao, "j") == 0){
		opCode = 000010;
		
		scanf("%d", &imediato);	
		jump(opCode, imediato);
	}
	
	else{
		gotoxy(25,40);
		printf("Instrucao Inexistente!");
	}
}

void logo(){
	int linha2 = 5, coluna = 20;	
	
	gotoxy(coluna,linha2);
	printf("###           ###   ###   ### ### ###   ### ###");
	gotoxy(coluna,linha2+1);
	printf("###           ###   ###   ### ### ###   ### ###");
	gotoxy(coluna,linha2+2);
	printf("###           ###   ###   ### ### ###   ### ###");
	gotoxy(coluna,linha2+3);
	printf("### ###   ### ###   ###   ###     ###   ###");
	gotoxy(coluna,linha2+4);
	printf("### ###   ### ###   ###   ###     ###   ###");
	gotoxy(coluna,linha2+5);
	printf("### ###   ### ###   ###   ###     ###   ###");
	gotoxy(coluna,linha2+6);
	printf("### ###   ### ###   ###   ### ### ###   ### ###");
	gotoxy(coluna,linha2+7);
	printf("### ###   ### ###   ###   ### ### ###   ### ###");
	gotoxy(coluna,linha2+8);
	printf("### ###   ### ###   ###   ### ### ###   ### ###");
	gotoxy(coluna,linha2+9);
	printf("###    ###    ###   ###   ###               ###");
	gotoxy(coluna,linha2+10);
	printf("###    ###    ###   ###   ###               ###");
	gotoxy(coluna,linha2+11);
	printf("###    ###    ###   ###   ###               ###");
	gotoxy(coluna,linha2+12);
	printf("###           ###   ###   ###           ### ###\n\n");
	gotoxy(coluna,linha2+13);
	printf("###           ###   ###   ###           ### ###\n\n");
	gotoxy(coluna,linha2+14);
	printf("###           ###   ###   ###           ### ###\n\n");
}

void animacao(int time){
	int linha2 = 15, coluna = 60;	
	
	gotoxy(coluna,linha2);
	printf("###           ###   ###   ### ### ###   ### ###");
	Sleep(time);
 	
	gotoxy(coluna,linha2+1);
	printf("###           ###   ###   ### ### ###   ### ###");
	Sleep(time);
 	
	gotoxy(coluna,linha2+2);
	printf("###           ###   ###   ### ### ###   ### ###");
	Sleep(time);


	gotoxy(coluna,linha2+3);
	printf("### ###   ### ###   ###   ###     ###   ###");
	Sleep(time);

	gotoxy(coluna,linha2+4);
	printf("### ###   ### ###   ###   ###     ###   ###");
	Sleep(time);

	 
	gotoxy(coluna,linha2+5);
	printf("### ###   ### ###   ###   ###     ###   ###");
	Sleep(time);

	gotoxy(coluna,linha2+6);
	printf("### ###   ### ###   ###   ### ### ###   ### ###");
	Sleep(time);

	gotoxy(coluna,linha2+7);
	printf("### ###   ### ###   ###   ### ### ###   ### ###");
	Sleep(time);

	gotoxy(coluna,linha2+8);
	printf("### ###   ### ###   ###   ### ### ###   ### ###");
	Sleep(time);

	 
	gotoxy(coluna,linha2+9);
	printf("###    ###    ###   ###   ###               ###");
	Sleep(time);

	gotoxy(coluna,linha2+10);
	printf("###    ###    ###   ###   ###               ###");
	Sleep(time);

	 
	gotoxy(coluna,linha2+11);
	printf("###    ###    ###   ###   ###               ###");
	Sleep(time);

	 
	gotoxy(coluna,linha2+12);
	printf("###           ###   ###   ###           ### ###\n\n");
	Sleep(time);

	 
	gotoxy(coluna,linha2+13);
	printf("###           ###   ###   ###           ### ###\n\n");
	Sleep(time);

	 
	gotoxy(coluna,linha2+14);
	printf("###           ###   ###   ###           ### ###\n\n");
	Sleep(time);
	
	system("cls");
}


void excute(){
	
}

//---------------------------------------- MENU ---------------------------------------------------

void Menu(){
  	animacao(50);
  
   while(1){
    int linha = 23;
	int opp;
	int coluna = 20;
	
	gotoxy(coluna,24);
	printf("           -------------------------------");
	gotoxy(coluna,25);
	printf("INSTRUCAO: | ");
	gotoxy(coluna+40,25);
	printf(" |");
	gotoxy(coluna,26);
	printf("           -------------------------------");
	
	logo();
	mostraPC();
	mostrarRegistradores();
	mostrarMemoria();
	setControl(0,0,0,0,0,00,0,0,0,0,0000);
	exibeControl();
	
	
	coluna = 30;
	linha = 30;
	
	gotoxy(coluna,linha+1);
	printf("1 - INSERIR INSTRUCAO MIPS");
	gotoxy(coluna,linha+2);
	printf("2 - ATUALIZAR REGISTRADORES");
	gotoxy(coluna,linha+3);
	printf("3 - ATUALIZAR MEMORIA");
	gotoxy(coluna,linha+4);
	printf("0 - SAIR DO SIMULADOR");
	
	coluna = 38;

	gotoxy(coluna,linha+6);
    
	printf("OPP: ");
	scanf("%d", &opp);
	switch(opp){
		case 1:
			inserir();
			exibeControl();
			mostraPC();
			mostrarRegistradores();
			mostrarMemoria();
			gotoxy(20,linha+12);
			setControl(0,0,0,0,0,00,0,0,0,0,0000);
			system("pause");
			system("cls");
		break;
		
		case 2:
			atualizarRegistradores();
			mostrarRegistradores();
			gotoxy(20,linha+12);
			system("pause");
			system("cls");
		break;
		
		case 3:
		    atualizarMemoria();
		    mostrarMemoria();
		    gotoxy(20,linha+12);
			system("pause");
			system("cls");
		break;
		
		case 0:
			system("cls");
			animacao(100);
			gotoxy(0,linha+12);
			printf("Fechado com Exito!");
			exit(1);
		break;
		
		default:
			gotoxy(35,40);
			printf("Opcao Invalida!");
			gotoxy(20,linha+12);
			system("pause");
			system("cls");
		break;
	}
	
  }
}

//---------------------------------------- MAIN ------------------------------------------------

main(){
	system("title SIMULADOR MIPS - CAMINHO DE DADOS by Wellison Rodrigues");
	ShowWindow( GetConsoleWindow(), SW_MAXIMIZE);
	system("color A");
	
	inicializaRegistradores();
	inicializaMemoria();
	
	Menu();
	  

}
