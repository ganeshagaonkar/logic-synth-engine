#pragma once
#include<vector>
#include<string>
#include<unordered_map>

enum class GateType{
    INPUT,
    OUTPUT,
    AND,
    OR,
    NOT,
    DFF
};
struct Node{
    int id;
    GateType type;
    int delay;
    bool current_value;
    std::vector<int>inputs;
    std::vector<int>outputs;
    Node(int id,GateType type,int delay=0):id(id),type(type),delay(delay),current_value(false){}
    Node() : id(-1), type(GateType::INPUT), delay(0), current_value(false) {}
};
class CircuitGraph{
    private:
    std::unordered_map<int,Node>nodes;
    std::vector<int>input_nodes;
    std::vector<int>output_nodes;
    public:
    void tick_clock();
    void add_node(int id,GateType type,int delay);
    void add_edge(int source_id,int dest_id);
    void set_inputs(const std::vector<bool>&input_values);
    void evaluate_topological();
    int get_critical_path();
    void print_state();

};