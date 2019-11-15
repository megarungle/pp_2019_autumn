// Copyright 2019 Utkin Konstantin
#include <mpi.h>
#include <random>
#include <vector>
#include "../../../modules/task_2/utkin_k_odd_even_sort/odd_even_sort.h"
// #include <string>
// #include <ctime>
// #include <algorithm>

std::vector<int> getRandVec(int size) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = gen() % 100;
    }
    return vec;
}

std::vector<int> seqSort(const std::vector<int>& vec) {
    std::vector<int> tmpVec(vec);
    int size = tmpVec.size();
    int tmp;
    for (int i = 0; i < size - 1; ++i) {
        tmp = -1;
        for (int j = 0; j < size - i - 1; ++j) {
            if (tmpVec[j] > tmpVec[j + 1]) {
                tmp = tmpVec[j];
                tmpVec[j] = tmpVec[j + 1];
                tmpVec[j + 1] = tmp;
            }
        }
        if (tmp == -1) {
            break;
        }
    }
    return tmpVec;
}

std::vector<int> compareSplitMin(int rank1, int rank2,
    const std::vector<int>& localVec, const int delta,
    const int remain) {
    if (rank1 == 0) {
        std::vector<int> receivedVec(delta);
        std::vector<int> tmpVec(delta + remain);
        std::vector<int> lVec(localVec);

        // Sendrecv (alternative)
        MPI_Send(&lVec[0], delta + remain, MPI_INT, rank2, 0,
            MPI_COMM_WORLD);
        MPI_Status status;
        MPI_Recv(&receivedVec[0], delta, MPI_INT, rank2, 0,
            MPI_COMM_WORLD, &status);

        int i1 = 0, i2 = 0, cur = 0;
        while ((i1 < delta + remain) && (i2 < delta)) {
            if (lVec[i1] < receivedVec[i2]) {
                tmpVec[cur++] = lVec[i1++];
            } else {
                tmpVec[cur++] = receivedVec[i2++];
            }
        }
        while (cur != delta + remain - 1) {
            tmpVec[cur++] = lVec[i1++];
        }

        return tmpVec;
    } else {
        std::vector<int> receivedVec(delta);
        std::vector<int> tmpVec(delta);
        std::vector<int> lVec(localVec);

        // Sendrecv (alternative)
        MPI_Send(&lVec[0], delta, MPI_INT, rank2, 0,
            MPI_COMM_WORLD);
        MPI_Status status;
        MPI_Recv(&receivedVec[0], delta, MPI_INT, rank2, 0,
            MPI_COMM_WORLD, &status);

        int i1 = 0, i2 = 0, cur = 0;
        while ((i1 < delta) && (i2 < delta) && (cur < delta)) {
            if (lVec[i1] < receivedVec[i2]) {
                tmpVec[cur++] = lVec[i1++];
            } else {
                tmpVec[cur++] = receivedVec[i2++];
            }
        }

        return tmpVec;
    }
}

std::vector<int> compareSplitMax(int rank1, int rank2,
    const std::vector<int>& localVec, const int delta,
    const int remain) {
    if (rank2 == 0) {
        std::vector<int> receivedVec(delta + remain);
        std::vector<int> tmpVec(delta);
        std::vector<int> lVec(localVec);

        // Sendrecv (alternative)
        MPI_Send(&lVec[0], delta, MPI_INT, rank1, 0,
            MPI_COMM_WORLD);
        MPI_Status status;
        MPI_Recv(&receivedVec[0], delta + remain, MPI_INT, rank1, 0,
            MPI_COMM_WORLD, &status);

        int i1 = 0, i2 = 0, cur = 0;
        while ((i1 < delta) && (i2 < delta + remain) && (cur < delta)) {
            if (lVec[i1] < receivedVec[i2]) {
                tmpVec[cur++] = lVec[i1++];
            } else {
                tmpVec[cur++] = receivedVec[i2++];
            }
        }

        return tmpVec;
    } else {
        std::vector<int> receivedVec(delta);
        std::vector<int> tmpVec(delta);
        std::vector<int> lVec(localVec);

        // Sendrecv (alternative)
        MPI_Send(&lVec[0], delta, MPI_INT, rank1, 0,
            MPI_COMM_WORLD);
        MPI_Status status;
        MPI_Recv(&receivedVec[0], delta, MPI_INT, rank1, 0,
            MPI_COMM_WORLD, &status);

        int i1 = 0, i2 = 0, cur = 0;
        while ((i1 < delta) && (i2 < delta) && (cur < delta)) {
            if (lVec[i1] < receivedVec[i2]) {
                tmpVec[cur++] = lVec[i1++];
            } else {
                tmpVec[cur++] = receivedVec[i2++];
            }
        }

        return tmpVec;
    }
}

std::vector<int> parSort(const std::vector<int>& globalVec) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::vector<int> resVec(globalVec);
    const int delta = resVec.size() / size;
    const int remain = resVec.size() % size;

    std::vector<int> localVec(delta);

    MPI_Scatter(&resVec[remain], delta, MPI_INT,
        &localVec[0], delta, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        localVec.resize(delta + remain);
        for (int ir = 0; ir < remain; ++ir) {
            localVec.push_back(resVec[delta * size + ir]);
        }
    }

    localVec = seqSort(localVec);

    for (int i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            if (rank % 2 == 0) {
                if (rank + 1 < size) {
                    localVec = compareSplitMin(rank,
                        rank + 1, localVec, delta, remain);
                }
            } else {
                localVec = compareSplitMax(rank,
                    rank - 1, localVec, delta, remain);
            }
        } else {
            if (rank % 2 == 0) {
                if (rank > 0) {
                    localVec = compareSplitMax(rank,
                        rank - 1, localVec, delta, remain);
                }
            } else {
                if (rank + 1 < size) {
                    localVec = compareSplitMin(rank,
                        rank + 1, localVec, delta, remain);
                }
            }
        }
    }
    if (rank != 0) {
        MPI_Send(&localVec[0], delta, MPI_INT, 0, 0,
            MPI_COMM_WORLD);
    } else {
        resVec = localVec;
        std::vector<int> tmpVec(delta);
        MPI_Status status;
        for (int proc = 1; proc < size; ++i) {
            MPI_Recv(&tmpVec[0], delta, MPI_INT, proc, 0,
                MPI_COMM_WORLD, &status);
            resVec.insert(resVec.end(), tmpVec.begin(),
                tmpVec.end());
        }
    }

    return resVec;
}
