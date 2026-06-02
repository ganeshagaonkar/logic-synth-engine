#include "quine_mccluskey.h"
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