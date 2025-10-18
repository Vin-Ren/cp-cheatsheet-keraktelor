mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

// [mn, mx]
template<class T> T rand_int(T mn, T mx) {
  return uniform_int_distribution<T>(mn, mx)(rng);
}
// [mn,mx) NOT TYPO
template<class T> T rand_float(T mn, T mx) {
  return uniform_real_distribution<T>(mn, mx)(rng);
}
