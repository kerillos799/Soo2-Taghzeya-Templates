/*
  persistent segment tree used to track previous changes and rollback to a certain point in time.
  verified using: https://cses.fi/problemset/task/1737
*/

#define int ll

struct pSeg{
    struct node{
        ll sum = 0;
        node *l , *r;
        node(ll val): sum(val), l(nullptr), r(nullptr){}
        node(node *l , node *r): l(l), r(r){
            sum = l->sum + r->sum;
        }
        node(node * cp): sum(cp->sum), l(cp->l), r(cp->r){}
    };
    vector<node*> root;
    int sz;

private:
    node* build(int l, int r, vector<ll> &a){
        if(l == r){
            return new node(a[l]);
        }
        int m = (l+r)/2;
        return new node(build(l,m,a) , build(m+1 , r, a));
    }
    node* set(node *v , int l, int r, int pos, ll val){
        if(l == r){
            return new node(val);
        }
        int m = (l + r)/2;
        if(pos <= m) return new node(set(v->l , l , m , pos , val), v->r);
        else return new node(v->l , set(v->r, m + 1, r , pos, val));
    }
    ll range_sum(node *v, int l, int r, int l1, int r1){
        if(r1 < l || l1 > r)
            return 0;
        if(r <= r1 && l >= l1)
            return v->sum;
        int m = (l+r)/2;
        return range_sum(v->l, l, m, l1, r1) + range_sum(v->r , m + 1, r, l1, r1);
    }
public:
    pSeg(vector<ll> arr): sz(arr.size()){
        root.push_back(build(0 , sz - 1,arr));
    }
    void set(int idx, int pos, ll val){
        root[idx] = set(root[idx] , 0, sz-1, pos, val);
    }
    ll range_sum(int idx, int l, int r){
        return range_sum(root[idx] , 0 , sz-1, l , r);
    }
    void copy(int idx){
        root.push_back(new node(root[idx]));
    }
};
