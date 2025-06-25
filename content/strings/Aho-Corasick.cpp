struct AhoCorasick {
    static constexpr int ALPHA = 26;
    struct Node { // lower cases, adjust accordindly 
        array<int, ALPHA> next;
        int link, out_link;
        vector<int> out;
        Node() {
            next.fill(0);
            link = out_link = 0;
        }
    };
    vector<Node> trie;
    int P = 0;
    AhoCorasick() {
        trie.emplace_back();
    }
    static inline int idx(char c) {
        return c - 'a';
    }
    int addPattern(const string& s) { // add patterns as much we need
        int u = 0;
        for (char ch : s) {
            int c = idx(ch);
            if (!trie[u].next[c]) {
                trie[u].next[c] = trie.size();
                trie.emplace_back();
            }
            u = trie[u].next[c];
        }
        trie[u].out.push_back(P);
        return P++;
    }
    void build() { // call it after adding patterns
        queue<int> q;
        for (int c = 0; c < ALPHA; ++c) {
            int v = trie[0].next[c];
            if (v) {
                q.push(v);
                trie[v].link = 0;
            }
        }
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int c = 0; c < ALPHA; ++c) {
                int& v = trie[u].next[c];
                if (!v) {
                    v = trie[trie[u].link].next[c];
                } else {
                    trie[v].link = trie[trie[u].link].next[c];
                    int f = trie[v].link;
                    trie[v].out_link = trie[f].out.empty() ? trie[f].out_link : f;
                    q.push(v);
                }
            }
        }
    }
    int step(int u, char ch) const {
        return trie[u].next[idx(ch)];
    }
    vector<pair<int, int>> search(const string& text) const {
        vector<pair<int, int>> hits;
        int u = 0;
        for (int i = 0; i < (int)text.size(); ++i) {
            u = step(u, text[i]);
            for (int v = u; v; v = trie[v].out_link) {
                for (int pid : trie[v].out) {
                    hits.emplace_back(pid, i);
                }
            }
        }
        return hits;
    }
};
