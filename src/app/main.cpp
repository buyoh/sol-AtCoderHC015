#include "header.hpp"

//

constexpr int N = 10;
constexpr int M = 100;

inline int cvP2I(int y, int x) { return y * N + x; }
inline int cvP2I(P p) { return cvP2I(p.y, p.x); }
//

struct State {
  F<char> field_;
  int turn_ = 0;
  // shared_ptr<CommandChain> cmds_ = CommandChain::createEmpty();

  State() : field_(N, N) {}

  void applySet(int y, int x, char cnd) {
    field_(y, x) = cnd;
    turn_ += 1;
  }

  void applySetIdx(int p, char cnd) {
    repeat(y, N) {
      repeat(x, N) {
        if (field_(y, x) != 0)
          continue;
        if (--p == 0) {
          applySet(y, x, cnd);
          return;
        }
      }
    }
  }

  // not char! URDL = {0, 1, 2, 3}
  void applyMove(char dir) {
    if (dir == 0)
      applyMoveF();
    else if (dir == 1)
      applyMoveR();
    else if (dir == 2)
      applyMoveD();
    else if (dir == 3)
      applyMoveL();
  }

  int score1() {
    // DFSのほうが良いかもね？
    Unionfind uf1(N * N);
    Unionfind uf2(N * N);
    Unionfind uf3(N * N);
    Unionfind *ufs[] = {&uf1, &uf2, &uf3};
    int numn[3] = {0, 0, 0};
    // int numd[3] = {0, 0, 0};
    repeat(y, N) {
      repeat(x, N) {
        int c = field_(y, x);
        if (c == 0)
          continue;
        int i = cvP2I(y, x);
        if (y + 1 < N && field_(y + 1, x) == c)
          ufs[c - 1]->connect(i, cvP2I(y + 1, x));
        if (x + 1 < N && field_(y, x + 1) == c)
          ufs[c - 1]->connect(i, cvP2I(y, x + 1));
        // chmax(numn[c - 1], ufs[c - 1]->size(i));
        // numd[c - 1] += 1;
      }
    }
    int score = 0;
    // score += numn[0] * 1000000 / types_num[0];
    // score += numn[1] * 1000000 / types_num[1];
    // score += numn[2] * 1000000 / types_num[2];

    repeat(i, N * N) {
      int c = field_.getA(i);
      if (c == 0)
        continue;
      --c;
      if (ufs[c]->root(i) == i) {
        int n = ufs[c]->size(i);
        score += n * n;
      }
    }
    return score;
  }

  template <typename O> void print(O &out) const {
    repeat(y, N) {
      repeat(x, N) { out << "./>#"[field_(y, x)]; }
      out << '\n';
    }
    out << "-\n";
  }

  void generateAdjacents(vector<State> &out) {
    out.push_back(*this);
    out.back().applyMoveF();
    out.push_back(*this);
    out.back().applyMoveR();
    out.push_back(*this);
    out.back().applyMoveD();
    out.push_back(*this);
    out.back().applyMoveL();
  }

private:
  void applyMoveL() {
    repeat(y, N) {
      int p = 0;
      repeat(x, N) {
        if (field_(y, x) != 0) {
          swap(field_(y, p), field_(y, x));
          ++p;
        }
      }
    }
    // cmds_ = CommandChain::pushed(cmds_, 3);
  }
  void applyMoveR() {
    repeat(y, N) {
      int p = N - 1;
      rrepeat(x, N) {
        if (field_(y, x) != 0) {
          swap(field_(y, p), field_(y, x));
          --p;
        }
      }
    }
    // cmds_ = CommandChain::pushed(cmds_, 1);
  }
  void applyMoveF() {
    repeat(x, N) {
      int p = 0;
      repeat(y, N) {
        if (field_(y, x) != 0) {
          swap(field_(p, x), field_(y, x));
          ++p;
        }
      }
    }
    // cmds_ = CommandChain::pushed(cmds_, 0);
  }
  void applyMoveD() {
    repeat(x, N) {
      int p = N - 1;
      rrepeat(y, N) {
        if (field_(y, x) != 0) {
          swap(field_(p, x), field_(y, x));
          --p;
        }
      }
    }
    // cmds_ = CommandChain::pushed(cmds_, 2);
  }
};

struct Problem {
  array<char, M> types_;
  array<P, M> actual_pos_;
  int score_den_ = 0;
  State actual_state_;

  Problem() { fill(all(actual_pos_), P{0, 0}); }

  inline int turn() const { return actual_state_.turn_; }

  void input(MaiScanner &scanner) {
    int types_num[] = {0, 0, 0};
    repeat(i, M) {
      int a;
      scanner >> a;
      types_[i] = a;
      types_num[a - 1] += 1;
    }
    score_den_ += types_num[0] * types_num[0];
    score_den_ += types_num[1] * types_num[1];
    score_den_ += types_num[2] * types_num[2];
  }

  void fetch(MaiScanner &scanner) {
    int current_turn = turn();
    int p;
    scanner >> p;
    repeat(y, N) {
      repeat(x, N) {
        if (actual_state_.field_(y, x) != 0)
          continue;
        if (--p == 0) {
          actual_state_.applySet(y, x, types_[current_turn]);
          // NOTE: この部分が欲しいのでapplySetIdxは使えない
          actual_pos_[current_turn] = P{y, x};
          goto l_break;
        }
      }
    }
  l_break:;
  }

  // not char! URDL = {0, 1, 2, 3}
  void postStdout(char dir) {
    char chr = "FRBL"[dir];
    cout << chr << endl;
    actual_state_.applyMove(dir);
  }
};

//

class SolverTurnDfs {
  const Problem &problem;
  const vector<int> &rand_seed;
  const int max_depth;

public:
  SolverTurnDfs(const Problem &problem, const vector<int> &rand_seed,
                int max_depth)
      : problem(problem), rand_seed(rand_seed), max_depth(max_depth) {}

  int dfs(State state, int depth = 1) {
    if (depth >= max_depth) {
      return state.score1();
    }
    int best = state.score1();
    repeat(c, 4) {
      State state2 = state;
      state2.applyMove(c);
      state2.applySetIdx(rand_seed[depth],
                         problem.types_[problem.turn() + depth]);
      chmax(best, dfs(move(state2), depth + 1));
    }
    return best;
  }
};

class Solver {

public:
  char solveTurn(const Problem &problem) {
    // 置くとターンをインクリメントする。ターン数＝あめのかず
    // 1ターン目の始まりに置ける位置は M - 1 個数なので、 [0, M-2]
    // tターン目の始まりに置ける位置は M - t 個数なので、 [0, M-1-t]
    // 99ターン目の始まりに置ける位置は M - 99 個数なので、 [0, 0]
    const int base_rem_turn = M - problem.turn();
    assert(base_rem_turn > 0);
    if (base_rem_turn <= 0)
      return 0;
    const int max_depth = min(5, base_rem_turn);
    vector<int> rand_seed(max_depth);
    repeat(i, max_depth) {
      //
      rand_seed[i] = rand(0, base_rem_turn - 1 - i);
    }
    pair<int, char> best_score(-1, 0);
    repeat(cmd0, 4) {
      State state1 = problem.actual_state_;
      state1.applyMove(cmd0);
      state1.applySetIdx(rand_seed[0], problem.types_[problem.turn()]);
      SolverTurnDfs solver_turn(problem, rand_seed, max_depth);
      int score = solver_turn.dfs(move(state1));
      // LOG << "URDL"[cmd0] << score;
      chmax(best_score, make_pair(score, (char)cmd0));
    }
    return best_score.second;
  }

private:
};

//

int main(int argc, char **argv) {
  Problem problem;

  MaiScanner *scannerp = &scanner;
  if (argc == 2) {
    FILE *fp = fopen(argv[1], "r");
    if (fp == nullptr)
      return 1;
    MaiScanner *s = new MaiScanner(fp);
    scannerp = s;
    // LEAK fp and s
  }
  problem.input(*scannerp);

  // MaiScanner scanner(stdin);

  Solver solver;

  problem.fetch(*scannerp);
  repeat(turn, M - 1) {
    // clog << "turn=" << turn << endl;
    // problem.actual_state_.print(clog);
    char dir = solver.solveTurn(problem);
    problem.postStdout(dir);
    // problem.actual_state_.print(clog);
    problem.fetch(*scannerp);
  }
  clog << "score="
       << problem.actual_state_.score1() * 100000 / problem.score_den_ << endl;

  // quick_exit(0);
  return 0;
}
