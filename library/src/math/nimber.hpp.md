---
layout: default
---

<!-- mathjax config similar to math.stackexchange -->
<script type="text/javascript" async
  src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.5/MathJax.js?config=TeX-MML-AM_CHTML">
</script>
<script type="text/x-mathjax-config">
  MathJax.Hub.Config({
    TeX: { equationNumbers: { autoNumber: "AMS" }},
    tex2jax: {
      inlineMath: [ ['$','$'] ],
      processEscapes: true
    },
    "HTML-CSS": { matchFontHeight: false },
    displayAlign: "left",
    displayIndent: "2em"
  });
</script>

<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
<script src="https://cdn.jsdelivr.net/npm/jquery-balloon-js@1.1.2/jquery.balloon.min.js" integrity="sha256-ZEYs9VrgAeNuPvs15E39OsyOJaIkXEEt10fzxJ20+2I=" crossorigin="anonymous"></script>
<script type="text/javascript" src="../../../assets/js/copy-button.js"></script>
<link rel="stylesheet" href="../../../assets/css/copy-button.css" />


# :heavy_check_mark: src/math/nimber.hpp

<a href="../../../index.html">Back to top page</a>

* category: <a href="../../../index.html#fb2ef479237c7a939531a404fd0e5cb7">src/math</a>
* <a href="{{ site.github.repository_url }}/blob/master/src/math/nimber.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-23 18:37:50+09:00




## Depends on

* :heavy_check_mark: <a href="../base.hpp.html">src/base.hpp</a>


## Verified with

* :heavy_check_mark: <a href="../../../verify/src/nimber.test.cpp.html">src/nimber.test.cpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#pragma once

#include "base.hpp"

struct Nimber64;
Nimber64 mul_naive(Nimber64 l, Nimber64 r);
struct Nimber64 {
    const static V<ull> factor;
    const static array<array<unsigned char, 256>, 256> small;
    const static array<array<array<Nimber64, 256>, 8>, 8> precalc;
    ull v;
    Nimber64() : v(0) {}
    Nimber64(ull _v) : v(_v) {}
    const Nimber64 operator+(Nimber64 r) const { return v ^ r.v; }
    const Nimber64 operator*(Nimber64 r) const {
        Nimber64 ans;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                ull x = (v >> (8 * i)) % 256;
                ull y = (r.v >> (8 * j)) % 256;
                ans += precalc[i][j][small[x][y]];
            }
        }
        return ans;
    }
    bool operator==(Nimber64 r) const { return v == r.v; }
    Nimber64& operator+=(Nimber64 r) { return *this = *this + r; }
    Nimber64& operator*=(Nimber64 r) { return *this = *this * r; }

    Nimber64 pow(ull n) const {
        Nimber64 x = *this, r = 1;
        while (n) {
            if (n & 1) r = r * x;
            x = x * x;
            n >>= 1;
        }
        return r;
    }

    ull discrete_logarithm(Nimber64 y) {
        ull rem = 0, mod = 1;
        for (ull p : factor) {
            ull STEP = 1;
            while (4 * STEP * STEP < p) STEP *= 2;
            auto inside = [&](Nimber64 x, Nimber64 z) {
                unordered_map<ull, int> mp;
                Nimber64 big = 1;  // x^m
                for (int i = 0; i < int(STEP); i++) {
                    mp[z.v] = i;
                    z *= x;
                    big *= x;
                }
                Nimber64 now = 1;
                for (int step = 0; step < int(p + 10); step += STEP) {
                    now *= big;
                    // check [step + 1, step + STEP]
                    if (mp.find(now.v) != mp.end()) {
                        return (step + STEP) - mp[now.v];
                    }
                }
                return ull(-1);
            };

            ull q = ull(-1) / p;
            ull res = inside((*this).pow(q), y.pow(q));
            if (res == ull(-1)) {
                return ull(-1);
            }
            res %= p;
            // mod p = v
            if (mod == 1) {
                rem = res;
                mod = p;
            } else {
                while (rem % p != res) rem += mod;
                mod *= p;
            }
        }
        return rem;
    }

    bool is_primitive_root() const {
        for (ull p : factor) {
            if ((*this).pow(ull(-1) / p).v == 1) return false;
        }
        return true;
    }
};
const V<ull> Nimber64::factor = {
    6700417, 65537, 641, 257, 17, 5, 3,
};

Nimber64 mul_naive(Nimber64 l, Nimber64 r) {
    ull a = l.v, b = r.v;
    if (a < b) swap(a, b);
    if (b == 0) return 0;
    if (b == 1) return a;
    int p = 32;
    while (max(a, b) < (1ULL << p)) p /= 2;
    ull power = 1ULL << p;
    if (a >= power && b >= power) {
        Nimber64 ans;
        ans += mul_naive(a % power, b % power);
        ans += mul_naive(a / power, b % power).v * power;
        ans += mul_naive(a % power, b / power).v * power;
        auto x = mul_naive(a / power, b / power);
        ans += x.v * power;
        ans += mul_naive(x.v, power / 2);
        return ans;
    } else {
        return Nimber64(mul_naive(a / power, b).v * power) +
               mul_naive(a % power, b);
    }
};

const array<array<unsigned char, 256>, 256> Nimber64::small = []() {
    array<array<unsigned char, 256>, 256> small;
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            small[i][j] = (unsigned char)(mul_naive(i, j).v);
        }
    }
    return small;
}();

const array<array<array<Nimber64, 256>, 8>, 8> Nimber64::precalc = []() {
    array<array<array<Nimber64, 256>, 8>, 8> precalc;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 256; k++) {
                precalc[i][j][k] =
                    mul_naive(mul_naive(1ULL << (8 * i), 1ULL << (8 * j)), k);
            }
        }
    }
    return precalc;
}();

```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
Traceback (most recent call last):
  File "/opt/hostedtoolcache/Python/3.8.3/x64/lib/python3.8/site-packages/onlinejudge_verify/docs.py", line 349, in write_contents
    bundled_code = language.bundle(self.file_class.file_path, basedir=pathlib.Path.cwd())
  File "/opt/hostedtoolcache/Python/3.8.3/x64/lib/python3.8/site-packages/onlinejudge_verify/languages/cplusplus.py", line 185, in bundle
    bundler.update(path)
  File "/opt/hostedtoolcache/Python/3.8.3/x64/lib/python3.8/site-packages/onlinejudge_verify/languages/cplusplus_bundle.py", line 307, in update
    self.update(self._resolve(pathlib.Path(included), included_from=path))
  File "/opt/hostedtoolcache/Python/3.8.3/x64/lib/python3.8/site-packages/onlinejudge_verify/languages/cplusplus_bundle.py", line 187, in _resolve
    raise BundleErrorAt(path, -1, "no such header")
onlinejudge_verify.languages.cplusplus_bundle.BundleErrorAt: base.hpp: line -1: no such header

```
{% endraw %}

<a href="../../../index.html">Back to top page</a>
