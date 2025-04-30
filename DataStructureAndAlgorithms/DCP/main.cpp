#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ull = unsigned long long;

class RerollDSU {
private:
    vector<size_t> par;
    vector<size_t> sz;
    size_t n;
    vector<pair<size_t*, size_t>> back;
public:
    RerollDSU() = default;

    RerollDSU(size_t n): n(n) {
        par.resize(n);
        sz.resize(n, 1);
        iota(par.begin(), par.end(), 0);
    }

    size_t root(size_t v) {
        return par[v] == v ? v : root(par[v]);
    }

    bool UnionSet(size_t u, size_t v) {
        u = root(u);
        v = root(v);
        if (u == v) return false;
        if (sz[u] < sz[v]) swap(u, v);

        back.push_back({&par[v], par[v]});
        par[v] = u;

        back.push_back({&sz[u], sz[u]});
        sz[u] += sz[v];

        return true;
    }

    void Reroll(size_t t) {
        while (back.size() > t) {
            *(back.back().first) = back.back().second;
            back.pop_back();
        }
    }

    vector<size_t> getPar() const { return par; }

    size_t getTime() {
        return back.size();
    }
};

struct edge {
    char t;
    size_t u;
    size_t v;

    bool operator<(const edge& other) const {
        if (t < other.t) return true;
        if (t > other.t) return false;
        return array<size_t, 2>{u, v} < array<size_t, 2>{other.u, other.v};
    }
};

struct node {
    vector<edge> e;
};

class DCP {
private:
    vector<node> t;
    size_t n;
    RerollDSU A;
    RerollDSU AB;

    void AddEdge(size_t v, size_t tl, size_t tr, size_t l, size_t r, edge x) {
        if (l > r) return;
        if (tl == l && tr == r) {
            t[v].e.push_back(x);
            return;
        }
        size_t tm = (tl + tr) >> 1;
        AddEdge(v << 1, tl, tm, l, min(tm, r), x);
        AddEdge(v << 1 | 1, tm + 1, tr, max(tm + 1, l), r, x);
    }

    void calc(size_t v, size_t tl, size_t tr, size_t compA, size_t compAB, vector<size_t>& ans) {
        int wasA = A.getTime();
        int wasAB = AB.getTime();

        for (const auto&[c, a, b] : t[v].e) {
            if (c == 'A') compA -= A.UnionSet(a, b);
            compAB -= AB.UnionSet(a, b);
        }

        if (tl == tr) {
            ans[tl] = compA - compAB;
            A.Reroll(wasA);
            AB.Reroll(wasAB);
            return;
        }

        size_t tm = (tl + tr) >> 1;
        calc(v << 1, tl, tm, compA, compAB, ans);
        calc(v << 1 | 1, tm + 1, tr, compA, compAB, ans);
        A.Reroll(wasA);
        AB.Reroll(wasAB);
    }

public:
    DCP(size_t n) : n(n) {
        t.resize(4 * n);
    }

    void AddEdge(size_t l, size_t r, edge x) {
        AddEdge(1, 0, n - 1, l, r, x);
    }

    vector<size_t> calc(size_t comp) {
        A = RerollDSU(comp);
        AB = RerollDSU(comp);
        vector<size_t> ans(n);
        calc(1, 0, n - 1, comp, comp, ans);
        return ans;
    }
};

void solve() {
    size_t n, q; cin >> n >> q;

    map<edge, vector<size_t>> upd;
    for (size_t i = 0; i < q; i++) {
        edge e; cin >> e.t >> e.u >> e.v;
        e.u--, e.v--;
        if (e.u > e.v) swap(e.u, e.v);
        upd[e].push_back(i);
    }

    DCP dcp(q);

    for (const auto& it : upd) {
        for (size_t i = 0; i < it.second.size(); i += 2) {
            dcp.AddEdge(it.second[i], i + 1 < it.second.size() ? it.second[i + 1] - 1 : q - 1,
                        it.first);
        }
    }

    vector<size_t> ans = dcp.calc(n);

    for (const auto& x : ans) {
        cout << x << endl;
    }
}

int32_t main() {
#ifdef __APPLE__
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
#endif

    ios_base::sync_with_stdio(0);

    int test = 1;
    //cin >> test;
    while (test--) solve();

    return 0;
}
