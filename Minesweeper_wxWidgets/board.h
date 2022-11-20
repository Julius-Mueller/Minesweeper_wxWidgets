#include <algorithm>
#include <iterator>
#include <vector>
#include <random>
#include <ctime>

class board {
  public:
    int xdim;
    int ydim;
    int N;
    std::vector<std::vector<int>> mines;
    std::vector<std::vector<int>> shroud;
    std::vector<std::vector<int>> neighbors;

    board (int N_cols, int N_rows, int N_mines) {
      xdim = N_cols;
      ydim = N_rows;
      N = N_mines;
      mines.insert(mines.begin(), ydim, std::vector<int>(xdim, 0));
      shroud.insert(shroud.begin(), ydim, std::vector<int>(xdim, 1));
      neighbors.insert(neighbors.begin(), ydim, std::vector<int>(xdim, 0));
    };

    void reload() {
        for (int x = 0; x < xdim; ++x) {
            for (int y = 0; y < ydim; ++y) {
                mines[y][x] = 0;
                shroud[y][x] = 1;
                neighbors[y][x] = 0;
            }
        }
    }

    void unveil (int x, int y) {
      if (shroud[y][x] == 1) {
        shroud[y][x] = 0;
        if (neighbors[y][x] == 0) {
          int xstr = std::max(0, x-1);
          int xstp = std::min(xdim-1, x+1);
          int ystr = std::max(0, y-1);
          int ystp = std::min(ydim-1, y+1);
          /*
          std::cout << std::endl;
          std::cout << "x,y:" << std::endl;
          std::cout << x;
          std::cout << ',';
          std::cout << y << std::endl;
          std::cout << std::endl;
          /**/
          for (int k = ystr; k <= ystp; ++k) {
            for (int i = xstr; i <= xstp; ++i) {
              /*
              std::cout << i;
              std::cout << ',';
              std::cout << k << std::endl;
              /**/
              unveil(i, k);
            };
          };
        };
      };
    };

    void generate(int x, int y) {
      // Determine if player clicked by border(s), choose nr. of clear squares:
      int xbrd = (x == 0 || x == xdim-1);
      int ybrd = (y == 0 || y == ydim-1);
      int xwnd = 3 - xbrd;
      int ywnd = 3 - ybrd;
      int nclear = xwnd*ywnd;

      // Create empty (linear) board, fill first N elements with mines, shuffle:
      int len = xdim*ydim;
      std::vector<int> mines_1d;
      unsigned int seed = time(0);
      auto rng = std::default_random_engine { seed };

      mines_1d.insert(mines_1d.end(), N, 1);
      mines_1d.insert(mines_1d.end(), len-N, 0);
      std::shuffle(mines_1d.begin(), mines_1d.end()-nclear, rng);
      std::shuffle(mines_1d.end()-nclear, mines_1d.end(), rng);
      // Shuffling the last nclear elements seperately ensures that they have
      // the least amount of mines possible (zero mines unless N > len-nclear).
      // These elements are now swapped to the xwnd*ywnd rectangle around the
      // square which the player has clicked to start the game.

      int xstr = std::max(0, x-1);
      int xstp = std::min(xdim-1, x+1);
      int ystr = std::max(0, y-1);
      int ystp = std::min(ydim-1, y+1);
      for (int k = 0; k < ywnd; ++k) {
        for (int i = 0; i < xwnd; ++i) {
          std::swap(mines_1d[(ystr + k)*xdim + (xstr + i)],
                    mines_1d[len - nclear + k*xwnd + i]);
        };
      };

      // Generate 2d grid of mines:
      for (int k = 0; k < ydim; ++k) {
        for (int i = 0; i < xdim; ++i) {
          mines[k][i] = mines_1d[k*xdim + i];
          std::cout << mines[k][i] << ',';
        };
        std::cout << '\n';
      };

      std::cout << std::endl;

      // Generate 2d grid of numbers of neighboring mines:
      for (int k = 0; k < ydim; ++k) {
        for (int i = 0; i < xdim; ++i) {
          int istr = std::max(0, i-1);
          int istp = std::min(xdim-1, i+1);
          int kstr = std::max(0, k-1);
          int kstp = std::min(ydim-1, k+1);
          for (int j = istr; j <= istp; ++j) {
            for (int l = kstr; l <= kstp; ++l) {
              neighbors[k][i] = neighbors[k][i] + mines[l][j];
            };
          };
          neighbors[k][i] = neighbors[k][i] - mines[k][i];
          std::cout << neighbors[k][i] << ',';
        };
        std::cout << '\n';
      };

      std::cout << std::endl;

      // Unveil player board where the player has clicked to start the game:
      unveil(x, y);
      for (int k = 0; k < ydim; ++k) {
        for (int i = 0; i < xdim; ++i) {
          std::cout << shroud[k][i] << ',';
        };
        std::cout << '\n';
      };

    };

    int remaining() {
        int n = 0;
        for (int x = 0; x < xdim; ++x) {
            for (int y = 0; y < ydim; ++y) {
                n += shroud[y][x];
            }
        }
        return n;
    };
};
