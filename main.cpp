#include <iostream>
#include <vector>
#include "quine_mccluskey.h"

int main() {
    int num_vars = 4;
    std::vector<int> minterms = {0, 1, 2, 5, 6, 7, 8, 9, 10, 14};

    std::cout << "Starting Prime Implicant Generation...\n";
    std::cout << "Minterms: ";
    for (int m : minterms) std::cout << m << " ";
    std::cout << "\n\n";

    std::vector<Implicant> primes = get_prime_implicants(minterms, num_vars);

    std::cout << "Found " << primes.size() << " Prime Implicants:\n";
    for (const Implicant& p : primes) {
        std::cout << p.to_string(num_vars) << " (Covers: ";
        for (int m : p.minterms) std::cout << m << " ";
        std::cout << ")\n";
    }

    return 0;
}