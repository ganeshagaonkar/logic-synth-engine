#include <iostream>
#include "graph.h"
int main() {
    CircuitGraph engine;

    // 1. Build the physical nodes
    engine.add_node(1, GateType::INPUT, 0);  // Input A
    engine.add_node(2, GateType::INPUT, 0);  // Input B
    engine.add_node(3, GateType::INPUT, 0);  // Input C
    
    engine.add_node(4, GateType::AND, 2);    // AND gate (Delay: 2ns)
    engine.add_node(5, GateType::OR, 2);     // OR gate (Delay: 2ns)
    
    engine.add_node(6, GateType::OUTPUT, 0); // Output Pin

    // 2. Wire them up: Out = (A AND B) OR C
    engine.add_edge(1, 4); // A -> AND
    engine.add_edge(2, 4); // B -> AND
    engine.add_edge(4, 5); // AND -> OR
    engine.add_edge(3, 5); // C -> OR
    engine.add_edge(5, 6); // OR -> Output

    // 3. Set Input States (A=1, B=1, C=0) -> Expected Output = 1
    std::cout << "Setting inputs: A=1, B=1, C=0\n";
    engine.set_inputs({true, true, false});

    // 4. Run your CP algorithms!
    engine.evaluate_topological();
    
    std::cout << "\nAfter Topological Evaluation:\n";
    engine.print_state(); // Node 6 (Output) should be 1

    std::cout << "\nCritical Path Delay: " << engine.get_critical_path() << " ns\n"; 
    // Expected Delay: 4ns (A/B -> AND(2ns) -> OR(2ns) -> Out)

    return 0;
}

