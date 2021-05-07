/*
Programação - Trabalho intermédio

Bubble shooter nível 3

Alunos:
Iara Figueiras - ist189668
Leonardo Duarte - ist189691

Comando para compilar e correr em SO Linux na norma C99, para a qual este programa foi desenvolvido
gcc bubbles.c -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -std=c99 -L/usr/lib/i386-linux-gnu -lm -lSDL2 -lSDL2_gfx -o runbubles && ./runbubles

*/

//---------------------------------------------------------------bibliotecas--------------------------------------------------------------
//bibliotecas gráficas
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

//bibiliotecas básicas
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//biblioteca para criação de seed "aleatória" para gerar numeros aleatórios
#include <time.h>

//-------------------------------------------------------------------defines---------------------------------------------------------------

//dimensoes janela
#define DIM_JANELA_X configDados[0] //número de bolhas na horizontal
#define DIM_JANELA_Y configDados[1] //número de bolhas na vertical
#define ALTURA_BARRA_SUPERIOR 1.5*DIM_BOLHA //altura da barra com os butoes em pixéis
#define ALTURA_BARRA_INFERIOR 1.75 //altura da barra inferior em termos de bolhas

//dimensoes bolhas
#define DIM_BOLHA 2*configDados[2] //diametro da bolha em pixéis
#define DL configDados[3] //distancia entre as bolhas em percentagem do diametro da bolha

//constates de jogabilidade
#define LINHAS_INICIAIS_PREENCHIDAS configDados[4] 
#define N_JOGADAS_NOVA_LINHA configDados[5]

//Dimensoes da matriz que contém as cores e posiçoes das bolhas
#define MAX_DIM_MATRIZ 52

//Dimensoes da matriz que extrai os dados do ficheiro de resultados
#define MAX_DIM_MATRIZ_RESULTADOS 500

//variaveis jogabilidade
#define NUMERO_JOGADAS configDados[6] //numero de bolhas lancadas pelo utilizador
#define PONTOS configDados[7] //pontuacao do jogador, 1 ponto por bokha rebentada

//Dimensoes da janela
#define WINDOW_SIZE_X configDados[8] //largura da janela
#define WINDOW_SIZE_Y configDados[9] //altura da janela

//limites janela
#define LARGURA_MINIMA1 (30*12+30*(0.1)*(12+1))
#define LARGURA_MINIMA  (30*5+30*(0.1)*(5+1))
#define LARGURA_MAXIMA (20*60+20*(0.1)*(60+1))
#define ALTURA_MAXIMA (1.5*20 + 20*30 + (0.1)*20*(30+1) + 1.75*20)

//defenicoes funcao 'desenhaBolhaASerLancada' 
#define MUDA 0
#define NAO_MUDA 1
#define DUAS_BOLHAS 1
#define SO_UMA 0

//defines funcao 'CoordenadasMatriztransformaParaRefJanela'
#define EIXO_X 0
#define EIXO_Y 1

//defines verifica se bolhas explodem
#define CIMA_0 coordenadasBolhasVizinhanca[0][0]
#define CIMA_1 coordenadasBolhasVizinhanca[0][1]
#define CIMA_PESQUISA coordenadasBolhasVizinhanca[0][2]

#define DIREITA_0 coordenadasBolhasVizinhanca[1][0]
#define DIREITA_1 coordenadasBolhasVizinhanca[1][1]
#define DIREITA_PESQUISA coordenadasBolhasVizinhanca[1][2]

#define ESQUERDA_0 coordenadasBolhasVizinhanca[2][0]
#define ESQUERDA_1 coordenadasBolhasVizinhanca[2][1]
#define ESQUERDA_PESQUISA coordenadasBolhasVizinhanca[2][2]

#define BAIXO_0 coordenadasBolhasVizinhanca[3][0]
#define BAIXO_1 coordenadasBolhasVizinhanca[3][1]
#define BAIXO_PESQUISA coordenadasBolhasVizinhanca[3][2]

#define DIAGONAL1_0 coordenadasBolhasVizinhanca[4][0]
#define DIAGONAL1_1 coordenadasBolhasVizinhanca[4][1]
#define DIAGONAL1_PESQUISA coordenadasBolhasVizinhanca[4][2]

#define DIAGONAL2_0 coordenadasBolhasVizinhanca[5][0]
#define DIAGONAL2_1 coordenadasBolhasVizinhanca[5][1]
#define DIAGONAL2_PESQUISA coordenadasBolhasVizinhanca[5][2]

#define DIAGONAL3_0 coordenadasBolhasVizinhanca[6][0]
#define DIAGONAL3_1 coordenadasBolhasVizinhanca[6][1]
#define DIAGONAL3_PESQUISA coordenadasBolhasVizinhanca[6][2]

#define DIAGONAL4_0 coordenadasBolhasVizinhanca[7][0]
#define DIAGONAL4_1 coordenadasBolhasVizinhanca[7][1]
#define DIAGONAL4_PESQUISA coordenadasBolhasVizinhanca[7][2]

//--------------------------------------------------------------------variaveis globais-------------------------------------------------------
SDL_Window* g_pWindow = 0; //apontador da janela
SDL_Renderer* g_pRenderer = 0; //apontador do renderer

//-------------------------------------------------------------------prototipos das funcoes---------------------------------------------------

int lerConfigDados(float configDados[]);
int iniciarSDL2(float configDados[]);
void bolhasIniciaisAleatorias(int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ], float configDados[]);
void atualizaBolhas(int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ], float configDados[]);
void atualizaJanela();
void corBolha(int codigoCor, int cor[]);
void atualiza_seta(float configDados[]);
void desenhaBolhaASerLancada(float configDados[], int corAtual[], int corSeguinte[], int muda, int asduas);
void transformaParaRefBolha(float p[], float configDados[]);
void transformaParaRefJanela(float  p[], float configDados[]);
float getTheta(float configDados[]);
int movimentoBolha(float configDados[], int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ], int corAtual[], int corSeguinte[]);
void atualizaPosBolhaLancada(float configDados[], float pos_bolha[], float theta);
void atualizaBolhaLancada(float configDados[], float pos_bolha[], int cor[]);
int detetaLimites(float configDados[], float pos_bolha[], int corAtual[], int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ]);
void poeBolhaNaMatriz(float configDados[], int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ], int corAtual[], int bolhaEspacoMaisProximo[]);
int detetaEspacoVazioMaisProximo(float configDados[], float pos_bolha[], int bolhaChoque[], int bolhaEspacoMaisProximo[], int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ]);
int detetaColisao(float configDados[], float pos_bolha[], int bolhaChoque[], int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ]);
float CoordenadasMatriztransformaParaRefJanela(float configDados[], int coordenada, int eixo);
int verificaSeBolhasExplodem(float configDados[], int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ], int corAtual[], int bolhaEspacoMaisProximo[]);
int valorMinimo(float d1, float d2, float d3);
int fimDoJogo(float configDados[]);
void bubleSortExtracaoFichResultados(int linha, int pontos[MAX_DIM_MATRIZ_RESULTADOS], char nome[MAX_DIM_MATRIZ_RESULTADOS][23]);
int guardaNovamenteNoficheiroResultados(int linha, int pontosFichResultados[MAX_DIM_MATRIZ_RESULTADOS], char nomeDoJogadorFichResultados[MAX_DIM_MATRIZ_RESULTADOS][23]);
void coodenadasMesmaCor(int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ], int codigoCorBolhaAtual, int coordenadasDaBolhaOrigemVizinhanca[], int coordenadasDeBolhasASerEliminadas[30][2], int* coordenadaAintroduzir);
void procuraCoordenadasBolhas1(int coordenadasDaBolhaOrigemVizinhanca[], int coordenadasDeBolhasASerEliminadas[50][2], int* coordenadaAintroduzir, int coordenadasBolhasVizinhanca[8][3]);
void procuraCoordenadasBolhas2(int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ], int coordenadasDeBolhasASerEliminadas[50][2], int* coordenadaAintroduzir, int coordenadasBolhasVizinhanca[8][3], int codigoCorBolhaAtual);
int RGBparaCodigoCores(int cor[]);
int desceLinha(float configDados[], int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ]);
int verificaBotoes(float configDados[]);
int verificaVitoria(float configDados[], int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ]);
void loopDoJogo(float configDados[], int* vitoria, int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ], int corAtual[], int corSeguinte[]);
void transformacaoVetorRotacao(float A[], float B[], float C[], float theta);

//----------------------------------------------------------------------MAIN----------------------------------------------------------------------

int main()
{
	float configDados[9]; // vetor que guarda os dados do ficheiro de confuguracoes
	switch (lerConfigDados(configDados)) //incoca a função lerConfigDados para extrair os dados do ficheiro de configurações
	{
	case 0:  printf("Erro: falha na leitura do ficheiro de configurações\n"); return 0; break;
	case -1: printf("Erro: um dos dados de 'config.txt' tem formato inválido\n"); return 0; break;
	case -2: printf("Erro: dados de 'config.txt' não são válidos\n"); return 0; break;
	}
	switch (iniciarSDL2(configDados)) //inicia a biblioteca gráfica e abre a janela do jogo
	{
	case 0: printf("Erro: falha na inicialização da biblioteca gráfica.\n"); return 0; break;
	}
	//--------------------------------------------------------------variaveis------------------------------------
	//variaveis de controlo do jogo e gestao de eventos

	int vitoria = 0;

	int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ]; //matriz que guarda em cada posição guarda a cor da bolha correspondente
	int corAtual[3]; //posiçao 0 guarda o valor r , 1 guada o valor de g e 2 guada o valor de b
	int corSeguinte[3];

	srand(time(NULL)); //seed "aleatória" para a gereação de numeros aleatorios

	//loop do jogo
	loopDoJogo(configDados, &vitoria, disposicaoBolhas, corAtual, corSeguinte);

	if (vitoria != 1) SDL_ShowSimpleMessageBox(0, "Terminado", "Ate a proxima ;)", g_pWindow);
	else SDL_ShowSimpleMessageBox(0, "Vitoria", "Ate a proxima ;)", g_pWindow);
	//destruir o renderer a janela e fecha a execução da biblioteca gráfca
	SDL_DestroyWindow(g_pWindow);
	SDL_HideWindow(g_pWindow);
	SDL_DestroyRenderer(g_pRenderer);
	SDL_Quit();
	if (vitoria != -1)
	{
		if (fimDoJogo(configDados) != 1)
		{
			printf("Erro: ao guardar a pontuação no ficheiro de configurações.\n");
		}
	}
	return 0;
}

//------------------------------------------------------------------------FUNÇÕES-------------------------------------------------------------------//

/*
Função que lê o ficheiro de configurações (config.txt) e guarda os dados no vetor configDados.
É também responsável pela verificação de que os dados introduzidos são válidos.

Esta função retorna: 1 - no caso de sucesso na extração dos dados do ficheiros e na sua validação.
					 0 - no caso de impossibilidade de abertura do ficheiro
					 -1 - impossibilidade de leitura de um dado
					 -2 - no caso de invalidade dos dados intriduzidos no ficheiro config.txt
*/
int lerConfigDados(float configDados[])
{

	FILE* configP;
	configP = fopen("config.txt", "r"); //abre o ficheiro no modo de leitura
	if (configP == NULL) return 0; //se não for possivel abrir o ficheiro

	int linhaALer = 0; //variavel que permitirá introduir os valoes lidos nas diferentes posições do vetor configDados
	char str[80] = ""; //string que receberá os dados da função fgets

	do
	{
		fgets(str, 80, configP); //le a linha e guarda-a na string
		if (str[0] == '/' || str[0] == '\n') continue; //se a string comecar por / ou não tiver conteúdo ignora-a
		else
		{
			if (linhaALer == 0) //se a linha a ler for a primeira o programa vai tentar extrair dois valores
			{
				if (sscanf(str, "%f %f", &configDados[0], &configDados[1]) != 2) return -1;
				linhaALer = 2;
			}
			else //caso contrário tenta extrair apenas um valor
			{
				if (sscanf(str, "%f", &configDados[linhaALer]) != 1) return -1;
				linhaALer++;
			}
		}
	} while (feof(configP) == 0); //equanto não atinge o final do ficheiro
	fclose(configP); //fecha o ficheiro de configurações

	WINDOW_SIZE_X = round(DIM_BOLHA * DIM_JANELA_X + DIM_BOLHA * (DL) * (DIM_JANELA_X + 1)); //calcula as dimensoes x e y da janela 
	WINDOW_SIZE_Y = round(ALTURA_BARRA_SUPERIOR + DIM_BOLHA * DIM_JANELA_Y + (DL)*DIM_BOLHA * (DIM_JANELA_Y + 1) + ALTURA_BARRA_INFERIOR * DIM_BOLHA);

	//----------------------------------------------------limitar valores----------------------------------------------
	if (DIM_JANELA_X >= 50 || DIM_JANELA_Y >= 50) {
		printf("Erro: Número excessivo de bolhas\n"); return -2;
	}
	else if (DIM_JANELA_X <= 0 || DIM_JANELA_Y <= 0 || DIM_BOLHA <= 0 || DL < 0 || LINHAS_INICIAIS_PREENCHIDAS<0 || LINHAS_INICIAIS_PREENCHIDAS > DIM_JANELA_Y || N_JOGADAS_NOVA_LINHA <= 0) {
		printf("Erro: um ou mais dados adotam valores inválidos (nulos ou negativos).\n"); return -2;
	}
	else if (DIM_BOLHA > 50) {
		printf("Erro: o raio da bolha não pode ser superior a 50px.\n"); return -2;
	}
	else if (DL >= 1) {
		printf("Erro: DL tem de adotar um valor entre 0 e 1.\n"); return -2;
	}
	else if (N_JOGADAS_NOVA_LINHA > 100000) {
		printf("Erro: valor inválido para o número de jogadas até à geração de uma nova linha.\n"); return -2;
	}
	else if (WINDOW_SIZE_X < LARGURA_MINIMA) {
		printf("Erro: Janela demasiado pequena, aumente o valor de alguns dos parametros\n"); return -2;
	}
	else if (WINDOW_SIZE_Y > ALTURA_MAXIMA) {
		printf("Erro: Janela demasiado grande, diminua o valor de alguns dos parametros\n"); return -2;
	}
	else if (WINDOW_SIZE_X > LARGURA_MAXIMA) {
		printf("Erro: Janela demasiado grande, diminua o valor de alguns dos parametros\n"); return -2;
	}
	return 1;
}

/*
Função principal do programa responsável pelo ciclo do jogo
Recebe o vetor configDados para aceder as definições de jogabilidade e da janela, recebe tambem a matriz que guarda a informaçãio referente às
ás posições e respetivas cores das bolhas para que no deccorer desta função se possa monitorizar as alterações e consequencias nestes valores
das ações do jogador.
*/
void loopDoJogo(float configDados[], int* vitoria, int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ], int corAtual[], int corSeguinte[])
{
	int jogoContinua = 1; //variavel que indica se o jogo terminou quer por indicação do utilizador ou por o jogo ter finalizado
	int jogoRepete = 0; //variavel que indica se o jogo se repetirá (ao carregar no botao novo jogo)
	SDL_Event event; //evento para detetar cliques

	do
	{
		SDL_SetRenderDrawColor(g_pRenderer, 255, 255, 255, 255);
		SDL_RenderClear(g_pRenderer); //limpar a janela com branco
		//iniciar variaveis jogabilidade
		PONTOS = 0;
		NUMERO_JOGADAS = 0;
		*vitoria = 0;
		jogoRepete = 0;
		jogoContinua = 1;
		//-----------------------------------------------------------condicoes iniciais------------------------------------
		bolhasIniciaisAleatorias(disposicaoBolhas, configDados); //Gera cores aleatorias e guarda-as nas posicoes permitidas 
		corBolha(1 + rand() % 9, corSeguinte); //gera uma cor aleatoria para a primeira bolha a ser lancada
		atualizaBolhas(disposicaoBolhas, configDados); //desenha as bolhas inicias na janela
		atualiza_seta(configDados); //desenha a seta no ecra
		desenhaBolhaASerLancada(configDados, corAtual, corSeguinte, MUDA, DUAS_BOLHAS); //desenha a bolha que sera lancada
		atualizaJanela(); //atualiza a janela para que as alterações sejam imprimidas
		//-----------------------------------------------------------loop do jogo---------------------------------------------
		while (jogoContinua == 1 && *vitoria != 1) //enquanto é para o jogo continuar a correr
		{
			SDL_WaitEvent(&event);
			switch (event.type)
			{
			case SDL_QUIT: jogoContinua = 0; jogoRepete = 0; *vitoria = -1; break; //caregar no X da janela
			case SDL_MOUSEMOTION: //quando o rato se mexer
				SDL_SetRenderDrawColor(g_pRenderer, 255, 255, 255, 255);
				SDL_RenderClear(g_pRenderer); //apagar a janela e pinta-la de branco
				atualizaBolhas(disposicaoBolhas, configDados); //desenhar as bolhas nas respetivas posições 
				atualiza_seta(configDados); //desenhar a seta na nova posição
				desenhaBolhaASerLancada(configDados, corAtual, corSeguinte, NAO_MUDA, DUAS_BOLHAS); //desenhar a bolha que será lancada
				atualizaJanela(); //imprimir as alerações na janela
				break;
			case SDL_MOUSEBUTTONUP: //quando um botao do rato deixar de ser premido
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT: //no caso de ser o botao esquerdo para mandar uma bolha ou ver se carregou num botao
					switch (verificaBotoes(configDados))
					{
					case 1: jogoContinua = 0; jogoRepete = 0; *vitoria = -1; break; //botao fim do jogo
					case 2: jogoRepete = 1; jogoContinua = 0; break; //botao novo jogo
					case 0: //nao carregou em nenhum botao portanto lanca bolha
						jogoRepete = 0;
						jogoContinua = movimentoBolha(configDados, disposicaoBolhas, corAtual, corSeguinte);
						*vitoria = verificaVitoria(configDados, disposicaoBolhas);
						break;
					}
					break;
				case SDL_BUTTON_RIGHT: //no caso de ser o butao direito é necessário verificar em que butao o utilizador carrregou se é que carregou
					switch (verificaBotoes(configDados))
					{
					case 1: jogoContinua = 0; jogoRepete = 0; *vitoria = -1; break; //botao fim do jogo
					case 2: jogoRepete = 1; jogoContinua = 0; break; //botao novo jogo
					}
					break;
				}
			}
		}
	} while (jogoRepete == 1); //se carregar em novo jogo 'jogoRepete' vai tomar o valor 1 e o jogo recomeca com as mesmas opcoes de inicialização
}


/*Função que inicia a bibioteca gráfica e que abre a janela.
Recebe os dados do ficheiro de configurações para calcular o tamanho da janela.

Esta função retorna: 0 no caso de erro de iniciação da biblioteca gráfica ou falha na abertura na janela.
					 1 no caso de sucesso.
*/
int iniciarSDL2(float configDados[])
{

	//inciar SDL2
	if (SDL_Init(SDL_INIT_EVERYTHING) >= 0) {

		//criar a janela
		g_pWindow = SDL_CreateWindow("Bubbles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE_X, WINDOW_SIZE_Y, SDL_WINDOW_SHOWN);

		//criar o renderer se a janela foi aberta com sucesso
		if (g_pWindow != 0)
		{
			g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, 0);
		}
		else return 0;
	}
	else return 0;

	SDL_SetRenderDrawColor(g_pRenderer, 255, 255, 255, 255); //pintar o background com branco
	SDL_RenderClear(g_pRenderer); //limpar a janela com branco

	return 1;
}

/*
Função que gera a disposição inicial das bolhas na janela.
Recebe a matriz que gurda para cada posição o codigo da cor correspondente e através da função rand() gera cores aleatórias
que preenche na matriz tendo em consideração as condições do ficheiro de configurações.
*/
void bolhasIniciaisAleatorias(int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ], float configDados[])
{
	for (int i = 0; i <= DIM_JANELA_Y; ++i) //ciclo que percorre as linhas
	{
		for (int j = 0; j < DIM_JANELA_X; ++j) //ciclo que perocrre as colunas de uma dada linha i
		{
			if (i < LINHAS_INICIAIS_PREENCHIDAS)
			{
				disposicaoBolhas[i][j] = 1 + rand() % 9; //da um valor aletório a cada espaco de 1-9 no numero de linhas iniciais preenchidas especificadas no ficheiro de configurações
			}
			else
			{
				disposicaoBolhas[i][j] = 0; //nas os restantes espaçoes assumem o valor de 0, o que representa a inexistencai de uma  bolha
			}
		}
	}
}

/*
Função que desenha os botoes e as bolhas em jogo
Recebe a disposiçao atual das bolhas e a pontuação do jogador para a partir desses dados desenhar na janela
*/
void atualizaBolhas(int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ], float configDados[])
{
	SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 205, 255); //selecionar a cor azul para desenhar linhas
	char strPontos[20] = ""; //string para imprimir no ecra a pontuação
	SDL_RenderDrawLine(g_pRenderer, 0, ALTURA_BARRA_SUPERIOR, WINDOW_SIZE_X, ALTURA_BARRA_SUPERIOR); //desenhar a linha superiror
	//Botao novo jogo
	SDL_RenderDrawLine(g_pRenderer, 0.02 * WINDOW_SIZE_X, 0.25 * DIM_BOLHA, 0.20 * WINDOW_SIZE_X, 0.25 * DIM_BOLHA);
	SDL_RenderDrawLine(g_pRenderer, 0.02 * WINDOW_SIZE_X, 0.25 * DIM_BOLHA, 0.02 * WINDOW_SIZE_X, 1.25 * DIM_BOLHA);
	SDL_RenderDrawLine(g_pRenderer, 0.02 * WINDOW_SIZE_X, 1.25 * DIM_BOLHA, 0.20 * WINDOW_SIZE_X, 1.25 * DIM_BOLHA);
	SDL_RenderDrawLine(g_pRenderer, 0.20 * WINDOW_SIZE_X, 0.25 * DIM_BOLHA, 0.20 * WINDOW_SIZE_X, 1.25 * DIM_BOLHA);
	if (WINDOW_SIZE_X > LARGURA_MINIMA1) { //escreve novo jogo numa linha se a largura da janela for superior a um determinado valor critico
		stringRGBA(g_pRenderer, 0.11 * WINDOW_SIZE_X - 37, 0.75 * DIM_BOLHA - 4, "Novo Jogo", 0, 0, 0, 255);
	}
	else {//se a largura for inferior a um valor critico imprime as duas palavras na vertical
		stringRGBA(g_pRenderer, 0.20 * WINDOW_SIZE_X - 30, 0.50 * DIM_BOLHA - 4, "Novo", 0, 0, 0, 255);
		stringRGBA(g_pRenderer, 0.20 * WINDOW_SIZE_X - 30, 0.90 * DIM_BOLHA - 4, "Jogo", 0, 0, 0, 255);
	}

	//desenha o botao fim
	SDL_RenderDrawLine(g_pRenderer, 0.25 * WINDOW_SIZE_X, 0.25 * DIM_BOLHA, 0.37 * WINDOW_SIZE_X, 0.25 * DIM_BOLHA);
	SDL_RenderDrawLine(g_pRenderer, 0.25 * WINDOW_SIZE_X, 0.25 * DIM_BOLHA, 0.25 * WINDOW_SIZE_X, 1.25 * DIM_BOLHA);
	SDL_RenderDrawLine(g_pRenderer, 0.25 * WINDOW_SIZE_X, 1.25 * DIM_BOLHA, 0.37 * WINDOW_SIZE_X, 1.25 * DIM_BOLHA);
	SDL_RenderDrawLine(g_pRenderer, 0.37 * WINDOW_SIZE_X, 0.25 * DIM_BOLHA, 0.37 * WINDOW_SIZE_X, 1.25 * DIM_BOLHA);
	stringRGBA(g_pRenderer, 0.31 * WINDOW_SIZE_X - 12, 0.75 * DIM_BOLHA - 4, "Fim", 0, 0, 0, 255);

	// desenha o quadro da pontucao e imprime os pontos do jogador
	SDL_RenderDrawLine(g_pRenderer, 0.60 * WINDOW_SIZE_X, 0.25 * DIM_BOLHA, 0.98 * WINDOW_SIZE_X, 0.25 * DIM_BOLHA);
	SDL_RenderDrawLine(g_pRenderer, 0.60 * WINDOW_SIZE_X, 0.25 * DIM_BOLHA, 0.60 * WINDOW_SIZE_X, 1.25 * DIM_BOLHA);
	SDL_RenderDrawLine(g_pRenderer, 0.60 * WINDOW_SIZE_X, 1.25 * DIM_BOLHA, 0.98 * WINDOW_SIZE_X, 1.25 * DIM_BOLHA);
	SDL_RenderDrawLine(g_pRenderer, 0.98 * WINDOW_SIZE_X, 0.25 * DIM_BOLHA, 0.98 * WINDOW_SIZE_X, 1.25 * DIM_BOLHA);
	stringRGBA(g_pRenderer, WINDOW_SIZE_X * 0.62, 0.75 * DIM_BOLHA - 4, "Pontos:", 0, 0, 0, 255);
	sprintf(strPontos, "%.0f", PONTOS); //imprime numa string os pontos do utilizador
	stringRGBA(g_pRenderer, WINDOW_SIZE_X * 0.62 + 56, 0.75 * DIM_BOLHA - 4, strPontos, 0, 0, 0, 255); //imprime os pontos

	for (int i = 0; i <= DIM_JANELA_Y; ++i)//percorre as linhas
	{
		for (int j = 0; j < DIM_JANELA_X; ++j)//percorre as colunas
		{
			if (disposicaoBolhas[i][j] != 0) //se existir uma bolha na posicao i,j desnha essa bolha no ecra
			{
				int cor[3]; //variavel que guarda a cor da bolha
				corBolha(disposicaoBolhas[i][j], cor);//a partir do codigo da cor que esta na matriz, da valores de rgbào vetor cor
				//desenha a bolha depois de transformadas as coordenas da matriz para o referencial(em pixeis) da janela
				filledCircleRGBA(g_pRenderer, CoordenadasMatriztransformaParaRefJanela(configDados, j, EIXO_X), CoordenadasMatriztransformaParaRefJanela(configDados, i, EIXO_Y), DIM_BOLHA / 2.0, cor[0], cor[1], cor[2], 255);
				circleRGBA(g_pRenderer, CoordenadasMatriztransformaParaRefJanela(configDados, j, EIXO_X), CoordenadasMatriztransformaParaRefJanela(configDados, i, EIXO_Y), (DIM_BOLHA / 2.0), 0, 0, 0, 255);

			}
		}
	}
}

/*
Função que recebe o uma cor sob a forma de um codiogo (1-9) e converte esse codigo para rgb que atribui ao vetor cor que recebe
É a função inversa a 'RGBparaCodigoCores'
*/
void corBolha(int codigoCor, int cor[])
{
	switch (codigoCor) //valor do codigo de cor
	{
	case 1: //vermelho
		cor[0] = 255;
		cor[1] = 0;
		cor[2] = 0;
		break;
	case 3://azul
		cor[0] = 0;
		cor[1] = 0;
		cor[2] = 255;
		break;
	case 2://roxo
		cor[0] = 128;
		cor[1] = 0;
		cor[2] = 128;
		break;
	case 4://cyan
		cor[0] = 0;
		cor[1] = 255;
		cor[2] = 255;
		break;
	case 5://verde
		cor[0] = 50;
		cor[1] = 205;
		cor[2] = 50;
		break;
	case 6://amarelo
		cor[0] = 255;
		cor[1] = 255;
		cor[2] = 0;
		break;
	case 7: //castanho
		cor[0] = 139;
		cor[1] = 69;
		cor[2] = 19;
		break;
	case 8: //preto
		cor[0] = 0;
		cor[1] = 0;
		cor[2] = 0;
		break;
	case 9: //branco
		cor[0] = 255;
		cor[1] = 255;
		cor[2] = 255;
		break;
	}
}

/*
Função que recebe uma cor no seu codigo rgb guardado nas posicoes 0,1 e 2 e retorna o código da cor correspondente
É a função ineversa a 'corBolha'
*/
int RGBparaCodigoCores(int corAtual[])
{
	if (corAtual[0] == 255 && corAtual[1] == 0 && corAtual[2] == 0) { //vermelho
		return 1;
	}
	else if (corAtual[0] == 128 && corAtual[1] == 0 && corAtual[2] == 128) { //roxo
		return 2;
	}
	else if (corAtual[0] == 0 && corAtual[1] == 0 && corAtual[2] == 255) { //azul
		return 3;
	}
	else if (corAtual[0] == 0 && corAtual[1] == 255 && corAtual[2] == 255) { //cyan
		return 4;
	}
	else if (corAtual[0] == 50 && corAtual[1] == 205 && corAtual[2] == 50) { //verde
		return 5;
	}
	else if (corAtual[0] == 255 && corAtual[1] == 255 && corAtual[2] == 0) { //amarelo
		return 6;
	}
	else if (corAtual[0] == 139 && corAtual[1] == 69 && corAtual[2] == 19) { //castanho
		return 7;
	}
	else if (corAtual[0] == 0 && corAtual[1] == 0 && corAtual[2] == 0) { //preto
		return 8;
	}
	else if (corAtual[0] == 255 && corAtual[1] == 255 && corAtual[2] == 255) { //branco
		return 9;
	}
	else if (corAtual[0] == 256 && corAtual[1] == 256 && corAtual[2] == 256) { //cor ficticia que representa a ausencia de uma bolha
		return 0;
	}

	return 0;
}

/*
Função que atualiza o renderer, utilizada para simplificara a linguagem e tornar o código mais compreensivel
*/
void atualizaJanela()
{

	SDL_RenderPresent(g_pRenderer);
}

/*
Função que de cada vez que é invocada atualiza a posição da seta no ecra, ou seja, desenha a seta no ecra com base na posição do rato
Desenha também a setinha na ponta do segmento de reta que indica a direção de lançamento
*/
void atualiza_seta(float configDados[])
{
	const int raio_seta = 4 * DIM_BOLHA; //a seta tem raio contante que depende do diametro da bolha
	int mouse[2];
	float mouse_pos[2];
	float seta_pos[2];
	SDL_GetMouseState(&mouse[0], &mouse[1]); // Get mouse position
	mouse_pos[0] = mouse[0]; //transformar a posição do rato para float
	mouse_pos[1] = mouse[1];

	transformaParaRefBolha(mouse_pos, configDados); //muda o referencia das cooordenas do rato da janela para o referencia da bolha
	//calcula o ponto onde termina a seta no referencia da bolha
	seta_pos[0] = raio_seta * (mouse_pos[0] / sqrt(mouse_pos[1] * mouse_pos[1] + mouse_pos[0] * mouse_pos[0]));
	seta_pos[1] = raio_seta * (mouse_pos[1] / sqrt(mouse_pos[1] * mouse_pos[1] + mouse_pos[0] * mouse_pos[0]));
	transformaParaRefJanela(seta_pos, configDados);//muda o referencia de volta para o referencial da janela 
	SDL_RenderDrawLine(g_pRenderer, seta_pos[0], seta_pos[1], WINDOW_SIZE_X / 2.0, WINDOW_SIZE_Y - DIM_BOLHA / 2); //desenha a seta

	//--------------------------------------------------desenhar setinha----------------------------------------

	//A setinha é definida por tres pontos num referencial onde o ponto B é o centro desse referencial  e os pontos A e C com ele uam seta que aponta para a direita
	float A[2];
	float B[2];
	float C[2];
	//coordenadas dos pontos no seu referencia-
	A[0] = -DIM_BOLHA / 2;
	A[1] = DIM_BOLHA / 4;
	B[0] = 0;
	B[1] = 0;
	C[0] = -DIM_BOLHA / 2;
	C[1] = -DIM_BOLHA / 4;

	//rotacao de theta radianos dos pontos para aconpanhar a posição da seta
	transformacaoVetorRotacao(A, B, C, getTheta(configDados));
	//transforma a posicao da seta para o referencial da bolha
	transformaParaRefBolha(seta_pos, configDados);

	//faz uma tranlaçao com o vetor da seta principal
	A[0] += seta_pos[0];
	A[1] += seta_pos[1];
	B[0] += seta_pos[0];
	B[1] += seta_pos[1];
	C[0] += seta_pos[0];
	C[1] += seta_pos[1];

	//transforma os tres pontos para o referencial da janela para dese
	transformaParaRefJanela(A, configDados);
	transformaParaRefJanela(B, configDados);
	transformaParaRefJanela(C, configDados);
	//desenha os segnmentos AB e CB
	SDL_RenderDrawLine(g_pRenderer, A[0], A[1], B[0], B[1]);
	SDL_RenderDrawLine(g_pRenderer, C[0], C[1], B[0], B[1]);
}

/*
Função que recebe tres vetores com cooredena x no indice 0 e coodenada y no indice 1 e lhes aplica uma transformação da matriz rotação em theta radianos
*/
void transformacaoVetorRotacao(float A[], float B[], float C[], float theta)
{
	float aux;
	aux = A[0];
	A[0] = A[0] * cos(theta) - A[1] * sin(theta);
	A[1] = aux * sin(theta) + A[1] * cos(theta);

	aux = B[0];
	B[0] = B[0] * cos(theta) - B[1] * sin(theta);
	B[1] = aux * sin(theta) + B[1] * cos(theta);

	aux = C[0];
	C[0] = C[0] * cos(theta) - C[1] * sin(theta);
	C[1] = aux * sin(theta) + C[1] * cos(theta);
}

/*
Função que transforma as coordenas de um vetor do referencial da janela para o referencial da bolha
O referencial da bolha esta defenido: A origem esta no centro da bolha que sera lancada
									  O eixo dos yy deste referencial tem sentido para cima
									  O eixo do xx deste referencial tem sentid para a direita
A função 'transformaParaRefJanela' é a inversa desta funcao
*/
void transformaParaRefBolha(float  p[], float configDados[])
{
	//equações da transformação de referenciais
	p[0] = p[0] - WINDOW_SIZE_X / 2;
	p[1] = ((WINDOW_SIZE_Y - DIM_BOLHA / 2) / (DIM_BOLHA / 2 - WINDOW_SIZE_Y)) * p[1] + WINDOW_SIZE_Y - DIM_BOLHA / 2;
}

/*
Função que transforma as coordenas de um vetor do referencial da bolha para o referencial da janela
O referencial da bolha esta defenido: A origem esta no centro da bolha que sera lancada
									  O eixo dos yy deste referencial tem sentido para cima
									  O eixo do xx deste referencial tem sentid para a direita
A função 'transformaParaRefBolha' é a inversa desta funcao
*/
void transformaParaRefJanela(float  p[], float configDados[])
{

	p[0] = p[0] + WINDOW_SIZE_X / 2;
	p[1] = ((-WINDOW_SIZE_Y + DIM_BOLHA / 2) / (-DIM_BOLHA / 2 + WINDOW_SIZE_Y)) * (p[1] - WINDOW_SIZE_Y + DIM_BOLHA / 2);
}

/*Função responsável pelo movimeto da bolha, desde o momento em que o utilizador da a indicação para um lancamento ate que, se for o caso,
a bolha lancada se combine com outras e desapareçam.
Esta função retorna: 1 se o utilizador puder lancar uma nova bolha, ou seja, o jogo nao ter acabado (as bolhas não tentaram assumir uma posição ilegal)
					0 se o jogo tiver acabado se por exemplo o excesso de bolhas faça com que sejam preenchidas mais bolhas na vertical do que o permitido
*/
int movimentoBolha(float configDados[], int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ], int corAtual[], int corSeguinte[])
{
	NUMERO_JOGADAS++; //o numero de jogadas aumenta por cada bolha lancada
	float pos_bolha[2]; //vetor que guarda a posição da bolha no referencial da bolha
	pos_bolha[0] = 0; //bolha inicialmete esta na origem
	pos_bolha[1] = 0;
	int parar = 0; //se parar for 1 significa que colidiu com outra bolha ou com os limites da janela, caso contrario pode continuar o seu percurso
	int bolhaChoque[2]; //variavel para guardar as coordenadas da matriz com que a bolha lancada vai chocar
	int jogoContinua = 1; //se for lancada uma bolha que embata em outra que esteja ja no limite vertical da janela e tiverem as condições para explodir, o jogo nao acaba
	float theta = getTheta(configDados);//variavel que guarda o angulo de lancamento dado pela funcao 'getTheta'

	while (parar != 1) {
		SDL_SetRenderDrawColor(g_pRenderer, 255, 255, 255, 255); //set window background white
		SDL_RenderClear(g_pRenderer);
		atualizaBolhas(disposicaoBolhas, configDados); //desenha as bolhas no ecra a partir da matriz das disposiçaõ das bolhas na janela
		atualizaPosBolhaLancada(configDados, pos_bolha, theta);//atualiza a posição da bolha numa iteração
		atualizaBolhaLancada(configDados, pos_bolha, corAtual); //desenha a bolha que esta em movimeto de acordo com a posiçaõ atualizada pela ultima iteraçao
		desenhaBolhaASerLancada(configDados, corAtual, corSeguinte, NAO_MUDA, SO_UMA);//desenha a bolha do canto inferior direito que será lancada no proximo lancamento
		atualizaJanela(); //atualiza o que foi desenhado na janela
		SDL_Delay(5);//delay para a ilusão de movimento
		if (detetaColisao(configDados, pos_bolha, bolhaChoque, disposicaoBolhas) == 1) //verifica se houve uma colisao
		{
			parar = 1; //a bolha para de se mexer
			int bolhaEspacoMaisProximo[2];
			//invocação da funcao para detetar qual o espaco vazio mais proximo a ser ocupado pela bolha, se for ilegal retorna 0 e o programa terminaraá
			jogoContinua = detetaEspacoVazioMaisProximo(configDados, pos_bolha, bolhaChoque, bolhaEspacoMaisProximo, disposicaoBolhas);
			poeBolhaNaMatriz(configDados, disposicaoBolhas, corAtual, bolhaEspacoMaisProximo); //coloca a nova bolha na matriz que guarda as suas posições
			//verifica se ocorre a explosao de bolhas e nesse caso a funcao invocada elimina-as
			//se a bolha que for lancada tiver adotado uma posição irregular mas tiver sido eliminada o jogo pode continuar
			if (verificaSeBolhasExplodem(configDados, disposicaoBolhas, corAtual, bolhaEspacoMaisProximo) == 1 && jogoContinua == 0) {
				jogoContinua = 1;
			}
		}
		else if (detetaLimites(configDados, pos_bolha, corAtual, disposicaoBolhas) == 1) { //se a bolha sair do limite da janela, ela desaparece
			parar = 1;
		}
	}

	SDL_SetRenderDrawColor(g_pRenderer, 255, 255, 255, 255); //set window background white
	SDL_RenderClear(g_pRenderer);
	atualizaBolhas(disposicaoBolhas, configDados); //desenha uma ultima vez as bolhas na matriz
	desenhaBolhaASerLancada(configDados, corAtual, corSeguinte, MUDA, DUAS_BOLHAS);//desenha a bolha a ser lancada e a que sera lancada na proxima ronda, trocado as suas cores
	atualiza_seta(configDados); //desneha a seta
	atualizaJanela();//mostra os desenhos novos

	if ((int)NUMERO_JOGADAS % (int)N_JOGADAS_NOVA_LINHA == 0) //de N_JOGADAS_NOVA_LINHA em N_JOGADAS_NOVA_LINHA é verdade
	{
		SDL_Delay(500); //delay para o utilizador perceber que a linha baixou
		SDL_SetRenderDrawColor(g_pRenderer, 255, 255, 255, 255); //set window background white
		SDL_RenderClear(g_pRenderer);
		jogoContinua = desceLinha(configDados, disposicaoBolhas); //desce a linha na matriz e se as bolhas da ultima linha adotarem uma posiçao ilegal o jogo nao continua
		atualizaBolhas(disposicaoBolhas, configDados); //desenha as bolhas ja com a nova linha
		desenhaBolhaASerLancada(configDados, corAtual, corSeguinte, NAO_MUDA, DUAS_BOLHAS); //desenha as bolhas que seram lancadas nas duas proximas rondas
		atualiza_seta(configDados);//desenha a seta
		atualizaJanela(); //mostra os desenhos no ecra
	}
	return jogoContinua;
}

/*
Função que determina o angulo de lancamento da bolha no referencial da bolha
Retorna o valor do angulo calculado a partir da posição do rato
*/
float getTheta(float configDados[])
{
	int mouse[2];
	float mouse_pos[2];

	SDL_GetMouseState(&mouse[0], &mouse[1]); // Get mouse position
	mouse_pos[0] = mouse[0]; //converte a posição do rato para float
	mouse_pos[1] = mouse[1];

	transformaParaRefBolha(mouse_pos, configDados); //transforma as coordenas do rato para o referencial da bolha
	float theta = atan2f(mouse_pos[1], mouse_pos[0]); //usar a função arco de tangente para determinar o angulo de lancamento em radianos

	return theta;
}

/*
Função que de cada vez que é invocada faz uma iteração para atualiar a posição da bolha no referencial da bolha
A posição da bolha difere de uma constante 'ds' de iteração para iteração, sendo a sua velociadade calculada por: ds/delay usado na função movimentoBolha
Ao receber o angulo de lancamento e as coordenadas da iteração anterior calcula as coordenadas da iteração seguinte
*/
void atualizaPosBolhaLancada(float configDados[], float pos_bolha[], float theta)
{
	const float ds = 0.15 * DIM_BOLHA; //deslocamento
	pos_bolha[0] = pos_bolha[0] + ds * cos(theta); //atualização das coordenadas
	pos_bolha[1] = pos_bolha[1] + ds * sin(theta);

}

/*
Função que desenha a bolha com base na sua posição calculada a partir das coordenadas da iteração anterior
*/
void atualizaBolhaLancada(float configDados[], float pos_bolha[], int cor[])
{
	transformaParaRefJanela(pos_bolha, configDados); //transforma para o referencal da janela para poder desenhar a bolha
	//desenha a bolha e o contorno
	filledCircleRGBA(g_pRenderer, (int)round(pos_bolha[0]), (int)round(pos_bolha[1]), DIM_BOLHA / 2, cor[0], cor[1], cor[2], 255);
	circleRGBA(g_pRenderer, (int)round(pos_bolha[0]), (int)round(pos_bolha[1]), DIM_BOLHA / 2, 0, 0, 0, 255);
	transformaParaRefBolha(pos_bolha, configDados); //transforma novamente para o referencial da bolha para poder calcular a proxima iteração
}

/*
Função que desenha nas janela as duas bolhas a ser lancadas nas duas proximas rondas
Se receber muda = 1 a cor que sera lancada na proxima jogada adota a cor da bolha do canto inferior direito e a bolha a ser lancada na proxiam ronda é gerada automaticamente
Se receber asduas= 1 esta funcao imprime as duas bolhas caso contrário só desenha a bolha a ser lancada no proximo lancamento
*/
void desenhaBolhaASerLancada(float configDados[], int corAtual[], int corSeguinte[], int muda, int asduas)
{
	if (muda == MUDA) //mudanca de cores e geração de nova cor para a bolha a ser lancada na ronda seguinte
	{
		corAtual[0] = corSeguinte[0];
		corAtual[1] = corSeguinte[1];
		corAtual[2] = corSeguinte[2];
		corBolha(1 + rand() % 9, corSeguinte); //cor aleatoria
	}
	//desenha a caixa da bolha 
	SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 205, 255);
	SDL_RenderDrawLine(g_pRenderer, 0, WINDOW_SIZE_Y - DIM_BOLHA - 2 * DL * DIM_BOLHA, DIM_BOLHA + 2 * DL * DIM_BOLHA, WINDOW_SIZE_Y - DIM_BOLHA - 2 * DL * DIM_BOLHA);
	SDL_RenderDrawLine(g_pRenderer, DIM_BOLHA + 2 * DL * DIM_BOLHA, WINDOW_SIZE_Y - DIM_BOLHA - 2 * DL * DIM_BOLHA, DIM_BOLHA + 2 * DL * DIM_BOLHA, WINDOW_SIZE_Y);

	if (asduas == DUAS_BOLHAS)
	{
		//desenha a bolha a ser mandada
		filledCircleRGBA(g_pRenderer, WINDOW_SIZE_X / 2.0, WINDOW_SIZE_Y - DIM_BOLHA / 2.0, DIM_BOLHA / 2.0, corAtual[0], corAtual[1], corAtual[2], 255);
		circleRGBA(g_pRenderer, WINDOW_SIZE_X / 2.0, WINDOW_SIZE_Y - DIM_BOLHA / 2.0, (DIM_BOLHA / 2.0), 0, 0, 0, 255);

	}

	//desenha a bolha a ser mandada na ronda seguinte
	filledCircleRGBA(g_pRenderer, DL * DIM_BOLHA + DIM_BOLHA / 2.0, WINDOW_SIZE_Y - DIM_BOLHA / 2.0 - DIM_BOLHA * DL, DIM_BOLHA / 2.0, corSeguinte[0], corSeguinte[1], corSeguinte[2], 255);
	circleRGBA(g_pRenderer, DL * DIM_BOLHA + DIM_BOLHA / 2.0, WINDOW_SIZE_Y - DIM_BOLHA / 2.0 - DIM_BOLHA * DL, DIM_BOLHA / 2.0, 0, 0, 0, 255);
}

/*
Função que deteta se a bolha atingiu os limites da janela
É invocada a cada iteração para verificar os limites
Retorna: 1 se a posição da bolha estiver fora dos limites da janela
Retorna: 0 se estiver dentro do limites da janela
Se a bolha atingir a linha azul superior, esta função calcula que posiçaõ ela adotara na matriz da disposição das bolhas
*/
int detetaLimites(float configDados[], float pos_bolha[], int corAtual[], int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ])
{

	transformaParaRefJanela(pos_bolha, configDados); //transforma as coordenadas do referencail da bolha para o referencial da janela, para poder comparar

	if (pos_bolha[0] >= WINDOW_SIZE_X - DIM_BOLHA / 2 || pos_bolha[0] <= DIM_BOLHA / 2) //fora dos limites no eixo x
	{
		return 1;
	}
	else if (pos_bolha[1] >= WINDOW_SIZE_Y - DIM_BOLHA / 2 || pos_bolha[1] <= DIM_BOLHA / 2) //fora dos limites no eixo y
	{
		return 1;
	}
	else if (pos_bolha[1] <= ALTURA_BARRA_SUPERIOR + DIM_BOLHA / 2) //se a bolha tocar na linha azul superior 
	{
		int bolhaEspacoMaisProximo[2]; //vetor qu guardará na matriz da bolha lançada
		bolhaEspacoMaisProximo[0] = 0; //a coordenada j da ,matriz é zero porque a bolha é colocada na primeira linha
		//ao arredondar a função inversa da conversão, no eixo x, do referencial da matriz para o referencial da bolha, obtemos a posição na matriz mais proxima das coordenadas de colisão da bolha
		bolhaEspacoMaisProximo[1] = round((1.0 / (DL + 1.0)) * (pos_bolha[0] / (DIM_BOLHA)-0.5 - DL));
		poeBolhaNaMatriz(configDados, disposicaoBolhas, corAtual, bolhaEspacoMaisProximo); //coloca na matriz a nova bolha
		return 1;
	}

	transformaParaRefBolha(pos_bolha, configDados); //trnsforma novamente as coordenadas para o referencial da bolha, para estarem preparadas para a procima iteração
	return 0; //está dentro dos limites e pode continuar as iterações de movimento
}

/*
Função que a cada iteração verifica a distancia entre a bolha em movimento e todas as bolha existentes na matriz
Se alguma das distancias for inferior a DL retorna 1, caso contrario retorna 0
*/
int detetaColisao(float configDados[], float pos_bolha[], int bolhaChoque[], int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ])
{
	//percorre todas a bolhas e verifica se a distancia da bolha de movimento a essas bolhas é meno que DL

	for (int i = 0; i < DIM_JANELA_Y; ++i) //perorre as linhas
	{
		for (int j = 0; j < DIM_JANELA_X; ++j) //percore as colunas
		{
			if (disposicaoBolhas[i][j] != 0) //se existir bolha
			{
				float x = (j + 1) * (DL)*DIM_BOLHA + (DIM_BOLHA / 2.0) * (2 * j + 1); //posição x da bolha em i,j no referencial da janela
				float y = (i + 1) * (DL)*DIM_BOLHA + (DIM_BOLHA / 2.0) * (2 * i + 1) + ALTURA_BARRA_SUPERIOR; //posicao y da bolha em i,j no referencial da janela
				transformaParaRefJanela(pos_bolha, configDados); //transforma a pos da bolha que esta em movimento para o referencial da janela

				float distancia = sqrt((x - pos_bolha[0]) * (x - pos_bolha[0]) + (y - pos_bolha[1]) * (y - pos_bolha[1])); //distancia entre os centros
				transformaParaRefBolha(pos_bolha, configDados);

				if (distancia <= DIM_BOLHA * DL + DIM_BOLHA) // se a distancia entre as bolhas for menor que dl
				{
					//coordenadas da bolha que sofreu o choque
					bolhaChoque[0] = i;
					bolhaChoque[1] = j;
					return 1;

				}

			}
		}

	}

	return 0;

}

/*
Função que recebe a bolha em que ocorreu o choque a posiçaõ da bolha em movimeto
Calcula as distancias entre a bolha que estava em movimeto e a possivel bolha que se encontarria à direita, baixo e esquerda da bolha onde se deu o choque
O espaco vazio cuja respetiva distancia à bolha for menor, é o espaco que a bolha ocupará na matriz
*/
int detetaEspacoVazioMaisProximo(float configDados[], float pos_bolha[], int bolhaChoque[], int bolhaEspacoMaisProximo[], int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ])
{
	transformaParaRefJanela(pos_bolha, configDados);
	int jogoContinua = 1;
	float d1; //diancia da bolha  ao espaco direito
	float d2; //distancia da bolha ao espaco em baixo
	float d3; // distancia da bolha ao espaco em baixo
	int i = bolhaChoque[0];
	int j = bolhaChoque[1];

	if (j < DIM_JANELA_X && disposicaoBolhas[i][j + 1] == 0) //se for possivel por a bolha à direita da de choque
	{
		//calcular d1
		float x = CoordenadasMatriztransformaParaRefJanela(configDados, j + 1, EIXO_X);
		float y = CoordenadasMatriztransformaParaRefJanela(configDados, i, EIXO_Y);
		d1 = sqrt((x - pos_bolha[0]) * (x - pos_bolha[0]) + (y - pos_bolha[1]) * (y - pos_bolha[1])); //distancia à bolha direita
	}
	else d1 = 100001; //valor estupidamente grande para indicar impossibilidade de esta ser o espaco vazio procurado


	if (j > 0 && disposicaoBolhas[i][j - 1] == 0) //se for possivel por a bolha à esqueda da de choque
	{
		//calcular d3
		float x = CoordenadasMatriztransformaParaRefJanela(configDados, j - 1, EIXO_X);
		float y = CoordenadasMatriztransformaParaRefJanela(configDados, i, EIXO_Y);
		d3 = sqrt((x - pos_bolha[0]) * (x - pos_bolha[0]) + (y - pos_bolha[1]) * (y - pos_bolha[1])); //distancia da bolha à esquerda

	}
	else d3 = 100001; //valor estupidamente grande para indicar impossibilidade de esta ser o espaco vazio procurado


	if (i < DIM_JANELA_Y - 1 && disposicaoBolhas[i + 1][j] == 0) //se o espaco abaixo da bolha de choque nao for inválido (pertencer às linhas permitidas)
	{
		float x = CoordenadasMatriztransformaParaRefJanela(configDados, j, EIXO_X);
		float y = CoordenadasMatriztransformaParaRefJanela(configDados, i + 1, EIXO_Y);
		d2 = sqrt((x - pos_bolha[0]) * (x - pos_bolha[0]) + (y - pos_bolha[1]) * (y - pos_bolha[1]));	 //calcula distancia à bolha de baixo
	}
	else //caso contrario, se esta for a menor distancia o jogo termina pois a bolha esta mais perto da posiçaõ ilegal, fora das linhas permitidas
	{
		float x = CoordenadasMatriztransformaParaRefJanela(configDados, j, EIXO_X);
		float y = CoordenadasMatriztransformaParaRefJanela(configDados, i + 1, EIXO_Y);
		d2 = sqrt((x - pos_bolha[0]) * (x - pos_bolha[0]) + (y - pos_bolha[1]) * (y - pos_bolha[1])); //calcula distancia à bolha de baixo

		if (valorMinimo(d1, d2, d3) == 2) jogoContinua = 0; //se o valor minimo das distancias for o da distancia ao espaco abaixo da bolha de choque o prograa termina	
	}

	switch (valorMinimo(d1, d2, d3))//valor minimo retorna um inteiro correspondente à menor distancia (1-d1, 2-d2, 3-d3)
	{
	case 1: //se a distancia à direita for a mais pequena
		bolhaEspacoMaisProximo[0] = i;
		bolhaEspacoMaisProximo[1] = j + 1;
		break;
	case 2://se a distancia a baixo for a mais pequena
		bolhaEspacoMaisProximo[0] = i + 1;
		bolhaEspacoMaisProximo[1] = j;
		break;
	case 3://se a distancia à esquerda for a mais pequena
		bolhaEspacoMaisProximo[0] = i;
		bolhaEspacoMaisProximo[1] = j - 1;
		break;
	}
	transformaParaRefBolha(pos_bolha, configDados); //transforma novamente o vetor posiçao da bolha em movimeto para o referencial da bolha
	return jogoContinua;
}

/*
Funcao que recebe as coordenadas da matriz do espaco onde é para introduzir a bolha e introduz nessas cordenadas da matriz de disposição de bolhas
o numero corresponde à cor da bolha em movimento que tambem recebe
*/
void poeBolhaNaMatriz(float configDados[], int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ], int corAtual[], int bolhaEspacoMaisProximo[])
{
	int codigoCor;
	codigoCor = RGBparaCodigoCores(corAtual); //transforma os valores rgb da bolha para um inteiro correspondentea ao codigo da cor
	disposicaoBolhas[bolhaEspacoMaisProximo[0]][bolhaEspacoMaisProximo[1]] = codigoCor; //atribui esse valor à posição onde é para inserir a bolha
}

/*
Função que transforma coordenas da matriz para coordenadas da janela
Recebe o valor da coordenada e indicação de que eixo é que essa coordenada se refere (0-X, 1-Y) e retorna o valor dessa coordenada no referencial da janela
*/
float CoordenadasMatriztransformaParaRefJanela(float configDados[], int coordenada, int eixo)
{
	if (eixo == EIXO_X)
	{
		return (coordenada + 1) * (DL)*DIM_BOLHA + (DIM_BOLHA / 2.0) * (2 * coordenada + 1); //equacao para os xx
	}
	else if (eixo == EIXO_Y)
	{
		return (coordenada + 1) * (DL)*DIM_BOLHA + (DIM_BOLHA / 2.0) * (2 * coordenada + 1) + ALTURA_BARRA_SUPERIOR; // eqaucao para os yy
	}

	return 0;

}

/*
Funcao que recebe tres valores reais e retorna um inteiro indicando qual dos tres valores recebidos é o mais pequeno
Retorna 1: d1 é o menor
		2: d2 é o menor
		3: d3 é o menor
*/
int valorMinimo(float d1, float d2, float d3)
{

	if (d1 < d2)
	{
		if (d3 < d1) return 3;
		else return 1;
	}
	else
	{
		if (d3 < d2) return 3;
		else return 2;
	}

	return 0;
}

/*
Funcao que no final do jogo pede o nome ao utilizador e guarda o seu nome associado à pontuação obtida no jogo no ficheiro resultados.txt
Esta função está tambem responsável pela ordenação do ficheiro de resultados, fazendo uso de outras funções
Retorna : 1 em caso de sucesso
		: 0 em caso de invalidade do ficheiro de resultados
*/
int fimDoJogo(float configDados[])
{
	char nomeDoJogador[22] = ""; //string que reseberá o nome do utilizador
	printf("Introduza o nome do jogador (max 20 ch): ");
	fflush(stdout);
	if (scanf("%20s", nomeDoJogador) != 1) //extrai a string com o nome do jogador
	{
		printf("Erro: leitura do nome do jogador não foi efetuada com sucesso.\n");
	}

	int pontosFichResultados[MAX_DIM_MATRIZ_RESULTADOS]; //vetor que guarda a extração dos daos do ficheiro de resultados
	char nomeDoJogadorFichResultados[MAX_DIM_MATRIZ_RESULTADOS][23]; //vetor de strings (matriz de caracteres) que guarda o nome do jogador
	char str[51] = ""; //string que será utilizada para ler as linhas e extrair informação do ficheiro de configurações
	int linha = 0;

	FILE* ficheiroResultados;
	ficheiroResultados = fopen("resultados.txt", "r"); //abre o ficheiro dos resultados no final e em modo leitura

	if (ficheiroResultados == NULL) //se há um erro na abertura do ficheiro (inexistencia na diretória do progrma)
	{
		printf("Erro: ficheiro resultados não encontrado\n");
		FILE* novoFicheiro; //cria um novo ficheiro
		novoFicheiro = fopen("resultados.txt", "w");
		fprintf(novoFicheiro, "Jogadores            Pontuação\n"); //imprimir o cabeçalho no ficheiro
		fclose(novoFicheiro);
		printf("Novo ficheiro criado...\n");

	}
	else
	{

		fgets(str, 50, ficheiroResultados);//ignora primeira e segunda linhas
		fgets(str, 50, ficheiroResultados);

		do
		{

			str[0] = '\0';
			fgets(str, 50, ficheiroResultados); //extrai uma linha
			if (sscanf(str, "%20s %d", nomeDoJogadorFichResultados[linha], &pontosFichResultados[linha]) != 2) //intrepreta a linha de onde extrai o nome e a pontuação
			{
				//se o numero de conversoes não for dois algo correu mal e o programa termina com erro (retorna -1)
				printf("Erro: o ficheiro 'resultados.txt' foi alterado de forma inválida.\n");
				fflush(stdout);
				return -1;
			}
			linha++;

		} while (feof(ficheiroResultados) == 0); //enquanto nao atinge o fim do ficheiro

		fclose(ficheiroResultados);//fecha o documento
	}

	pontosFichResultados[linha] = PONTOS; //adiciona os pontos e nome do jogador atual`aos extraidos do ficheiro
	strcpy(nomeDoJogadorFichResultados[linha], nomeDoJogador);

	//organiza as pontuações dos jogadores partindo do principio que já estvam posteriormente organizadas de forma decrescente
	bubleSortExtracaoFichResultados(linha, pontosFichResultados, nomeDoJogadorFichResultados);

	//guarda as pontuações organizadas no fichheiro
	if (guardaNovamenteNoficheiroResultados(linha, pontosFichResultados, nomeDoJogadorFichResultados) == 0)
	{
		return -1;
	}

	return 1;
}

/*
Função que recebe os vetores da pontuação dos jogadores e os seus nomes em que na ultima posição estão os dados do jogador anterior
A partir dessas matrizes organiza os dados de forma decrescente pela pontuação, supondo que os dados estavam já previamnete organizados, utilizando o algoritmo Buble Sort
*/
void bubleSortExtracaoFichResultados(int linha, int pontos[MAX_DIM_MATRIZ_RESULTADOS], char nome[MAX_DIM_MATRIZ_RESULTADOS][23])
{
	for (int i = linha; i > 0; --i) //percorre os indices do vetor começando pela nova pontuação
	{
		//verifica se a pontuação acima é mias alta, senão é feita uma troca
		//seguidamente se na posição acima da qual o valor mais recente foi tranferido estiver uma pontuação inferior, é feita uma outra troca e assim sucessivamente
		if (pontos[i] > pontos[i - 1]) //se a nova pontuação for superior à de cada casa troca as pontuações e os nomes de ordem
		{
			//troca dos pontos entre os indices i e i -1
			int aux = pontos[i - 1];
			pontos[i - 1] = pontos[i];
			pontos[i] = aux;

			//troca dos nomes entre os indices i e i-1
			char strAux[23] = "";
			strcpy(strAux, nome[i - 1]);
			strcpy(nome[i - 1], nome[i]);
			strcpy(nome[i], strAux);
		}
		else break; //caso contrário o vetor estºa já organizado e é inutil continuar a percorrer as linhas

	}
}

/*
Função que recebe as matrizes dos nomes e vetor dos pontos dos jogadores já organizadas de forma decrecente de pontos e que imprime os resultados no ficheiro correspondente
Retorna 1: no caso de sucesso
		0: no caso de insucesso na abertura do ficheiro 'resulttados.txt'
*/
int guardaNovamenteNoficheiroResultados(int linha, int pontosFichResultados[MAX_DIM_MATRIZ_RESULTADOS], char nomeDoJogadorFichResultados[MAX_DIM_MATRIZ_RESULTADOS][23])
{

	FILE* ficheiroResultados;
	ficheiroResultados = fopen("resultados.txt", "w"); //abre o ficheiro em modo de escrita e apaga o seu conteudo

	if (ficheiroResultados == NULL) //se for impossivel abrir o ficheiro
	{
		printf("Erro: a abertura do ficheiro resultados.\n");
		return 0;
	}

	fprintf(ficheiroResultados, "Jogadores            Pontuação\n"); //imprimir o cabeçalho no ficheiro

	for (int i = 0; i <= linha; ++i)
	{
		fprintf(ficheiroResultados, "\n%-20s %d", nomeDoJogadorFichResultados[i], pontosFichResultados[i]); //imprimir as pontuações já organizadas
	}

	printf("Pontuação guardada com sucesso!\n"); //mensagem de sucesso no terminal

	return 1;
}

/*
Função que verifica se as bolha que foi recentemente lancada ficou presa junto de uma ou mais da mesma cor, se sim trata de eliminar as bolhas
Recebe a disposição atual das bolhas, a cor da bolha que acabou de ficar presa às outras e as suas coordenadas
Por cada bola que desaparece o jogador recebe 1 ponto
Esta função retorna: 1 se eliminar bolhas
					 0 se não eliminar bolhas
*/
int verificaSeBolhasExplodem(float configDados[], int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ], int corAtual[], int coordenadasBolhaAtual[])
{

	int semBolha[3]; //cor da bolha inexistente, declarada para ser possivel passar à função poeBolhaNaMatriz a informação das bolhas que é para suprimir
	semBolha[0] = 266; //rgb invaĺido represetando a inexistencia de bolha
	semBolha[1] = 266;
	semBolha[2] = 266;


	int codigoCorBolhaAtual;
	codigoCorBolhaAtual = RGBparaCodigoCores(corAtual); //converte a cor rgb da cor da bolha que foi lancada para o codigo de cores implementado

	//-----------------------------------------------------nivel 3-----------------------------------------------------------------

	int coordenadasDeBolhasASerEliminadas[50][2]; //matriz que guarda as coordenadas em relaçao à matriz disposicaoBolhas das bolhas que serao eliminadas
	coordenadasDeBolhasASerEliminadas[0][0] = coordenadasBolhaAtual[0]; //a primeira a entrar na matriz é a bolha mais recentemente lancada
	coordenadasDeBolhasASerEliminadas[0][1] = coordenadasBolhaAtual[1];

	int coordenadaAintroduzir = 1; //numero de bolhas ja introduzidas na matriz

	//verifica se há bolhas da mesma cor na vizinhança da bolha que foi recentemente lancada. se sim, insere-as na matriz de bolhas a eliminar
	coodenadasMesmaCor(disposicaoBolhas, codigoCorBolhaAtual, coordenadasBolhaAtual, coordenadasDeBolhasASerEliminadas, &coordenadaAintroduzir);

	/*
	Em cada iteracao procura-se as bolhas na vizinhanca das bolhas que ja estão marcadas para ser eliminadas na matriz
	Como de cada vez que é adicionada uma nova bolha 'coordenadaAintroduzir' aumenta em 1, este ciclo so terminará quando nao existirem mais bolhas
	da mesma cor na vizinhança de todas as bolhas marcadas para eliminar na matriz 'coordenadasDeBolhasASerEliminadas'
	*/
	for (int i = 1; i < coordenadaAintroduzir; ++i)
	{
		coodenadasMesmaCor(disposicaoBolhas, codigoCorBolhaAtual, coordenadasDeBolhasASerEliminadas[i], coordenadasDeBolhasASerEliminadas, &coordenadaAintroduzir);
	}

	if (coordenadaAintroduzir >= 3) //se as bolhas que fazem caminhos sao tres ou mais, elas sao eliminadas, caso contrario nao
	{

		for (int i = 0; i < coordenadaAintroduzir; ++i)
		{
			//introduz na matriz da disposição das bolhas a informação de bolha inexistente para todas as bolhas marcadas para ser eliminadas
			poeBolhaNaMatriz(configDados, disposicaoBolhas, semBolha, coordenadasDeBolhasASerEliminadas[i]);

		}

		PONTOS += coordenadaAintroduzir; //o jogador recebe um ponto por cada bolha eliminada
		return 1;
	}

	return 0;
}


/*
Função que recebe a disposiçaõ atual das bolhas, as coordenadas de uma bolha a partir da qual verifica se há bolhas na vizinhança. Se sim introduz
as coordenadas dessa ou dessas bolhas na matriz de bolhas selecionadas para ser eliminadas.
No entanto, como esta função vai ser invocada para procurar bolhas da mesma cor na vizinhança de cada bolha marcada para ser eliminada, é necessário restringir
as bolhas de procura, caso contrario, o ciclo seria infinito.
Assim, esta função so verifica se as bolhas na sua vizinhanca são da mesma cor se elas nao estiverem ja na matriz de bolhas marcadas para ser eliminadas
*/
void coodenadasMesmaCor(int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ], int codigoCorBolhaAtual, int coordenadasDaBolhaOrigemVizinhanca[], int coordenadasDeBolhasASerEliminadas[50][2], int* coordenadaAintroduzir)
{

	/*
		matriz da forma

		i     j      pesquisaBolhaij

		.
		.
		.

		em que i e j são as coordenadas das bolhas na vizinhamca e pesquisaBolha ij adota o valor de 0 ou 1 caso ja esteja marcada para ser eliminada ou nao respetivamente
	*/
	int coordenadasBolhasVizinhanca[8][3];
	procuraCoordenadasBolhas1(coordenadasDaBolhaOrigemVizinhanca, coordenadasDeBolhasASerEliminadas, coordenadaAintroduzir, coordenadasBolhasVizinhanca);
	procuraCoordenadasBolhas2(disposicaoBolhas, coordenadasDeBolhasASerEliminadas, coordenadaAintroduzir, coordenadasBolhasVizinhanca, codigoCorBolhaAtual);

}

/*
Função que dá as coordenadas às bolhas na vizinhanca da bolha na origem e verifica que elas não estao ja marcadas para ser eliminadas
*/
void procuraCoordenadasBolhas1(int coordenadasDaBolhaOrigemVizinhanca[], int coordenadasDeBolhasASerEliminadas[50][2], int* coordenadaAintroduzir, int coordenadasBolhasVizinhanca[8][3])
{

	//em principio todoas as bolhas são pesquisadas, senão é lhes atribuido o valor o nas linhas seguintes
	CIMA_PESQUISA = 1;
	DIREITA_PESQUISA = 1;
	ESQUERDA_PESQUISA = 1;
	BAIXO_PESQUISA = 1;
	DIAGONAL1_PESQUISA = 1;
	DIAGONAL2_PESQUISA = 1;
	DIAGONAL3_PESQUISA = 1;
	DIAGONAL4_PESQUISA = 1;

	//atribui as coordenadas
	CIMA_0 = coordenadasDaBolhaOrigemVizinhanca[0] - 1;
	CIMA_1 = coordenadasDaBolhaOrigemVizinhanca[1];
	for (int i = 0; i < *coordenadaAintroduzir; ++i) { //verifica que as coordenadas das bolhas nao estao ja selecionadas para ser eliminadas (previne ciclo infinito)
		if (CIMA_0 == coordenadasDeBolhasASerEliminadas[i][0] && CIMA_1 == coordenadasDeBolhasASerEliminadas[i][1]) {
			CIMA_PESQUISA = 0;
			break;
		}
	}

	DIREITA_0 = coordenadasDaBolhaOrigemVizinhanca[0];
	DIREITA_1 = coordenadasDaBolhaOrigemVizinhanca[1] + 1;
	for (int i = 0; i < *coordenadaAintroduzir; ++i)
	{
		if (DIREITA_0 == coordenadasDeBolhasASerEliminadas[i][0] && DIREITA_1 == coordenadasDeBolhasASerEliminadas[i][1])
		{
			DIREITA_PESQUISA = 0;
			break;
		}
	}

	ESQUERDA_0 = coordenadasDaBolhaOrigemVizinhanca[0];
	ESQUERDA_1 = coordenadasDaBolhaOrigemVizinhanca[1] - 1;
	for (int i = 0; i < *coordenadaAintroduzir; ++i)
	{
		if (ESQUERDA_0 == coordenadasDeBolhasASerEliminadas[i][0] && ESQUERDA_1 == coordenadasDeBolhasASerEliminadas[i][1])
		{
			ESQUERDA_PESQUISA = 0;
			break;
		}
	}

	BAIXO_0 = coordenadasDaBolhaOrigemVizinhanca[0] + 1;
	BAIXO_1 = coordenadasDaBolhaOrigemVizinhanca[1];
	for (int i = 0; i < *coordenadaAintroduzir; ++i)
	{
		if (BAIXO_0 == coordenadasDeBolhasASerEliminadas[i][0] && BAIXO_1 == coordenadasDeBolhasASerEliminadas[i][1])
		{
			BAIXO_PESQUISA = 0;
			break;
		}
	}

	DIAGONAL1_0 = coordenadasDaBolhaOrigemVizinhanca[0] - 1;
	DIAGONAL1_1 = coordenadasDaBolhaOrigemVizinhanca[1] + 1;
	for (int i = 0; i < *coordenadaAintroduzir; ++i)
	{
		if (DIAGONAL1_0 == coordenadasDeBolhasASerEliminadas[i][0] && DIAGONAL1_1 == coordenadasDeBolhasASerEliminadas[i][1])
		{
			DIAGONAL1_PESQUISA = 0;
			break;
		}
	}

	DIAGONAL2_0 = coordenadasDaBolhaOrigemVizinhanca[0] - 1;
	DIAGONAL2_1 = coordenadasDaBolhaOrigemVizinhanca[1] - 1;
	for (int i = 0; i < *coordenadaAintroduzir; ++i)
	{
		if (DIAGONAL2_0 == coordenadasDeBolhasASerEliminadas[i][0] && DIAGONAL2_1 == coordenadasDeBolhasASerEliminadas[i][1])
		{
			DIAGONAL2_PESQUISA = 0;
			break;
		}
	}

	DIAGONAL3_0 = coordenadasDaBolhaOrigemVizinhanca[0] + 1;
	DIAGONAL3_1 = coordenadasDaBolhaOrigemVizinhanca[1] - 1;
	for (int i = 0; i < *coordenadaAintroduzir; ++i)
	{
		if (DIAGONAL3_0 == coordenadasDeBolhasASerEliminadas[i][0] && DIAGONAL3_1 == coordenadasDeBolhasASerEliminadas[i][1])
		{
			DIAGONAL3_PESQUISA = 0;
			break;
		}
	}

	DIAGONAL4_0 = coordenadasDaBolhaOrigemVizinhanca[0] + 1;
	DIAGONAL4_1 = coordenadasDaBolhaOrigemVizinhanca[1] + 1;
	for (int i = 0; i < *coordenadaAintroduzir; ++i)
	{
		if (DIAGONAL4_0 == coordenadasDeBolhasASerEliminadas[i][0] && DIAGONAL4_1 == coordenadasDeBolhasASerEliminadas[i][1])
		{
			DIAGONAL4_PESQUISA = 0;
			break;
		}
	}

}

/*
Função que verifica se as as bolhas na vizinhança sao da mesma cor no caso de ainda nao existirem na matriz
*/
void procuraCoordenadasBolhas2(int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ], int coordenadasDeBolhasASerEliminadas[50][2], int* coordenadaAintroduzir, int coordenadasBolhasVizinhanca[8][3], int codigoCorBolhaAtual)
{

	//se a bolha acima tem a mesma cor que a bolha recentemente mandada introduz as suas coordenas na matriz de bolhas a ser eliminadas
	if (disposicaoBolhas[CIMA_0][CIMA_1] == codigoCorBolhaAtual && CIMA_PESQUISA == 1) {
		coordenadasDeBolhasASerEliminadas[*coordenadaAintroduzir][0] = CIMA_0;
		coordenadasDeBolhasASerEliminadas[*coordenadaAintroduzir][1] = CIMA_1;
		(*coordenadaAintroduzir)++;
	}

	if (disposicaoBolhas[DIREITA_0][DIREITA_1] == codigoCorBolhaAtual && DIREITA_PESQUISA == 1) {
		coordenadasDeBolhasASerEliminadas[*coordenadaAintroduzir][0] = DIREITA_0;
		coordenadasDeBolhasASerEliminadas[*coordenadaAintroduzir][1] = DIREITA_1;
		(*coordenadaAintroduzir)++;
	}

	if (disposicaoBolhas[ESQUERDA_0][ESQUERDA_1] == codigoCorBolhaAtual && ESQUERDA_PESQUISA == 1) {
		coordenadasDeBolhasASerEliminadas[*coordenadaAintroduzir][0] = ESQUERDA_0;
		coordenadasDeBolhasASerEliminadas[*coordenadaAintroduzir][1] = ESQUERDA_1;
		(*coordenadaAintroduzir)++;
	}

	if (disposicaoBolhas[BAIXO_0][BAIXO_1] == codigoCorBolhaAtual && BAIXO_PESQUISA == 1) {
		coordenadasDeBolhasASerEliminadas[*coordenadaAintroduzir][0] = BAIXO_0;
		coordenadasDeBolhasASerEliminadas[*coordenadaAintroduzir][1] = BAIXO_1;
		(*coordenadaAintroduzir)++;
	}

	if (disposicaoBolhas[DIAGONAL1_0][DIAGONAL1_1] == codigoCorBolhaAtual && DIAGONAL1_PESQUISA == 1) {
		coordenadasDeBolhasASerEliminadas[*coordenadaAintroduzir][0] = DIAGONAL1_0;
		coordenadasDeBolhasASerEliminadas[*coordenadaAintroduzir][1] = DIAGONAL1_1;
		(*coordenadaAintroduzir)++;
	}

	if (disposicaoBolhas[DIAGONAL2_0][DIAGONAL2_1] == codigoCorBolhaAtual && DIAGONAL2_PESQUISA == 1) {
		coordenadasDeBolhasASerEliminadas[*coordenadaAintroduzir][0] = DIAGONAL2_0;
		coordenadasDeBolhasASerEliminadas[*coordenadaAintroduzir][1] = DIAGONAL2_1;
		(*coordenadaAintroduzir)++;
	}

	if (disposicaoBolhas[DIAGONAL3_0][DIAGONAL3_1] == codigoCorBolhaAtual && DIAGONAL3_PESQUISA == 1) {
		coordenadasDeBolhasASerEliminadas[*coordenadaAintroduzir][0] = DIAGONAL3_0;
		coordenadasDeBolhasASerEliminadas[*coordenadaAintroduzir][1] = DIAGONAL3_1;
		(*coordenadaAintroduzir)++;
	}

	if (disposicaoBolhas[DIAGONAL4_0][DIAGONAL4_1] == codigoCorBolhaAtual && DIAGONAL4_PESQUISA == 1) {
		coordenadasDeBolhasASerEliminadas[*coordenadaAintroduzir][0] = DIAGONAL4_0;
		coordenadasDeBolhasASerEliminadas[*coordenadaAintroduzir][1] = DIAGONAL4_1;
		(*coordenadaAintroduzir)++;
	}
}

/*
Função que desce uma linha na matriz disposicaoBolhas e gera a linha superior aleatoriamente
Retorna: 0 se o jogo acabar por uma bolha adotar uma posição ilegal
		 1 se o jogo puder continuar
*/
int desceLinha(float configDados[], int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ])
{
	int jogoContinua = 1;

	for (int i = DIM_JANELA_Y; i >= 0; i--) //percorre as linhas desde o fundo até à segunda
	{
		for (int j = 0; j < DIM_JANELA_X; ++j) //percorre as colunas
		{
			if (i == 0)
			{
				disposicaoBolhas[0][j] = 1 + rand() % 9; //aleatoriza numeros na primeira linha
			}
			else
			{
				disposicaoBolhas[i][j] = disposicaoBolhas[i - 1][j]; //as linhas avançam uma posição

				if (disposicaoBolhas[i][j] != 0 && i == DIM_JANELA_Y) //se adotarem uma posição ilegal o jogo para
				{
					jogoContinua = 0;
				}

			}

		}
	}


	return jogoContinua;
}

/*
Função que verifica se o utilizador carregou nos botoes
Retorna: 0 se nao carregou em nada
		 1 se carrega no botao fim do jogo
		 2 se carrega no botao novo jogo
*/
int verificaBotoes(float configDados[])
{
	int mouse_pos[2];
	SDL_GetMouseState(&mouse_pos[0], &mouse_pos[1]); //deteta a posição do rato

	if (mouse_pos[0] >= 0.02 * WINDOW_SIZE_X && mouse_pos[0] <= 0.20 * WINDOW_SIZE_X && mouse_pos[1] >= 0.25 * DIM_BOLHA && mouse_pos[1] <= 1.25 * DIM_BOLHA) //limites do botao novo jogo
	{
		return 2;
	}
	else if (mouse_pos[0] >= 0.25 * WINDOW_SIZE_X && mouse_pos[0] <= 0.37 * WINDOW_SIZE_X && mouse_pos[1] >= 0.25 * DIM_BOLHA && mouse_pos[1] <= 1.25 * DIM_BOLHA) //linmites do botao fim do jogo
	{
		return 1;
	}

	return 0;

}

/*
Função que verifica se o utilizador conseguiu eliminar todas as bolhas
Retorna 1: caso ainda haja bolhas na janela
		0: caso tiver ganho o jogo
*/
int verificaVitoria(float configDados[], int disposicaoBolhas[MAX_DIM_MATRIZ][MAX_DIM_MATRIZ])
{

	for (int i = 0; i < DIM_JANELA_Y; ++i) //percorre as linhas
	{
		for (int j = 0; j < DIM_JANELA_X; ++j)//percorre as colunas
		{
			if (disposicaoBolhas[i][j] != 0) //se existir uma bolha retorna logo 1
			{
				return 0;
			}
		}
	}

	return 1;
}