#include <iostream>
#include "quine_mccluskey.h"

int main() {
    int num_vars = 4;
    
    // Create minterm 9 (1001) and minterm 11 (1011)
    Implicant m9(9);
    Implicant m11(11);

    std::cout << "Minterm 9:  " << m9.to_string(num_vars) << "\n";
    std::cout << "Minterm 11: " << m11.to_string(num_vars) << "\n";

    if (m9.can_merge(m11)) {
        Implicant merged = m9.merge(m11);
        std::cout << "Success! Merged into: " << merged.to_string(num_vars) << "\n";
        
        std::cout << "Covers original minterms: ";
        for (int m : merged.minterms) std::cout << m << " ";
        std::cout << "\n";
    } else {
        std::cout << "Failed to merge.\n";
    }

    return 0;
}