#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>
#include <pthread.h>

#define TAM 1000000
#define NUM_THREADS 2

typedef struct {
    int* vet;
    int tam;
} vet;

int getMax(int arr[], int inicio, int fim) 
{
    int mx = arr[inicio];
    int i;
    for (i = inicio + 1; i <= fim; i++)
        if (arr[i] > mx)
            mx = arr[i];
    return mx;
}
 
void countSort(int arr[], int inicio, int fim, int exp) 
{
    int output[fim - inicio]; // output array
    int i, count[10] = { 0 };
 
    // Store count of occurrences in count[]
    for (i = inicio; i <= fim; i++)
        count[(arr[i] / exp) % 10]++;
 
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];
 
    // Build the output array
    for (i = fim; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }
 
    for (i = 0; i <= fim; i++)
        arr[i] = output[i];
}

void* routine1(void* param)
{
    vet* vetor = (vet*)(param);

    int* arr = vetor->vet;
    int n = vetor->tam;

    int inicio = 0;
    int fim = n/2 - 1;

    int m = getMax(arr, inicio, fim);

    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, inicio, fim, exp);
}

void* routine2(void* param)
{
    vet* vetor = (vet*)(param);

    int* arr = vetor->vet;
    int n = vetor->tam;

    int inicio = n/2;
    int fim = n - 1;

    int m = getMax(arr, inicio, fim);

    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, inicio, fim, exp);
}
 
// The main function to that sorts arr[] of size n using Radix Sort
void ordena(int* arr, int n) 
{
    pthread_t t1, t2;
    vet vetor;
    vetor.vet = arr; vetor.tam = n;

    pthread_create( &t1, NULL, routine1, (void*)(&vetor));
    pthread_create( &t2, NULL, routine2, (void*)(&vetor));

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}

void mostra(int *vet, int tam)
{
    for (int i=0; i<tam; i++){
    printf("%i, ", vet[i]);
    }
    printf("\n");
}

int main(){
    //int vet[]={1, 5, 7, 1, 4, 3, 1, 2, 5, 8};
    int vet[TAM];
    //generating random values in array
    for (int i = 0; i < TAM; i++)
        vet[i] = rand() % 100000;

    int n = sizeof(vet) / sizeof(vet[0]);
    
    struct timespec begin, end; 
    clock_gettime(CLOCK_REALTIME, &begin);

    ordena(vet, n);

    clock_gettime(CLOCK_REALTIME, &end);
    long seconds = end.tv_sec - begin.tv_sec;
    long nanoseconds = end.tv_nsec - begin.tv_nsec;
    double elapsed = seconds + nanoseconds*1e-9;

    printf("Tempo: %.9f segundos.\n", elapsed);
    //mostra(vet, TAM);
}

