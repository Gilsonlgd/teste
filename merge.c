/*
    MERGESORT COM MULTI-THREAD

    i.  Integrantes do Grupo
        Eduardo Fritzzen, Gilson Garcia, Jhuan Luis

    ii. Referencias
        Código de mergesorting multi-thread em c++
        https://www.geeksforgeeks.org/merge-sort-using-multi-threading/

        Correção dos problemas do código (não ordenação correta de vetores de tam impar)
        por um usuário do StackOverflow
        Link encurtado: https://shre.ink/J74

    iii.O código utiliza o método mergesort com multi-threading para ordenar o vetor. Há
        na main de forma comentada um laço que preenche o vetor de tam TAM, com valores
        aleatórios para facilitar testagem. Para vetores pequenos recomenda-se a utilização
        de um numero baixo de threads, e para vetores significativamente grandes um numero 
        maior.
*/

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

// numero de elementos no vetor
#define TAM         10
// numero de threads
#define NUM_THREADS 4

// parametros de controle das threads
struct tsk {
    int* vet;
    int tsk_num;
    int tsk_low;
    int tsk_high;
};

// função merge para ordenar duas partes
void merge(int* vet, int low, int mid, int high)
{
    // n1 é o tamanho da parte esquerda e n2 é o tamanho da parte da direita
    int n1 = mid - low + 1;
    int n2 = high - mid;

    int* left = malloc(n1 * sizeof(int));
    int* right = malloc(n2 * sizeof(int));

    // armazenando valores na parte esquerda
    for (int i = 0; i < n1; i++)
        left[i] = vet[i + low];

    // armazenando valores na parte direita
    for (int i = 0; i < n2; i++)
        right[i] = vet[i + mid + 1];

    int k = low;
    int i = 0, j = 0;

    // ordenando parte esquerda e direita em ordem crescente
    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            vet[k++] = left[i++];
        else
            vet[k++] = right[j++];
    }

    // inserindo valores que sobraram na esquerda
    while (i < n1)
        vet[k++] = left[i++];

    // inserindo valores que sobraram na direita
    while (j < n2)
        vet[k++] = right[j++];

    // libera memória.
    free(left);
    free(right);
}

// função do merge
void merge_sort(int* vet, int low, int high)
{
    // calculando mid point do vetor
    int mid = low + (high - low) / 2;

    if (low < high) {
        // mandando primeira metade
        merge_sort(vet, low, mid);

        // mandando segunda metade
        merge_sort(vet, mid + 1, high);

        // mandando vetor inteiro
        merge(vet, low, mid, high);
    }
}

// função thread para o multi-threading
void* merge_sortMulti(void* arg)
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
        merge_sort(tsk->vet, low, mid);
        merge_sort(tsk->vet, mid + 1, high);
        merge(tsk->vet, low, mid, high);
    }

    return 0;
}

// ordena o vetor dado utilizando mergeSort em multi-threading
void ordena (int* vet, int tam)
{
    struct tsk *tsk;

    // iniciando as threads
    pthread_t threads[NUM_THREADS];
    struct tsk tsklist[NUM_THREADS];
    //tam de cada thread
    int len = tam / NUM_THREADS;
    // low = inicio de cada parte
    int low = 0;
    
    // definindo a divisão do vetor nas threads
    for (int i = 0; i < NUM_THREADS; i++, low += len) {
        tsk = &tsklist[i];
        tsk->tsk_num = i;

        tsk->tsk_low = low;
        tsk->tsk_high = low + len - 1;
        tsk->vet = vet;

        if (i == (NUM_THREADS - 1))
            tsk->tsk_high = tam - 1;
    }
    
    //criando as threads
    for (int i = 0; i < NUM_THREADS; i++) {
        tsk = &tsklist[i];
        pthread_create(&threads[i], NULL, merge_sortMulti, tsk);
    }

    // joining as threads
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);

    // ordenando as partes finais
    struct tsk *tskm = &tsklist[0];
    for (int i = 1; i < NUM_THREADS; i++) {
        struct tsk *tsk = &tsklist[i];
        merge(tskm->vet, tskm->tsk_low, tsk->tsk_low - 1, tsk->tsk_high);
    }
}
// imprime vetor
void mostra(int *vet, int tam){
    for (int i = 0; i< tam; i++){
        printf("%i, ", vet[i]);
    }
    printf("\n");
}

int main(int argc, char **argv)
{
    int vet[]= {1, 5, 7, 1, 4, 3, 1, 2, 5, 8};

    /* gerando vetor aleatório
    int vet[TAM];
    for (int i = 0; i < TAM; i++) { 
		vet[i] = rand() % 100;
    }*/

    struct timespec begin, end; 
    clock_gettime(CLOCK_REALTIME, &begin);

    ordena(vet, TAM);

    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;
 
    printf("Tempo: %.9f segundos.\n", elapsed);
    
    //mostra(vet, TAM);    
}