#include <iostream>
#include "interactor.h"

int main() {
    interactor_with_automaton inter(std::cin, std::cout);
    inter.run();
    return 0;
}
