#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include "sorts.h"

/*
	selection_shift
	selection_swap
	selection_square

	bubble_classic
	bubble_early_exit
	bubble_swap_check
	cocktail_shaker
	comb_sort_factor_2
	comb_sort_factor_1_3
*/

// вспомогательна€ функци€ дл€ обмена
static void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// —ќ–“»–ќ¬ ј ¬џЅќ–ќћ

static void selection_shift(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_index = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_index]) min_index = j;
        }

        if (min_index != i) {
            int min = arr[min_index];

            for (int j = min_index; j > i; j--) {
                arr[j] = arr[j - 1];
            }

            arr[i] = min;
        }
    }
}

static void selection_swap(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_index = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_index]) min_index = j;
        }

        if (min_index != i) {
            swap(&arr[i], &arr[min_index]);
        }
    }
}

static void selection_square(int arr[], int n) {
    int block_size = (int)sqrt(n);
    if (block_size < 1) block_size = 1;

    int* used = (int*)calloc(n, sizeof(int));

    for (int pos = 0; pos < n; pos++) {
        int best_val = INT_MAX;
        int best_idx = -1;

        // проходим по всем блокам
        for (int block = 0; block * block_size < n; block++) {
            int start = block * block_size;
            int end = start + block_size;
            if (end > n) end = n;

            // ищем минимум в блоке среди неиспользованных
            int block_best_val = INT_MAX;
            int block_best_idx = -1;

            for (int i = start; i < end; i++) {
                if (!used[i] && arr[i] < block_best_val) {
                    block_best_val = arr[i];
                    block_best_idx = i;
                }
            }

            // сравниваем с глобальным минимумом
            if (block_best_idx != -1 && block_best_val < best_val) {
                best_val = block_best_val;
                best_idx = block_best_idx;
            }
        }

        // ставим найденный минимум на позицию pos
        if (best_idx != pos) {
            swap(&arr[pos], &arr[best_idx]);
            used[pos] = 1;
        }
        else {
            used[best_idx] = 1;
        }
    }

    free(used);
}

// —ќ–“»–ќ¬ ј ѕ”«џ–№ ќћ

static void bubble_classic(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - 1; j++) {
            if (arr[j] > arr[j + 1]) swap(&arr[j], &arr[j + 1]);
        }
    }
}

static void bubble_early_exit(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) swap(&arr[j], &arr[j + 1]);
        }
    }
}

static void bubble_swap_check(int arr[], int n) {
    
    for (int i = 0; i < n; i++) {
        bool flag = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                flag = 1;
            }
        }

        if (flag) continue;
        else break;
    }
}

static void cocktail_shaker(int arr[], int n) {
    int left = 0, right = n - 1;

    while (left < right) {
        for (int j = left; j < right; j++) {
            if (arr[j] > arr[j + 1]) swap(&arr[j], &arr[j + 1]);
        }

        right--;
        for (int j = right; j > left; j--) {
            if (arr[j] < arr[j - 1]) swap(&arr[j], &arr[j - 1]);
        }
        left++;
    }

}

static void comb_sort_factor_2(int arr[], int n) {
    int gap = n, factor = 2;
    bool swapped;
    
    do {
        gap /= factor;

        if (gap < 1) gap = 1;

        swapped = 0;
        for (int i = 0; i < n - gap; i++) {
            if (arr[i] > arr[i + gap]) {
                swap(&arr[i], &arr[i + gap]);
                swapped = 1;
            }
        }

    } while (gap > 1 || swapped);
}

static void comb_sort_factor_1_3(int arr[], int n) {
    int gap = n;
    float factor = 1.3;

    bool swapped;

    do {
        gap = (int)(gap / factor);

        if (gap < 1) gap = 1;

        swapped = 0;
        for (int i = 0; i < n - gap; i++) {
            if (arr[i] > arr[i + gap]) {
                swap(&arr[i], &arr[i + gap]);
                swapped = 1;
            }
        }

    } while (gap > 1 || swapped);
}

// »Ќ“≈–‘≈…— —ќ–“»–ќ¬ќ 

__declspec(dllexport) sort_info_t* GetSortList(int* count) {
	static sort_info_t list[9];
	*count = 9;

    // выбор со сдвигом
	list[0].sort = selection_shift;
	list[0].family = SORT_SELECTION;
	list[0].complexity = SORT_QUADRATIC;
	list[0].name = "Selection sort (shift, stable)";

    // выбор с обменом
    list[1].sort = selection_swap;
    list[1].family = SORT_SELECTION;
    list[1].complexity = SORT_QUADRATIC;
    list[1].name = "Selection sort (swap, unstable)";

    // квадратичный выбор
    list[2].sort = selection_square;
    list[2].family = SORT_SELECTION;
    list[2].complexity = SORT_QUADRATIC;
    list[2].name = "Selection sort (square root blocks)";

    // пузырЄк классический
    list[3].sort = bubble_classic;
    list[3].family = SORT_BUBBLE;
    list[3].complexity = SORT_QUADRATIC;
    list[3].name = "Bubble sort (classic)";

    // пузырЄк с остановкой на 1 раньше
    list[4].sort = bubble_early_exit;
    list[4].family = SORT_BUBBLE;
    list[4].complexity = SORT_QUADRATIC;
    list[4].name = "Bubble sort (early exit)";

    // пузырЄк с проверкой обменов
    list[5].sort = bubble_swap_check;
    list[5].family = SORT_BUBBLE;
    list[5].complexity = SORT_QUADRATIC;
    list[5].name = "Bubble sort (swap check)";

    // шейкер
    list[6].sort = cocktail_shaker;
    list[6].family = SORT_BUBBLE;
    list[6].complexity = SORT_QUADRATIC;
    list[6].name = "Cocktail shaker sort";

    // расчЄска фактор 2
    list[7].sort = comb_sort_factor_2;
    list[7].family = SORT_COMB;
    list[7].complexity = SORT_QUASILINEAR;
    list[7].name = "Comb sort (factor 2)";

    // расчЄска фактор 1.3
    list[8].sort = comb_sort_factor_1_3;
    list[8].family = SORT_COMB;
    list[8].complexity = SORT_QUASILINEAR;
    list[8].name = "Comb sort (factor 1.3)";

	return list;
}