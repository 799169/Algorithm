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


# :warning: src/math/matrix.hpp

<a href="../../../index.html">Back to top page</a>

* category: <a href="../../../index.html#fb2ef479237c7a939531a404fd0e5cb7">src/math</a>
* <a href="{{ site.github.repository_url }}/blob/master/src/math/matrix.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-22 19:22:07+09:00




## Depends on

* :heavy_check_mark: <a href="../bitop.hpp.html">src/bitop.hpp</a>
* :warning: <a href="../datastructure/bitvector.hpp.html">src/datastructure/bitvector.hpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#include "datastructure/bitvector.hpp"

template <class D> struct Mat : VV<D> {
    using VV<D>::VV;
    using VV<D>::size;
    int h() const { return int(size()); }
    int w() const { return int((*this)[0].size()); }
    Mat operator*(const Mat& r) const {
        assert(w() == r.h());
        Mat res(h(), V<D>(r.w()));
        for (int i = 0; i < h(); i++) {
            for (int j = 0; j < r.w(); j++) {
                for (int k = 0; k < w(); k++) {
                    res[i][j] += (*this)[i][k] * r[k][j];
                }
            }
        }
        return res;
    }
    Mat& operator*=(const Mat& r) { return *this = *this * r; }
    Mat pow(ll n) const {
        assert(h() == w());
        Mat x = *this, r(h(), V<D>(w()));
        for (int i = 0; i < h(); i++) r[i][i] = D(1);
        while (n) {
            if (n & 1) r *= x;
            x *= x;
            n >>= 1;
        }
        return r;
    }
};

template <class D> V<D> solve_linear(Mat<D> a, V<D> b, D eps) {
    int h = a.h(), w = a.w();
    int r = 0;
    V<int> idxs;
    for (int x = 0; x < w; x++) {
        for (int y = r + 1; y < h; y++) {
            D d = hypot(a[r][x], a[y][x]);
            if (abs(d) <= eps) continue;
            D c = a[r][x] / d, s = a[y][x] / d;
            auto rot = [&](D& u, D& v) {
                tie(u, v) = make_pair(c * u + s * v, c * v - s * u);
            };
            rot(b[r], b[y]);
            for (int k = x; k < w; k++) rot(a[r][k], a[y][k]);
        }
        if (a[r][x] <= eps) continue;
        r++;
        idxs.push_back(x);
        if (r == h) break;
    }
    V<D> v(w);
    for (int y = r - 1; y >= 0; y--) {
        int f = idxs[y];
        v[f] = b[y];
        for (int x = f + 1; x < w; x++) {
            v[f] -= a[y][x] * v[x];
        }
        v[f] /= a[y][f];
    }
    return v;
}

template <class Mint> V<Mint> solve_linear(Mat<Mint> a, V<Mint> b) {
    int h = a.h(), w = a.w();
    int r = 0;
    V<int> idxs;
    for (int x = 0; x < w; x++) {
        int my = -1;
        for (int y = r; y < h; y++) {
            if (a[y][x]) {
                my = y;
                break;
            }
        }
        if (my == -1) continue;
        if (r != my) swap(a[r], a[my]);
        swap(b[r], b[my]);
        for (int y = r + 1; y < h; y++) {
            if (!a[y][x]) continue;
            auto freq = a[y][x] / a[r][x];
            for (int k = x; k < w; k++) a[y][k] -= freq * a[r][k];
            b[y] -= freq * b[r];
        }
        r++;
        idxs.push_back(x);
        if (r == h) break;
    }
    V<Mint> v(w);
    for (int y = r - 1; y >= 0; y--) {
        int f = idxs[y];
        v[f] = b[y];
        for (int x = f + 1; x < w; x++) {
            v[f] -= a[y][x] * v[x];
        }
        v[f] /= a[y][f];
    }
    return v;
}

template <class Mint> int calc_rank(Mat<Mint> a) {
    int h = a.h(), w = a.w();
    int r = 0;
    V<int> idxs;
    for (int x = 0; x < w; x++) {
        int my = -1;
        for (int y = r; y < h; y++) {
            if (a[y][x]) {
                my = y;
                break;
            }
        }
        if (my == -1) continue;
        if (r != my) swap(a[r], a[my]);
        for (int y = r + 1; y < h; y++) {
            if (!a[y][x]) continue;
            auto freq = a[y][x] / a[r][x];
            for (int k = x; k < w; k++) a[y][k] -= freq * a[r][k];
        }
        r++;
        idxs.push_back(x);
        if (r == h) break;
    }
    return r;
}

template <class Mint> Mint calc_det(Mat<Mint> a) {
    assert(a.h() == a.w());
    int n = a.h();

    bool flip = false;
    for (int x = 0; x < n; x++) {
        int my = -1;
        for (int y = x; y < n; y++) {
            if (a[y][x]) {
                my = y;
                break;
            }
        }
        if (my == -1) return 0;
        if (x != my) {
            swap(a[x], a[my]);
            if ((x - my) % 2) flip = !flip;
        }
        for (int y = x + 1; y < n; y++) {
            if (!a[y][x]) continue;
            auto freq = a[y][x] / a[x][x];
            for (int k = x; k < n; k++) a[y][k] -= freq * a[x][k];
        }
    }
    Mint det = (!flip ? 1 : -1);
    for (int i = 0; i < n; i++) {
        det *= a[i][i];
    }
    return det;
}

template <class Mint> Mat<Mint> inverse(Mat<Mint> a) {
    assert(a.h() == a.w());
    int n = a.h();

    Mat<Mint> b(n, V<Mint>(n));
    for (int i = 0; i < n; i++) b[i][i] = 1;

    for (int x = 0; x < n; x++) {
        int my = -1;
        for (int y = x; y < n; y++) {
            if (a[y][x]) {
                my = y;
                break;
            }
        }
        if (my == -1) return {};
        if (x != my) {
            swap(a[x], a[my]);
            swap(b[x], b[my]);
        }
        auto freq = a[x][x];
        for (int j = 0; j < n; j++) {
            a[x][j] /= freq;
            b[x][j] /= freq;
        }
        for (int y = 0; y < n; y++) {
            if (x == y) continue;
            if (!a[y][x]) continue;
            freq = a[y][x];
            for (int k = 0; k < n; k++) a[y][k] -= freq * a[x][k];
            for (int k = 0; k < n; k++) b[y][k] -= freq * b[x][k];
        }
    }
    return b;
}

struct Mat2 : V<BitVec> {
    using V<BitVec>::V;
    using V<BitVec>::size;
    int h() const { return int(size()); }
    int w() const { return int((*this)[0].size()); }
    Mat2 operator*(const Mat2& r) const {
        assert(w() == r.h());
        Mat2 r_t = Mat2(r.h(), BitVec(r.w()));
        for (int y = 0; y < r_t.h(); y++) {
            for (int x = 0; x < r_t.w(); x++) {
                r_t[y].set(x, r[x][y]);
            }
        }
        Mat2 res(h(), BitVec(r_t.h()));
        for (int i = 0; i < h(); i++) {
            for (int j = 0; j < r_t.h(); j++) {
                res[i].set(j, ((*this)[i] ^ r_t[j]).count() % 2 == 1);
            }
        }
        return res;
    }
};

int calc_rank(Mat2 a) {
    int h = a.h(), w = a.w();
    int r = 0;
    V<int> idxs;
    for (int x = 0; x < w; x++) {
        int my = -1;
        for (int y = r; y < h; y++) {
            if (a[y][x]) {
                my = y;
                break;
            }
        }
        if (my == -1) continue;
        if (r != my) swap(a[r], a[my]);
        for (int y = r + 1; y < h; y++) {
            if (!a[y][x]) continue;
            a[y] ^= a[r];
        }
        r++;
        idxs.push_back(x);
        if (r == h) break;
    }
    return r;
}

BitVec solve_linear(Mat2 a, BitVec b) {
    int h = a.h(), w = a.w();
    int r = 0;
    V<int> idxs;
    for (int x = 0; x < w; x++) {
        int my = -1;
        for (int y = r; y < h; y++) {
            if (a[y][x]) {
                my = y;
                break;
            }
        }
        if (my == -1) continue;
        if (r != my) swap(a[r], a[my]);
        b.swap_elms(r, my);
        for (int y = r + 1; y < h; y++) {
            if (!a[y][x]) continue;
            a[y] ^= a[r];
            b.set(y, b[y] ^ b[r]);
        }
        r++;
        idxs.push_back(x);
        if (r == h) break;
    }
    BitVec v(w);
    for (int y = r - 1; y >= 0; y--) {
        int f = idxs[y];
        v.set(f, b[y]);
        for (int x = f + 1; x < w; x++) {
            v.set(f, v[f] ^ (a[y][x] && v[x]));
        }
    }
    return v;
}

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
onlinejudge_verify.languages.cplusplus_bundle.BundleErrorAt: datastructure/bitvector.hpp: line -1: no such header

```
{% endraw %}

<a href="../../../index.html">Back to top page</a>
