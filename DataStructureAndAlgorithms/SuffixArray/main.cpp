#include <iostream>
#include "SuffixArray.h"

using namespace std;

int32_t main() {
    vector<int> a = {5, 1, 3, 2, 1};
    SuffixArray SA(a);

    for (int i = 0; i < 5; i++) {
        cout << SA.getP(i) << " ";
    }
    cout << "\n";

    return 0;
}