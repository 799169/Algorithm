#define PROBLEM "https://judge.yosupo.jp/problem/zalgorithm"

#include "base.hpp"
#include "util/fast_io.hpp"
#include "string/onlinez.hpp"

int main() {
    Scanner sc(stdin);
    Printer pr(stdout);
    string s;
    sc.read(s);
    int n = int(s.size());
    s += "$";

    V<int> z(n, -1);
    z[0] = n;
    OnlineZ oz;
    for (int i = 0; i <= n; i++) {
        auto v = oz.inc(s[i]);
        for (int x: v) {
            z[x] = i - x;
        }
    }
    pr.writeln(z);
}
