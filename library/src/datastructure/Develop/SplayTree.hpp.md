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
<script type="text/javascript" src="../../../../assets/js/copy-button.js"></script>
<link rel="stylesheet" href="../../../../assets/css/copy-button.css" />


# :warning: src/datastructure/Develop/SplayTree.hpp

<a href="../../../../index.html">Back to top page</a>

* category: <a href="../../../../index.html#6ded3f220a7ec9530d5ce67338fd1fda">src/datastructure/Develop</a>
* <a href="{{ site.github.repository_url }}/blob/master/src/datastructure/Develop/SplayTree.hpp">View this file on GitHub</a>
    - Last commit date: 2020-05-08 21:35:33+09:00




## Code

<a id="unbundled"></a>
{% raw %}
```cpp
struct NTree {
    using D = ll;
    struct Node;
    using NP = Node*;
    static Node last_d;
    static NP last;
    struct Node {
        NP p, l, r;
        int sz;
        D v, sm;
        Node(D v) :p(nullptr), l(last), r(last), sz(1), v(v), sm(v) {}
        Node() : l(nullptr), r(nullptr), sz(0), v(0), sm(0) {}
        inline int pos() {
            if (p) {
                if (p->l == this) return -1;
                if (p->r == this) return 1;
            }
            return 0;
        }
        void rot() {
            NP qq = p->p;
            int pps = p->pos();
            if (p->l == this) {
                p->l = r; r->p = p;
                r = p; p->p = this;
            } else {
                p->r = l; l->p = p;
                l = p; p->p = this;
            }
            p->update(); update();
            p = qq;
            if (!pps) return;
            if (pps == -1) {
                qq->l = this;
            } else {
                qq->r = this;
            }
            qq->update();
        }
        void splay() {
            assert(this != last);
            supush();
            int ps;
            while ((ps = pos())) {
                int pps = p->pos();
                if (!pps) {
                    rot();
                } else if (ps == pps) {
                    p->rot(); rot();
                } else {
                    rot(); rot();
                }
            }
        }
        NP splay(int k) {
            assert(this != last);
            assert(0 <= k && k < sz);
            if (k < l->sz) {
                return l->splay(k);
            } else if (k == l->sz) {
                splay();
                return this;
            } else {
                return r->splay(k-(l->sz+1));
            }
        }
        void supush() {
            if (pos()) {
                p->supush();
            }
            push();
        }
        //pushをすると、pushをした頂点とその子の"すべて"の値の正当性が保証される
        void push() { 
            assert(sz);
        }
        NP update() {
            assert(this != last);
            sz = 1+l->sz+r->sz;
            sm = v;
            if (l->sz) {
                sm += l->sm;
            }
            if (r->sz) {
                sm += r->sm;
            }
            return this;
        }
    } *n;
    static NP merge(NP l, NP r) {
        if (r == last) {
            return l;
        }
        r = r->splay(0);
        assert(r->l == last);
        r->l = l;
        l->p = r;
        return r->update();
    }
    static pair<NP, NP> split(NP x, int k) {
        assert(0 <= k && k <= x->sz);
        if (k == x->sz) {
            return {x, last};
        }
        x = x->splay(k);
        NP l = x->l;
        l->p = nullptr;
        x->l = last;
        return {l, x->update()};
    }
    static NP built(int sz, ll d[]) {
        if (!sz) return last;
        int md = sz/2;
        NP x = new Node(d[md]);
        x->l = built(md, d);
        x->l->p = x;
        x->r = built(sz-(md+1), d+(md+1));
        x->r->p = x;
        return x->update();
    }
    NTree() : n(last) {}
    NTree(NP n) : n(n) {}
    NTree(int sz, D d[]) {
        n = built(sz, d);
    }
    int sz() {
        return n->sz;
    }
    void insert(int k, D v) {
        auto u = split(n, k);
        n = merge(merge(u.first, new Node(v)), u.second);
    }
    void erase(int k) {
        auto u = split(n, k);
        n = merge(u.first, split(u.second, 1).second);
    }
    void merge(NTree t) {
        n = merge(n, t.n);
    }
    NTree split(int l) {
        auto a = split(n, l);
        n = a.first;
        return NTree(a.second);
    }
    D get(int l) {
        auto a = split(n, l);
        auto b = split(a.second, 1);
        D res = b.first->v;
        n = merge(merge(a.first, b.first), b.second);
        return res;
    }
    D sum(int l, int r) {
        auto b = split(n, r);
        auto a = split(b.first, l);
        D res = a.second->sm;
        n = merge(merge(a.first, a.second), b.second);
        return res;
    }
};
NTree::Node NTree::last_d = NTree::Node();
NTree::NP NTree::last = &last_d;
```
{% endraw %}

<a id="bundled"></a>
{% raw %}
```cpp
#line 1 "src/datastructure/Develop/SplayTree.hpp"
struct NTree {
    using D = ll;
    struct Node;
    using NP = Node*;
    static Node last_d;
    static NP last;
    struct Node {
        NP p, l, r;
        int sz;
        D v, sm;
        Node(D v) :p(nullptr), l(last), r(last), sz(1), v(v), sm(v) {}
        Node() : l(nullptr), r(nullptr), sz(0), v(0), sm(0) {}
        inline int pos() {
            if (p) {
                if (p->l == this) return -1;
                if (p->r == this) return 1;
            }
            return 0;
        }
        void rot() {
            NP qq = p->p;
            int pps = p->pos();
            if (p->l == this) {
                p->l = r; r->p = p;
                r = p; p->p = this;
            } else {
                p->r = l; l->p = p;
                l = p; p->p = this;
            }
            p->update(); update();
            p = qq;
            if (!pps) return;
            if (pps == -1) {
                qq->l = this;
            } else {
                qq->r = this;
            }
            qq->update();
        }
        void splay() {
            assert(this != last);
            supush();
            int ps;
            while ((ps = pos())) {
                int pps = p->pos();
                if (!pps) {
                    rot();
                } else if (ps == pps) {
                    p->rot(); rot();
                } else {
                    rot(); rot();
                }
            }
        }
        NP splay(int k) {
            assert(this != last);
            assert(0 <= k && k < sz);
            if (k < l->sz) {
                return l->splay(k);
            } else if (k == l->sz) {
                splay();
                return this;
            } else {
                return r->splay(k-(l->sz+1));
            }
        }
        void supush() {
            if (pos()) {
                p->supush();
            }
            push();
        }
        //pushをすると、pushをした頂点とその子の"すべて"の値の正当性が保証される
        void push() { 
            assert(sz);
        }
        NP update() {
            assert(this != last);
            sz = 1+l->sz+r->sz;
            sm = v;
            if (l->sz) {
                sm += l->sm;
            }
            if (r->sz) {
                sm += r->sm;
            }
            return this;
        }
    } *n;
    static NP merge(NP l, NP r) {
        if (r == last) {
            return l;
        }
        r = r->splay(0);
        assert(r->l == last);
        r->l = l;
        l->p = r;
        return r->update();
    }
    static pair<NP, NP> split(NP x, int k) {
        assert(0 <= k && k <= x->sz);
        if (k == x->sz) {
            return {x, last};
        }
        x = x->splay(k);
        NP l = x->l;
        l->p = nullptr;
        x->l = last;
        return {l, x->update()};
    }
    static NP built(int sz, ll d[]) {
        if (!sz) return last;
        int md = sz/2;
        NP x = new Node(d[md]);
        x->l = built(md, d);
        x->l->p = x;
        x->r = built(sz-(md+1), d+(md+1));
        x->r->p = x;
        return x->update();
    }
    NTree() : n(last) {}
    NTree(NP n) : n(n) {}
    NTree(int sz, D d[]) {
        n = built(sz, d);
    }
    int sz() {
        return n->sz;
    }
    void insert(int k, D v) {
        auto u = split(n, k);
        n = merge(merge(u.first, new Node(v)), u.second);
    }
    void erase(int k) {
        auto u = split(n, k);
        n = merge(u.first, split(u.second, 1).second);
    }
    void merge(NTree t) {
        n = merge(n, t.n);
    }
    NTree split(int l) {
        auto a = split(n, l);
        n = a.first;
        return NTree(a.second);
    }
    D get(int l) {
        auto a = split(n, l);
        auto b = split(a.second, 1);
        D res = b.first->v;
        n = merge(merge(a.first, b.first), b.second);
        return res;
    }
    D sum(int l, int r) {
        auto b = split(n, r);
        auto a = split(b.first, l);
        D res = a.second->sm;
        n = merge(merge(a.first, a.second), b.second);
        return res;
    }
};
NTree::Node NTree::last_d = NTree::Node();
NTree::NP NTree::last = &last_d;

```
{% endraw %}

<a href="../../../../index.html">Back to top page</a>
