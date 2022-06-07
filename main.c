// CPP Program to implement merge sort using
// multi-threading
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

// numero de elementos no vetor
#define TAM 37
// numero de threads
#define NUM_THREADS 2

int part = 0;

// função merge para ordenar duas partes
void merge(int esq, int meio, int dir, int* vet)
{
  int *vtemp, p1, p2, tamanho;
  int fim1 = 0, fim2 = 0;
  tamanho = dir - esq + 1;
  p1 = esq;       // Índice do vetor dividido a ser comparado
  p2 = meio + 1 ; // Índice do outro vetor dividido a ser comparado
  vtemp = (int *) malloc(tamanho*sizeof(int)); // Vetor resultante da ordenação de p1 e p2

  if (vtemp != NULL) {
    for (int i = 0; i < tamanho; i++) {
      if (!fim1 && !fim2) {        
        if (vet[p1] < vet[p2]) { // Compara os vetores e vai combinando
          vtemp[i] = vet[p1++];    
        } else {
          vtemp[i] = vet[p2++];    
        }

        if (p1 > meio) fim1 = 1;  // Se p1 acabou

        if (p2 > dir) fim2 = 1;  // Se p2 acabou

      } else {
        if (!fim1) { // Coloca o que sobrou de p1
          vtemp[i] = vet[p1++];
        } else { // Coloca o que sobrou de p2
          vtemp[i] = vet[p2++];
        }
      }
    }

    for(int j = 0, k = esq; j < tamanho; j++, k++){ //Copia do vetor auxiliar e passa para o original
      vet[k] = vtemp[j];
    }
  }

  free(vtemp); //Libera o vetor auxiliar
}

/* a variável esq é para o índice esquerdo e dir é para o índice direito do sub-vetor de vet para ser ordenado */
void merge_sort(int esq, int dir, int* vet){
  int meio; 
    
  if (esq < dir){ // Se o início do vetor for menor que o final.(0 < 9)
    meio = (esq + dir) / 2; // meio do vetor 
    // Sort first and second halves
    merge_sort(esq, meio, vet); // Chama o merge_sort do ínicio do vetor até a metade.
    merge_sort(meio + 1, dir, vet); // Chama o merge_sort do meio do vetor até o fim.
    merge(esq, meio, dir, vet); // Combina as duas partes do vetor de forma ordenada.
  }
}

// thread function for multi-threading
void* merge_sort_multi(void* param)
{
  // convertendo void* -> int*
  int* vet = (int*)(param);
	// qual das duas partes
	int thread_part = part++;


	// calculating low and high
	int esq = thread_part * (TAM / 2);
	int dir = (thread_part + 1) * (TAM / 2) - 1;

  // calculando mid point
  int meio = esq + (dir - esq) / 2;

  if (esq < dir){           // Se o início do vetor for menor que o final.(0 < 9)
    meio = (esq + dir) / 2; // meio do vetor 

    // Ordena primeira e segunda metades
    merge_sort(esq, meio, vet);     // Chama o merge_sort do ínicio do vetor até a metade.
    merge_sort(meio + 1, dir, vet); // Chama o merge_sort do meio do vetor até o fim.
    merge(esq, meio, dir, vet);     // Combina as duas partes do vetor de forma ordenada
    
  }
}

void ordena(int* vet, int tam)
{
  //cria as threads
  pthread_t threads[NUM_THREADS];
  pthread_create(&threads[0], NULL, merge_sort_multi,(void*)vet);
  pthread_create(&threads[1], NULL, merge_sort_multi,(void*)vet);

  pthread_join(threads[0], NULL);// joining all 2 threads
  pthread_join(threads[1], NULL);

  // merging as 2 partes finais
  merge(0, (tam / 2 - 1) / 2, tam / 2 - 1,vet);
	merge(tam / 2, tam/2 + (tam-1-tam/2)/2, tam - 1, vet);
	merge(0, (tam - 1)/2, tam - 1, vet);
}
// imprime vetor
void mostra(int *vet, int tam){
 for (int i = 0; i < tam; i++){
  printf("%i ", vet[i]);
 }
 printf("\n");
}

int main()
{
  // array de tamanho MAX
  int vet[TAM];
  for (int i = 0; i < TAM; i++) {
		vet[i] = rand() % 100;
  }
  struct timespec begin, end; 
  //mostra(vet, TAM);
  clock_gettime(CLOCK_REALTIME, &begin);
  
  ordena(vet, TAM);
  
  clock_gettime(CLOCK_REALTIME, &end);
  long seconds = end.tv_sec - begin.tv_sec;
  long nanoseconds = end.tv_nsec - begin.tv_nsec;
  double elapsed = seconds + nanoseconds*1e-9;
  mostra(vet, TAM);

  printf("Tempo: %.9f segundos.\n", elapsed);

  return 0;
}


