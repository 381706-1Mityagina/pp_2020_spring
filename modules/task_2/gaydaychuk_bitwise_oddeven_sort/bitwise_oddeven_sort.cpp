// Copyright 2020 Gaydaychuk Yury
#include <omp.h>
#include <vector>
#include <list>
#include <string>
#include <ctime>
#include <iostream>
#include <algorithm>
#include "../../../modules/task_2/gaydaychuk_bitwise_oddeven_sort/bitwise_oddeven_sort.h"

#define MINIMAL_SINGLE_ARRAY_LENGTH 2

void NetworkBuilder::setNetworkSize(int size){
        this->size = size;
        twoDegree = 1;
        while(twoDegree < size){
            twoDegree *= 2;
        }
        indexArray.clear();
        for(int i = 1; i <= twoDegree; i++){
            indexArray.push_back(i);
        }
        auxiliaryNodeSetArray.clear();
        parallelBlockArray.clear();
    }
void NetworkBuilder::computeNetwork(){
    pseudoSort_2(0, (int)indexArray.size());
}

void NetworkBuilder::pseudoSort_2(int lo, int n)
{
    if (n>1)
    {
        int m=n/2;
        pseudoSort_2(lo, m);
        pseudoSort_2(lo+m, m);
        bMerge_2(lo, n, 1);
    }
}

void NetworkBuilder::bMerge_2(int lo, int n, int r)
{
    // lo is the starting position and
    // n is the length of the piece to be merged,
    // r is the distance of the elements to be compared
    int m=r*2;
    if (m<n)
    {
        bMerge_2(lo, n, m);      // even subsequence
        bMerge_2(lo+r, n, m);    // odd subsequence
        for (int i=lo+r; i+r<lo+n; i+=m){
            comparatorArray.push_back(std::pair<int, int>(indexArray[i], indexArray[i + r]));
            addComparatorAnother(indexArray[i], indexArray[i + r]);
        }
    }
    else{
        addComparatorAnother(indexArray[lo], indexArray[lo + r]);
        comparatorArray.push_back(std::pair<int, int>(indexArray[lo], indexArray[lo + r]));
    }
}

void NetworkBuilder::addComparator(int i, int j){
    if(auxiliaryNodeSetArray.empty()){
        std::vector<int> newBlockNodeSet;

        newBlockNodeSet.push_back(i);
        newBlockNodeSet.push_back(j);
        auxiliaryNodeSetArray.push_back(newBlockNodeSet);

        std::vector<std::pair <int, int> > newBlock;
        newBlock.push_back(std::pair<int, int>(i, j));
        parallelBlockArray.push_back(newBlock);
    }
    else{
        bool pairAndSetIntersectionFound = false;
        std::list<std::vector<std::pair <int, int> > >::iterator parallelBlocksIterator = parallelBlockArray.begin();
        for(
            std::list<std::vector<int > >::iterator itBlocks = auxiliaryNodeSetArray.begin(); 
            itBlocks != auxiliaryNodeSetArray.end(); 
            ++itBlocks) {
            pairAndSetIntersectionFound = false;
            auto currBlock = *itBlocks;
            for(std::vector<int> ::iterator itInBlock = currBlock.begin(); itInBlock != currBlock.end(); ++itInBlock){
                if(i == *itInBlock || j == *itInBlock){
                    pairAndSetIntersectionFound = true;
                    break;
                }
            };
            if(pairAndSetIntersectionFound == true){
                  if((itBlocks != auxiliaryNodeSetArray.end()) && (next(itBlocks) == auxiliaryNodeSetArray.end())){
                    auxiliaryNodeSetArray.push_back({i, j});
                    parallelBlockArray.push_back(std::vector<std::pair <int, int> >({std::pair <int, int>(i, j)}));
                    break;
                }
                else{
                    continue;
                }
            }
            else{
                parallelBlocksIterator->push_back(std::pair<int, int>(i, j));
                itBlocks->push_back(i);
                itBlocks->push_back(j);
                break;
            }
            ++parallelBlocksIterator;
        };
    }
}

void NetworkBuilder::addComparatorAnother(int i, int j){
    if(auxiliaryNodeSetArray.empty()){
        std::vector<int> newBlockNodeSet;

        newBlockNodeSet.push_back(i);
        newBlockNodeSet.push_back(j);
        auxiliaryNodeSetArray.push_back(newBlockNodeSet);

        std::vector<std::pair <int, int> > newBlock;
        newBlock.push_back(std::pair<int, int>(i, j));
        parallelBlockArray.push_back(newBlock);
    }
    else{
        bool pairAndSetIntersectionFound = false;
        std::list<std::vector<std::pair <int, int> > >::reverse_iterator  parallelBlocksIterator = parallelBlockArray.rbegin();
        for(
            std::list<std::vector<int > >::reverse_iterator  itBlocks = auxiliaryNodeSetArray.rbegin(); 
            itBlocks != auxiliaryNodeSetArray.rend(); 
            ++itBlocks) {
            pairAndSetIntersectionFound = false;
            auto currBlock = *itBlocks;
            for(std::vector<int> ::iterator itInBlock = currBlock.begin(); itInBlock != currBlock.end(); ++itInBlock){
                if(i == *itInBlock || j == *itInBlock){
                    pairAndSetIntersectionFound = true;
                    break;
                }
            };
            if(pairAndSetIntersectionFound == true){
                if(itBlocks == auxiliaryNodeSetArray.rbegin()){
                    auxiliaryNodeSetArray.push_back({i, j});
                    parallelBlockArray.push_back(std::vector<std::pair <int, int> >({std::pair <int, int>(i, j)}));
                    break;
                }
                else{
                    --itBlocks;
                    --parallelBlocksIterator;
                    parallelBlocksIterator->push_back(std::pair<int, int>(i, j));
                    itBlocks->push_back(i);
                    itBlocks->push_back(j);
                    break;
                }
            }
            else{
                if(next(itBlocks) != auxiliaryNodeSetArray.rend()){
                    ++parallelBlocksIterator;
                    continue;
                }
                else{
                    parallelBlocksIterator->push_back(std::pair<int, int>(i, j));
                    itBlocks->push_back(i);
                    itBlocks->push_back(j);
                }
            }
            ++parallelBlocksIterator;
        };
    }
}

std::string NetworkBuilder::printComp(){
    std::string result;
    for(std::vector<std::pair<int, int>>::iterator it = comparatorArray.begin(); it != comparatorArray.end(); ++it) {
        if(it->first < 0){
            result = result + "\t\t";
        }
        if(it->first == 0){
            result = result + "\t\t\t\t";
        }
        
        result = result + "Pair: (" + std::to_string(it->first) + " ," + std::to_string(it->second) + ")\n";
    }
    result = result + "\nsize = " + std::to_string(parallelBlockArray.size()) + "\n";
    return result;
};

std::string NetworkBuilder::getState(){ return state; }

std::string NetworkBuilder::printParallelComp(){
    std::string result;
    for(std::list<std::vector<std::pair <int, int> > >::iterator blockIt = parallelBlockArray.begin(); blockIt != parallelBlockArray.end(); ++blockIt) {
        result = result + "\n ------------ \n";
        for(std::vector<std::pair <int, int> >::iterator it = blockIt->begin(); it != blockIt->end(); ++it) {
            result = result + "Pair: (" + std::to_string(it->first) + " ," + std::to_string(it->second) + ")\n";
        }
    }
    return result;
}

std::string NetworkBuilder::printAux(){
    std::string result = "\n ================ \n ==============\n";
    for(std::list<std::vector<int> >::iterator blockIt = auxiliaryNodeSetArray.begin(); blockIt != auxiliaryNodeSetArray.end(); ++blockIt) {
        result = result + "\n ------------ \n";
        for(std::vector<int>::iterator it = blockIt->begin(); it != blockIt->end(); ++it) {
            result = result + "Aux: " + std::to_string(*it) + "\n";
        }
    }
    return result;
}

void NetworkBuilder::printIndexArray(){
    state = state + "\nIndexArray = ";
    for(std::vector<int>::iterator it = indexArray.begin(); it != indexArray.end(); ++it) {
        state = state + ", " + std::to_string(*it);
    }
    state = state + "\n";
}

bool checkAscending(std::vector<int> vec) {
    return std::is_sorted(std::begin(vec), std::end(vec));
}

bool checkAscending(int *p, int length){
    return std::is_sorted(p, p + length);
}

int getMax(std::vector<int> *vec) {
    int n = vec->size();
    int max = vec->at(0);
    for (int i = 1; i < n; i++) {
        if (vec->at(i) > max) {
            max = vec->at(i);
        }
    }
    return max;
}

int getMax(int *p, int length){
    int max = *p;
    for (int i = 1; i < length; i++) {
        if (*(p + i) > max) {
            max = *(p + i);
        }
    }
    return max;
}

void sortByDigit(std::vector<int> *vec, int digit) {
    int n = vec->size();
    std::vector<int> res(n);
    int i, count[10] = {0};
    for (i = 0; i < n; i++) {
        count[(vec->at(i) / digit) % 10]++;
    }
    for (i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }
    for (i = n - 1; i >= 0; i--) {
        res[count[(vec->at(i) / digit) % 10] - 1] = vec->at(i);
        count[(vec->at(i) / digit) % 10]--;
    }
    for (i = 0; i < n; i++) {
        vec->at(i) = res[i];
    }
}

void sortByDigit(int *p, int length, int digit) {
    std::vector<int> res(length);
    int i, count[10] = {0};
    for (i = 0; i < length; i++) {
        count[((*(p + i)) / digit) % 10]++;
    }
    for (i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }
    for (i = length - 1; i >= 0; i--) {
        res[count[((*(p + i)) / digit) % 10] - 1] = *(p + i);
        count[((*(p + i)) / digit) % 10]--;
    }
    for (i = 0; i < length; i++) {
        *(p + i) = res[i];
    }
}

void bitwiseSort(std::vector<int> *vec) {
    int max = getMax(vec);
    for (int digit = 1; max / digit > 0; digit *= 10)
        sortByDigit(vec, digit);
}

void bitwiseSort(int *p, int length) {
    int max = getMax(p, length);
    for (int digit = 1; max / digit > 0; digit *= 10)
        sortByDigit(p, length, digit);
}

void printThreadNum(int maxThreadNumber){
    omp_set_num_threads(maxThreadNumber);
    int threadNumber = 0;
    std::string result = "";
#pragma omp parallel private(threadNumber)
    {
        threadNumber = omp_get_thread_num();
#pragma omp critical
        {
        std::cout<<"\n ------------- " << std::to_string(threadNumber)<<"\n";
        }
    }
}

void printThreadArea(int arraySize, int maxThreadCount){

    int effectiveThreadCount = 0;
    if(arraySize < MINIMAL_SINGLE_ARRAY_LENGTH * maxThreadCount) {
        if (arraySize % MINIMAL_SINGLE_ARRAY_LENGTH == 0) {
            effectiveThreadCount = (int) (arraySize / MINIMAL_SINGLE_ARRAY_LENGTH);
        }
        else {
            effectiveThreadCount = (int) (arraySize / MINIMAL_SINGLE_ARRAY_LENGTH) + 1;
        }
    } else {
        if (arraySize < (MINIMAL_SINGLE_ARRAY_LENGTH + 1) * (maxThreadCount - 1) + 1){
            effectiveThreadCount = maxThreadCount - 1;
        } else {
            effectiveThreadCount = maxThreadCount;
        }
    }
    omp_set_num_threads(effectiveThreadCount);

    int threadNumber = 0, beginIndex = 0, endIndex = 0;  // indexes are supposed to be inclusive
    #pragma omp parallel private(threadNumber, beginIndex, endIndex)
    {
        #pragma omp single
        {
            std::cout<<"\n  PRINTING THREAD AREA \n";
        }
        threadNumber = omp_get_thread_num();

        if(threadNumber > effectiveThreadCount){
            beginIndex = endIndex = -1; // do nothing
        } else {
            if(arraySize % effectiveThreadCount == 0) {
                int localSize = (int) (arraySize / effectiveThreadCount);
                beginIndex = threadNumber * localSize;
                endIndex = beginIndex + localSize - 1;
            }else{
                if(effectiveThreadCount == 1){
                    beginIndex = 0;
                    endIndex = arraySize - 1;
                }
                else{
                    int localSize = (int) (arraySize / effectiveThreadCount) + 1;
                    beginIndex = threadNumber * localSize;
                    if (threadNumber == effectiveThreadCount - 1) {
                        endIndex = arraySize - 1;
                    }else{
                        endIndex = beginIndex + localSize - 1;
                    }
                }
            }
        }
        #pragma omp critical
        {
            std::cout<<"\n ------------- \n"
            << "ThreadNumber: " << std::to_string(threadNumber) << "\t\t"
            << "EffectiveThreadCount: " << std::to_string(effectiveThreadCount) << "\t\t"
            << "begin: " << std::to_string(beginIndex) << "\t"
            << "end: " << std::to_string(endIndex)<<"\n";
        }
    }
}

void mergeAndSplit(int *arr1, int size1, int *arr2, int size2) {
  for (int i = size2 - 1; i >= 0; i--) {
    int j, last = arr1[size1 - 1];
    for (j = size1 - 2; j >= 0 && arr1[j] > arr2[i]; j--)
      arr1[j + 1] = arr1[j];
    if (j != size1 - 2 || last > arr2[i]) {
      arr1[j + 1] = arr2[i];
      arr2[i] = last;
    }
  }
}

bool parallelBitwiseBatcherSort(int *array, int arraySize, int maxThreadCount){
    // definr effective threadCount
    // define areas
    // sort
    // check every one

    bool allSubarraysSortedCorrectly = true;
    int effectiveThreadCount = 0;
    if(arraySize < MINIMAL_SINGLE_ARRAY_LENGTH * maxThreadCount) {
        if (arraySize % MINIMAL_SINGLE_ARRAY_LENGTH == 0) {
            effectiveThreadCount = (int) (arraySize / MINIMAL_SINGLE_ARRAY_LENGTH);
        }
        else {
            effectiveThreadCount = (int) (arraySize / MINIMAL_SINGLE_ARRAY_LENGTH) + 1;
        }
    } else {
        if (arraySize < (MINIMAL_SINGLE_ARRAY_LENGTH + 1) * (maxThreadCount - 1) + 1){
            effectiveThreadCount = maxThreadCount - 1;
        } else {
            effectiveThreadCount = maxThreadCount;
        }
    }
    omp_set_num_threads(effectiveThreadCount);

    int threadNumber = 0, beginIndex = 0, endIndex = 0;  // indexes are supposed to be inclusive
    #pragma omp parallel private(threadNumber, beginIndex, endIndex)
    {
        #pragma omp single
        {
            std::cout<<"\n SORTING OF EACH SUBARRAY \n";
        }
        threadNumber = omp_get_thread_num();

        if(threadNumber > effectiveThreadCount){
            beginIndex = endIndex = -1; // do nothing
        } else {
            if(arraySize % effectiveThreadCount == 0) {
                int localSize = (int) (arraySize / effectiveThreadCount);
                beginIndex = threadNumber * localSize;
                endIndex = beginIndex + localSize - 1;
            }else{
                if(effectiveThreadCount == 1){
                    beginIndex = 0;
                    endIndex = arraySize - 1;
                }
                else{
                    int localSize = (int) (arraySize / effectiveThreadCount) + 1;
                    beginIndex = threadNumber * localSize;
                    if (threadNumber == effectiveThreadCount - 1) {
                        endIndex = arraySize - 1;
                    }else{
                        endIndex = beginIndex + localSize - 1;
                    }
                }
            }
        }

        bitwiseSort(array + beginIndex, endIndex - beginIndex + 1);
        bool sortedLocally = checkAscending(array + beginIndex, endIndex - beginIndex + 1);
        #pragma omp critical
        {
            allSubarraysSortedCorrectly &= sortedLocally;
            std::cout<<"\n ------------- \n"
            << "ThreadNumber: " << std::to_string(threadNumber) << "\t\t"
            // << "EffectiveThreadCount: " << std::to_string(effectiveThreadCount) << "\t\t"
            << "begin: " << std::to_string(beginIndex) << "\t"
            << "end: " << std::to_string(endIndex)<<"\nARRAY {";
            for(int i = beginIndex; i <= endIndex; i++){
                std::cout<<array[i] << "\t";
            }
            std::cout<<(sortedLocally ? "}\n SORTED CORRECTLY \n" : "}\n SORTED NOT CORRECTLY \n");
        }
        #pragma omp barrier
        #pragma omp single
        {
            std::cout<<(allSubarraysSortedCorrectly ? "\n ARRAYS SORTED CORRECTLY \n" : "\n ARRAYS SORTED NOT CORRECTLY \n");
        }
    }
    return allSubarraysSortedCorrectly;
}