#include "pgcd.h"

int pgcd (int a, int b){
    int max = (a>b)?a:b;//compare a>b et répond a, sinon b
    int min = (a>b)?b:a;//idem au dessus

    if (min==0){
        return max;
    }

    while (max-min){
        int d = max - min;
        max = (min>d)?min:d;
        min = (min>d)?d:min;
    }
    return min;
} 
