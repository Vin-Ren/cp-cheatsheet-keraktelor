struct SuffixArray {
  string s;
  vector<int> p, c, lcp;
  int n;
  SuffixArray(string _s) : s(_s) {
    s += '$';
    n = (int)s.size();
    p.resize(n);
    c.resize(n);
    {
      // calculate for k = 0
      vector<pair<char, int>> v(n);
      for(int i = 0; i < n; ++i)
        v[i] = make_pair(s[i], i);
      sort(all(v));
      for(int i = 0; i < n; ++i)
        p[i] = v[i].se;
      c[p[0]] = 0;
      for(int i = 1; i < n; ++i)
        c[p[i]] = c[p[i - 1]] + (v[i].fi != v[i - 1].fi);
    }
    const auto countingSort = [](vector<int>& p, vector<int>& c) -> void {
      int n = (int)p.size();
      vector<int> cnt(n), pos(n);
      for(auto& i : c)
        ++cnt[i];
      for(int i = 1; i < n; ++i)
        pos[i] = pos[i - 1] + cnt[i - 1];
      vector<int> pNew(n);
      for(auto& i : p)
        pNew[pos[c[i]]++] = i;
      p = pNew;
    };
    for(int k = 0; (1 << k) < n; ++k) {
      for(int i = 0; i < n; ++i) {  // transition k -> k + 1
        // shift p[i] by 2^k to the left, so that second elements are
        // sorted
        p[i] = (p[i] + n - (1 << k)) % n;
      }
      countingSort(p, c);
      vector<int> cNew(n);
      for(int i = 1; i < n; ++i) {
        pair<int, int> cur = make_pair(
                               c[p[i]], c[(p[i] + (1 << k)) % n]
                             );
        pair<int, int> pre = make_pair(
                               c[p[i - 1]], c[(p[i - 1] + (1 << k)) % n]
                             );
        cNew[p[i]] = cNew[p[i - 1]] + (cur != pre);
      }
      c = cNew;
    }
    // lcp[i]: longest common prefix of s[p[i]] and s[p[i-1]]
    lcp.resize(n); // iterate from the longest suffix
    for(int i = 0, k = 0; i + 1 < n; ++i) {
      int pi = c[i]; // rank of suffix [i..]
      int j = p[pi - 1];
      for(; s[i + k] == s[j + k]; ++k)
        ;
      lcp[pi] = k;
      k = max(k - 1, 0);
    }
  }
};
