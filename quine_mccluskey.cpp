#include "quine_mccluskey.h"
#include<set>
#include<algorithm>
Implicant::Implicant():value(0),mask(0),is_prime(true){}
Implicant::Implicant(int m):value(m),mask(0),is_prime(true){minterms.push_back(m);}
int Implicant::num_ones()const{
    return __builtin_popcount(value);
}
bool Implicant::can_merge(const Implicant&other)const{
if(mask!=other.mask)
return false;
int diff=value^other.value;
return (diff!=0) && ((diff & (diff-1))==0);
}

Implicant Implicant::merge(const Implicant&other) const{
    int diff=value^other.value;
    Implicant result;
    result.value=value&other.value;
    result.mask=mask|diff;
    result.is_prime=true;
    result.minterms=minterms;
    result.minterms.insert(result.minterms.end(),other.minterms.begin(),other.minterms.end());
    return result;
}

std::string Implicant::to_string(int num_vars) const {
    std::string s = "";
    for (int i = num_vars - 1; i >= 0; i--) {
        if (mask & (1 << i)) {
            s += '-';
        } else if (value & (1 << i)) {
            s += '1';
        } else {
            s += '0';
        }
    }
    return s;
}
bool Implicant::operator<(const Implicant&other)const{
    if(value!=other.value)return value<other.value;
    return mask<other.mask;
}
std::vector<Implicant> get_prime_implicants(const std::vector<int>& minterms, int num_vars) {
    std::vector<std::vector<Implicant>> current_column(num_vars + 1);

    // 1. Group initial minterms by their bit weight
    for (int m : minterms) {
        Implicant imp(m);
        current_column[imp.num_ones()].push_back(imp);
    }

    std::vector<Implicant> prime_implicants;
    bool merges_occurred = true;

    // 2. Iteratively merge columns until no more merges are possible
    while (merges_occurred) {
        merges_occurred = false;
        std::vector<std::vector<Implicant>> next_column(num_vars + 1);
        std::set<Implicant> unique_next;

        for (int i = 0; i < num_vars; ++i) {
            for (Implicant& a : current_column[i]) {
                for (Implicant& b : current_column[i + 1]) {
                    if (a.can_merge(b)) {
                        a.is_prime = false;
                        b.is_prime = false;
                        Implicant merged = a.merge(b);
                        if (unique_next.find(merged) == unique_next.end()) {
                            unique_next.insert(merged);
                            next_column[merged.num_ones()].push_back(merged);
                            merges_occurred = true;
                        }
                    }
                }
            }
        }

        // 3. Harvest any implicants that could not be merged (they are prime)
        for (int i = 0; i <= num_vars; ++i) {
            for (const Implicant& imp : current_column[i]) {
                if (imp.is_prime) {
                    prime_implicants.push_back(imp);
                }
            }
        }

        current_column = next_column;
    }

    return prime_implicants;
}
std::vector<Implicant> get_minimum_cover(std::vector<Implicant> primes, const std::vector<int>& original_minterms) {
    std::vector<Implicant> minimum_cover;
    std::set<int> uncovered_minterms(original_minterms.begin(), original_minterms.end());
    std::set<int> used_prime_indices;

    // Phase 1: Find Essential Prime Implicants
    for (int minterm : uncovered_minterms) {
        int cover_count = 0;
        int last_covering_index = -1;

        for (int i = 0; i < primes.size(); ++i) {
            // Check if this prime covers the current minterm
            if (std::find(primes[i].minterms.begin(), primes[i].minterms.end(), minterm) != primes[i].minterms.end()) {
                cover_count++;
                last_covering_index = i;
            }
        }

        // If exactly one prime covers this minterm, it is essential
        if (cover_count == 1 && used_prime_indices.find(last_covering_index) == used_prime_indices.end()) {
            minimum_cover.push_back(primes[last_covering_index]);
            used_prime_indices.insert(last_covering_index);
        }
    }

    // Remove all minterms covered by the essential primes
    for (const Implicant& epi : minimum_cover) {
        for (int m : epi.minterms) {
            uncovered_minterms.erase(m);
        }
    }

    // Phase 2: Greedy Set-Cover for remaining minterms
    while (!uncovered_minterms.empty()) {
        int best_index = -1;
        int max_covered = 0;

        for (int i = 0; i < primes.size(); ++i) {
            if (used_prime_indices.find(i) != used_prime_indices.end()) continue;

            int current_covered = 0;
            for (int m : primes[i].minterms) {
                if (uncovered_minterms.find(m) != uncovered_minterms.end()) {
                    current_covered++;
                }
            }

            if (current_covered > max_covered) {
                max_covered = current_covered;
                best_index = i;
            }
        }

        // Safety break to prevent infinite loops on malformed data
        if (best_index == -1) break; 

        minimum_cover.push_back(primes[best_index]);
        used_prime_indices.insert(best_index);

        for (int m : primes[best_index].minterms) {
            uncovered_minterms.erase(m);
        }
    }

    return minimum_cover;
}