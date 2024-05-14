// sz for path queries
// sub, vsub for subtree queries
// x->access() brings x to the top and propagates it,
// x's left subtree is path from x to root
// after access, sub is no. of nodes in CC of x,
// vsub is no. of nodes under x
// use makeRoot() for arbitrary path queries

typedef struct Node* Np;
struct Node {
  Np p, c[2];    // parent, children
  bool flip = 0; // subtree flipped or not
  int vtx;
  ll val, sz;        // value in node, # nodes in current splay tree
  int sub, vsub = 0; // # of nodes in CC
  ll stsum;          // sum of all val in the splay tree
  // after access(): from original root to this node
  Node(int _vtx, int _val = 1) : vtx(_vtx), val(_val) {
    p = c[0] = c[1] = nullptr;
    calc();
  }

  friend int getSz(Np x) {
    return x ? x->sz : 0;
  }
  friend int getSub(Np x) {
    return x ? x->sub : 0;
  }
  friend ll getStSum(Np x) {
    return x ? x->stsum : 0;
  }

  void prop() { // lazy prop
    if(!flip)
      return;
    swap(c[0], c[1]);
    flip = 0;
    for(int i = 0; i < 2; i++) {
      if(c[i])
        c[i]->flip ^= 1;
    }
  }
  void calc() { // recalc vals
    for(int i = 0; i < 2; i++) {
      if(c[i])
        c[i]->prop();
    }
    sz = 1 + getSz(c[0]) + getSz(c[1]);
    sub = 1 + getSub(c[0]) + getSub(c[1]) + vsub;
    stsum = val + getStSum(c[0]) + getStSum(c[1]);
  }

  // SPLAY TREE OPERATIONS
  int dir() { // p is path-parent pointer
    if(!p)
      return -2;
    for(int i = 0; i < 2; i++) {
      if(p->c[i] == this)
        return i;
    }
    return -1; // -> not in current splay tree
  }
  // test if root of current splay tree
  bool isRoot() {
    return dir() < 0;
  }
  friend void setLink(Np x, Np y, int d) { // x is orig parent
    if(y)
      y->p = x;
    if(d >= 0)
      x->c[d] = y;
  }
  void rot() { // assume p and p->p propagated
    assert(!isRoot());
    int x = dir();
    Np pa = p;
    setLink(pa->p, this, pa->dir());
    setLink(pa, c[x ^ 1], x);
    setLink(this, pa, x ^ 1);
    pa->calc();
  }
  void splay() { // bring this node to the root of splay tree
    while(!isRoot() && !p->isRoot()) {
      p->p->prop(), p->prop(), prop();
      dir() == p->dir() ? p->rot() : rot();
      rot();
    }
    if(!isRoot())
      p->prop(), prop(), rot();
    prop();
    calc();
  }
  Np fbo(int b) { // find by order
    prop();
    int z = getSz(c[0]); // of splay tree
    if(b == z) {
      splay();
      return this;
    }
    return b < z ? c[0]->fbo(b) : c[1]->fbo(b - z - 1);
  }

  // BASE OPERATIONS
  // bring this to top of splay tree (not impacting original tree)
  void access() {
    for(Np v = this, pre = nullptr; v; v = v->p) {
      v->splay(); // now switch virtual children
      if(pre)
        v->vsub -= pre->sub;
      if(v->c[1])
        v->vsub += v->c[1]->sub;
      v->c[1] = pre;
      v->calc();
      pre = v;
    }
    splay();
    assert(!c[1]); // right subtree is empty
  }
  void makeRoot() { // of the original tree
    access();
    flip ^= 1;
    access();
    assert(!c[0] && !c[1]);
  }

  // QUERIES
  friend Np lca(Np x, Np y) {
    if(x == y)
      return x;
    x->access(), y->access();
    if(!x->p)
      return nullptr;
    x->splay();
    return x->p ? : x; // y was below x in latter case
  }                     // access at y did not affect x -> not connected
  friend bool connected(Np x, Np y) {
    return lca(x, y);
  }

  // no. of nodes above; distance to root in original tree
  int distRoot() {
    access();
    return getSz(c[0]);
  }
  Np getRoot() { // get root of LCT component in original tree
    access();
    Np a = this;
    while(a->c[0])
      a = a->c[0], a->prop();
    a->access();
    return a;
  }
  Np getPar(int b) { // get b-th parent on path to root
    access();
    b = getSz(c[0]) - b;
    assert(b >= 0);
    return fbo(b);
  } // can also get min, max on path to root, etc

  // MODIFICATIONS
  void setVal(int v) {
    access();
    val = v;
    calc();
  }
  void addVal(int v) {
    access();
    val += v;
    calc();
  }
  friend void link(Np x, Np y, bool force = 1) {
    assert(!connected(x, y));
    if(force) {
      y->makeRoot(); /// make x par of y; x -> y
    } else {
      y->access();
      assert(!y->c[0]);
    }
    x->access();
    setLink(y, x, 0);
    y->calc();
  }
  friend void cut(Np y) { // cut y from its parent
    y->access();
    assert(y->c[0]);
    y->c[0]->p = NULL;
    y->c[0] = NULL;
    y->calc();
  }
  friend void cut(Np x, Np y) { // if x, y adj in tree
    x->makeRoot();
    y->access();
    assert(y->c[0] == x && !x->c[0] && !x->c[1]);
    cut(y);
  }
};
Np LCT[maxV];
