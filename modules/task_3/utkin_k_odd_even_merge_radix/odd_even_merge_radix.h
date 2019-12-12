// Copyright 2019 Utkin Konstantin
#ifndef MODULES_TASK_3_UTKIN_K_ODD_EVEN_MERGE_RADIX_ODD_EVEN_MERGE_RADIX_H_
#define MODULES_TASK_3_UTKIN_K_ODD_EVEN_MERGE_RADIX_ODD_EVEN_MERGE_RADIX_H_

#include <vector>

typedef struct {
    int rank1;
    int rank2;
} pair;

std::vector<double> getRandVec(int size);

std::vector<double> sortingByCounting(std::vector<double> vec1,
    std::vector<double> vec2, int byte);
std::vector<double> radixSort(std::vector<double> vec);

void batcher(int countOfProc);
void buildNetwork(std::vector<int> prcsVec);
void buildConnection(std::vector<int> upPrcsVec,
    std::vector<int> downPrcsVec);

#endif  // MODULES_TASK_3_UTKIN_K_ODD_EVEN_MERGE_RADIX_ODD_EVEN_MERGE_RADIX_H_
