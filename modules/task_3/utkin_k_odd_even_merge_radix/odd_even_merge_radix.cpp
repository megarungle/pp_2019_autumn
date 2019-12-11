// Copyright 2019 Utkin Konstantin
#include <mpi.h>
#include <random>
#include <vector>
#include <ctime>
#include "../../../modules/task_3/utkin_k_odd_even_merge_radix/odd_even_merge_radix.h"

std::vector<double> getRandVec(int size) {
    std::mt19937 gen(time(0));
    std::uniform_real_distribution<> urd(0, 55);
    
    std::vector<double> vec(size);

    for (int i = 0; i < size; ++i) {
        vec[i] = urd(gen);
    }

    return vec;
}

std::vector<double> sortingByCounting(std::vector<double>& vec1,
    std::vector<double>& vec2, int byteNum) {
    if (vec1.size() != vec2.size()) {
        throw -1;
    }

    int size = vec1.size();

    // byte reading
    unsigned char* byteArr = (unsigned char*)vec1.data();

    int iOffset = 0; // offset for indices
    int counter[256] = { 0 };

    for (int i = 0; i < size; ++i) {
        counter[byteArr[8 * i + byteNum]]++;
    }

    int j = 0;
    while (j < 256) {
        if (counter[j] != 0)
            break;
        ++j;
    }

    iOffset = counter[j];
    counter[j] = 0;
    ++j;

    // calculate the correct indices
    while (j < 256) {
        int tmp = counter[j];
        counter[j] = iOffset;
        iOffset += tmp;
        ++j;
    }

    // sort by byteNum
    for (int i = 0; i < size; ++i) {
        vec2[counter[byteArr[8 * i + byteNum]]] = vec1[i];
        counter[byteArr[8 * i + byteNum]]++;
    }

    return vec2;
}

std::vector<double> radixSort(std::vector<double>& vec) {
    std::vector<double> tmp(vec.size());

    // sorting by each byte (least significant digit)
    tmp = sortingByCounting(vec, tmp, 0);
    vec = sortingByCounting(tmp, vec, 1);
    tmp = sortingByCounting(vec, tmp, 2);
    vec = sortingByCounting(tmp, vec, 3);
    tmp = sortingByCounting(vec, tmp, 4);
    vec = sortingByCounting(tmp, vec, 5);
    tmp = sortingByCounting(vec, tmp, 6);
    vec = sortingByCounting(tmp, vec, 7);

    // to sort an array with negative values,
    // the last byte must be sorted differently

    return vec;
}
