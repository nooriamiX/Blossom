#include <vector>
#include <iostream>
#include <string.h>
#include <set>

using namespace std;

const int MAXN = 10; // максимально возможное число вершин во входном графе

vector<int> g[MAXN];
int match[MAXN], p[MAXN], base[MAXN], q[MAXN];
bool used[MAXN], blossom[MAXN];

struct comp
{
    template<typename T>
    bool operator()(const T& l, const T& r) const
    {
        if (l.first == r.first) {
            return l.second > r.second;
        }

        return l.first < r.first;
    }
};

int lca(int a, int b) {
    bool used[MAXN] = { 0 };
    // поднимаемся от вершины a до корня, помечая все чётные вершины
    for (;;) {
        a = base[a];
        used[a] = true;
        if (match[a] == -1)  break; // дошли до корня
        a = p[match[a]];
    }
    // поднимаемся от вершины b, пока не найдём помеченную вершину
    for (;;) {
        b = base[b];
        if (used[b])  return b;
        b = p[match[b]];
    }
}

void mark_path(int v, int b, int children) {
    while (base[v] != b) {
        blossom[base[v]] = blossom[base[match[v]]] = true;
        p[v] = children;
        children = match[v];
        v = p[match[v]];
    }
}

int find_path(int root) {
    memset(used, 0, sizeof used);
    memset(p, -1, sizeof p);
    for (int i = 0; i < MAXN; i++)
        base[i] = i;

    used[root] = true;
    int qh = 0, qt = 0;
    q[qt++] = root;
    while (qh < qt) {
        int v = q[qh++];
        for (size_t i = 0; i < g[v].size(); i++) {
            int to = g[v][i];
            if (base[v] == base[to] || match[v] == to)  continue;
            if (to == root || match[to] != -1 && p[match[to]] != -1) {
                int curbase = lca(v, to);
                memset(blossom, 0, sizeof blossom);
                mark_path(v, curbase, to);
                mark_path(to, curbase, v);
                for (int i = 0; i < MAXN; i++)
                    if (blossom[base[i]]) {
                        base[i] = curbase;
                        if (!used[i]) {
                            used[i] = true;
                            q[qt++] = i;
                        }
                    }
            }
            else if (p[to] == -1) {
                p[to] = v;
                if (match[to] == -1)
                    return to;
                to = match[to];
                used[to] = true;
                q[qt++] = to;
            }
        }
    }
    return -1;
}

int main() {
    vector<int> copy[MAXN];
    copy[0] = vector<int>({ 1, 2, 3, 5 });
    copy[1] = vector<int>({ 0, 2, 3, 7, 9 });
    copy[2] = vector<int>({ 0, 1, 6 });
    copy[3] = vector<int>({ 0, 1, 7, 9 });
    copy[4] = vector<int>({ 6 });
    copy[5] = vector<int>({ 0 });
    copy[6] = vector<int>({ 2, 4 });
    copy[7] = vector<int>({ 1, 3, 8 });
    copy[8] = vector<int>({ 7 });
    copy[9] = vector<int>({ 1, 3 });

    for (size_t i = 0; i < 10; i++)
    {
        for (size_t j = 0; j < copy[i].size(); j++)
        {
            g[i].push_back(copy[i][j]);
        }
    }
    memset(match, -1, sizeof match);
    for (int i = 0; i < MAXN; i++) {
        if (match[i] == -1) {
            int v = find_path(i);
            while (v != -1) {
                int pv = p[v], ppv = match[pv];
                match[v] = pv, match[pv] = v;
                v = ppv;
            }
        }
    }
    std::set<std::pair<int, int>, comp> set;

    for (size_t i = 0; i < MAXN; i++) {
        std::pair<int, int> p;
        if (i < match[i]) {
            p = std::pair<int, int>(i, match[i]);
        }
        else {
            p = std::pair<int, int>(match[i], i);
        }
        set.insert(p);
    }

    for (auto const& p : set) {
        std::cout << "{" << p.first << "," << p.second << "}\n";
    }

    return 0;

}