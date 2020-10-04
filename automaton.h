#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#pragma once

struct _edge {
    size_t from;
    size_t to;
    char symb;
};

bool operator<(const _edge& left, const _edge& right);
std::vector<std::vector<size_t>> get_all_bynary_vectors(size_t len);

class automaton {
private:
    size_t _start;
    std::unordered_set<size_t> _finish;
    std::vector<std::vector<_edge>> _edges;
    std::vector<char> _alphabet;
    size_t _number_of_vertexes;

    size_t _get_out_vertex(size_t from, char symb);
    std::vector<size_t> _step_of_minimazing(const std::vector<size_t>& old_color);
    std::vector<size_t> _edges_from_set(const std::vector<size_t>& st, char symb) const;

public:

    automaton();
    template<typename Container>
    automaton(size_t Q, size_t start, const Container& finish, const std::vector<char>& alphabet);
    std::vector<_edge> get_edges() const;
    automaton get_automaton_for_addition();
    void add_edge(size_t from, size_t to, char symb);
    void clear_equal_edges();
    std::vector<size_t> get_reachable_vertexes();
    bool is_finishing(size_t vertex) const;
    void clear_zero_vertexes();
    void make_finish(size_t q);
    automaton get_equivalent_DKA() const;
    automaton get_minimum_DKA();
    bool valid_string(const std::string& s);

    friend std::ostream& operator<<(std::ostream& os, const automaton& x);
};