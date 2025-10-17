#include<complex> // Required!!!
typedef int ftype; // ll/ld/int will do
typedef complex<ftype> point;
#define x real
#define y imag

ftype dot(point a, point b) { return (conj(a) * b).x(); }
ftype cross(point a, point b) { return (conj(a) * b).y(); }

struct CHT {
    // Currently min, to switch to max, flip all <0 and >0.
    vector<point> hull, vecs;

    void add_line(ftype k, ftype b) {
        point nw = {k, b};
        while (!vecs.empty() && dot(vecs.back(), nw - hull.back()) < 0) {
            hull.pop_back();
            vecs.pop_back();
        }
        if (!hull.empty()) vecs.push_back(point(0, 1) * (nw - hull.back()));
        hull.push_back(nw);
    }

    ftype get(ftype x) const {
        point query = {x, 1};
        auto it = lower_bound(vecs.begin(), vecs.end(), query,
                              [](point a, point b) { return cross(a, b) > 0; });
        int idx = it - vecs.begin();
        if (idx == (int)hull.size()) idx--;
        return dot(query, hull[idx]);
    }
};
