/*
recursive segment tree [l1,r1]
tested using https://cses.fi/problemset/task/1648/
*/
struct seg_tree{
    vector<ll> tree;
    int sz;
    seg_tree(int n, vector<ll> &arr) : sz(arr.size()) , tree(4*n){
        build(0 , sz - 1, 1, arr);
    }
    void build(int l, int r, int node, vector<ll> &a){
        if(l == r){
            tree[node] = a[l];
        }else{
            int mid = (l+r)/2;
            build(l , mid, 2*node , a);
            build(mid + 1, r, 2*node + 1 , a);
            tree[node] = combine(tree[2*node] , tree[2*node + 1]);
        }
    }
    ll combine (ll a, ll b){
        return a + b;
    }
    void set(int idx, ll  val, int l , int r, int node){
        if(l == r){
            tree[node] = val;
        }else {
            int mid = (l + r) / 2;
            if (idx <= mid) {
                set(idx, val, l, mid, node * 2);
            } else {
                set(idx, val, mid + 1, r, node * 2 + 1);
            }
            tree[node] = combine(tree[2 * node], tree[2 * node + 1]);
        }
    }
    void set(int idx, ll val){
        set(idx, val, 0 , sz - 1, 1);
    }

    ll get(int l1, int r1, int l, int r, int node){
        if (r1 < l || l1 > r) { return 0; }
        if (l >= l1 && r <= r1) { return tree[node]; }
        int m = (l + r) / 2;
        return get(l1 , r1 , l , m, 2*node) + get(l1 , r1 , m + 1 , r, 1 + 2*node);
    }

    ll get(int l , int r){
        return get(l , r , 0 , sz - 1, 1);
    }
};
