#pragma once

#include <vector>
#include <string>
#include <map>

using namespace std;

vector<int> OrderOfSort(const vector<pair<int, int>>& a) {
    int maxVal = 0;
    for (const auto& [l, r] : a) {
        maxVal = max({maxVal, l, r});
    }

    vector<vector<int>> gr(maxVal + 1);
    for (size_t i = 0; i < a.size(); i++) {
        gr[a[i].second].push_back(i);
    }

    vector<int> preOrder;
    for (int i = 0; i <= maxVal; i++) {
        for (const auto& x : gr[i]) {
            preOrder.push_back(x);
        }
        gr[i].clear();
    }

    for (size_t i = 0; i < preOrder.size(); i++) {
        gr[a[preOrder[i]].first].push_back(preOrder[i]);
    }

    vector<int> order;
    for (int i = 0; i <= maxVal; i++) {
        for (const auto& x : gr[i]) {
            order.push_back(x);
        }
        gr[i].clear();
    }

    return order;
}

class SuffixArray {
private:
    vector<vector<int>> c;
    vector<int> p;

public:
    SuffixArray();

    SuffixArray(vector<int> s) {
        int n = (int)s.size();
        int cnt = 0;
        int cls = 0;

        c.push_back(vector<int>(n));
        p.resize(n);

        map<int, vector<int>> t;
        for (int i = 0; i < n; i++) {
            t[s[i]].push_back(i);
        }

        for (const auto &x : t) {
            for (const int& u : x.second) {
                c[0][u] = cls;
                p[cnt++] = u;
            }
            cls++;
        }

        for (int l = 1; (1 << (l - 1)) <= n; l++) {
            c.push_back(vector<int>(n));

            int len = 1 << (l - 1);

            vector<pair<int, int>> a(n);

            for (int i = 0; i < n; i++) {
                int L = c[l - 1][i],
                        R = c[l - 1][(i + len < n ? i + len : i + len - n)];
                a[i] = {L, R};
            }

            p = OrderOfSort(a);

            cls = 0;
            for (int i = 0; i < n; i++) {
                if (i && a[p[i - 1]] != a[p[i]]) cls++;
                c[l][p[i]] = cls;
            }
        }
    }

    size_t Csize() { return c.size(); }

    const int getC(int lg, int pos) { return c[lg][pos]; }
    const int getP(int pos) { return p[pos]; }
};