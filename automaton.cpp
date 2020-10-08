#include <automaton.h>

std::tuple<size_t, size_t, char> _edge::to_tuple() const {
	return std::make_tuple(from, to, symb);
}

bool operator<(const _edge& left, const _edge& right) {
    return left.to_tuple() < right.to_tuple();
}


std::vector<std::vector<size_t>> get_all_bynary_vectors(size_t len) {
    std::vector<std::vector<size_t>> answer;
    std::vector<size_t> current(len, 0);
    for (size_t i = 0; i < (1 << len); ++i) {
        answer.emplace_back(current);
        size_t cur_place = 0;
        while (cur_place < len && current[cur_place] == 1) {
            current[cur_place] = 0;
            ++cur_place;
        }
        if (cur_place != len) {
            ++current[cur_place];
        }
    }
    return answer;
}

size_t automaton::get_number_of_vertices() const {
    return _number_of_vertices;
}

size_t automaton::_get_out_vertex(size_t from, char symb) {
    for (auto& edge: _edges[from]) {
        if (edge.symb == symb) {
            return edge.to;
        }
    }
    throw std::logic_error("There is no such edge");
}

std::vector<size_t> automaton::_step_of_minimazing(const std::vector<size_t>& old_color) {
    std::map<std::vector<size_t>, int> recolor;
    std::vector<size_t> color(_number_of_vertices);
    for (size_t i = 0; i < _number_of_vertices; ++i) {
        std::vector<size_t > colors_v({old_color[i]});
        for (auto c : _alphabet) {
            colors_v.push_back(old_color[_get_out_vertex(i, c)]);
        }
        if (!recolor.count(colors_v)) {
            recolor[colors_v] = recolor.size();
        }
        color[i] = recolor[colors_v];
    }
    return color;
}

automaton::automaton() = default;

template<typename Container>
automaton::automaton(size_t number_of_vertices, size_t start, const Container& finish, const std::vector<char>& alphabet): _start(start),
                     _finish(finish.cbegin(), finish.cend()), _edges(number_of_vertices), _alphabet(alphabet), _number_of_vertices(number_of_vertices) {
}

std::vector<_edge> automaton::get_edges() const {
    std::vector<_edge> answer;
    for (auto& v : _edges) {
        for (auto e : v) {
            answer.push_back(e);
        }
    }
    return answer;
}


automaton automaton::get_automaton_for_addition() {
    automaton answer = get_equivalent_DKA();
    std::unordered_set<size_t> new_finish;
    for (size_t i = 0; i < _number_of_vertices; ++i) {
        if (!answer.is_finishing(i)) {
            new_finish.insert(i);
        }
    }
    answer._finish = new_finish;
    return answer;
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

std::vector<size_t> automaton::get_reachable_vertices() {
    std::vector<size_t> reach({_start});
    std::vector<bool> visited(_number_of_vertices, 0);
    visited[_start] = 1;
    for (size_t i = 0; i < reach.size(); ++i) {
        auto& v = reach[i];
        for (auto edge : _edges[v]) {
            if (!visited[edge.to]) {
                visited[edge.to] = 1;
                reach.push_back(edge.to);
            }
        }
    }
    return reach;
}

bool automaton::is_finishing(size_t vertex) const {
    return _finish.count(vertex);
}

void automaton::clear_zero_vertices() {
    std::vector<size_t> reachable = get_reachable_vertices();
    std::unordered_set<size_t> new_finish;
    std::vector<size_t> renumeration(_number_of_vertices);
    for (size_t i = 0; i < reachable.size(); ++i) {
        if (is_finishing(reachable[i])) {
            new_finish.insert(i);
        }
        renumeration[reachable[i]] = i;
    }
    size_t new_start = renumeration[_start];
    std::vector<std::vector<_edge>> new_edges(reachable.size());
    for (auto v : reachable) {
        for (auto edge : _edges[v]) {
            new_edges[renumeration[v]].push_back({renumeration[edge.from], renumeration[edge.to], edge.symb});
        }
    }
    _number_of_vertices = reachable.size();
    _start = new_start;
    _finish = new_finish;
    _edges = new_edges;
    clear_equal_edges();
}

std::vector<size_t> automaton::_edges_from_set(const std::vector<size_t>& st, char symb) const {
    std::vector<size_t> result(_number_of_vertices, 0);
    for (size_t u = 0; u < _number_of_vertices; ++u) {
        if (st[u]) {
            for (auto edge: _edges[u]) {
                if (edge.symb == symb) {
                    result[edge.to] = 1;
                }

            }
        }
    }
    return result;
}

void automaton::make_finish(size_t q) {
    _finish.insert(q);
}

automaton automaton::get_equivalent_DKA() const {
    auto vertices = get_all_bynary_vectors(_number_of_vertices);
    std::map<std::vector<size_t>, size_t> renum;
    for (size_t i = 0; i < vertices.size(); ++i) {
        renum[vertices[i]] = i;
    }
    std::vector<size_t> new_finish;
    size_t new_start;
    std::vector<std::vector<_edge>> new_edges(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i) {
        auto& u = vertices[i];
        for (auto symb : _alphabet) {
            std::vector<size_t> new_u = _edges_from_set(u, symb);
            new_edges[i].push_back({i, renum[new_u], symb});
        }
        for (size_t j = 0; j < _number_of_vertices; ++j)
            if (u[j] && is_finishing(j)) {
                new_finish.push_back(i);
            }
    }

    std::vector<size_t> start_v(_number_of_vertices, 0);
    start_v[_start] = 1;
    new_start = renum[start_v];

    automaton answer(vertices.size(), new_start, new_finish, _alphabet);
    answer._edges = new_edges;
    answer.clear_zero_vertices();
    return answer;
}

automaton automaton::get_minimum_DKA() {
    std::vector<size_t> color(_number_of_vertices);
    for (size_t i = 0; i < _number_of_vertices; ++i) {
        color[i] = is_finishing(i);
    }

    while (color != _step_of_minimazing(color)) {
        color = _step_of_minimazing(color);
    }

    size_t new_start = color[_start];
    std::unordered_set<size_t> new_finish;
    std::vector<std::vector<_edge>> new_edges(_number_of_vertices);
    for (size_t i = 0; i < _number_of_vertices; ++i) {
        if (is_finishing(i)) {
            new_finish.insert(color[i]);
        }
        for (auto edge : _edges[i]) {
            new_edges[color[edge.from]].push_back({color[edge.from], color[edge.to], edge.symb});
        }
    }
    automaton answer(_number_of_vertices, new_start, new_finish, _alphabet);
    answer._edges = new_edges;
    answer.clear_zero_vertices();
    return answer;
}

bool automaton::valid_string(const std::string& s) {
    std::vector<std::pair<size_t, std::string> > reacheble_positions;
    std::set<std::pair<size_t, std::string> > visited;
    reacheble_positions.emplace_back(_start, s);
    visited.emplace(_start, s);
    for (size_t i = 0; i < reacheble_positions.size(); ++i) {
        if (reacheble_positions[i].second.size() > 0) {
            for (auto edge: _edges[reacheble_positions[i].first]) {
                if (reacheble_positions[i].second[0] == edge.symb) {
                    std::string new_str = reacheble_positions[i].second.substr(1, reacheble_positions[i].second.size() - 1);
                    if (!visited.count({edge.to, new_str})) {
                        visited.emplace(edge.to, new_str);
                        reacheble_positions.emplace_back(edge.to, new_str);
                    }
                }
            }
        } else {
            if (is_finishing(reacheble_positions[i].first)) {
                return true;
            }
        }
    }
    return false;
}
