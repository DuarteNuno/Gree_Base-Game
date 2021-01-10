#include <cstring>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <termios.h>
#include <cstdio>
#include <cstdlib>

static struct termios old, current;

#define RESET   	"\033[0m"
#define BLACK   	"\033[30m"      		/* Black */
#define RED     	"\033[31m"      		/* Red */
#define GREEN   	"\033[32m"      		/* Green */
#define YELLOW  	"\033[33m"      		/* Yellow */
#define BLUE    	"\033[34m"      		/* Blue */
#define MAGENTA 	"\033[35m"      		/* Magenta */
#define CYAN    	"\033[36m"      		/* Cyan */
#define WHITE   	"\033[37m"      		/* White */
#define BOLDBLACK   "\033[1m\033[30m"       /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"       /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"       /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"       /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"       /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"       /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"       /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"       /* Bold White */

#define altura 4
#define largura 7

#define Disp  '0'
#define Ndis  '-'
#define Boss  'B'
#define Empr  'E'
			
#define inv "       invalido      "
#define Adm "        Admin        "
#define Mrk "      Marketing      "
#define Fin "      Financeiro     "
#define Q_A "Controlo de Qualidade"
#define H_R "  Recursos Humanos   "
#define C_S " Servico ao Cliente  "
#define Leg "        Legal        "
#define Log "      Logistica      "
#define Mnf "      Producao       "
#define Wrh "       Armazem       "

using namespace std;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  current = old; /* make new settings same as old settings */
  current.c_lflag &= ~ICANON; /* disable buffered i/o */
  if (echo) {
      current.c_lflag |= ECHO; /* set echo mode */
  } else {
      current.c_lflag &= ~ECHO; /* set no echo mode */
  }
  tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(1);
}


char tabuleiro[altura][largura]={{Ndis,Ndis,Ndis,Empr,Ndis,Ndis,Ndis},
								 {Ndis,Ndis,Ndis,Empr,Ndis,Ndis,Ndis},
								 {Ndis,Ndis,Ndis,Boss,Ndis,Ndis,Ndis},
								 {Empr,Empr,Boss,Boss,Boss,Empr,Empr}};

string nomedep[altura][largura]={{ inv, inv, inv, Mnf, inv, inv, inv},
							  	 { inv, inv, inv, Q_A, inv, inv, inv},
								 { inv, inv, inv, H_R, inv, inv, inv},
								 { Wrh, C_S, Mrk, Adm, Fin, Leg, Log}};

typedef struct Jogador {
	bool alive;
	int saldo;
	int income;
	string nome;
	bool isBoss;
	bool departamentos[altura][largura];

} Jogador;	

Jogador jogadores[4];//Ceo,e1,e2,e3;

void printTab(){
int b;

for (int a=0;a<altura; a++)
	{
		if (a==0)
		{
			for (b=0; b<=largura; b++)
			{
				if (b<largura)
					cout << "+---";
				else
					putchar('+');
			}
			putchar('\n');
		}
		
		for (b=0; b<=largura; b++)
		{
			if (b<largura)
				cout <<"| "<< tabuleiro[a][b]  << ' ';
			else
				cout << " |" ;
		}

		putchar('\n');
		for (b=0; b<=largura; b++) {
			if (b<largura)
				cout << "+---";
			else
				putchar('+');
		}
		putchar('\n');
	}

}

void printDeps(){
	for(int a = 0 ; a<altura ; a++){
      for(int b = 0 ; b<largura ; b++){
      	cout << nomedep[a][b];
      }
      cout << endl;
    }
    return;
}

void printJDeps(int n){
	for(int a = 0 ; a<altura ; a++){
      for(int b = 0 ; b<largura ; b++){
		if(jogadores[n].departamentos[a][b]==true)
      		cout << nomedep[a][b];
      	else
      		cout << "          -          ";
      }
      cout << endl;
    }
    return;
}

void printProfile(int n){
	cout <<"	nome: " << jogadores[n].nome<<endl;
	cout <<"	saldo: " << jogadores[n].saldo<<"€"<<endl;
	cout <<"	redimento: " << jogadores[n].income<<"€/semana\n\n";
}
void regras(){

	cout<<"A empresa e constituida por 1 Chefe e 3 empregados\nEstes dividem os seus departamentos\n";
	cout<<"No inicio do jogo o Chefe comeca com 7 departamentos e os empregados com 3(1 cada)\n";
	
	cout<<"\nO objetivo do Chefe e aguentar 24 semanas no comado da empresa\n";
	cout<<"Para o Chefe vencer tem de rescindir o contrato com todos os empregados ou aguentar as 24 semanas sem que lhe comprem o departamento Admin\n";

	cout<<"\nO objetivo dos Empregados e expulsar o chefe antes que ele rescinda os seus contratatos\n";
	cout<<"Para os empregados ganharem entao um deles tem que comprar o departamento Admin ao chefe\n";

	cout<<"\nDe 2 em 2 semanas acotecera um evento aleatorio que tera impacto nas contas dos acionistas\n";
	cout<<"Cada semana cada jogador pode escolher uma acao dentro das disponiveis por exemplo:\n";
	cout<<"-Trabalhar\n-Aumentar rendimento\nentre outras\n";
	cout<<"Se algum jogador ficar com saldo negativo entao os seus departamentos sao entregues ao chefe, o seu saldo e o seu rendimento sao zerados\n";
	cout<<"Se o chefe ficar com saldo negativo entao perde automaticamete\n";
	char c;
	c=getch();
}

bool fimdejogo(){
	if(!(jogadores[0].alive) || !(jogadores[1].alive) && !(jogadores[2].alive) && !(jogadores[3].alive) )
		return true;
	return false;
}

string jogVivo(){
	if(!jogadores[0].alive)
		return "Empregados";
	else  
		return jogadores[0].nome;
}

Jogador mkJgd(string n, int c){
	Jogador j1;
	j1.alive=true;
	j1.nome=n;

	switch (c) {
	  case 0://BOSS
	    j1.saldo = 4000;
	    j1.income = 500; 
		j1.isBoss = true; 
		j1.departamentos[3][2]=true;
		j1.departamentos[3][3]=true;
		j1.departamentos[3][4]=true;
		j1.departamentos[2][3]=true;
		//j1.departamentos[1][3]=true;
		//j1.departamentos[3][1]=true;
		//j1.departamentos[3][5]=true;

	    break;

	  case 1://Empregado 1 
	    j1.saldo=1500;
	    j1.income = 200; 
		j1.isBoss=false;
		j1.departamentos[3][2]=false;
		j1.departamentos[3][3]=false;
		j1.departamentos[3][4]=false;
		j1.departamentos[2][3]=false;
		j1.departamentos[1][3]=false;
		//j1.departamentos[3][1]=false;
		j1.departamentos[3][5]=false;

		j1.departamentos[3][0]=true;
		j1.departamentos[3][1]=true;
	    break;

	  case 2://Empregado 2
	    j1.saldo=1500;
	    j1.income = 200; 
		j1.isBoss=false;
		j1.departamentos[3][2]=false;
		j1.departamentos[3][3]=false;
		j1.departamentos[3][4]=false;
		j1.departamentos[2][3]=false;
		j1.departamentos[3][0]=false;
		j1.departamentos[3][1]=false;
		//j1.departamentos[1][3]=false;
		j1.departamentos[3][1]=false;
		j1.departamentos[3][5]=false;

		j1.departamentos[0][3]=true;
		j1.departamentos[1][3]=true;
	    break;

	  case 3://Empregado 3
	    j1.saldo=1500;
	    j1.income = 200; 
		j1.isBoss=false;
		j1.departamentos[3][2]=false;
		j1.departamentos[3][3]=false;
		j1.departamentos[3][4]=false;
		j1.departamentos[2][3]=false;
		j1.departamentos[3][0]=false;
		j1.departamentos[3][1]=false;
		j1.departamentos[0][3]=false;
		j1.departamentos[1][3]=false;
		j1.departamentos[1][3]=false;
		j1.departamentos[3][1]=false;
		//j1.departamentos[3][5]=false;

		j1.departamentos[3][5]=true;
		j1.departamentos[3][6]=true;
	    break;
	}
	return j1;
}

void evento(){

	int jogAfetado;
	bool pst=true;

	string cartas[20]={
					   "Crise Financeira",//0-Financial
					   "Explosao Financeira" ,//1-Financial
					   "Processo de direitos de autor",//2-Legal
					   "Prova que as acusacoes de suborno sob a empresa eram falsas ",//3-Legal
					   "Indices de resolucao de problemas dos clientes baixos",//4-Customer Service
					   "Feedback dos clientes muito positivo",//5-Customer Service
					   "Empregados ameacam fazer greve",//6-Human resources
					   "Empregados recentemente contratados tem bons resultados",//7-Human resources
					   "Preco da publicidade demasiado caro",	//8-Marketing
					   "Publicidade de alto nivel",//9-Marketing
					   "Estrutura desatualizada",//10-Quality assurance
					   "Melhores produtos que a competicao"//11-Quality assurance
					   "Os produtos mais recentes sairam todos com avarias",//12-Manufacturing
					   "Novas maquinas de producao",//13-Manufacturing
					   "Atraso de entregas devido a atraso no armazem",//14-warehouse
					   "Armazem melhorado",//15-warehouse
					   "Transporte de mercadoria pouco eficaz",//16-logistic
					   "A equipa de logistica encontra forma de melhorar o rendimento da empresa",//17-logistics
					   "Festa na empresa",//18
					   "Acoes da empresa desvalorizam"//19
					};

	int rn=rand()%20;
	int r = rn;
	system("clear");
	cout << cartas[r]<<'\n'<<'\n';
	cout<<r<<endl;

	switch (r){
		case 0:
			if(jogadores[0].departamentos[3][4]){
				jogadores[0].saldo-=1000;
				jogAfetado=0;
			}
			else{
				jogadores[3].saldo-=750;
				jogAfetado=3;
			}
			cout << "Devido a recente crise o departamento financeiro foi o mais afetado e sera retirado um montante ao seu titular\n";
			pst=false;
			break;
		case 1:
			if(jogadores[0].departamentos[3][4]){
				jogadores[0].saldo+=1000;
				jogAfetado=0;
			}
			else{
				jogadores[3].saldo+=750;
				jogAfetado=3;
			}
			cout << "Visto que o lucro da empresa tem vindo a aumentar o titular do departamento financeiro sera recompensado\n";
			break;
		case 2:
			if(jogadores[0].departamentos[3][5]){
				jogadores[0].saldo-=1000;
				jogAfetado=0;
			}
			else{
				jogadores[3].saldo-=750;
				jogAfetado=3;
			}
			cout << "A empresa foi condenada a pagar uma indemenizacao por nao ter patente dos seus produtos\n";
			cout << "O titular do departamento legal coimado\n";
			pst=false;
			break;
		case 3:
			if(jogadores[0].departamentos[3][5]){
				jogadores[0].saldo+=1000;
				jogAfetado=0;
			}
			else{
				jogadores[3].saldo+=750;
				jogAfetado=3;
			}
			cout << "O departamento legal da empresa venceu este caso e o seu titular sera remonerado\n";
			break;
		case 4:
			if(jogadores[0].departamentos[3][1]){
				jogadores[0].saldo-=1000;
				jogAfetado=0;
			}
			else{
				jogadores[1].saldo-=750;
				jogAfetado=1;
			}
			cout << "Varias queixas foram feitas ao apoio ao cliente face a mau servico prestado pela empresa\n";
			cout << "O seu titular do departamento Servico ao Cliente sera punido\n";
			pst=false;
			break;
		case 5:
			if(jogadores[0].departamentos[3][1]){
				jogadores[0].saldo+=1000;
				jogAfetado=0;
			}
			else{
				jogadores[1].saldo+=750;
				jogAfetado=1;
			}
			cout << "Excelente feedback recebido por parte dos clientes\n";
			cout << "O titular do departamento Servico ao Cliente sera remonerado\n";
			break;
		case 6:
			if(jogadores[0].departamentos[2][3]){
				jogadores[0].saldo-=1000;
				jogAfetado=0;
			}
			else{
				jogadores[2].saldo-=750;
				jogAfetado=2;
			}
			cout << "Sera retirado um montante ao titular do departamento dos recursos Humanos\n";
			pst=false;
			break;
		case 7:
			if(jogadores[0].departamentos[2][3]){
				jogadores[0].saldo+=1000;
				jogAfetado=0;
			}
			else{
				jogadores[2].saldo+=750;
				jogAfetado=2;
			}
			cout << "Departamento dos Recursos Humanos premiado pela aquisicao dos novos trabalhadores\n";
			break;
		case 8:
			if(jogadores[0].departamentos[3][2]){
				jogadores[0].saldo-=1000;
				jogAfetado=0;
			}
			else{
				jogadores[1].saldo-=750;
				jogAfetado=1;
			}
			cout << "O titular do departamento do Marketing tera que recompensar a empresa\n";
			pst=false;
			break;
		case 9:
			if(jogadores[0].departamentos[3][2]){
				jogadores[0].saldo+=1000;
				jogAfetado=0;
			}
			else{
				jogadores[1].saldo+=750;
				jogAfetado=1;
			}
			cout << "O departamento do Marketing esta de parabens por este feito e vai premiar o seu titular\n";
			break;
		case 10:
			if(jogadores[0].departamentos[1][3]){
				jogadores[0].saldo-=1000;
				jogAfetado=0;
			}
			else{
				jogadores[2].saldo-=750;
				jogAfetado=2;
			}
			cout << "O titular do departamento Controlo de Qualidade decide investir do proprio bolso para melhorar equipamento\n";
			pst=false;
			break;
		case 11:
			if(jogadores[0].departamentos[1][3]){
				jogadores[0].saldo+=1000;
				jogAfetado=0;
			}
			else{
				jogadores[2].saldo+=750;
				jogAfetado=2;
			}
			cout << "O ultimo relatorio mostra que a empresa conseguiu ultrapassar a qualidade da concorrencia\n";
			cout << "O titular do departamento Controlo de Qualidade sera recompensado\n";
			break;
		case 12:
			if(jogadores[0].departamentos[0][3]){
				jogadores[0].saldo-=1000;
				jogAfetado=0;
			}
			else{
				jogadores[2].saldo-=750;
				jogAfetado=2;
			}
			cout << "O titular do departamento de Producao vai ter que investir no seu proprio departamento\n";
			pst=false;
			break;
		case 13:
			if(jogadores[0].departamentos[0][3]){
				jogadores[0].saldo+=1000;
				jogAfetado=0;
			}
			else{
				jogadores[2].saldo+=750;
				jogAfetado=2;
			}
			cout << "A empresa decidiu investir o departamento de Producao o seu titular recebera uma quantia cnsideravel\n";
			break;
		case 14:
			if(jogadores[0].departamentos[3][0]){
				jogadores[0].saldo-=1000;
				jogAfetado=0;
			}
			else{
				jogadores[1].saldo-=750;
				jogAfetado=1;
			}
			cout <<"O titular do departamento Armazem tera que recompesar os clientes afetados\n";
			pst=false;
			break;
		case 15:
			if(jogadores[0].departamentos[3][0]){
				jogadores[0].saldo+=1000;
				jogAfetado=0;
			}
			else{
				jogadores[1].saldo+=750;
				jogAfetado=1;
			}
			cout <<"Um investidor decide ajudar o departamento Armazem oferecendo uma quantia consideravel ao seu titular\n";
			break;
		case 16:
			if(jogadores[0].departamentos[3][6]){
				jogadores[0].saldo-=1000;
				jogAfetado=0;
			}
			else{
				jogadores[3].saldo-=750;
				jogAfetado=3;
			}
			pst=false;
			cout << "A equipa da logistica vai ter que gastar os seus recursos para recompesar os gastos desnecessarios da empresa\n";
			break;
		case 17:
			if(jogadores[0].departamentos[3][6]){
				jogadores[0].saldo+=1000;
				jogAfetado=0;
			}
			else{
				jogadores[3].saldo+=750;
				jogAfetado=3;
			}
			cout << "O titular do departamento Logistica sera recompesado\n";
			break;
		case 18:
			jogadores[0].saldo+=1000;
			jogadores[1].saldo+=1000;
			jogadores[2].saldo+=1000;
			jogadores[3].saldo+=1000;
			cout << "A empresa vai recompensar todos os seus funcionarios (+1000 €)\n";
			break;
		case 19:
			jogadores[0].saldo-=1000;
			jogadores[1].saldo-=1000;
			jogadores[2].saldo-=1000;
			jogadores[3].saldo-=1000;
			cout << "A empresa vai cortar orcamento dos seus funcionarios (-1000 €)\n";
			break;
	}

	if(jogAfetado==0 && !pst){
		cout << "Foi retirado " << 1000 <<"€"<< " ao jogador "<<jogadores[0].nome<<endl;
	}
	else if(jogAfetado==0 && pst){
		cout << "Foi adicionado " << 1000 <<"€"<< " ao jogador "<<jogadores[0].nome<<endl;
	}
	else if(pst){
		cout << "Foi adicionado " << 750 <<"€"<< " ao jogador "<<jogadores[jogAfetado].nome<<endl;
	}
	else{
		cout << "Foi retirado " << 750 <<"€"<< " ao jogador "<<jogadores[jogAfetado].nome<<endl;
	}

	cout<<r<<endl;

	char c;
	c=getch();
}
void printMoney(){
	for(int i = 0;i<4;i++){
		cout << jogadores[i].nome << ": " << jogadores[i].saldo<< "€" << endl; 
	}
}

void joga(int n){
	cout << "Vez do " << jogadores[n].nome << endl;
	printProfile(n);
	if(n!=0){//employees
		cout << "	Pressiona tecla 1 para mostrar os seus departamentos\n";
		cout << "	Pressiona tecla 2 para mostrar saldo dos trabalhadores\n";
		cout << "	Pressiona tecla 3 para trabalhar\n";
		if(jogadores[n].saldo<500)
			cout << RED << "	Pressiona tecla 4 para aumentar rendimento (-500 € -> +150€/semana)\n"<< RESET;
		else
			cout <<"	Pressiona tecla 4 para aumentar rendimento (-500 € -> +150€/semana)\n";
	
		if(jogadores[n].saldo<750)
			cout << RED << "   	Pressiona tecla 5 para fazer queixa aos recursos humanos do chefe (-1000€ -> -1250€ Chefe)\n"<< RESET;
		else
			cout << "	Pressiona tecla 5 para fazer queixa do chefe aos recursos humanos (-1000€ -> -1250€ Chefe)\n";

		if(jogadores[n].saldo<2500)
			cout << RED <<"	Pressiona tecla 6 para comprar departamento adjacente da empresa (-2500€)\n" << RESET;
		else
			cout <<"	Pressiona tecla 6 para comprar departamento adjacente da empresa (-2500€)\n";

		char c;
		while((c = getch())=='1'||c=='2'){
			if(c=='1'){
				printJDeps(n);
			}
			if(c=='2'){
				printMoney();
			}
		}

		switch (c){
			case '3':
				cout << RED  <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
				jogadores[n].saldo+=jogadores[n].income;
				cout << GREEN <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
				break;
			case '4':
				if(jogadores[n].saldo<500){
					cout << "SKIP\n";
					cout << RED  <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
					jogadores[n].saldo+=jogadores[n].income;
					cout << GREEN <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
				}
				else{
					cout <<"	Saldo: " << jogadores[n].saldo << "€" <<endl;
					jogadores[n].saldo-=500;
					cout << RED  <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
					jogadores[n].income+=150;
					cout << GREEN <<"	Rendimento: " << jogadores[n].income << "€" << RESET <<endl;
				}
				break;
			case '5':
				if(jogadores[n].saldo<750){
					cout << "SKIP\n";
					cout << RED  <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
					jogadores[n].saldo+=jogadores[n].income;
					cout << GREEN <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
				}
				else{
					cout <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
					jogadores[n].saldo-=1000;
					cout << RED  <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
					jogadores[0].saldo-=1250;
					cout << RED  <<"	Saldo do Jogador: "<< jogadores[0].nome << " " <<jogadores[0].saldo << "€" << RESET <<endl;
				}
				break;
			case '6':
				if(jogadores[n].saldo<2500){
					cout << "SKIP\n";
					cout << RED  <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
					jogadores[n].saldo+=jogadores[n].income;
					cout << GREEN <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
				}
				else{
					cout <<"	Saldo: " << jogadores[n].saldo << "€"  <<endl;
					jogadores[n].saldo-=2500;
					cout << RED  <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
					if(n==1){
						if(jogadores[n].departamentos[3][1]){
							if(jogadores[n].departamentos[3][2]){
								cout << "Adquiriu :"<< nomedep[3][3] <<endl;
								jogadores[n].departamentos[3][3]=true;
								jogadores[0].alive=false;
							}
							else{
								cout << "Adquiriu :"<< nomedep[3][2] <<endl;
								jogadores[0].departamentos[3][2]=false;
								tabuleiro[3][2]=Empr;
								jogadores[n].departamentos[3][2]=true;
							}
						}
						else{
							cout << "Adquiriu :"<< nomedep[3][1] <<endl;
							jogadores[0].departamentos[3][1]=false;
							tabuleiro[3][1]=Empr;
							jogadores[n].departamentos[3][1]=true;
						}
					}
					if(n==2){
						if(jogadores[n].departamentos[1][3]){
							if(jogadores[n].departamentos[2][3]){
								cout << "Adquiriu :"<< nomedep[3][3] <<endl;
								jogadores[n].departamentos[3][3]=true;
								jogadores[0].alive=false;
								tabuleiro[3][3]=Empr;
							}
							else{
								cout << "Adquiriu :"<< nomedep[2][3] <<endl;
								jogadores[0].departamentos[2][3]=false;
								tabuleiro[2][3]=Empr;
								jogadores[n].departamentos[2][3]=true;
							}
						}
						else{
							cout << "Adquiriu :"<< nomedep[1][3] <<endl;
							jogadores[0].departamentos[1][3]=false;
							tabuleiro[1][3]=Empr;
							jogadores[n].departamentos[1][3]=true;
						}
					}
					if(n==3){
						if(jogadores[n].departamentos[3][5]){
							if(jogadores[n].departamentos[3][4]){
								cout << "Adquiriu :"<< nomedep[3][3] <<endl;
								jogadores[n].departamentos[3][3]=true;
								jogadores[0].alive=false;
								tabuleiro[3][3]=Empr;
							}
							else{
								cout << "Adquiriu :"<< nomedep[3][4] <<endl;
								jogadores[0].departamentos[3][4]=false;
								jogadores[n].departamentos[3][4]=true;
								tabuleiro[3][4]=Empr;
							}
						}
						else{
							cout << "Adquiriu :"<< nomedep[3][5] <<endl;
							jogadores[0].departamentos[3][5]=false;
							jogadores[n].departamentos[3][5]=true;
							tabuleiro[3][5]=Empr;
						}
					}
				}
		}
	}
	else{//BOSS

		cout << "	Pressiona tecla 1 para mostrar os seus departamentos\n";
		cout << "	Pressiona tecla 2 para mostrar saldo dos trabalhadores\n";
		cout << "	Pressiona tecla 3 para trabalhar\n";
		if(jogadores[n].saldo<1000)
			cout << RED << "	Pressiona tecla 4 para aumentar rendimento (-1000 € -> +250€/semana)\n"<< RESET;
		else
			cout <<"	Pressiona tecla 4 para aumentar rendimento (-1000 € -> +250€/semana)\n";
	
		if(jogadores[n].saldo<1000)
			cout << RED << "	Pressiona tecla 5 para delegar horas extras nao remoneradas (-1000 € -> -550 € Empregados)\n"<< RESET;
		else
			cout << "	Pressiona tecla 5 para delegar horas extras nao remoneradas (-1000 € -> -550 € Empregados)\n";

		if(jogadores[n].saldo<4000)
			cout << RED <<"	Pressiona tecla 6 para rescindir contrato com o jogador com maior rendimento (-4000 €)\n" << RESET;
		else
			cout <<"	Pressiona tecla 6 para rescindir contrato com o jogador com maior rendimento (-4000 €)\n";

		char c;
		while((c = getch())=='1'||c=='2'){
			if(c=='1'){
				printJDeps(n);
			}
			if(c=='2'){
				printMoney();
			}
		}
		switch (c){
			case '3':
				cout << RED  <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
				jogadores[n].saldo+=jogadores[n].income;
				cout << GREEN <<"	" << jogadores[n].saldo << "€" << RESET <<endl;
				break;
			case '4':
				if(jogadores[n].saldo<1000){
					cout << "SKIP\n";
					cout << RED  <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
					jogadores[n].saldo+=jogadores[n].income;
					cout << GREEN <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
				}
				else{
					cout <<"	Saldo: " << jogadores[n].saldo << "€" <<endl;
					jogadores[n].saldo-=1000;
					cout << RED  <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
					jogadores[n].income+=250;
					cout << GREEN  <<"	Rendimento: " << jogadores[n].income << "€" << RESET <<endl;
				}
				break;
			case '5':
				if(jogadores[n].saldo<1000){
					cout << "SKIP\n";
					cout << RED  <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
					jogadores[n].saldo+=jogadores[n].income;
					cout << GREEN <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
				}
				else{
					cout  <<"	Saldo: " << jogadores[0].saldo << "€" <<endl;
					jogadores[0].saldo-=1000;
					cout  <<RED<<"	Saldo: " << jogadores[0].saldo << "€" << RESET <<endl;
					jogadores[1].saldo-=550;
					jogadores[2].saldo-=550;
					jogadores[3].saldo-=550;
					cout << RED  <<"	Saldo do Jogador: "<< jogadores[1].nome << " " <<jogadores[1].saldo << "€" << RESET <<endl;
					cout << RED  <<"	Saldo do Jogador: "<< jogadores[2].nome << " " <<jogadores[2].saldo << "€" << RESET <<endl;
					cout << RED  <<"	Saldo do Jogador: "<< jogadores[3].nome << " " <<jogadores[3].saldo << "€" << RESET <<endl;
				}
				break;
			case '6':
				if(jogadores[n].saldo<4000){
					cout << "SKIP\n";
					cout << RED  <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
					jogadores[n].saldo+=jogadores[n].income;
					cout << GREEN <<"	Saldo: " << jogadores[n].saldo << "€" << RESET <<endl;
				}
				else{
					jogadores[0].saldo-=4000;
					if(jogadores[1].income>jogadores[2].income){
						if(jogadores[1].income>jogadores[3].income){
							cout<<"O jogador :" << jogadores[0].nome << " rescindiu com " << jogadores[1].nome<<endl;
							jogadores[1].alive=false;
							jogadores[0].departamentos[3][0]=true;
							jogadores[0].departamentos[3][1]=true;
							jogadores[0].departamentos[3][2]=true;
						}
						else{
							cout<<"O jogador :" << jogadores[0].nome << " rescindiu com " << jogadores[3].nome<<endl;
							jogadores[3].alive=false;
							jogadores[0].departamentos[3][6]=true;
							jogadores[0].departamentos[3][5]=true;
							jogadores[0].departamentos[3][4]=true;
						}
					}
					else if(jogadores[2].income>jogadores[3].income){
						cout<<"O jogador :" << jogadores[0].nome << " rescindiu com " << jogadores[2].nome<<endl;
						jogadores[0].departamentos[0][3]=true;
						jogadores[0].departamentos[1][3]=true;
						jogadores[0].departamentos[2][3]=true;
						jogadores[2].alive=false;
					}
					else {
						cout<<"O jogador :" << jogadores[0].nome << " rescindiu com " << jogadores[3].nome<<endl;
						jogadores[3].alive=false;
						jogadores[0].departamentos[3][6]=true;
						jogadores[0].departamentos[3][5]=true;
						jogadores[0].departamentos[3][4]=true;
					}
						
				}
				break;								
		}
	}
	char c;
	c=getch();
}

int main(){

	cout<<"1-Iniciar Jogo\n2-Tabuleiro\n3-Departamentos\n4-Regras\n";
    
    char tmp;
	tmp=getch();
	while(tmp!='1'){
		if(tmp=='2'){
			printTab();
			tmp=getch();
		}
		if(tmp=='3'){
			printDeps();
			tmp=getch();
		}
		if(tmp=='4'){
			regras();
			tmp=getch();
		}
	}

	system("clear");

    string nome;
    for(int i = 0; i<4 ; i++){
    	cout << "Introduza o nome do " << i+1 << "º jogador\n";
    	cin >> nome;
		jogadores[i]=mkJgd(nome,i);
    }
    int t1 = getch();
    system("clear");
    int c=0;
    bool inc = true;

    for(int j=0;j<=24;j++){
    	system("clear");
    	cout << j+1 << "ª semana\n";
		t1 = getch();    	
   		if(fimdejogo() || j==24){
   			cout << "Winner!\n"<< jogVivo() <<endl;
   			t1 = getch();
   			return 0;
   		}
   		if(c>=3){
   			inc=false;
   			c=3;
   		}
   		else{
   			inc = true;
   			c=0;
   		}
   		while(c>=0 && c<4){
   			system("clear");
   			if(inc){
   				if(!jogadores[c].alive){
   					c++;
   					continue;
   				}
   				joga(c);
   				c++;
   			}
   			else{
   				if(!jogadores[c].alive){
   					c--;
   					continue;
   				}
   				joga(c);
   				c--;
   			}
   		}

   		if(!inc){
   			evento();
   		}

   		system("clear");
   		printTab();
   		t1 = getch();

   		if(!jogadores[0].alive || jogadores[0].saldo<0){
   			jogadores[0].alive=false;
   			tabuleiro[3][0]=Empr;
   			tabuleiro[3][1]=Empr;
   			tabuleiro[3][2]=Empr;
   			tabuleiro[3][0]=Empr;
   			tabuleiro[3][1]=Empr;
   			tabuleiro[3][2]=Empr;
   			tabuleiro[3][6]=Empr;
   			tabuleiro[3][5]=Empr;
   			tabuleiro[3][4]=Empr;
   			tabuleiro[3][3]=Empr;
			cout<<"Os seus servicos nao serao mais necessarios nesta empresa " << jogadores[0].nome << endl;
   			t1 = getch();
   		}
   		if(!jogadores[1].alive || jogadores[1].saldo<0){
   			jogadores[1].alive=false;
   			jogadores[0].departamentos[3][0]=true;
   			jogadores[0].departamentos[3][1]=true;
   			jogadores[0].departamentos[3][2]=true;
   			jogadores[0].departamentos[3][3]=true;
   			//jogadores[1].departamentos[3][0]=true;
   			jogadores[1].departamentos[3][1]=false;
   			jogadores[1].departamentos[3][2]=false;
   			jogadores[1].departamentos[3][3]=false;

   			tabuleiro[3][0]=Boss;
   			tabuleiro[3][1]=Boss;
   			tabuleiro[3][2]=Boss;
   			tabuleiro[3][3]=Boss;

   			cout<<"Os seus servicos nao serao mais necessarios nesta empresa " << jogadores[1].nome << endl;
   			t1 = getch();
   		}
   		if(!jogadores[2].alive  || jogadores[2].saldo<0){
   			jogadores[2].alive=false;
   			jogadores[0].departamentos[0][3]=true;
   			jogadores[0].departamentos[1][3]=true;
   			jogadores[0].departamentos[2][3]=true;
   			tabuleiro[0][3]=Boss;
   			tabuleiro[1][3]=Boss;
   			tabuleiro[2][3]=Boss;
   			jogadores[2].departamentos[0][3]=false;
   			jogadores[2].departamentos[1][3]=false;
   			jogadores[2].departamentos[2][3]=false;
   			cout<<"Os seus servicos nao serao mais necessarios nesta empresa " << jogadores[2].nome << endl;
   			t1 = getch();
   		}
   		if(!jogadores[3].alive  || jogadores[3].saldo<0){
   			jogadores[3].alive=false;
   			jogadores[0].departamentos[3][6]=true;
   			jogadores[0].departamentos[3][5]=true;
   			jogadores[0].departamentos[3][4]=true;
   			jogadores[0].departamentos[3][3]=true;
   			tabuleiro[3][6]=Boss;
   			tabuleiro[3][5]=Boss;
   			tabuleiro[3][4]=Boss;
   			jogadores[3].departamentos[3][6]=false;
   			jogadores[3].departamentos[3][5]=false;
   			jogadores[3].departamentos[3][4]=false;
   			cout<<"Os seus servicos nao serao mais necessarios nesta empresa " << jogadores[3].nome << endl;
   			t1 = getch();
   		}
    }
}