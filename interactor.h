#pragma ONCE
#include "automaton.h"

std::ostream& operator<<(std::ostream& os, const _edge& x);

std::ostream& operator<<(std::ostream& os, const automaton& x);

class interactor_with_automaton {
private:
    automaton mach;
    std::istream& is;
    std::ostream& os;

    void _read_NKA();

public:
    interactor_with_automaton(std::istream& in_stream, std::ostream& os_stream);
    void run();
};