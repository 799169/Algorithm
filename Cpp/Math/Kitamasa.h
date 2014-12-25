template<ll MD>
struct Kitamasa {
    int k;
    ll *d, *res, *buf;
    Kitamasa(int k) :k(k) {
        d = new ll[k];
        res = new ll[k];
        buf = new ll[2*k];
    }
    ~Kitamasa() {
        delete[] d;
        delete[] res;
        delete[] buf;
    }
    void plus() {
        rotate(res, res+(k-1), res+k);
        ll l = res[0]; res[0] = 0;
        for (int i = 0; i < k; i++) {
            res[i] += d[i]*l;
            res[i] %= MD;
        }
    }
    void dbl() {
        fill_n(buf, 2*k, 0);
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < k; j++) {
                buf[i+j] += res[i]*res[j];
                buf[i+j] %= MD;
            }
        }
        copy(d, d+k, res);
        for (int i = k; i < 2*k; i++) {
            for (int j = 0; j < k; j++) {
                buf[j] += buf[i]*res[j];
                buf[j] %= MD;
            }
            plus();
        }
        copy(buf, buf+k, res);
    }
    void exec(ll n) {
        fill_n(res, k, 0);
        res[0] = 1;
        for (ll i = 62; i >= 0; i--) {
            dbl();
            if ((n&(1LL<<i))) {
                plus();
            }
        }
    }
};