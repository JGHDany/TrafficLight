
/**
 * @author Danielson Rodrigues
 *
 *    Programa que simule o tráfego de veículos por um semáforo.
 *
 * a) O semáforo controla duas pistas que se cruzam, e cada uma possui direcção única.
 * b) O semáforo possui apenas as luzes verde e vermelha, levando 15 segundos para cada mudança de luz.
 * c) A pista por onde o carro vai (1 ou 2) será escolhida aleatoriamente.
 * d) Cada carro chegará ao semáforo num intervalo (aleatório) de 1 a 30 segundos após outro.
 * e) Quando o semáforo estiver verde, cada carro da fila leva um tempo de 3 segundos após a saída do carro da frente para dar partida.
 *
 * 		ESTADO DO CODIGO <-- OK.
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>		//alocacao de memoria a ser utilizada.
#include <time.h>		//inclusao da biblioteca que trabalha com a hora do sistema em milisegundos.

#define TROCA_DE_LUZ 15	//tempo que as luzes levam para se trocar (VERMELHO-VERDE/VEDE-VERMELHO)
#define TRUE 1			//Variaveis booleanas
#define FALSE 0			//Variaveis booleanas
#define CARRUAGEM 10 	//total de carros que se pretende gerar por cada simulacao
#define GERA_CARRO 15	//tempo (aleatorio) maximo que um carro leva a chegar na fila.
#define GERA_PISTA 2	//numero de pistas que os carros podem aceder
#define VERDE 1			//luz verde acesa
#define VERMELHA 0		//luz vermelha
#define WAIT_TIME_EXIT 3//tempo que cada carro leva para dar partida na frente da fila

//Definição das Estrutura de Dados:
typedef int telem;
typedef struct no {
	telem dado;
	struct no* prox;
} tno;

typedef struct fila {
	tno* inicio;
	tno* final;
} tfila;

typedef struct semaf{
	int luz ;
}Semaforo;

typedef struct pis{
	int pista ;
}Pista;

//Operações: Prototipos da funcoes
//1. Criar - cria uma fila vazia
void criar (tfila *F);

//2. Vazia - testa se uma fila está vazia
int vazia (tfila F);

//3. Primeiro - obtém o elemento do início de uma fila
int primeiro (tfila F, telem *elem);

//4. Inserir - insere um elemento no fim de uma fila
int inserir (tfila *F, telem valor);

//5. Remover - remove o elemento do início de uma fila, retornando o elemento removido
int remover (tfila *F, telem *valor);

//7. Esvaziar - remove todos os elementos da fila.
int esvaziarFila(tfila *F);

//8. ImprimirFila - imprime todos os elementos da fila.
void imprimirFila(tfila F);

//funcao que gera um valor aleatorio no intervalo de 0 ao valor no argumento.
int geraValor(int maxValue);

//demora o tempo passado pelo argumento
void wait(int tespera);

//Inicializa o semaforo com a luz verde na pista 0
void iniciaSemaforoEPista(Semaforo *s, Pista *p);

//troca as luzes e as pistas dos semaforos depois de 15 segundos.
void trocaLuz(Semaforo *s);

//imprime os carros a passarem de 3/3 seg e a situacao das filas
void ImprimePistaComPassagemNoSemaforo(telem carro, tfila fila1, tfila fila2, int p);

int main()
{
   printf("\nMAIN:");
	Semaforo s; //semaforo
	Pista p;	//pista
	iniciaSemaforoEPista(&s, &p);//inicia o semaforo e a pista a trabalhar
	int flag = 0;
	tfila fila1, fila2;
	//inicializacao das filas
	criar(&fila1);
	criar(&fila2);

	while(TRUE){
		//DECLARACAO DE VARIAVEIS
		telem carro;
		int totalCarros = 0; //total de carros a serem gerados em cada situacao
		//Ciclo para a geracao inicial de carros e colocacao na respectiva pista
		do{

			int pistaGerada;
			//parte entrada no semaforo
			carro = geraValor(GERA_CARRO);//gera carros para colocar nas filas
			pistaGerada = geraValor(GERA_PISTA); //gera a pista a colocar o carro.
			wait(carro); //tempo de espera na geracao de um carro

			if(pistaGerada == 0){
				inserir(&fila1,carro);
			}else inserir(&fila2,carro);
			totalCarros++; //contabiliza o total de carros gerados (insere-os na filas)

			//estatistica
			/*printf("\nInseriu o carro {%d} na pista {%d}",carro,pistaGerada);
			if(pistaGerada == 0){
				printf("\nFILA #1::");
				imprimirFila(fila1);
			}
			else{
				printf("\nFILA #2::");
				imprimirFila(fila2);
			}*/
		}while(totalCarros < CARRUAGEM);

		//imprime as informacoes dos carros gerados
		printf("\nCARROS GERADOS FILA #1::");imprimirFila(fila1);
		printf("\nCARROS GERADOS FILA #2::");imprimirFila(fila2);

		//tratamento dos carros na fila para a passagem no semaforo
		int totaltime = 0;
		do{
			if((s.luz == VERDE) && (flag == 0) && (vazia(fila2) == 0)){
				remover(&fila1, &carro);
				wait(WAIT_TIME_EXIT); //espera 3 segundos para a saida do proximo carro.
				totaltime += WAIT_TIME_EXIT;
			}else if((s.luz == VERDE) && (flag == 1) &&(vazia(fila2)== 0)){
				remover(&fila2, &carro);
				wait(WAIT_TIME_EXIT); //espera 3 segundos para a saida do proximo carro.
				totaltime += WAIT_TIME_EXIT;
			}
			//verifica se ha necessidade de trocar as luzes e pista (se ja passou 15 segundos com luz verde na mesma pista)
			if(totaltime >= TROCA_DE_LUZ){
				//printf("\nOBS: Vai trocar as luzes e pistas");
				trocaLuz(&s);			//troca a luz do semaforo
				totaltime = 0; 			//actualiza o tempo total para contar os proximos 15 segundos para a troca de luz
				if(flag == 0) flag = 1;	//muda de pista uma vez que a luz alterou
				else flag =0;
			}
			//imprime os carros a passarem de 3/3 seg e a situacao das filas
			ImprimePistaComPassagemNoSemaforo(carro, fila1, fila2, flag);
			//printf("\n faltam passar %d carros",totalCarros );
			totalCarros--; //efectua passagem para todos os carros gerados (elimina-os das filas)
		}while(totalCarros >= 0);
		//Estatisticas por cada Geracao.
	}
	return 0;
}

//Operações:

//imprime os carros a passarem de 3/3 seg e a situacao das filas
void ImprimePistaComPassagemNoSemaforo(telem carro, tfila fila1, tfila fila2,int p ){

	printf("\n\n [Deu passagem o carro [%d] da pista %d]", carro, p);
	printf("\n");
	printf("\n## ESTADO FILA 1 ##");imprimirFila(fila1);
	printf("\n## ESTADO FILA 2 ##");imprimirFila(fila2);
	printf("\n");

}

//Inicializa o semaforo com a luz verde na pista 0
void iniciaSemaforoEPista(Semaforo *s, Pista *p){
	s->luz = VERDE;
	p->pista = 0;
}

//troca as luzes dos semaforos.
void trocaLuz(Semaforo *s){

	if(s->luz == VERDE)
		s->luz == VERMELHA;
	else
		s->luz == VERDE;
}

//demora o tempo passado pelo argumento
void wait(int tespera){

	clock_t t1;
	t1 = clock();
	while(((clock()-t1)/CLOCKS_PER_SEC) < tespera);
}

//funcao que gera um valor aleatorio no intervalo de 0 ao valor no argumento.
int geraValor(int maxValue){

	return (rand()%maxValue);
}

//1. Criar - cria uma fila vazia
void criar (tfila *F)
{
	F->inicio = F->final = NULL;
}

//2. Vazia - testa se uma fila está vazia
int vazia (tfila F)
{
	return (F.inicio == NULL && F.final == NULL);
}

//3. Primeiro - obtém o elemento do início de uma fila
int primeiro (tfila F, telem *elem)
{
	if (vazia(F)) { // Erro: fila vazia
		printf("\n\tprimeiro: Fila Vazia");
		return 0;
	}
	*elem = (F.inicio)->dado;
	return 1;
}

//4. Inserir - insere um elemento no fim de uma fila
int inserir (tfila *F, telem valor)
{
	tno *novo;
	novo = (tno*) malloc(sizeof(tno));
	if (novo == NULL) return 0; /* Erro: mem¢ria insuficiente */
	novo->dado = valor;
	novo->prox = NULL;
	if (vazia(*F))
		F->inicio = novo;
	else
		(F->final)->prox = novo;
	F->final = novo;
	return 1;
}

//5. Remover - remove o elemento do início de uma fila, retornando o elemento removido
int remover (tfila *F, telem *valor)
{
	tno *aux;
	if (vazia(*F)){ // Erro: fila vazia
		printf(" Fila Vazia");
		return 0;
	}

	primeiro(*F,valor);
	if(F->inicio == F->final)
		F->final = NULL;
	aux = F->inicio;
	F->inicio = (F->inicio)->prox;
	free(aux);
	return 1;
}

//7. Esvaziar - remove todos os elementos da fila.
int esvaziarFila(tfila *F){

	if(vazia(*F)) { // Erro: fila vazia
		printf("\n\tesvaziarFila: Fila Vazia");
		return 0;
	}
	tfila *faux;
	faux->final = F->final;
	faux->inicio = F->inicio;

	F->inicio = NULL;
	F->final = NULL;
	free(faux);
	free(F);

	return 1;
}

//8. ImprimirFila - imprime todos os elementos da fila sem alterar a fila
void imprimirFila(tfila F){

	if (vazia(F)){ // Erro: fila vazia
		printf(" Fila Vazia");
		return ;
	}
	else{
		printf(" %d",(F.inicio)->dado);
		F.inicio = (F.inicio)->prox;
		if(F.inicio != NULL)
			printf(" <- ");

		if(F.inicio == NULL) return;
		imprimirFila(F); //lista os restantes elementos
	}
}
