#include "quine_mccluskey.h"
#include<set>
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