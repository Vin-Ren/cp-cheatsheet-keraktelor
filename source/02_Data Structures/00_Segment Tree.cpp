struct segment_tree {
    ll SZ=1;
    vector<ll> arr;
    ll default_value = 0;
    segment_tree(int _sz=(int)2.5e5) {
        while (SZ<_sz) {SZ<<=1;}
        arr.resize(SZ<<1);
        for (auto& x:arr) x=default_value;
    }

    ll merge(ll a, ll b) {
        return a+b;
    }

    void update(ll idx, ll val) {
        ll nd=idx+SZ;
        arr[nd]=val;
        while (nd>1) { //update par
            nd/=2;
            arr[nd] = merge(arr[2*nd], arr[2*nd+1]);
        }
    }

    ll query(ll nd, ll cl, ll cr, ll ql, ll qr) {
        if (ql<=cl && cr<=qr) return arr[nd];
        else if (cl>qr || cr<ql) return default_value;
        else {
            ll mid = (cl+cr)/2;
            return merge(query(2*nd, cl, mid, ql, qr), query(2*nd+1, mid+1, cr, ql, qr));
        }
    }
};
