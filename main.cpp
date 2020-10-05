#include <iostream>
#include "interactor.h"
#include <gtest/gtest.h>

int main() {
    interactor_with_automaton inter(std::cin, std::cout);
    inter.run();
    return 0;
}
