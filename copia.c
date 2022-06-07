#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

// numero de elementos no vetor
#define TAM       1000000
// numero de threads
#define NUM_THREADS 4

// array of size TAM
int *a;

// parametros de controle das threads
struct tsk {
    int tsk_num;
    int tsk_low;
    int tsk_high;
};

// função merge para ordenar duas partes
void merge(int low, int mid, int high)
{
    // n1 é o tamanho da parte esquerda e n2 é o tamanho da parte da direita
    int n1 = mid - low + 1;
    int n2 = high - mid;

    int *left = malloc(n1 * sizeof(int));
    int *right = malloc(n2 * sizeof(int));


    // armazenando valores na parte esquerda
    for (int i = 0; i < n1; i++)
        left[i] = a[i + low];

    // armazenando valores na parte direita
    for (int i = 0; i < n2; i++)
        right[i] = a[i + mid + 1];

    int k = low;
    int i = 0, j = 0;

    // ordenando parte esquerda e direita em ordem crescente
    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            a[k++] = left[i++];
        else
            a[k++] = right[j++];
    }

    // inserindo valores que sobraram na esquerda
    while (i < n1)
        a[k++] = left[i++];

    // inserindo valores que sobraram na direita
    while (j < n2)
        a[k++] = right[j++];

    // libera memória.
    free(left);
    free(right);
}

// função do merge
void merge_sort(int low, int high)
{
    // calculando mid point do vetor
    int mid = low + (high - low) / 2;

    if (low < high) {
        // mandando primeira metade
        merge_sort(low, mid);

        // mandando segunda metade
        merge_sort(mid + 1, high);

        // mandando vetor inteiro
        merge(low, mid, high);
    }
}

// função thread para o multi-threading
void * merge_sort123(void *arg)
{
    struct tsk *tsk = arg;
    int low;
    int high;

    // calculando low e high
    low = tsk->tsk_low;
    high = tsk->tsk_high;

    // definindo o mid point
    int mid = low + (high - low) / 2;

    if (low < high) {
        merge_sort(low, mid);
        merge_sort(mid + 1, high);
        merge(low, mid, high);
    }

    return 0;
}

// imprime vetor
void mostra(int *vet, int tam){
    for (int i=0; i<tam; i++){
        printf("%i, ", vet[i]);
    }
    printf("\n");
}

// Driver Code
int main(int argc, char **argv)
{
    struct tsk *tsk;
    // aloca o vetor
    a = malloc(sizeof(int) * TAM);
    // gerando vetor aleatório
    for (int i = 0; i < TAM; i++) { 
		a[i] = rand() % 100;
    }

    struct timespec begin, end; 
    clock_gettime(CLOCK_REALTIME, &begin);
    // iniciando as threads
    pthread_t threads[NUM_THREADS];
    struct tsk tsklist[NUM_THREADS];
    //tam de cada thread
    int len = TAM / NUM_THREADS;
    // low = inicio de cada parte
    int low = 0;
    
    // definindo a divisão do vetor nas threads
    for (int i = 0; i < NUM_THREADS; i++, low += len) {
        tsk = &tsklist[i];
        tsk->tsk_num = i;

        tsk->tsk_low = low;
        tsk->tsk_high = low + len - 1;

        if (i == (NUM_THREADS - 1))
            tsk->tsk_high = TAM - 1;
    }

    // criando Numero de threads definido
    for (int i = 0; i < NUM_THREADS; i++) {
        tsk = &tsklist[i];
        pthread_create(&threads[i], NULL, merge_sort123, tsk);
    }

    // joining as threads
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    // ordenando as partes finais
    struct tsk *tskm = &tsklist[0];
    for (int i = 1; i < NUM_THREADS; i++) {
        struct tsk *tsk = &tsklist[i];
        merge(tskm->tsk_low, tsk->tsk_low - 1, tsk->tsk_high);
    }

    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;
 
    printf("Tempo: %.9f segundos.\n", elapsed);
    
    //mostra(a, TAM);    
}