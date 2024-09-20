 
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FILHOS TAMANHO+(TAMANHO/2) 

#define FALSE 0
#define TRUE !FALSE
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))


int nodosInstanciados = 0;
int nodosAlocados = 0;

int camposAlocados = 0;

int jogando = 1;

int TAMANHO;
char jogador;
char inimigo;


typedef struct Node {
	struct Node * filhos;
	int numFilhos;
	int nivel;
	char * campo;
	char  acao[32];
	int heuristic;
	unsigned int derrotasDir;
	unsigned int vitoriasDir;
	//para o jogador da direita, a diferença (vitoriasDir-derrotasDir) deve ser maximizada.
	//o contrário ocorre para o jogador da direita.
} Node;







void copiaCampo (char * campo, char * clone) {
	int i;
	for (i=0;i<=TAMANHO;++i) {
		clone[i] = campo[i];
	}

}



void alocaFilhos(Node * n) {
	if(n->filhos == NULL) {
		
		nodosAlocados+=MAX_FILHOS;
		n->filhos = (Node *) malloc (sizeof(Node)*MAX_FILHOS);



	}
	
}



void initNode(Node * n, char * campo, char * acao,int nivel) {
	nodosInstanciados
	++;
	camposAlocados++;
	n->campo =(char *) malloc(sizeof(char)*TAMANHO); //inicializa string

	n->filhos = NULL;
	copiaCampo(campo,n->campo);
	n->numFilhos = 0;
	n->vitoriasDir = 0;
	n->derrotasDir = 0;
	n->nivel = nivel+1;
	strcpy(n->acao,acao);
	//printf("%s",n->campo );

}



int getBola(char * campo) {
	int i;
	for(i = 0; i <= TAMANHO; ++i){
		if(campo[i] == 'o') return i; //bola na posicao i
	}
	return -1; //bola nao encontrada
}



int moveBola(char * campo, char direcao, Node * node,int combo, char * pulosAnteriores){ //chuta a bola
	
		int bolaPos = getBola(campo);
		int i=1;
		int posicao;

		if(direcao == 'd') { //chutar para a direita
			while (campo[bolaPos+i] == 'f' && campo[bolaPos+i] != '.') {
				campo[bolaPos+i] = '.';
				++i;
			}
			campo[bolaPos+i] = 'o';
			campo[bolaPos] = '.';
			posicao = bolaPos+i;

			char pulo[10];
			pulo[0]='\0';
			sprintf(pulo, "%d ", posicao+1);

			strcat(pulosAnteriores,pulo);
			

		}

		else if(direcao == 'e') { //chutar para a esquerda
			while (campo[bolaPos-i] == 'f'&& campo[bolaPos-i] != '.' ) {
				campo[bolaPos-i] = '.';
				++i;
			}
			campo[bolaPos-i] = 'o';
			campo[bolaPos] = '.';
			posicao = bolaPos-i;

			char pulo[30];
			pulo[0]='\0';
			sprintf(pulo, "%d ", posicao+1);

			strcat(pulosAnteriores,pulo);

		}

			if (1!=i) { //houve movimento
		//		printf("%s",campo);
				char acao[32];
				acao[0] = 'o';
				acao[1] = ' ';
				sprintf(&acao[2], "%d ", combo);
		
				strcat(acao,pulosAnteriores);
				//printf(" %s\n",acao);
				initNode(&(node->filhos[node->numFilhos]),campo,acao,node->nivel);
				node->numFilhos++;
				return 1;
			}
			else {
				return 0;
			}
		
	
}


int novoFilosofo(char * campo, int posicao, Node * node){
	if(posicao < TAMANHO && posicao >= 0){ //in-bounds
		if(campo[posicao] == '.') {
			campo[posicao] = 'f';
			char acao[10];
			acao[0] = 'f';
			acao[1] = ' ';
			acao[2] = '\0';
			

			char posicaoV[4];
			sprintf(posicaoV, "%d", posicao+1);
			strcat(acao,posicaoV);

			initNode(&(node->filhos[node->numFilhos]),campo,acao,node->nivel);
			node->numFilhos++;
//			printf("%s %s\n",campo,acao);

			return 1;
		}
		
	}
	return 0;

}
void printNodeValues(Node * node){
	// mostra a soma do numero de vitorias e a soma do numero de derrotas.
		printf("%s %u - %u = %li \n",node->campo,node->vitoriasDir,node->derrotasDir,(long int)node->vitoriasDir-node->derrotasDir);
		printf("Direita %%: %4.2f Esquerda: %4.2f \n",(double)node->vitoriasDir/(node->vitoriasDir+node->derrotasDir),(double)node->derrotasDir/(node->vitoriasDir+node->derrotasDir));
}

int gameOver(Node *node){

	if(getBola(node->campo) == TAMANHO) { //VITORIA DA ESQUERDA
		++node->derrotasDir;
		return 1;
	}
	
	int i =0;
	while (node->acao[i]!='\0') {
		++i;
	}
//	printf("%c%c ",node->acao[i-3],node->acao[i-2]);
	if(node->acao[i-3] == '-' && node->acao[i-2] == '1') { //VITORIA DA DIREITA
		++node->vitoriasDir;
		return 1;
	}
	
	//printf("olar\n");
	return 0;
}

void freeChildren(Node * n, int flag) {

	if (n == NULL) return;
	int i;
	for(i = 0; i < n->numFilhos; ++i) {
		free(n->campo);
		camposAlocados--;



	}
	

	
	free(n->filhos);


}


void geraNodos(Node * node,int season) {

	if(gameOver(node)) {
	
		return;
	}

	if(!season){

		return;
	}

		int i = 0;
		alocaFilhos(node);
		do {
			char * novoCampo;
		
			novoCampo = (char *) malloc(sizeof(char)*TAMANHO+1);
			
			copiaCampo(node->campo,novoCampo);

			if (novoFilosofo(novoCampo,i,node)){
				geraNodos(&(node->filhos[node->numFilhos-1]),season-1); //gera campos filhos que criam filosofos
				node->vitoriasDir+= node->filhos[node->numFilhos-1].vitoriasDir;
				node->derrotasDir+= node->filhos[node->numFilhos-1].derrotasDir;
			} 
			 ++i;
		

			free(novoCampo);
		} while (i <= TAMANHO);

		//printf("\n");
		

		int movimento = 1;

		char tempCampo[TAMANHO+1];

		//tempCampo = (char *) malloc(sizeof(char)*TAMANHO+1);
		copiaCampo(node->campo,tempCampo);
		int combo = 1;

		char pulosAnteriores[30];
		pulosAnteriores[0] = '\0';
		while (movimento) { //chutes para a esquerda
				
			movimento = moveBola(tempCampo,'e',node,combo,pulosAnteriores); //modifica temp campo e adiciona ao nodo
			if (!movimento) break;
		    geraNodos(&(node->filhos[node->numFilhos-1]),season-1);
			node->vitoriasDir+= node->filhos[node->numFilhos-1].vitoriasDir;
			node->derrotasDir+= node->filhos[node->numFilhos-1].derrotasDir;	

		    char * novoCampo;
	

			novoCampo = (char *) malloc(sizeof(char)*TAMANHO+1);
			
			copiaCampo(tempCampo,novoCampo); //guarda o novo campo em um nodo
		
			free(novoCampo);
			++combo;

		}  //enquanto ouver movimento

		//printf("\n");
		copiaCampo(node->campo,tempCampo);
		combo = 1;
		movimento=1;
		pulosAnteriores[0] = '\0';

		while (movimento) { //chutes para a direita
				
			movimento = moveBola(tempCampo,'d',node,combo,pulosAnteriores); //modifica temp campo e adiciona ao nodo
			if (!movimento) break;
			geraNodos(&(node->filhos[node->numFilhos-1]),season-1);

			node->vitoriasDir+= node->filhos[node->numFilhos-1].vitoriasDir;
			node->derrotasDir+= node->filhos[node->numFilhos-1].derrotasDir;

		    char * novoCampo;
		  
			novoCampo = (char *) malloc(sizeof(char)*TAMANHO+1);
			

			copiaCampo(tempCampo,novoCampo); //guarda o novo campo em um nodo
		

			free(novoCampo);
			++combo;

		}  //enquanto ouver movimento
		

		
	//	free(tempCampo); 
		//esse FREE trava as coisas (?)



//	printf("%d fios\n",node->numFilhos);
//	printf("End season %d--------------------------\n",season);
	//}
	
		/*if(season < 1 ) {
			int i=0;
			for(i;i<node->numFilhos;++i) {
				freeChildren(&node->filhos[i],0);
			}
			printf("*********************************");
		}
		*/
		return;
}



void printNodo(Node *n) {
	if(gameOver(n)) {
		printf("%d: %s %sFIM\n", n->nivel,n->campo,n->acao);
		return;

	}

	printf("%d: %s %s \n", n->nivel,n->campo,n->acao);
	
}
void mostraNodos(Node * n) {
	if(n) printNodo(n);

	if(n->numFilhos==0) return;
	int i;
	for(i = 0;i<n->numFilhos;++i){
		mostraNodos(&n->filhos[i]);
	}
	return;

	
}

void expandTree (Node* node, int generations) {
	int seasons = 120/TAMANHO; //a partir de tamanho 61, não é possível abrir a árvore em largura.
	//printf("Seasons: %d\n",seasons );
	if(seasons < 1) seasons = 1; //para ver pelo menos um passo à frente.

	geraNodos(node,seasons); // gera filhos do nodo atual



	//mostraNodos(node);

	/*if(generations != 0) {
		int i;
		for (i=0;i <node->numFilhos;++i) {
			expandTree(&node->filhos[i],generations-1);
		}
	}*/

	return  ;
}


int acabou (char * campo) {
	int i;
	for (i = 0; i < TAMANHO; ++i)
	{
		if (campo[i] != '.') return 0;
	}

	return 0;
}

int heuristic (Node * node, int player) {
	if(player == TRUE) {
		if(jogador == 'd'){
			return node->vitoriasDir;
		}

		else {
			return node->derrotasDir;
		}
	}

	else {
		if(jogador == 'd'){
			return node->derrotasDir;

			
		}

		else {
			return node->vitoriasDir;
		}

	}
	
}
     
//(* Initial call for maximizing player *)
//minimax(origin, depth, TRUE)

 int minimax(Node *node, int depth,int maximizingPlayer) {
	if (depth = 0 || gameOver(node)) {
		return heuristic(node,maximizingPlayer);
	}
    
    float bestValue;    
    int v; //valor
	if (maximizingPlayer) {
 
        bestValue = -INFINITY;
        int i;
        for (i=0;i< node->numFilhos; ++i) {
        	v = minimax(&node->filhos[i],depth-1,FALSE);

            bestValue = MAX(bestValue, v);
        }   
        return bestValue;
	}
	
    else { //  (* minimizing player *) (enemy)

      	int i;
        bestValue = INFINITY;
        for (i=0;i< node->numFilhos; ++i) {


            v = minimax(&node->filhos[i], depth-1, TRUE);
            bestValue = MIN(bestValue, v);
        }
        return bestValue;
	}



 }

void freeTree(Node *node) {
	int i;
	for(i=0;i<node->numFilhos;++i) {
		freeTree(&node->filhos[i]);
	}
	
	node->numFilhos = 0;
	free(node->campo);
	free(node->filhos);
	return;
}
 void estrategia(Node* root, Node * move){

	expandTree(root,1);
	int i = 0;
	double percentDir ;
	double percentEsq ;
	double bDireita = 0;
	double bEsquerda = 0;
	Node * bestDireita = NULL;
	Node * bestEsquerda = NULL;
	for (i;i<root->numFilhos;++i){ //TODO REVIEW MIN MAX
	//	printf("=========================\n");
		//printNodeValues(&root->filhos[i]);


		percentDir =(double) root->filhos[i].vitoriasDir/(root->filhos[i].vitoriasDir+root->filhos[i].derrotasDir);
		percentEsq =(double) root->filhos[i].derrotasDir/(root->filhos[i].vitoriasDir+root->filhos[i].derrotasDir);

		bDireita= MAX(bDireita,percentDir);
		bEsquerda= MAX(bEsquerda,percentEsq);
		if(bDireita == percentDir) { //o valor mudou
		bestDireita = &root->filhos[i];
		}

		if(bEsquerda == percentEsq) { //o valor mudou
			bestEsquerda = &root->filhos[i];
		}
		//printf ("minimax:%s %d\n",root->filhos[i].campo,minimax(&root->filhos[i], depth, TRUE));
		//printf("=========================\n");
	}
	/*
	if(bestEsquerda) {
		printf("==E==================\n");
		printNodeValues(bestEsquerda);
		printf("=========================\n");
	}
	
	if(bestDireita) {
		printf("==D==================\n");
		printNodeValues(bestDireita);
		printf("=========================\n");
	}

	*/

	if(jogador == 'd') {
		
		if(bestDireita){
			printf("%c %s\n",jogador,bestDireita->acao);
			jogando = !gameOver(bestDireita);
		}

		else {
			int chute = rand()%root->numFilhos;
			printf("%c %s\n",jogador,root->filhos[chute].acao);
			jogando = !gameOver(&root->filhos[chute]);
		}
		
	}

	else if(jogador == 'e') {
		if(bestEsquerda){
			printf("%c %s\n",jogador,bestEsquerda->acao);
			jogando = !gameOver(bestEsquerda);
		}

		else {
			int chute = rand()%root->numFilhos;
			printf("%c %s\n",jogador,root->filhos[chute].acao);
			jogando = !gameOver(&root->filhos[chute]);
		}
		
	}

	freeTree(root);
	root->campo = (char *) malloc(sizeof(char)*TAMANHO+1);

	 //jogador, campo

	//nodosAlocados-=MAX_FILHOS;
	//freeChildren(&root);


	
}

void main() {

	srand(time(0)); 
	int fim = 0;
	char inimigo;
	char acao;
	int * vetAcoes;
	int vetAcoesTam = 0;

	Node * move;
	char novaAcao;
	Node * root =(Node *) malloc (sizeof(Node));
	root->numFilhos = 0;
	root->filhos = NULL;

	char * campo = (char *) malloc(sizeof(char)*TAMANHO+1) ;	
	scanf("%c %d %s %c %c", &jogador,&TAMANHO,campo,&inimigo,&acao);

	root->campo = (char *) malloc(sizeof(char)*TAMANHO+1);
	root->nivel = 0;
	root->acao[0] =jogador;
	root->acao[1] =' ';
	root->acao[2] =acao;
	root->vitoriasDir = 0;
	root->derrotasDir = 0;
	root->acao[3] ='\0';

	copiaCampo(campo,root->campo);

    for(;;) {
    //	printf("%d\n", getBola(campo));
    	if(getBola(campo)==-1) return; //perdeuuu	
		//if(jogador != inimigo){ //eh a minha vez
    						///////////////////////////////////////////
		estrategia(root,move); 
		if(jogando == 0) return; //venceu
		scanf("%c %d %s %c %c", &jogador,&TAMANHO,campo,&inimigo,&acao);
		


	 	root->filhos=NULL;
		root->nivel = 0;
		root->numFilhos = 0;
		root->vitoriasDir = 0;
		root->derrotasDir = 0;
		root->acao[0] =jogador;
		root->acao[1] =' ';
		root->acao[2] =acao;
		root->acao[3] ='\0';


		copiaCampo(campo,root->campo);

		//}

	

	

		
	}
	
	

}