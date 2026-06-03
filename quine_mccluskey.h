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
    bool operator<(const Implicant &other) const;
};
std::vector<Implicant>get_prime_implicants(const std::vector<int>& minterms, int num_vars);
std::vector<Implicant>get_minimum_cover(std::vector<Implicant>primes,const std::vector<int>&original_minterms);
