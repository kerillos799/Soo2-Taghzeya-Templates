
template <typename T> class LazySegtree {
private:
    const int sz;
    vector<T> tree;      // tree[i] = sum of this node's range
    vector<int> lazy;  // lazy[i] = lazy update for the range

    /** builds the segtree nodes */
    void build(int v, int l, int r, const vector<T> &a) {
        lazy[v] = 1;
        if (l == r) {
            tree[v] = a[l];
        } else {
            int m = (l + r) / 2;
            build(2 * v, l, m, a);
            build(2 * v + 1, m + 1, r, a);
            tree[v] = tree[2 * v] + tree[2 * v + 1];
        }
    }

    /** applies lazy update to tree[v], places update at lazy[v] */
    void apply(int v, int len, int x) {
        tree[v] *= x;
        lazy[v] *= x;
    }

    /** pushes down lazy update to children of v */
    void push_down(int v, int l, int r) {
        int m = (l + r) / 2;
        apply(2 * v, m - l + 1, lazy[v]);
        apply(2 * v + 1, r - m, lazy[v]);
        lazy[v] = 1;
    }

    void range_update(int v, int l, int r, int ql, int qr, int x) {
        if (qr < l || ql > r) { return; }
        if (ql <= l && r <= qr) {
            apply(v, r - l + 1, x);
        } else {
            push_down(v, l, r);
            int m = (l + r) / 2;
            range_update(2 * v, l, m, ql, qr, x);
            range_update(2 * v + 1, m + 1, r, ql, qr, x);
            tree[v] = tree[2 * v] + tree[2 * v + 1];
        }
    }

    T range_sum(int v, int l, int r, int ql, int qr) {
        if (qr < l || ql > r) { return 0; }
        if (l >= ql && r <= qr) { return tree[v]; }
        push_down(v, l, r);
        int m = (l + r) / 2;
        return range_sum(2 * v, l, m, ql, qr) + range_sum(2 * v + 1, m + 1, r, ql, qr);
    }

public:
    LazySegtree(const vector<T> &a) : sz(a.size()), tree(4 * sz), lazy(4 * sz) {
        build(1, 0, sz - 1, a);
    }

    /** updates [ql, qr] with the update x */
    void range_update(int ql, int qr, int x) {
        range_update(1, 0, sz - 1, ql, qr, x);
    }

    /** sum of array values on [ql, qr] */
    T range_sum(int ql, int qr) { return range_sum(1, 0, sz - 1, ql, qr);}
};
