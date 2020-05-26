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


# :heavy_check_mark: src/datastructure/hashset.hpp

<a href="../../../index.html">Back to top page</a>

* category: <a href="../../../index.html#057cdb199a48f765d2786c323ec11d3a">src/datastructure</a>
* <a href="{{ site.github.repository_url }}/blob/master/src/datastructure/hashset.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-26 03:02:59+09:00




## Depends on

* :heavy_check_mark: <a href="../base.hpp.html">src/base.hpp</a>
* :heavy_check_mark: <a href="../util/hash.hpp.html">src/util/hash.hpp</a>
* :heavy_check_mark: <a href="../util/random.hpp.html">src/util/random.hpp</a>


## Required by

* :warning: <a href="../graph/balancedseparator.hpp.html">src/graph/balancedseparator.hpp</a>
* :heavy_check_mark: <a href="../graph/treedecomp.hpp.html">src/graph/treedecomp.hpp</a>


## Verified with

* :heavy_check_mark: <a href="../../../verify/src/hashset.test.cpp.html">src/hashset.test.cpp</a>
* :heavy_check_mark: <a href="../../../verify/src/hashset_hashmap.test.cpp.html">src/hashset_hashmap.test.cpp</a>
* :heavy_check_mark: <a href="../../../verify/src/treedecomp_width2.test.cpp.html">src/treedecomp_width2.test.cpp</a>


## Code

<a id="unbundled"></a>
{% raw %}
```cpp
#pragma once

#include "util/hash.hpp"

template <class K, class H = Hasher<>> struct HashSet {
    using P = pair<unsigned char, K>;
    uint s, mask, filled, sz;  // data.size() == 1 << s
    P* key;

    HashSet(uint _s = 2) : s(_s), mask((1U << s) - 1), filled(0), sz(0) {
        key = new P[1 << s];
    }

    void rehash() {
        uint pmask = mask;
        P* pkey = key;
        s++;
        mask = (1U << s) - 1;
        filled = 0;
        sz = 0;
        key = new P[1 << s];
        for (uint i = 0; i <= pmask; i++) {
            if (pkey[i].first == 1) {
                insert(pkey[i].second);
            }
        }
        delete[] pkey;
    }

    bool count(K k) {
        uint id = H::hash(k) & mask;
        int gc = 0;
        while (key[id].first && key[id].second != k) {
            gc++;
            id = (id + 1) & mask;
        }
        if (key[id].first != 1 || key[id].second != k) return false;
        return true;
    }

    void insert(K k) {
        uint id = H::hash(k) & mask;
        while (key[id].first && key[id].second != k) id = (id + 1) & mask;
        if (key[id].first == 0) {
            filled++;
            if (filled * 2 > mask) {
                rehash();
                insert(k);
                return;
            }
        }
        if (key[id].first != 1) sz++;
        key[id] = {1, k};
    }

    bool erase(K k) {
        uint id = H::hash(k) & mask;
        while (key[id].first && key[id].second != k) id = (id + 1) & mask;
        if (key[id].first != 1) return false;
        sz--;
        key[id].first = 2;
        return true;
    }

    size_t size() const {
        return sz;
    }

    V<int> to_vec() const {
        V<int> result;
        for (uint i = 0; i <= mask; i++) {
            if (key[i].first == 1) {
                result.push_back(key[i].second);
            }
        }
        return result;
    }
};

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
onlinejudge_verify.languages.cplusplus_bundle.BundleErrorAt: util/hash.hpp: line -1: no such header

```
{% endraw %}

<a href="../../../index.html">Back to top page</a>
