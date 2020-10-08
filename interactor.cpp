#include "interactor.h"

std::ostream& operator<<(std::ostream& os, const _edge& x) {
    os << x.from << ' ' << x.to << ' ' << x.symb;
    return os;
}

std::ostream& operator<<(std::ostream& os, const automaton& x) {
    os << "|Q| = " << x._number_of_vertices << "\n";
    os << "q0 = " << x._start << "\n";
    os << "|F| = " << x._finish.size() << "\n";
    os << "Finish: ";
    for (auto y: x._finish)
        os << y << ' ';
    os << "\n";
    os << "|Alphabet| = " << x._alphabet.size() << "\n";
    auto edges = x.get_edges();
    os << "|Edges| = " << edges.size() << "\n";
    for (auto e : edges)
        os << e << "\n";
    os << "\n";
    return os;
}

interactor_with_automaton::interactor_with_automaton(std::istream& in_stream, std::ostream& os_stream): is(in_stream), os(os_stream) {}

void interactor_with_automaton::_read_NKA() {
    size_t Q;
    std::vector<size_t> finish;
    size_t start;
    os << "Write |Q|" << std::endl;
    is >> Q;
    os << "Write start vertex" << std::endl;
    is >> start;
    os << "Write |F|" << std::endl;
    size_t k;
    is >> k;
    finish.resize(k);
    os << "Write F" << std::endl;
    for (auto& x : finish)
        is >> x;
    os << "Write |Alplhabet| " << std::endl;
    is >> k;
    std::vector<char> alphabet(k);
    os << "Write Alplhabet " << std::endl;
    for (auto& x : alphabet)
        is >> x;
    automaton machine(Q, start, finish, alphabet);
    os << "Write |Edges|" << std::endl;
    is >> k;
    for (size_t i = 0; i < k; ++i) {
        os << "Write edge" << std::endl;
        size_t from, to;
        char symb;
        is >> from >> to >> symb;
        machine.add_edge(from, to, symb);
    }
    mach = machine;
    os << "Done!" << std::endl;
}

void interactor_with_automaton:: run() {
    std::string command = "";
    is >> command;
    while (command != "exit") {
        if (command == "read") {
            _read_NKA();
        }
        if (command == "write") {
            os << mach << std::endl;
        }
        if (command == "determinaze") {
            mach = mach.get_equivalent_DKA();
            os << "Determinization done!" << std::endl;
        }
        if (command == "inverse") {
            mach = mach.get_automaton_for_addition();
            os << "Inversing done!" << std::endl;
        }
        if (command == "minimaze") {
            mach = mach.get_minimum_DKA();
            os << "Minimazing done!" << std::endl;
        }
        is >> command;
    }
}