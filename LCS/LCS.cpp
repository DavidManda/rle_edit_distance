#include <iostream>
#include "LCS.hpp"
#include "binary_tree.hpp"

int max(int a, int b, int c)
{
  return std::max(std::max(a, b), c);
}

void read_string(std::istream &fin, int len, std::string &s)
{
  char ch;
  for (int i = 0; i < len; i++)
  {
    fin >> ch;
    s += ch;
  }
}

int get_LCS(const int N, const int M, const std::string &s0, const std::string &s1)
{
  int dyn[N + 1][M + 1];
  memset(dyn, 0, sizeof(dyn));

  for (int i = 1; i <= N; i++)
  {
    for (int j = 1; j <= M; j++)
    {
      dyn[i][j] = max(dyn[i - 1][j - 1] + (s0[i] == s1[j]), dyn[i - 1][j], dyn[i][j - 1]);
    }
  }

  return dyn[N][M];
}

void forced_path_update(const std::vector<RLE_run> s0, const std::vector<RLE_run> s1, int i, int j, std::vector<std::vector<int>> &dyn)
{
  int v_offset = 0, h_offset = 0;

  if (s0[i].len == s1[j].len)
    return;

  while (i < s0.size() && j < s1.size())
  {
    // proceed vertically
    if (s0[i].len - v_offset < s1[j].len - h_offset)
    {
      // exited through bottom
      h_offset = s0[i].len - v_offset;
      v_offset = 0;
      int next_i = i + 1;
      while (next_i < s0.size() && s0[next_i].ch != s1[j].ch)
      {
        ++next_i;
      }
      // reached end of s0
      if (next_i >= s0.size())
      {
        return;
      }
      int d = std::min(s0[next_i].len - v_offset, s1[j].len - h_offset);
      dyn[next_i][j] = std::max(dyn[next_i][j], dyn[i][j] + d);
      i = next_i;
    }
    // proceed horizontally
    else if (s0[i].len - v_offset > s1[j].len - h_offset)
    {
      // exited through right-hand side
      h_offset = 0;
      v_offset = s1[j].len - h_offset;
      int next_j = j + 1;
      while (next_j < s1.size() && s0[i].ch != s1[next_j].ch)
      {
        ++next_j;
      }
      // reached end of s1
      if (next_j >= s1.size())
      {
        return;
      }

      int d = std::min(s0[i].len - v_offset, s1[next_j].len);
      dyn[i][next_j] = std::max(dyn[i][next_j], dyn[i][j] + d);
      j = next_j;
    }
    else
    {
      // exiting though corner
      return;
    }
  }
  return;
}

void print_2d_vect(std::vector<std::vector<int>> vect)
{
  for (int i = 1; i < vect.size(); i++)
  {
    for (int j = 1; j < vect[i].size(); j++)
    {
      std::cout << vect[i][j] << ' ';
    }
    std::cout << '\n';
  }
  std::cout << '\n';
}

int get_rle_lcs(const std::vector<RLE_run> s0, const std::vector<RLE_run> s1)
{
  const int N = s0.size();
  const int M = s1.size();
  std::vector<std::vector<int>> dyn(N, std::vector<int>(M));

  for (int i = 1; i < N; i++)
  {
    for (int j = 1; j < M; j++)
    {
      // miss match block
      if (s0[i].ch != s1[j].ch)
      {
        dyn[i][j] = std::max(dyn[i - 1][j], dyn[i][j - 1]);
        continue;
      }
      // match block
      int d = std::min(s0[i].len, s1[j].len);
      int curr_val = dyn[i][j];
      // start forced path at i j
      dyn[i][j] = dyn[i - 1][j - 1] + d;
      forced_path_update(s0, s1, i, j, dyn);

      dyn[i][j] = max(curr_val, dyn[i][j], std::max(dyn[i - 1][j], dyn[i][j - 1]));
      // print_2d_vect(dyn);
    }
  }

  return dyn[N - 1][M - 1];
}

void precompute(std::vector<std::map<char, int>> &vec, std::vector<RLE_run> s)
{
  std::map<char, int> last_position;
  char ch;
  int len, prev_val;
  for (int i = 1; i < s.size(); i++)
  {
    ch = s[i].ch;
    len = s[i].len;
    if (last_position.find(ch) == last_position.end())
    {
      prev_val = 0;
    }
    else
    {
      prev_val = vec[last_position[ch]][ch];
    }
    vec[i - 1][ch] = prev_val;
    vec[i][ch] = prev_val + len;
    last_position[ch] = i;
  }
}

void keep_tree_sorted(binarySearchTree &my_bst, int rank, int val, int x, int y, char ch, std::vector<std::map<char, int>> char_run_sum, std::string type)
{
  treeNode *predec = my_bst.find_predec(rank);
  if (predec)
  {
    int predec_val;
    if (type == "column")
    {
      predec_val = predec->val + char_run_sum[y][ch] - char_run_sum[predec->y - 1][ch];
    }
    else
    {
      predec_val = predec->val + char_run_sum[x][ch] - char_run_sum[predec->x - 1][ch];
    }
    if (predec_val > val)
    {
      my_bst.delete_node(rank);
    }
  }
  treeNode *succ = my_bst.find_succ(rank);
  int succ_val;
  while (succ)
  {
    if (type == "column")
    {
      succ_val = succ->val + char_run_sum[y][ch] - char_run_sum[succ->y - 1][ch];
    }
    else
    {
      succ_val = succ->val + char_run_sum[x][ch] - char_run_sum[succ->x - 1][ch];
    }
    if (succ_val <= val)
    {
      my_bst.delete_node(succ->key);
      succ = my_bst.find_succ(rank);
    }
    else
      break;
  }
}

int get_rle_lcs_fast(const std::vector<RLE_run> s0, const std::vector<RLE_run> s1)
{
  const int M = s0.size();
  const int N = s1.size();
  std::vector<std::map<char, int>> TOP(N);
  std::vector<std::map<char, int>> LEFT(M);
  std::vector<std::vector<int>> dyn(M, std::vector<int>(N));
  precompute(TOP, s1);
  precompute(LEFT, s0);
  std::map<char, binarySearchTree> column_paths;
  std::map<char, binarySearchTree> row_paths;
  for (int i = 1; i < M; i++)
  {
    for (int j = 1; j < N; j++)
    {
      // mismatch block
      if (s0[i].ch != s1[j].ch)
      {
        dyn[i][j] = std::max(dyn[i - 1][j], dyn[i][j - 1]);
        continue;
      }
      // match block
      // Step I: Insert new path starting at this block
      char ch = s0[i].ch;
      // The rank is the order in which forced paths cross rows and columns respectively
      // The column rank increases with the row number at which the columns are intersected, i.e.
      // when LEFT increases, so does the row number. The row rank is the opposite, increasing with
      // the column number
      int rank_col = LEFT[i - 1][ch] - TOP[j - 1][ch];
      int rank_row = TOP[j - 1][ch] - LEFT[i - 1][ch];
      int val = dyn[i - 1][j - 1];
      column_paths[ch].insert(rank_col, val, i, j);
      row_paths[ch].insert(rank_row, val, i, j);
      // print_2D(column_paths.root);
      keep_tree_sorted(column_paths[ch], rank_col, val, i - 1, j - 1, ch, TOP, "column");
      keep_tree_sorted(row_paths[ch], rank_row, val, i - 1, j - 1, ch, LEFT, "row");
      int max_column_rank = LEFT[i][ch] - TOP[j][ch];
      int max_row_rank = TOP[j][ch] - LEFT[i][ch];
      int min_column_rank = LEFT[i - 1][ch] - TOP[j][ch];
      int min_row_rank = TOP[j - 1][ch] - LEFT[i][ch];
      treeNode *best_col_node = column_paths[ch].find_predec(max_column_rank + 1);
      treeNode *best_row_node = row_paths[ch].find_predec(max_row_rank + 1);
      int C = 0;
      if (best_col_node && best_col_node->key >= min_column_rank)
      {
        C = best_col_node->val + TOP[j][ch] - TOP[best_col_node->y - 1][ch];
      }
      int R = 0;
      if (best_row_node && best_row_node->key >= min_row_rank)
      {
        R = best_row_node->val + LEFT[i][ch] - LEFT[best_row_node->x - 1][ch];
      }
      // if(i == 6 && j == 1){
      //     std::cout<<val<<" "<<best_col_node->x <<" "<<best_col_node->y<<"\n";
      //     print_2D(column_paths[ch].root);
      // }
      dyn[i][j] = std::max(dyn[i - 1][j], max(dyn[i][j - 1], C, R));
      // print_2d_vect(dyn);
    }
  }
  // print_2d_vect(dyn);
  return dyn[M - 1][N - 1];
}
