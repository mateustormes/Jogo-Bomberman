//Mateus Tormes Gervazioni
#include <allegro.h>
#include "funções para o joystick funcionar\xbox.c"
//função para fechar o jogo
volatile int exit_program = FALSE;
int cont1=200,cont2=200;
void fecha_program(){
exit_program=TRUE;
}END_OF_FUNCTION(fecha_program)

//retorna true a cada intervalo de milisegundos
bool AtualizaTempoPersonagem1(int tmilisegundos){
static double tempoPERS;
static clock_t t0pac=clock(), tfpac;
tfpac=clock();
tempoPERS=( (double)(tfpac - t0pac)) / (CLOCKS_PER_SEC/1000);
if(tempoPERS>tmilisegundos){
    tempoPERS=0;
    t0pac=clock();
    return true;
}return false;
}
bool AtualizaTempoPersonagem2(int tmilisegundos){
static double tempoPERS;
static clock_t t0pac=clock(), tfpac;
tfpac=clock();
tempoPERS=( (double)(tfpac - t0pac)) / (CLOCKS_PER_SEC/1000);
if(tempoPERS>tmilisegundos){
    tempoPERS=0;
    t0pac=clock();
    return true;
}return false;
}

//retorna true a cada intervalo de milisegundos
bool AtualizaTempoBomba(int tmilisegundos) // tempo para bomba sumir
{
static double tempoPERS;
static clock_t t0pac=clock(), tfpac;
tfpac=clock();
tempoPERS=( (double)(tfpac - t0pac)) / (CLOCKS_PER_SEC/1000);
if(tempoPERS>tmilisegundos){
    tempoPERS=0;
    t0pac=clock();
    return true;
}return false;
}

//retorna true a cada intervalo de milisegundos
bool AtualizaTempoFogo(int tmilisegundos) // tempo para o fogo sumir
{
static double tempoPERS;
static clock_t t0pac=clock(), tfpac;
tfpac=clock();
tempoPERS=( (double)(tfpac - t0pac)) / (CLOCKS_PER_SEC/1000);
if(tempoPERS>tmilisegundos){
    tempoPERS=0;
    t0pac=clock();
    return true;
}return false;}

// estrutura de informacoes dos personagens
typedef struct{
	int x;
	int y;
	int bomba;
	int raio;
	int life;
    int vida;
    int morreu;
	BITMAP *sprite;
}PERSONAGEM;

PERSONAGEM personagem_1,personagem_2;  //criando os 2 personagens

int mapa[13][13]={
	{1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,9,9,9,9,9,9,9,9,9,9,9,1},	//1 - bloco fixo                             //20 - caveira
	{1,9,1,8,1,8,1,8,1,8,1,9,1},	//2 - bomba do personagem                    //13 - vida bonus
	{1,9,8,8,8,8,8,8,8,8,8,9,1},	//3 - explosao
	{1,9,1,8,1,8,1,8,1,8,1,9,1},	//4 - poder bomba
	{1,9,8,8,8,8,8,8,8,8,8,9,1},	//5 - poder de fogo
	{1,9,1,8,1,8,11,8,1,8,1,9,1},	//6 - bomba do personagem2
	{1,9,8,8,8,8,8,8,8,8,8,9,1},    //7 - mina terrestre
	{1,9,1,8,1,8,1,8,1,8,1,9,1},	//8 - bloco destrutivel
	{1,9,8,8,8,8,8,8,8,8,8,9,1},	//9 - espaco em branco
	{1,9,1,8,1,8,1,8,1,8,1,9,1},    //10 - bota de velocidade
	{1,9,9,9,9,9,9,9,9,9,9,9,1},   //11 - portal
	{1,1,1,1,1,1,1,1,1,1,1,1,1}             //12 gelo
};
int auxiliapersonagem1y,auxiliapersonagem1x,auxiliapersonagem2y,auxiliapersonagem2x;
int timeFogo[13][13] = {0}; // matriz para calcular o tempo do fogo quando chegar em 0 o fogo some
int timeBomba[13][13] = {0}; // matriz para calcular o tempo da bomba cada personagem tem o tempo especifico
void explosao(int x, int y, int sx, int sy, int i, int p) // funcao para explodir o local onde ira aparecer o fogo
{// x e y posicao no eixo x do personagem   ; sx e sy posicao anterior no eixo x do personagem,  i  local exato onde o personagem está, e o p o tamanho do raio
	if(i>=p)return; // se o local for maior que o raio da bomba ele retorna por que nao conseguiu acertar
	if(mapa[x][y]==1)return; // se o local for um bloco fixo retorna
	if(mapa[x][y]==11)return; //se o local for o portal retorna

	if(mapa[x][y]==8){ // se o local tiver um bloco destrutivel explode e retorna fogo ou poder
		int r = (rand()%14); // aleatorio para guardar uma numeracao e depois verificar se vai ter poder ou nao
		if(r==4){ // se o aleatorio receber 4 aparece no local o poder de mais bomba e retorna
			mapa[x][y]=4;
			return;
		}
		if(r==5){ // se o aleatorio receber 5 aparece no local o poder para aumentar o raio da bomba e retorna
			mapa[x][y]=5;
			return;
		}
		if(r==10){
            mapa[x][y]=10;
            return;
		}
		if(r==12){
            mapa[x][y]=12;
            return;
		}
		if(r==4){
            mapa[x][y]=4;
		}
		if(r==13){
            mapa[x][y]=13;
            return;
		}

		mapa[x][y]=3;
		timeFogo[x][y]=2;
		return;
	}
	if(mapa[x][y]==2){ // se o local tiver uma bomba do personagem 1 o tempo recebe 1 fazendo-a explodir em 1 segundo ou menos
		timeBomba[x][y]=1;
		return;
	}
	if(mapa[x][y]==6){ // se o local tiver uma bomba do personagem 2 o tempo recebe 8 fazendo-a explodir em 1 segundo ou menos
		timeBomba[x][y]=1;
		return;
	}
	if(x==personagem_1.y && y==personagem_1.x && personagem_1.morreu==0){ // se o local estiver o personagem 1 ele volta para a posicao inicial e e recebe -1 life
        auxiliapersonagem1y=personagem_1.y;
        auxiliapersonagem1x=personagem_1.x;
        personagem_1.y=1;
		personagem_1.x=1;
		personagem_1.vida= personagem_1.vida -1; // se a bomba acertar ele ,perde uma vida
		cont1=200;
		personagem_1.morreu=30;
	}
	if(x==personagem_2.y && y==personagem_2.x && personagem_2.morreu==0){ // se o local estiver o personagem 2 ele volta para a posicao inicial e e recebe -1 life
		auxiliapersonagem2y=personagem_2.y;
		auxiliapersonagem2x=personagem_2.x;
		personagem_2.y=11;
		personagem_2.x=11;
        personagem_2.vida= personagem_2.vida -1; // se a bomba acertar ele ,perde uma vida
        cont2=200;
        personagem_2.morreu=30;
	}
	mapa[x][y]=3;
	timeFogo[x][y] = 2;
	explosao(x+sx,y+sy,sx,sy,i+1,p); // ir para a proxima posicao
	if(personagem_2.vida<3)
    mapa[auxiliapersonagem2y][auxiliapersonagem2x]=20;
    if(personagem_1.vida<3)
    mapa[auxiliapersonagem1y][auxiliapersonagem1x]=20;
}

void explode(int x,int y, int p) // funcao para explodir as posições ao redor da bomba
{
	explosao(x,y,1,0,0,p); //em baixo
	explosao(x,y,-1,0,0,p); //em cima
	explosao(x,y,0,-1,0,p); //esquerda
    explosao(x,y,0,1,0,p); //direita
}
//chamando função padrão do allegro
void init();
void deinit();
//prototipo da função menu
int menu();
void parar_tela();
void jogar();
//variavel que vai receber e guardar o valor da função escolhida, menu ou sair
int op;

int main() {

init();
menu();

op = menu(); //variavelc que recebe uma funcao chamado menu e retorna um inteiro, 1 - jogar  2 - sair
personagem_1.morreu=0;
personagem_2.morreu=0;
switch(op)
{
	case 2:{ //2 - sai do jogo
		return 0;
		exit(1);
		break;
	}
}
//atributos do personagem 1
personagem_1.x = 1;
personagem_1.y = 1;
personagem_1.vida = 3;
personagem_1.bomba = 1;
personagem_1.raio = 2;
personagem_1.sprite=load_bitmap("sprites/ordi_1.bmp", NULL);

//atributos do personagem 2
personagem_2.x = 11; // inicia na posicao 11 do eixo x
personagem_2.y = 11; // inicia na posicao 11 do eixo y
personagem_2.vida = 3; //inicia com 3 vidas
personagem_2.bomba = 1; //inicia podendo usar apenas 1 bomba
personagem_2.raio = 2; //inicia com o raio da bomba emtorno do personagem em 2
personagem_2.sprite=load_bitmap("sprites/ordi_2.bmp", NULL); //carrega a imagem do bomberman

	//carregamento das imagens
	BITMAP *blocoF=load_bitmap("sprites/blocoFixo.bmp",NULL);
	BITMAP *bloco=load_bitmap("sprites/bloco.bmp", NULL);
	BITMAP *bomba=load_bitmap("sprites/bomba.bmp", NULL);
	BITMAP *fogo=load_bitmap("sprites/fire.bmp", NULL);
	BITMAP *poderbomba=load_bitmap("sprites/poderbomba.bmp", NULL);
	BITMAP *poderfogo=load_bitmap("sprites/poderfogo.bmp",NULL);
	BITMAP *podervelocidade=load_bitmap("sprites/boots.bmp",NULL);
	BITMAP *portal=load_bitmap("sprites/portal.bmp",NULL);
	BITMAP *podergelo=load_bitmap("sprites/gelo.bmp",NULL);
	BITMAP *caveira=load_bitmap("sprites/caveira.bmp",NULL);
	BITMAP *mina=load_bitmap("sprites/mina.bmp",NULL);
	BITMAP *vida=load_bitmap("sprites/vida.bmp",NULL);
	BITMAP *fundo=create_bitmap(800,600);


	//variaveis recebe o tempo de execucao
    bool movimentaPersonagem1 = false;
    bool movimentaPersonagem2 = false;
    bool executaBomba = false;
    bool executaFogo = false;

	//som durante o jogo
	MIDI *MusicaInicio;
    MusicaInicio=load_midi("sons/level.mid");
    play_midi(MusicaInicio,TRUE);
while (!exit_program) {//enquanto não for pressionado a tecla esc nao é fechado esse enquanto
    //se o usuario pressionar a tecla esc, chama a função que retorna true e fecha o enquanto
    clear_to_color(fundo,makecol(28,183,32));
    xbox_input(); // chama a função do joystick
    if(key[KEY_ESC]){
            fecha_program();
    }
	if(key[KEY_P]){
        parar_tela();
	}
	if(personagem_1.morreu>0)
	personagem_1.morreu--;

	if(personagem_2.morreu>0)
	personagem_2.morreu--;

	//variaveis atribui o valord do tempo nas funcoes
	movimentaPersonagem1 = AtualizaTempoPersonagem1(cont1);
	movimentaPersonagem2 = AtualizaTempoPersonagem2(cont2);
	executaBomba = AtualizaTempoBomba(400);
	executaFogo = AtualizaTempoFogo(200);
	//mover o personagem_1
	int randomx,randomy; //variaveis para guardar valor aleatorio de teletransporte
	if(mapa[personagem_1.y][personagem_1.x]==11){
        randomx = (rand()%12);
        randomy = (rand()%12);
        personagem_1.y=randomy;
        personagem_1.x=randomx;
	}
	if(mapa[personagem_1.y][personagem_1.x]==8 || mapa[personagem_1.y][personagem_1.x]==1){
            randomx = (rand()%12);
            randomy = (rand()%12);
            personagem_1.y=randomy;
            personagem_1.x=randomx;
    }
    if(mapa[personagem_2.y][personagem_2.x]==11){
        randomx = (rand()%12);
        randomy = (rand()%12);
        personagem_2.y=randomy;
        personagem_2.x=randomx;
	}
	if(mapa[personagem_2.y][personagem_2.x]==8 || mapa[personagem_2.y][personagem_2.x]==1){
            randomx = (rand()%12);
            randomy = (rand()%12);
            personagem_2.y=randomy;
            personagem_2.x=randomx;
    }
	if(key[KEY_RIGHT] && mapa[personagem_1.y][personagem_1.x + 1] != 1 && mapa[personagem_1.y][personagem_1.x + 1] != 8 && mapa[personagem_1.y][personagem_1.x + 1] != 2 && mapa[personagem_1.y][personagem_1.x + 1] != 6 && movimentaPersonagem1){
		personagem_1.x++;
		if(personagem_1.x==personagem_2.x && personagem_1.y==personagem_2.y){ // faz o teste para o player 1 não entrar no player 2
                personagem_1.x--;
        }
        if(mapa[personagem_1.y][personagem_1.x+1]==2){
            mapa[personagem_1.y][personagem_1.x+1]=0;//apaga a bomba
            mapa[personagem_1.y][personagem_1.x+2]=2;//coloca a bomba 1 posicao a frente
        }
	}else
	if(key[KEY_LEFT] && mapa[personagem_1.y][personagem_1.x - 1] != 1 && mapa[personagem_1.y][personagem_1.x - 1] != 8 && mapa[personagem_1.y][personagem_1.x - 1] != 2 && mapa[personagem_1.y][personagem_1.x - 1] != 6 && movimentaPersonagem1){
		personagem_1.x--;
        if(personagem_1.x==personagem_2.x && personagem_1.y==personagem_2.y){ // faz o teste para o player 1 não entrar no player 2
                personagem_1.x++;
        }
	}else
	if(key[KEY_UP] && mapa[personagem_1.y - 1][personagem_1.x] != 1 && mapa[personagem_1.y - 1][personagem_1.x] != 8 && mapa[personagem_1.y - 1][personagem_1.x] != 2 && mapa[personagem_1.y - 1][personagem_1.x] != 6 && movimentaPersonagem1){
		personagem_1.y--;
		if(personagem_1.x==personagem_2.x && personagem_1.y==personagem_2.y){ // faz o teste para o player 1 não entrar no player 2
                personagem_1.y++;
        }
	}else
	if(key[KEY_DOWN] && mapa[personagem_1.y + 1][personagem_1.x] != 1 && mapa[personagem_1.y + 1][personagem_1.x] != 8 && mapa[personagem_1.y + 1][personagem_1.x] != 2 && mapa[personagem_1.y + 1][personagem_1.x] != 6 && movimentaPersonagem1){
		personagem_1.y++;
		if(personagem_1.x==personagem_2.x && personagem_1.y==personagem_2.y){ // faz o teste para o player 1 não entrar no player 2
                personagem_1.y--;
        }
	}
	//controle do personagem pelo joystick
	if(xbox[LSTICK_RIGHT] && mapa[personagem_1.y][personagem_1.x + 1] != 1 && mapa[personagem_1.y][personagem_1.x + 1] != 8 && mapa[personagem_1.y][personagem_1.x + 1] != 2 && mapa[personagem_1.y][personagem_1.x + 1] != 6 && movimentaPersonagem1){
		personagem_1.x++;
		if(personagem_1.x==personagem_2.x && personagem_1.y==personagem_2.y){ // faz o teste para o player 1 não entrar no player 2
                personagem_1.x--;
        }
        if(mapa[personagem_1.y][personagem_1.x+1]==2){
            mapa[personagem_1.y][personagem_1.x+1]=0;//apaga a bomba
            mapa[personagem_1.y][personagem_1.x+2]=2;//coloca a bomba 1 posicao a frente
        }
	}else
	if(xbox[LSTICK_LEFT] && mapa[personagem_1.y][personagem_1.x - 1] != 1 && mapa[personagem_1.y][personagem_1.x - 1] != 8 && mapa[personagem_1.y][personagem_1.x - 1] != 2 && mapa[personagem_1.y][personagem_1.x - 1] != 6 && movimentaPersonagem1){
		personagem_1.x--;
        if(personagem_1.x==personagem_2.x && personagem_1.y==personagem_2.y){ // faz o teste para o player 1 não entrar no player 2
                personagem_1.x++;
        }
	}else
	if(xbox[LSTICK_UP] && mapa[personagem_1.y - 1][personagem_1.x] != 1 && mapa[personagem_1.y - 1][personagem_1.x] != 8 && mapa[personagem_1.y - 1][personagem_1.x] != 2 && mapa[personagem_1.y - 1][personagem_1.x] != 6 && movimentaPersonagem1){
		personagem_1.y--;
		if(personagem_1.x==personagem_2.x && personagem_1.y==personagem_2.y){ // faz o teste para o player 1 não entrar no player 2
                personagem_1.y++;
        }
	}else
	if(xbox[LSTICK_DOWN] && mapa[personagem_1.y + 1][personagem_1.x] != 1 && mapa[personagem_1.y + 1][personagem_1.x] != 8 && mapa[personagem_1.y + 1][personagem_1.x] != 2 && mapa[personagem_1.y + 1][personagem_1.x] != 6 && movimentaPersonagem1){
		personagem_1.y++;
		if(personagem_1.x==personagem_2.x && personagem_1.y==personagem_2.y){ // faz o teste para o player 1 não entrar no player 2
                personagem_1.y--;
        }
	}
	//solta a bomba pelo joystick
	if(xbox[BUTTON_X] && mapa[personagem_1.y][personagem_1.x] != 2 && mapa[personagem_1.y][personagem_1.x] != 6 && personagem_1.bomba > 0 && mapa[personagem_1.y][personagem_1.x]!=10 && mapa[personagem_1.y][personagem_1.x]!=12){
		mapa[personagem_1.y][personagem_1.x] = 2;
		timeBomba[personagem_1.y][personagem_1.x]=6;
		personagem_1.bomba--;
	}
	//pausa a leta pelo joystick
	if(xbox[BUTTON_B])
        parar_tela();
	//solta bomba player 1
	if(key[KEY_END] && mapa[personagem_1.y][personagem_1.x] != 2 && mapa[personagem_1.y][personagem_1.x] != 6 && personagem_1.bomba > 0 && mapa[personagem_1.y][personagem_1.x]!=10 && mapa[personagem_1.y][personagem_1.x]!=12){
		mapa[personagem_1.y][personagem_1.x] = 2;
		timeBomba[personagem_1.y][personagem_1.x]=6;
		personagem_1.bomba--;
	}
	//ganha uma bomba player 1
	if(mapa[personagem_1.y][personagem_1.x]==4){
		mapa[personagem_1.y][personagem_1.x]=0;
		personagem_1.bomba++;
	}
	//aumenta o raio da bomba player 1
	if(mapa[personagem_1.y][personagem_1.x]==5){
		mapa[personagem_1.y][personagem_1.x]=0;
		personagem_1.raio++;
	}
	//aumenta a velocidade do player 1
	if(mapa[personagem_1.y][personagem_1.x]==10){
        mapa[personagem_1.y][personagem_1.x]=0;
        cont1=50;
	}
	//diminui a velocidade do player 1
	if(mapa[personagem_1.y][personagem_1.x]==12){
        mapa[personagem_1.y][personagem_1.x]=0;
        cont1=1500;
    }
    if(mapa[personagem_1.y][personagem_1.x]==7){
        mapa[personagem_1.y][personagem_1.x]=0;
        personagem_1.vida=personagem_1.vida -1;
        personagem_1.x=1;
        personagem_1.y=1;
    }
    if(mapa[personagem_2.y][personagem_2.x]==7){
        mapa[personagem_2.y][personagem_2.x]=0;
        personagem_2.vida=personagem_2.vida -1;
        personagem_2.x=11;
        personagem_2.y=11;
    }
    if(mapa[personagem_2.y][personagem_2.x]==10){
        mapa[personagem_2.y][personagem_2.x]=0;
        cont2=50;
	}
	if(personagem_2.vida<3 && mapa[personagem_2.y][personagem_2.x]==13){
        mapa[personagem_2.y][personagem_2.x]=0;
        personagem_2.vida=personagem_2.vida+1;
	}
	if(personagem_1.vida<3 && mapa[personagem_1.y][personagem_1.x]==13){
        mapa[personagem_1.y][personagem_1.x]=0;
        personagem_1.vida=personagem_1.vida+1;
	}
    //diminui a velocidade do player 2
    if(mapa[personagem_2.y][personagem_2.x]==12){
        mapa[personagem_2.y][personagem_2.x]=0;
        cont2=1500;
    }

	//movimentacao personagem_2
	if(key[KEY_D] && mapa[personagem_2.y][personagem_2.x + 1] != 1 && mapa[personagem_2.y][personagem_2.x + 1] != 8 && mapa[personagem_2.y][personagem_2.x + 1] != 2 && mapa[personagem_2.y][personagem_2.x + 1] != 6 && movimentaPersonagem2){
        personagem_2.x++;
        if(personagem_2.x==personagem_1.x && personagem_2.y==personagem_1.y){ // faz o teste para o player 2 não entrar no player 1
                personagem_2.x--;
        }
	}else
	if(key[KEY_A] && mapa[personagem_2.y][personagem_2.x - 1] != 1 && mapa[personagem_2.y][personagem_2.x - 1] != 8 && mapa[personagem_2.y][personagem_2.x - 1] != 2 && mapa[personagem_2.y][personagem_2.x - 1] != 6 && movimentaPersonagem2){
		personagem_2.x--;
        if(personagem_2.x==personagem_1.x && personagem_2.y==personagem_1.y){ // faz o teste para o player 2 não entrar no player 1
                personagem_2.x++;
        }
	}else
	if(key[KEY_W] && mapa[personagem_2.y - 1][personagem_2.x] != 1 && mapa[personagem_2.y - 1][personagem_2.x] != 8 && mapa[personagem_2.y - 1][personagem_2.x] != 2 && mapa[personagem_2.y - 1][personagem_2.x] != 6 && movimentaPersonagem2){
        personagem_2.y--;
        if(personagem_2.x==personagem_1.x && personagem_2.y==personagem_1.y){ // faz o teste para o player 2 não entrar no player 1
                personagem_2.y++;
        }
	}else
	if(key[KEY_S] && mapa[personagem_2.y + 1][personagem_2.x] != 1 && mapa[personagem_2.y + 1][personagem_2.x] != 8 && mapa[personagem_2.y + 1][personagem_2.x] != 2 && mapa[personagem_2.y + 1][personagem_2.x] != 6 && movimentaPersonagem2){
		personagem_2.y++;
		if(personagem_2.x==personagem_1.x && personagem_2.y==personagem_1.y){ // faz o teste para o player 2 não entrar no player 1
                personagem_2.y--;
        }
	}
	//solta bomba player 2
	if(key[KEY_SPACE] && mapa[personagem_2.y][personagem_2.x] != 2 && mapa[personagem_2.y][personagem_2.x] != 6 && personagem_2.bomba > 0 && mapa[personagem_2.y][personagem_2.x]!=10 && mapa[personagem_2.y][personagem_2.x]!=12){
		mapa[personagem_2.y][personagem_2.x] = 6;
		timeBomba[personagem_2.y][personagem_2.x]=6;
		personagem_2.bomba--;
	}
	//função que ganha uma bomba player 2
	if(mapa[personagem_2.y][personagem_2.x]==4){
		mapa[personagem_2.y][personagem_2.x]=0;
		personagem_2.bomba++;
	}
	//função para aumentar o raio da bomba player 2
	if(mapa[personagem_2.y][personagem_2.x]==5){
		mapa[personagem_2.y][personagem_2.x]=0;
		personagem_2.raio++;
	}
	//clear_to_color(fundo,makecol(28,183,32)); //limpa fundo com a cor branca

       for(int i=0;i<13;i++)    //varre o mapa
          for(int j=0;j<13;j++)
          {
             //imprime o mapa-se encontrar 1, imprime bloco fixo
             //imprime a bloco indestrutivel
             if(mapa[i][j]==1)
             {
                 masked_blit(blocoF,fundo,0,0, j*50, i*45,50,45);
             }
            //imprime o bloco destrutivel com bomba
			 if(mapa[i][j] == 8)
			 {
             	masked_blit(bloco,fundo,0,0, j*50, i*45, 50, 45);
             }
             if(mapa[i][j]==10){
                masked_blit(podervelocidade,fundo,0,0,j*50,i*45,50,45);
             }
             if(mapa[i][j] ==11){
                masked_blit(portal,fundo,0,0,j*50,i*45,50,45);
             }
             if(mapa[i][j]==12){
                masked_blit(podergelo,fundo,0,0,j*50,i*45,50,45);
             }
             if(mapa[i][j]==7){
                masked_blit(mina,fundo,0,0,j*50,i*45,50,45);
             }
             if(mapa[i][j]==20){
                masked_blit(caveira,fundo,0,0,j*50,i*45,50,45);
             }
             if(mapa[i][j]==13){
                masked_blit(vida,fundo,0,0,j*50,i*45,50,45);
             }
             //se no mapa[i][j] ==2 imprime a bomba personagem 1
	         if(mapa[i][j] == 2)
			 {
	            masked_blit(bomba, fundo, 0, 0, j*50+5, i*45+5, 40, 35);
				if(executaBomba)timeBomba[i][j]--;//diminui um na matriz para chegar em 0
				if(timeBomba[i][j]==0) { //quando chegar ao valor 0 a posicao recebe o valor de fogo
				   mapa[i][j]=3;
				   timeFogo[i][j] = 2;
				   explode(i,j,personagem_1.raio); //chama a funcao explode para acionar a bomba e executar a explosao
				   personagem_1.bomba++;
			    }
	         }
             //se no mapa[i][j] ==6 imprime a bomba personagem 2
	         if(mapa[i][j] == 6)
			 {
	            masked_blit(bomba, fundo, 0, 0, j*50+5, i*45+5, 40, 35);
				if(executaBomba)timeBomba[i][j]--;//diminui um na matriz para chegar em 0
				if(timeBomba[i][j]==0) { //quando chegar ao valor 0 a posicao recebe o valor de fogo
				   mapa[i][j]=3;
				   timeFogo[i][j] = 2;
				   explode(i,j,personagem_2.raio); //chama a funcao explode para acionar a bomba e executar a explosao
				   personagem_2.bomba++;
				}
			}
			//se no mapa[i][j]==10 imprime a bota que aumenta a velocidade do personagem
			if(mapa[i][j] == 10){
                masked_blit(podervelocidade,fundo,0,0,j*50,i*45,50,45);
			}
            //se no mapa[i][j] ==3 imprime o fogo da bomba
	         if(mapa[i][j] == 3)
			 {
				 masked_blit(fogo, fundo, 0, 0, j*50, i*45, 50, 45);
				if(executaFogo)timeFogo[i][j]--;// tempo da matriz de fogo quando chegar em 0 aciona para o fogo sumir
			 	if(timeFogo[i][j] < 1){
			 		mapa[i][j] = 0;
			 	}
	         }
	         //se no mapa[i][j] ==4 imprime o poder para aumentar o poder da bomba
	         if(mapa[i][j] == 4)
			 {
			 	masked_blit(poderbomba, fundo, 0, 0, j*50, i*45, 50, 45);
			 }
			 if(mapa[i][j]==12){
                masked_blit(podergelo,fundo,0,0,j*50,i*45,50,45);
			 }
			 //se no mapa[i][j] ==5 imprime o poder para aumentar o raio da bomba
			 if(mapa[i][j] == 5)
			 {
			 	masked_blit(poderfogo, fundo, 0, 0, j*50, i*45, 50, 45);
			 }
			 //imprime o personagem_1 na tilemap
			 if(personagem_1.x == j && personagem_1.y == i)
			 {
			 	masked_blit(personagem_1.sprite, fundo,0, 0, j*50, i*45, 50, 45);
			 }
			 //imprime o personagem 2 na tilemap
			 if(personagem_2.x == j && personagem_2.y == i)
             {
			 	masked_blit(personagem_2.sprite, fundo,0, 0, j*50, i*45, 50, 45);
			 }

          }

    //escreve na tela nossos nomes
    textprintf_ex(fundo,font,650,10,makecol(0,0,255), -1, "Vida Jogador 1");
    textprintf_ex(fundo,font,650,50,makecol(0,0,255), -1, "Vida Jogador 2");
    textout_ex(fundo,font, "Mateus Tormes",670,500,makecol(0,0,255),-1);
    textout_ex(fundo,font, "William Sarti",670,510,makecol(0,0,255),-1);

            BITMAP *vida=load_bitmap("sprites/vida.bmp",NULL);
            if(personagem_2.vida==3){
            masked_blit(vida,fundo,0,0,650,20,25,25);
            masked_blit(vida,fundo,0,0,680,20,25,25);
            masked_blit(vida,fundo,0,0,710,20,25,25);
            }
            if(personagem_2.vida==2){
            masked_blit(vida,fundo,0,0,650,20,25,25);
            masked_blit(vida,fundo,0,0,680,20,25,25);
            }
            if(personagem_2.vida==1){
            masked_blit(vida,fundo,0,0,650,20,25,25);
            }
            //desenha os corações de vida
            if(personagem_1.vida==3){
            masked_blit(vida,fundo,0,0,650,60,25,25);
            masked_blit(vida,fundo,0,0,680,60,25,25);
            masked_blit(vida,fundo,0,0,710,60,25,25);
            }
            if(personagem_1.vida==2){
            masked_blit(vida,fundo,0,0,650,60,25,25);
            masked_blit(vida,fundo,0,0,680,60,25,25);
            }
            if(personagem_1.vida==1){
            masked_blit(vida,fundo,0,0,650,60,25,25);
            }

    if(personagem_1.vida <=0){
        BITMAP *j1=load_bitmap("sprites/jogador1venceu.bmp",NULL);
        masked_blit(j1, fundo, 0, 0, 0, 0, 800, 600);
       // blit(fundo,screen,0,0,0,0,800,600);
    }
    if(personagem_2.vida <=0){
        BITMAP *j2=load_bitmap("sprites/jogador2venceu.bmp",NULL);
        masked_blit(j2, fundo, 0, 0, 0, 0, 800, 600);

      //  blit(fundo,screen,0,0,0,0,800,600);
    }

  	blit(fundo,screen,0,0,0,0,800,600); //imprime tudo na screen

} // fim do while do KEY_ESC;

stop_midi();//para o som da fase
    destroy_bitmap(fundo);
    destroy_bitmap(blocoF);
    destroy_bitmap(bloco);
    destroy_bitmap(bomba);
    destroy_bitmap(fogo);
    destroy_bitmap(podergelo);
    destroy_bitmap(podervelocidade);
    destroy_bitmap(portal);
    destroy_bitmap(caveira);
    destroy_bitmap(personagem_1.sprite);
    destroy_bitmap(personagem_2.sprite);
return 0;
}END_OF_MAIN()

void parar_tela(){
while(key[KEY_P]);
    clear_keybuf();
    xbox_input();
	stop_midi();//para a midia do menu
	BITMAP *fundo =create_bitmap(800,600);
	BITMAP *menu=load_bitmap("sprites/pause.bmp", NULL);
	masked_blit(menu, fundo, 0, 0, 0, 0, 800, 600);
    blit(fundo,screen,0,0,0,0,800,600);
while(!keypressed());


//SOM
    MIDI *MusicaInicio;
    MusicaInicio=load_midi("sons/level.mid");
    play_midi(MusicaInicio,TRUE);

}

int menu(){
	int i = 300;
	int escolha;

	//instala o som e recebe o som do menu
    MIDI *MusicaInicio;
    MusicaInicio=load_midi("sons/menu.mid");
    play_midi(MusicaInicio,TRUE);
    install_joystick(JOY_TYPE_AUTODETECT);

	//load nas imagens do menu
    BITMAP *menu=load_bitmap("sprites/menu.bmp", NULL);
	BITMAP *select=load_bitmap("sprites/select.bmp", NULL);
	BITMAP *fundo =create_bitmap(800,600);

	do{
        xbox_input();
		if(key[KEY_DOWN] || xbox[LSTICK_DOWN])
		{//move em cima da posicao sair
			i = 400;
			escolha = 2;
		}
		if(key[KEY_UP] || xbox[LSTICK_UP])
		{//move em cima da posicao jogar
			i = 300;
			escolha = 1;
		}
        if(xbox[BUTTON_X]){
            stop_midi();
            return escolha;
        }


		//imprime o menu
		masked_blit(menu, fundo, 0, 0, 0, 0, 800, 600);
		masked_blit(select, fundo, 0, 0, 330, i, 250, 100);
		blit(fundo,screen,0,0,0,0,800,600); //imprime tudo na screen
	}
while(!key[KEY_ENTER]);
	stop_midi();//para a midia do menu
return escolha;
}

void init() {
	int depth, res;
	allegro_init();
	set_window_title("BOMBER MAN");
	depth = desktop_color_depth();
    set_close_button_callback(fecha_program);
    install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL);
    install_joystick(JOY_TYPE_AUTODETECT);
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
    }

	install_timer();
	install_keyboard();
	install_mouse();
	/* add other initializations here */
}

void deinit() {
	clear_keybuf();
	/* add other deinitializations here */
}
