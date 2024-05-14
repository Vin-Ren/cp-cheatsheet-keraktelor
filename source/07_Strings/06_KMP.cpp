auto get_kmp = [&](string S, string T) -> vector<int> {
  // S is the text, T is the pattern // ababa aba -> expected: {1 2 3 2 3}
  int N = S.size(), M = T.size();
  vector<int> lps(M), kmp(N);
  for(int i = 1, j = 0; i < M;) {
    if(T[i] == T[j])
      lps[i++] = ++j;
    else {
      if(j)
        j = lps[j - 1];
      else
        lps[i++] = 0;
    }
  }
  for(int i = 0, j = 0; i < N;) {
    if(S[i] == T[j]) {
      kmp[i++] = ++j;
      if(j == M)
        j = lps[j - 1];
    } else {
      if(j)
        j = lps[j - 1];
      else
        kmp[i++] = 0;
    }
  }
  return kmp;
};
