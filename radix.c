#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>


#define TAM 1000000
#define NUM_THREADS 2

int getMax(int arr[], int n) {
    int mx = arr[0];
    int i;
    for (i = 1; i < n; i++)
        if (arr[i] > mx)
            mx = arr[i];
    return mx;
}
 
void countSort(int arr[], int n, int exp) {
    int output[n]; // output array
    int i, count[10] = { 0 };
 
    // Store count of occurrences in count[]
    for (i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;
 
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];
 
    // Build the output array
    for (i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }
 
    for (i = 0; i < n; i++)
        arr[i] = output[i];
}
 
// The main function to that sorts arr[] of size n using Radix Sort
void ordena(int* arr, int n) {
    int m = getMax(arr, n);
 
    int exp;
    for (exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, n, exp);
}

void mostra(int *vet, int tam){
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

