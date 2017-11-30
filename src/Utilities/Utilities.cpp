//
// Created by Ibrahim Radwan on 11/29/17.
//

#include "Utilities.h"

vector<int> Utilities::primes;
bool Utilities::notPrimesArr[];

void
Utilities::sieve() {
    notPrimesArr[0] = notPrimesArr[1] = 1;
    for (int i = 2; i < 1e5; ++i) {
        if (notPrimesArr[i]) continue;

        primes.push_back(i);
        for (int j = i * 2; j < 1e5; j += i) {
            notPrimesArr[j] = 1;
        }
    }
}

void
Utilities::addPrimesToVector(int n, vector<int> &probPrimes) {
    for (int i = 0; i < primes.size(); ++i) {
        while (n % primes[i]) {
            n /= primes[i];
            probPrimes[i]++;
        }
    }
}