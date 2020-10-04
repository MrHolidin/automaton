#include "automaton.h"

bool operator<(const _edge& left, const _edge& right) {
    return std::vector<size_t>{left.from, left.to, (size_t)left.symb} < std::vector<size_t>{right.from, right.to, (size_t)right.symb};
}


std::vector<std::vector<size_t>> get_all_bynary_vectors(size_t len) {
    std::vector<std::vector<size_t>> ans;
    std::vector<size_t> current(len, 0);
    for (size_t i = 0; i < (1 << len); ++i) {
        ans.emplace_back(current);
        size_t cur_place = 0;
        while (cur_place < len && current[cur_place] == 1) {
            current[cur_place] = 0;
            ++cur_place;
        }
        if (cur_place != len)
            ++current[cur_place];
    }
    return ans;
}

size_t automaton::_get_out_vertex(size_t from, char symb) {
    for (auto& edge: _edges[from])
        if (edge.symb == symb)
            return edge.to;
    throw std::logic_error("There is no such edge");
}

std::vector<size_t> automaton::_step_of_minimazing(const std::vector<size_t>& old_color) {
    std::map<std::vector<size_t>, int> recolor;
    std::vector<size_t> color(_number_of_vertexes);
    for (size_t i = 0; i < _number_of_vertexes; ++i) {
        std::vector<size_t > colors_v({old_color[i]});
        for (auto c : _alphabet)
            colors_v.push_back(old_color[_get_out_vertex(i, c)]);
        if (!recolor.count(colors_v)) {
            recolor[colors_v] = recolor.size();
        }
        color[i] = recolor[colors_v];
    }
    return color;
}

automaton::automaton() = default;

template<typename Container>
automaton::automaton(size_t Q, size_t start, const Container& finish, const std::vector<char>& alphabet): _start(start),
                     _finish(finish.cbegin(), finish.cend()), _edges(Q), _alphabet(alphabet), _number_of_vertexes(Q) {
}

std::vector<_edge> automaton::get_edges() const {
    std::vector<_edge> ans;
    for (auto& v : _edges)
        for (auto e : v)
            ans.push_back(e);
    return ans;
}


automaton automaton::get_automaton_for_addition() {
    automaton ans = get_equivalent_DKA();
    std::unordered_set<size_t> new_finish;
    for (size_t i = 0; i < _number_of_vertexes; ++i)
        if (!ans.is_finishing(i))
            new_finish.insert(i);
    ans._finish = new_finish;
    return ans;
}

void automaton::add_edge(size_t from, size_t to, char symb) {
    _edges[from].push_back({from, to, symb});
}

void automaton::clear_equal_edges() {
    for (auto& v : _edges) {
        std::vector<_edge> new_v;
        std::set<_edge> was;
        for (auto e: v) {
            if (!was.count(e)) {
                was.insert(e);
                new_v.push_back(e);
            }
        }
        v = new_v;
    }
}

std::vector<size_t> automaton::get_reachable_vertexes() {
    std::vector<size_t> reach({_start});
    std::vector<bool> visited(_number_of_vertexes, 0);
    visited[_start] = 1;
    for (size_t i = 0; i < reach.size(); ++i) {
        auto& v = reach[i];
        for (auto edge : _edges[v])
            if (!visited[edge.to]) {
                visited[edge.to] = 1;
                reach.push_back(edge.to);
            }
    }
    return reach;
}

bool automaton::is_finishing(size_t vertex) const {
    return _finish.count(vertex);
}

void automaton::clear_zero_vertexes() {
    std::vector<size_t> reachable = get_reachable_vertexes();
    std::unordered_set<size_t> new_finish;
    std::vector<size_t> renumeration(_number_of_vertexes);
    for (size_t i = 0; i < reachable.size(); ++i) {
        if (is_finishing(reachable[i])) {
            new_finish.insert(i);
        }
        renumeration[reachable[i]] = i;
    }
    size_t new_start = renumeration[_start];
    std::vector<std::vector<_edge>> new_edges(reachable.size());
    for (auto v : reachable)
        for (auto edge : _edges[v])
            new_edges[renumeration[v]].push_back({renumeration[edge.from], renumeration[edge.to], edge.symb});
    _number_of_vertexes = reachable.size();
    _start = new_start;
    _finish = new_finish;
    _edges = new_edges;
    clear_equal_edges();
}

std::vector<size_t> automaton::_edges_from_set(const std::vector<size_t>& st, char symb) const {
    std::vector<size_t> res(_number_of_vertexes, 0);
    for (size_t u = 0; u < _number_of_vertexes; ++u)
        if (st[u]) {
            for (auto edge: _edges[u])
                if (edge.symb == symb)
                    res[edge.to] = 1;
        }
    return res;
}

void automaton::make_finish(size_t q) {
    _finish.insert(q);
}

automaton automaton::get_equivalent_DKA() const {
    auto vertexes = get_all_bynary_vectors(_number_of_vertexes);
    std::map<std::vector<size_t>, size_t> renum;
    for (size_t i = 0; i < vertexes.size(); ++i) {
        renum[vertexes[i]] = i;
    }
    std::vector<size_t> new_finish;
    size_t new_start;
    std::vector<std::vector<_edge>> new_edges(vertexes.size());
    for (size_t i = 0; i < vertexes.size(); ++i) {
        auto& u = vertexes[i];
        for (auto symb : _alphabet) {
            std::vector<size_t> new_u = _edges_from_set(u, symb);
            new_edges[i].push_back({i, renum[new_u], symb});
        }
        for (size_t j = 0; j < _number_of_vertexes; ++j)
            if (u[j] && is_finishing(j)) {
                new_finish.push_back(i);
            }
    }
    {//finding start vertex
        std::vector<size_t> start_v(_number_of_vertexes, 0);
        start_v[_start] = 1;
        new_start = renum[start_v];
    }
    automaton ans(vertexes.size(), new_start, new_finish, _alphabet);
    ans._edges = new_edges;
    ans.clear_zero_vertexes();
    return ans;
}

automaton automaton::get_minimum_DKA() {
    std::vector<size_t> color(_number_of_vertexes);
    for (size_t i = 0; i < _number_of_vertexes; ++i)
        color[i] = is_finishing(i);

    while (color != _step_of_minimazing(color)) {
        color = _step_of_minimazing(color);
    }

    size_t new_start = color[_start];
    std::unordered_set<size_t> new_finish;
    std::vector<std::vector<_edge>> new_edges(_number_of_vertexes);
    for (size_t i = 0; i < _number_of_vertexes; ++i) {
        if (is_finishing(i))
            new_finish.insert(color[i]);
        for (auto edge : _edges[i])
            new_edges[color[edge.from]].push_back({color[edge.from], color[edge.to], edge.symb});
    }
    automaton ans(_number_of_vertexes, new_start, new_finish, _alphabet);
    ans._edges = new_edges;
    ans.clear_zero_vertexes();
    return ans;
}

bool automaton::valid_string(const std::string& s) {
    std::vector<std::pair<size_t, std::string> > bfs_vec;
    std::set<std::pair<size_t, std::string> > visited;
    bfs_vec.emplace_back(_start, s);
    visited.emplace(_start, s);
    for (size_t i = 0; i < bfs_vec.size(); ++i) {
        if (bfs_vec[i].second.size() > 0) {
            for (auto edge: _edges[bfs_vec[i].first])
                if (bfs_vec[i].second[0] == edge.symb) {
                    std::string new_str = bfs_vec[i].second.substr(1, bfs_vec[i].second.size() - 1);
                    if (!visited.count({edge.to, new_str})) {
                        visited.emplace(edge.to, new_str);
                        bfs_vec.emplace_back(edge.to, new_str);
                    }
                }
        } else {
            if (is_finishing(i)) {
                return true;
            }
        }
    }
    return false;
}
