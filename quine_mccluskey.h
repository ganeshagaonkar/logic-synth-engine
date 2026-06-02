#pragma once
#include<vector>
#include<string>
struct Implicant{
    int value;
    int mask;
    bool is_prime;
    std::vector<int>minterms;
    Implicant();
    Implicant(int m);
    int num_ones() const;
    bool can_merge(const Implicant&other) const;
    Implicant merge(const Implicant& other) const;
    std::string to_string(int num_vars) const;
};