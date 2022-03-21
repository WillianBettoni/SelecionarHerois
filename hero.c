#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<conio.h>   // bilbioteca para mudar a cor da fonte

typedef struct HERO{
	char name[40];
	int power;	
	struct HERO *inferior;
}  HERO;

char nome[40];
int poder; 

FILE *fp;	
	char s[] = "arquivo_teste.txt";
	
int getTamanho(HERO *ptime, int start) {
	if (ptime == NULL)
		return start;
	return getTamanho(ptime->inferior, (start+1));
}	

HERO *getPorPosicao(HERO *ptime, int pos, int atual) {
	if ((atual >= pos)||(ptime == NULL))
		return ptime;
	return getPorPosicao(ptime->inferior, pos, (atual+1));
}

void troca (int *p, int *q)
{
   int temp;
   temp = p; p = q; q = temp;
}

HERO* ordenarPorPoder(HERO *ptime) {
	//booble sort
	int size = getTamanho(ptime,0);
	printf("size %d ", size);
	int i;
	int j;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			HERO *aux = getPorPosicao(ptime, j,0);
			if ((aux==NULL)||(aux->inferior==NULL))
				continue;
			if ((aux->power) < (aux->inferior->power)){
							
				HERO *aux1 = NULL;
				HERO *aux2 = NULL;
				HERO *aux3 = NULL;
				HERO *aux4 = NULL;
				
				//caputar item de menor valor a ser trocado
				aux2 = aux;
				//captura item de maior valor a ser trocado
				if (aux!=NULL)
					aux3 = aux->inferior;
				//captura item de  anteior ao de menor valor a ser trocado
				if (j!=0)
					aux1 = getPorPosicao(ptime, (j-1), 0);
				//catura o item posterior ao de maior valor a ser trocado
				if (aux3!=NULL)
					aux4 = aux3->inferior;	
				if (aux1!=NULL)
					aux1->inferior = aux3;
				if (aux2!=NULL)
					aux2->inferior = aux4;
				if (aux3 != NULL)
					aux3->inferior = aux2;
				
				if (j==0)
					ptime = aux3;																				 
			}		
		}
	}
	return ptime;
}

HERO *localizarUltimo(HERO *ptime) {
	HERO *aux = ptime;
	if (aux->inferior == NULL)
		return aux;
	return localizarUltimo(aux->inferior);
}

HERO *incluir(HERO *ptime, char *hname, int hpower){
	HERO *newHero = malloc(sizeof(HERO));
	
	strcpy(newHero->name, hname);       //copia dados a serem inseridos 
	newHero->power = hpower;             // copia dados a serem inseridos 
	newHero->inferior = NULL;			
	if (ptime == NULL) {
		return newHero;
	}
	HERO *aux = localizarUltimo(ptime);	
	aux->inferior = newHero;
	return ordenarPorPoder(ptime);
}

HERO* localizar(char nome[40], HERO* pteam) {
	HERO *aux = pteam;
	if (aux==NULL) {
		printf("nome nao encontrado \n");
		return NULL;
	}
	if (strcmp(aux->name, nome)==0)
		return aux;
	return localizar(nome, aux->inferior);
}

HERO* localizarAnterior(char nome[40], HERO* pteam) {
	HERO *aux = pteam;
	if (aux->inferior==NULL) {
		return NULL;
	}
	HERO *inferior = aux->inferior;
	if (!strcmp(inferior->name, nome))
		return aux;
	return localizarAnterior(nome, aux->inferior);
}

HERO *excluir(HERO *ptime){  //exclui primeiro da fila
	HERO *aux = ptime;
	
	if(aux==NULL)	
		return;
	char nome[40];
	puts("entre com o nome do heroi a ser excluido: ");
	scanf("%s", nome);
	HERO *aExcluir = localizar(nome, aux);
	if (aExcluir==NULL)
		return ptime;
	HERO *aAntExcluir = localizarAnterior(nome, aux);
	if (aAntExcluir==NULL) {
		HERO* retorno = aExcluir->inferior;
		free(aExcluir);
		return retorno;
	}
	aAntExcluir->inferior = aExcluir->inferior;
	free(aExcluir);
	return ptime;
}

void alterar(HERO *ptime){   
	HERO *aux = ptime;
	
	char nome[40];
	int novopoder;	
			
	puts("Entre com o nome do heroi que deseja alterar");
	scanf("%s", nome);
	HERO *aAlterar = localizar(nome, aux);
	if (aAlterar==NULL)
		return;
	puts("entre com o novo poder do heroi: ");
	scanf("%d", &novopoder);	
	aAlterar->power=novopoder;            
}
	
void mostrar(HERO *ptime){
	HERO *aux = ptime;
	if(aux==NULL){
		puts("fim da lista");
		return;
	} else{
		printf("\n\nnome: %s \npoder: %d \n\n", aux->name, aux->power);
		mostrar(aux->inferior);
	} 
}

HERO *recuperar(){
	HERO *retorno = NULL;
	HERO *aux = NULL;
	int count = 0;
	while (!feof(fp)) {
		HERO *novo = malloc(sizeof(HERO));
		fread(novo, sizeof(HERO), 1, fp);
		if (retorno == NULL) 
			retorno = novo;
		else
			aux->inferior = novo;
		aux = novo;
		count ++;
	}
	HERO *bug = getPorPosicao(retorno, (count-1), 0);
	if (bug!=NULL)
		free(bug);
	HERO *antBug = getPorPosicao(retorno, (count-2), 0);
	if (antBug!=NULL)
		antBug->inferior = NULL;
	return retorno;
}

void salvar(HERO *ptime){
	if (ptime == NULL) {
		printf("Salvo!");	
		return;
	}
	fwrite(ptime, sizeof(HERO), 1, fp);
	salvar(ptime->inferior);
}

main(){
	
	system ("color 0B"); // cor de fundo preta e letras azuis brilhantes 
	HERO* time = NULL;
	int op;

	do{
		printf("\n\nOla, escolha uma opcao: \n");
		printf("opcao 1 - incluir heroi \n");
		printf("opcao 2 - excluir heroi \n");
		printf("opcao 3 - alterar heroi \n");
		printf("opcao 4 - mostrar herois \n");
		printf("opcao 5 - recuperar do arquivo \n");
		printf("opcao 6 - salvar no arquivo \n");
		printf("opcao 0 - sair \n\n");
	
		scanf("%d", &op);
			
		switch (op){
			
			case 1:
				puts("\n\ninsira o nome do super heroi: ");
				scanf("%s", nome);
				puts("\n\ninsira a quantia de poder do super heroi: ");
				scanf("%d", &poder);
				time = incluir(time, nome, poder);
				break;
			
			case 2:	 
				time = excluir(time);	
				break;
		
			case 3: 
			    alterar(time);	
			    time = ordenarPorPoder(time);
			break;
		
			case 4:
				mostrar(time);	
			break;
			
			case 5:
				fp = fopen(s,"rb"); 
	
				if(fp==NULL)
					printf("Impossivel abrir arquivo %s\n",s);
				else{
					printf("Arquivo %s aberto com sucesso, a ler... \n",s);
				}
				time = recuperar();	 
				fclose(fp);
			break;
									
			case 6:
					fp = fopen(s,"wb");          //modo escrita
					// Verifica se a abertura ocorreu com sucesso
					if(fp==NULL)
						printf("Impossivel abrir arquivo %s\n",s);
					else{
						printf("Arquivo %s aberto com sucesso, a salvar... \n",s);
					}
					salvar(time);
					fclose(fp);
			break;
			
			case 0:
				exit;				 		
			break;
		
		default: printf("\nopcao invalida\n");
		}
	
	}while(op != 0);
	
}
