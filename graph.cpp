#include "graph.h"
#include<unordered_map>
#include<vector>
#include<iostream>
#include<queue>
void CircuitGraph::add_node(int id,GateType type,int delay)
{
nodes.emplace(id,Node(id,type,delay));
if(type==GateType::INPUT)
input_nodes.push_back(id);
else if(type==GateType::OUTPUT)
output_nodes.push_back(id);}

void CircuitGraph::add_edge(int source_id,int dest_id)
{nodes[source_id].outputs.push_back(dest_id);
nodes[dest_id].inputs.push_back(source_id);}

void CircuitGraph::set_inputs(const std::vector<bool>&input_values)
{for(int i=0;i<input_nodes.size();i++)
{nodes[input_nodes[i]].current_value=input_values[i];}}

void CircuitGraph::print_state() {
    std::cout << "--- Cycle State ---\n";
    for (const auto& pair : nodes) {
        std::cout << "Node " << pair.first << " : Logic [" << pair.second.current_value << "]\n";
    }
}

void CircuitGraph::evaluate_topological()
{std::queue<int>q;
std::unordered_map<int,int>dep;
for(auto x:nodes)
{int id=x.first;
    GateType type=x.second.type;
    if(type==GateType::INPUT || type==GateType::DFF)
{dep[id]=0;
q.push(id);}
else{
    dep[id]=x.second.inputs.size();
    if(dep[id]==0)q.push(id);
}
}
while(q.size()!=0)
{int current_id=q.front();
Node&current_node=nodes[current_id];
switch(current_node.type)
{case GateType::INPUT:break;
case GateType::AND:{
    bool res=true;
    for(int in_id:current_node.inputs)
    res=res&&nodes[in_id].current_value;
    current_node.current_value=res;
    break;}
case GateType::OR:{
    bool res=false;
    for(int in_id:current_node.inputs)
    res=res||nodes[in_id].current_value;
    current_node.current_value=res;
    break;
}
case GateType::NOT:{
    current_node.current_value=!nodes[current_node.inputs[0]].current_value;
    break;
}
case GateType::OUTPUT:{
    current_node.current_value=nodes[current_node.inputs[0]].current_value;
    break;
}
case GateType::DFF:break;}

q.pop();
for(auto x:current_node.outputs)
{dep[x]--;
if(dep[x]==0)
q.push(x);}}
}
int CircuitGraph::get_critical_path()
{std::queue<int>q;
std::unordered_map<int,int>dep;
std::vector<int>topo;
for(auto x:nodes)
{int id=x.first;
    GateType type=x.second.type;
    if(type==GateType::INPUT||type==GateType::DFF)
    {dep[id]=0;
    q.push(id);}
    else
    {dep[id]=x.second.inputs.size();
    if(dep[id]==0)
    q.push(id);}
}
while(q.size()!=0)
{int node=q.front();
q.pop();
topo.push_back(node);
for(auto x:nodes[node].outputs)
{dep[x]--;
if(dep[x]==0)
q.push(x);}}
std::unordered_map<int,int>dp;
for(auto x:nodes)
dp[x.first]=0;
for(int i:topo)
{int maxi=0;
for(int in_id:nodes[i].inputs)
maxi=std::max(maxi,dp[in_id]);
dp[i]=maxi+nodes[i].delay;}
int ans=0;
for(int out:output_nodes)
ans=std::max(ans,dp[out]);
return ans;
}

void CircuitGraph::tick_clock()
{evaluate_topological();
std::unordered_map<int,bool>next_dff_states;
for(auto x:nodes)
{if(x.second.type==GateType::DFF)
{if(!x.second.inputs.empty())
{int in_id=x.second.inputs[0];
next_dff_states[x.first]=nodes[in_id].current_value;}}}

for(auto x:next_dff_states)
{nodes[x.first].current_value=x.second;}}