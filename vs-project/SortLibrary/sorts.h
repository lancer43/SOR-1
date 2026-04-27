#ifndef SORTS_H
#define SORTS_H

#ifdef _WIN32
    #ifdef SORTLIBRARY_EXPORTS
	    #define SORT_API __declspec(dllexport)
    #else
	    #define SORT_API __declspec(dllimport)
#endif
#else
    #define SORT_API
#endif

// Тип указателя на функцию сортировки
typedef void (*sort_func_t)(int array[], int size);

// Семейства сортировок
typedef enum {
    SORT_NA = -1,
    SORT_SELECTION,
    SORT_BUBBLE,
    SORT_COMB
} sort_family_t;

// Вычислительная сложность
typedef enum {
    SORT_QUADRATIC,
    SORT_QUASILINEAR 
} sort_complexity_t;

// Описание одной сортировки
typedef struct {
    sort_func_t sort;
    sort_family_t family;
    sort_complexity_t complexity;
    char const* name;
} sort_info_t;

// Единственная экспортируемая функция
SORT_API sort_info_t* GetSortList(int* count);

#endif
