#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "sorts.h"

#define MIN_SIZE 50
#define MAX_SIZE 2000
#define STEP_SIZE 50

#define REPEAT_COUNT 10

// вспомогательная функция сравнения для qsort
int compare(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

double time_sort(sort_func_t sort, int* arr, int size, LARGE_INTEGER freq) {
    int* copy = (int*)malloc(size * sizeof(int));
    memcpy(copy, arr, size * sizeof(int));

    LARGE_INTEGER start, end;
    QueryPerformanceCounter(&start);
    sort(copy, size);
    QueryPerformanceCounter(&end);

    double time_ms = (double)(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
    free(copy);
    return time_ms;
}

int main(void) {
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

    // подгружаем библиотеку
    HMODULE dll = LoadLibrary(L"SortLibrary.dll");
    if (dll == NULL) {
        printf("ERROR: Cannot load SortLibrary.dll\n");
        printf("Error code: %lu\n", GetLastError());
        return 1;
    }
    printf("DLL loaded successfully\n");

    // получаем адрес функции GetSortList
    typedef sort_info_t* (*GetSortList_t)(int*);
    GetSortList_t GetSortList = (GetSortList_t)GetProcAddress(dll, "GetSortList");

    if (GetSortList == NULL) {
        printf("ERROR: Cannot find GetSortList function\n");
        FreeLibrary(dll);
        return 1;
    }
    printf("GetSortList found\n");

    // получаем список сортировок и их количество
    int sort_count;
    sort_info_t* sorts = GetSortList(&sort_count);

    // интерфейс со списком наименований сортировок
    printf("\nLoaded %d sorting algorithms:\n", sort_count);
    for (int i = 0; i < sort_count; i++) {
        printf("%d. %s\n", i + 1, sorts[i].name);
    }

    // открываем файл .csv
    FILE* csv = fopen("results.csv", "w");
    if (csv == NULL) {
        printf("error: cannot create results.csv\n");
        FreeLibrary(dll);
        return 1;
    }

    // заголовок .csv
    fprintf(csv, "Size");
    for (int i = 0; i < sort_count; i++) {
        fprintf(csv, ";%s", sorts[i].name);
    }
    fprintf(csv, "\n");

    // основная работа программы
    
    srand(52); // просто семечко, его можно менять

    // будем сначала задавать размер массива, 
    // а затем увеличивать его с заданным шагом
    for (int size = MIN_SIZE; size <= MAX_SIZE;
        size += STEP_SIZE) {

        printf("\ntesting size: %d...\n", size);

        // массив для накопления суммарного времени по каждой сортировке
        double* total_times = (double*)calloc(sort_count, sizeof(double));

        for (int i = 0; i < REPEAT_COUNT; i++) {
            // создаем массив с рандомным заполнением
            int* arr = (int*)malloc(size * sizeof(int));
            if (arr == NULL) {
                printf("memory allocation failed!\n");
                fclose(csv);
                FreeLibrary(dll);
                return 1;
            }

            for (int j = 0; j < size; j++) {
                arr[j] = rand() % INT_MAX;
            }
            
            // получаем эталон сортировки через qsort
            int* ref = (int*)malloc(size * sizeof(int));
            memcpy(ref, arr, size * sizeof(int));
            qsort(ref, size, sizeof(int), compare);

            // тест сортировок
            for (int s = 0; s < sort_count; s++) {
                // замер времени
                double time_ms = time_sort(sorts[s].sort, arr, size, freq);
                total_times[s] += time_ms;

                // сверка с эталоном
                int* copy = (int*)malloc(size * sizeof(int));
                memcpy(copy, arr, size * sizeof(int));
                sorts[s].sort(copy, size);
                if (memcmp(copy, ref, size * sizeof(int)) != 0) {
                    printf("error: %s failed on repeat %d, size %d\n",
                        sorts[s].name, i, size);
                }
                free(copy);
            }
            free(arr);
            free(ref);
        }

        // записываем среднее время в csv для статистики
        fprintf(csv, "%d", size);
        for (int s = 0; s < sort_count; s++) {
            double avg_time = total_times[s] / REPEAT_COUNT;
            fprintf(csv, ";%.3f", avg_time);
            printf("   %s: %.3f ms (avg)\n", sorts[s].name, avg_time);
        }
        fprintf(csv, "\n");

        free(total_times);
    }

    fclose(csv);
    


    // освобождаем библиотеку
    FreeLibrary(dll);
    
    // культурно завершаем работу программы
    printf("\nResults saved to results.csv\n");
    printf("\nThe sorting analysis was successful :)");

	return 0;
}