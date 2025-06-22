/*
HLD answer path queries on trees in log^2(n)
*/


int id[200100] , sz[200100], p[200100], top[200100], arr[200100], dep[200100];
vector<int> gr[200100];
vector<int> v;
// Tree sg(200100);  (Include Lazy seg tree or normal seg tree according to the question. 
 
 
int getsz(int node = 1, int par = 1, int d = 0){
    dep[node] = d;
    p[node] = par;
    int t = 1;
    for(auto s : gr[node]){
        if(s != par){
            t += getsz(s , node, d + 1);
        }
    }
    return sz[node] = t;
}
void init_hld(int node = 1, int par = 1, int t = 1){
    top[node] = t;
    id[node] = v.size();
    v.push_back(arr[node]);
    sg.update(id[node] , v[id[node]]);
    int mx = 0, bch = -1;
    for(auto s: gr[node]){
        if(s!= par){
            if(sz[s] > mx){
                mx = sz[s];
                bch = s;
            }
        }
    }
    if(bch == -1)
        return;
    init_hld(bch , node, t);
    for(auto s : gr[node]){
        if(s != par && s != bch){
            init_hld(s , node, s);
        }
    }
}
 
int ans(int a, int b){
    int ret = 0;
    while(top[a] != top[b]){
        if(dep[top[a]] < dep[top[b]])
            swap(a,b);
        ret = max(ret, sg.query(id[top[a]] , id[a] + 1));
        a = p[top[a]];
    }
    if(dep[a] > dep[b])
        swap(a,b);
    ret = max(ret, sg.query(id[a] , id[b] + 1));
    return ret;
}
