using ll = long long;
static inline uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}
static const int HSIZE = 1 << 18;
static const int HMASK = HSIZE - 1;
static const ll EMPTY = LLONG_MIN;
static int hcount;  
static ll  hkeys[HSIZE];
static int hvals[HSIZE];
static void hash_init() {
    for (int i = 0; i < HSIZE; i++) {
        hkeys[i] = EMPTY;
        hvals[i] = 0;
    }
}
static inline int &hash_ref(ll key) {
    uint64_t mixed = splitmix64((uint64_t)key);
    int idx = (int)(mixed & HMASK);
    while (true) {
        if (hkeys[idx] == EMPTY) {
            hkeys[idx] = key;
            hvals[idx] = 0;
            return hvals[idx];
        }
        if (hkeys[idx] == key) {
            return hvals[idx];
        }
        idx = (idx + 1) & HMASK;
    }
}
static inline int hash_get(ll key) {
    uint64_t mixed = splitmix64((uint64_t)key);
    int idx = (int)(mixed & HMASK);
    while (true) {
        if (hkeys[idx] == EMPTY) {
            return 0;
        }
        if (hkeys[idx] == key) {
            return hvals[idx];
        }
        idx = (idx + 1) & HMASK;
    }
}
static inline void hash_decrement(ll key) {
    uint64_t mixed = splitmix64((uint64_t)key);
    int idx = (int)(mixed & HMASK);
    while (true) {
        if (hkeys[idx] == key) {
            hvals[idx]--;
            return;
        }
        idx = (idx + 1) & HMASK;
    }
}
static inline bool hash_contains(ll key) {
    uint64_t mixed = splitmix64((uint64_t)key);
    int idx = (int)(mixed & HMASK);
    while (hkeys[idx] != EMPTY) {
        if (hkeys[idx] == key) return true;
        idx = (idx + 1) & HMASK;
    }
    return false;
}
static inline bool hash_erase(ll key) {
    uint64_t mixed = splitmix64((uint64_t)key);
    int idx = (int)(mixed & HMASK);
    while (hkeys[idx] != EMPTY && hkeys[idx] != key) {
        idx = (idx + 1) & HMASK;
    }
    if (hkeys[idx] == EMPTY) return false; 
    int next = (idx + 1) & HMASK;
    while (hkeys[next] != EMPTY) {
        uint64_t ideal = splitmix64((uint64_t)hkeys[next]) & HMASK;
        if (((next - ideal + HSIZE) & HMASK)> ((next - idx   + HSIZE) & HMASK)){
            hkeys[idx] = hkeys[next];
            hvals[idx] = hvals[next];
            idx = next;
        }
        next = (next + 1) & HMASK;
    }
    hkeys[idx] = EMPTY;
    hvals[idx] = 0;
    hcount--;
    return true;
}
static inline int hash_size() { return hcount; }
static inline void hash_clear() { hash_init(); }
static vector<ll> hash_keys() {
    vector<ll> out;
    out.reserve(hcount);
    for (int i = 0; i < HSIZE; i++) {
        if (hkeys[i] != EMPTY) {
            out.push_back(hkeys[i]);
        }
    }
    return out;
}
static inline uint64_t pair_mix(uint64_t a, uint64_t b) {
    uint64_t h1 = splitmix64(a);
    uint64_t h2 = splitmix64(b ^ 0x9e3779b97f4a7c15ULL);
    return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL  + (h1 << 6) + (h1 >> 2));
}

static inline int &hash_ref_pair(ll a, ll b) {
    auto key = (ll)pair_mix((uint64_t)a, (uint64_t)b);
    return hash_ref(key);
}

static inline int  hash_get_pair(ll a, ll b) {
    auto key = (ll)pair_mix((uint64_t)a, (uint64_t)b);
    return hash_get(key);
}

static inline bool hash_contains_pair(ll a, ll b) {
    auto key = (ll)pair_mix((uint64_t)a, (uint64_t)b);
    return hash_contains(key);
}

static inline bool hash_erase_pair(ll a, ll b) {
    auto key = (ll)pair_mix((uint64_t)a, (uint64_t)b);
    return hash_erase(key);
}
static inline void hash_decrement_pair(ll a, ll b) {
    auto key = (ll)pair_mix((uint64_t)a, (uint64_t)b);
    hash_decrement(key);
}
//set 
static inline bool set_insert(ll key) {
    if (!hash_contains(key)) { hash_ref(key) = 1; return true; }
    return false;
}
static inline bool set_contains_key(ll key) { return hash_contains(key); }
static inline bool set_erase_key(ll key) { return hash_erase(key); }
static inline int set_size() { return hash_size(); }
static inline void set_clear() { hash_clear(); }
static inline vector<ll> set_elements() { return hash_keys(); }

/*
 * 1) init: firstly call hash_init(); 
 * 2) Basic Key -> int Map 
 *    hash_ref(ll key)
 *    hash_ref(key) = 42;
 *    hash_ref(key)++;
 *    hash_get(ll key)
 *    v = hash_get(key);
 *    hash_decrement(ll key)
 *    exmples
 *    // freq count:
 *    hash_init();
 *    for (ll x : data) { hash_ref(x)++; }
 *    cout << "count of 5 is " << hash_get(5) <<endl;
 *    vector<pair<ll,ll>> v = {{1,2},{3,4},{1,2},{5,6}};
 *    for (auto [x,y] : v) { hash_ref_pair(x,y)++; }
 *    cout << "count(1,2) = " << hash_get_pair(1,2) <<endl;
 *    cout << "contains(3,4)? " << hash_contains_pair(3,4) <<endl;
 *    // erase one:
 *    hash_erase_pair(1,2);
 *    cout << "count(1,2) = " << hash_get_pair(1,2) << endl;    
 *    // using as a set:
 *    set_insert(123);
 *    if (set_contains_key(123))cout << "123 is in the set" <<endl;
 *
 *    (Notes)
 *     capacity is fixed of HSIZE entries (here 2^18).there is no auto resize.
 /
 
| Pair‑key                       | map<pair<ll,ll>,ll>                   |
|--------------------------------|----------------------------------------|
| hash_ref_pair(a,b)             |  mp[{a,b}]                             |
| hash_get_pair(a,b)             | (auto it=mp.find({a,b}))!=mp.end() ? it->second : 0 |
| hash_contains_pair(a,b)        | mp.find({a,b}) != mp.end()             |
| hash_erase_pair(a,b)           | mp.erase({a,b})                        |
| hash_decrement_pair(a,b)       | mp[{a,b}]--;                           |

| Set                            | set<ll>                                |
|--------------------------------|----------------------------------------|
|  set_insert(key)               | auto [it, inserted] = s.insert(key);   |
|  set_contains_key(key)         | s.count(key)>0                         |
|  set_erase_key(key)            | s.erase(key)                           |
|  set_size()                    | s.size()                               |
|  set_clear()                   | s.clear()                              |
 */
