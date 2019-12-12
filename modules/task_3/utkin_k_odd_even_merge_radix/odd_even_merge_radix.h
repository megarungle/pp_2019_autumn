// Copyright 2019 Utkin Konstantin
#ifndef MODULES_TASK_3_UTKIN_K_ODD_EVEN_MERGE_RADIX_ODD_EVEN_MERGE_RADIX_H
#define MODULES_TASK_3_UTKIN_K_ODD_EVEN_MERGE_RADIX_ODD_EVEN_MERGE_RADIX_H

#include <vector>

std::vector<double> getRandVec(int size);

std::vector<double> sortingByCounting(std::vector<double> vec1,
    std::vector<double> vec2, int byte);
std::vector<double> radixSort(std::vector<double> vec);

#endif  // MODULES_TASK_3_UTKIN_K_ODD_EVEN_MERGE_RADIX_ODD_EVEN_MERGE_RADIX_H
