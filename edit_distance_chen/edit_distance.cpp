#include <iostream>
#include <cmath> 
#include <math.h>
#include "edit_distance.hpp"
#include <deque>
#include <assert.h>

#define MAX_SIZE 37000
typedef std::vector<Point> border_type;
typedef std::vector<rle::RLE_run> rle_string;


int dyn[2 * MAX_SIZE];
// The following are a graphical representation for the Input (split into LEFT and TOP)
// and Output border for each block
border_type LEFT[MAX_SIZE];
border_type TOP[MAX_SIZE];
border_type OUT[2 * MAX_SIZE];
int min(int a, int b, int c)
{
  return std::min(a, std::min(b, c));
}

Point min(Point p1, Point p2)
{
  if(p1.y < p2.y)
  {
    return p1;
  }
  return p2;
}

std::string traj_to_string(std::vector<Point> &A)
{
  std::stringstream ss;
  ss<<"[ ";
  for(int i = 0; i < A.size(); i++)
  {
    ss << A[i].to_string() << " ";
  }
  ss << "]\n";
  return ss.str();
}

float get_val_at_coord_(float coord, Point p1, Point p2)
{
  if(coord < p1.x || coord > p2.x)
  {
    // std::cout<<"Coord should be between the two given points!\n";
    return 0;
  }
  float slope = (p2.y - p1.y)/(p2.x - p1.x);
  
  return p1.y + slope * (coord - p1.x);
}

void add_point(std::vector<Point>& points, Point p)
{ 
  if(points.empty())
  {
    points.push_back(p);
    return;
  }
  if(points.back().x == p.x && points.back().y == p.y)
  {
    return;
  }
  if(points.size() < 2)
  {
    points.push_back(p);
    return;
  }
  int l = points.size() - 1;
  if(std::abs((points[l].y - points[l-1].y)*(p.x - points[l-1].x) - (p.y - points[l-1].y) * (points[l].x - points[l-1].x)) < 0.0001)
  {
    points.pop_back();
  }
  points.push_back(p);
}

void add_point(std::deque<Point>& points, Point p)
{ 
  if(points.empty())
  {
    points.push_back(p);
    return;
  }
  if(points.back().x == p.x && points.back().y == p.y)
  {
    return;
  }
  if(points.size() < 2)
  {
    points.push_back(p);
    return;
  }
  int l = points.size() - 1;
  if(std::abs((points[l].y - points[l-1].y)*(p.x - points[l-1].x) - (p.y - points[l-1].y) * (points[l].x - points[l-1].x)) < 0.0001)
  {
    points.pop_back();
  }
  points.push_back(p);
}

void init_input_border(border_type LEFT[], border_type TOP[], int M, int N, const rle_string &s0, const rle_string &s1)
{
  int char_count = 0;
  for (int i = 1; i < M; i++)
  {
    LEFT[i] = std::vector<Point>();
    LEFT[i].push_back(Point(1, s0[i].len + char_count));
    LEFT[i].push_back(Point(s0[i].len + 1, char_count));
    char_count += s0[i].len;
  }

  char_count = 0;
  for (int j = 1; j < N; j++)
  {
    TOP[j] = std::vector<Point>();
    TOP[j].push_back(Point(1, char_count));
    TOP[j].push_back(Point(s1[j].len + 1, s1[j].len + char_count));
    char_count += s1[j].len;
  }
}

void get_input_border(border_type LEFT[], border_type TOP[], border_type OUT[], int i, int j, int dyn[], rle_string s0, rle_string s1, int M, int N, int current_row)
{
  // LEFT[i][j] might have been initialised (if j == 1) so we don't have to do anything in that case
  if (j > 1)
  {
    LEFT[i] = std::vector<Point>();
    // This is the first point
    add_point(LEFT[i], Point(1, dyn[current_row * N + (j - 1)]));
    // width and height of block to the left
    int w = s1[j - 1].len + 1;
    int h = s0[i].len + 1;
    for (int k = 0; k < OUT[current_row * N + (j-1)].size(); k++)
    {
      Point p = OUT[current_row * N + (j - 1)][k];
      // These points are part of the left input border
      // The very last point should also be carried over here
      if (p.x > w)
      {
        int index = p.x - w + 1;
        int value = p.y;
        add_point(LEFT[i], Point(index, value));
      }
    }
  }

  if (i > 1)
  {
    TOP[j] = std::vector<Point>();
    // Width and height of block above
    int w = s1[j].len + 1;
    int h = s0[i - 1].len + 1;
    // The first point should be added here automatically
    for (int k = 0; k < OUT[(1-current_row) * N + j].size(); k++)
    {
      Point p = OUT[(1-current_row) * N + j][k];
      if(p.x < w)
      {
        add_point(TOP[j], p);
      }
      else
      {
        break;
      }
    }
    // Adding the very last point
    add_point(TOP[j], Point(w, dyn[(1-current_row) * N + j]));
  }
}

int get_val_at_coord(int coord, std::vector<Point>& points)
{
  int i = 0;
  while(i < points.size() && points[i].x < coord)
  {
    i++;
  }
  if(points[i].x == coord)
  {
    return points[i].y;
  }
  else
  {
    if(points[i].x >= coord && points[i-1].x <= coord)
    {
      return get_val_at_coord_(coord, points[i-1], points[i]);
    }
    std::cout<<"gotcha\n";
    return 0;    
  }
}

int get_val_at_coord(int coord, std::deque<Point>& points)
{
  int i = 0;
  while(i < points.size() && points[i].x < coord)
  {
    i++;
  }
  if(points[i].x == coord)
  {
    return points[i].y;
  }
  else
  {
    if(points[i].x >= coord && points[i-1].x <= coord)
    {
      return get_val_at_coord_(coord, points[i-1], points[i]);
    }
    std::cout<<"gotcha\n";
    return 0;    
  }
}

int get_coord_for_val(float val, Point p1, Point p2)
{
  if (val < p1.y || val > p2.y)
  {
    std::cout << "Value should not be outside of the range of the segment given!\n";
    return 0;
  }

  float s = (p2.y - p1.y)/(p2.x - p1.x);
  if(s == 0)
  {
    std::cout<<"Slope should not be 0 in get_coord_for_val!\n";
    return 0;
  }
  return (val - p1.y)/s + p1.x;
}


Point get_intersection(Point a1, Point a2, Point b1, Point b2)
{
  // std::cout<<a1.to_string()<<' ';
  // std::cout<<a2.to_string()<<' ';
  // std::cout<<b1.to_string()<<' ';
  // std::cout<<b2.to_string()<<'\n';
  float s1 = (a2.y - a1.y)/(a2.x - a1.x);
  float s2 = (b2.y - b1.y)/(b2.x - b1.x);
  if(s1 == s2)
  {
    std::cout<<"Slopes should not be equal!\n";
    return Point(0,0);
  }

  float x = (b1.y - a1.y + s1*a1.x - s2*b1.x)/(s1-s2);
  float y = a1.y + (x - a1.x) * s1;
  return Point(x, y);
}

bool intersects(Point a1, Point a2, Point b1, Point b2)
{
  // TODO and this
  if(a1.x != b1.x || a2.x != b2.x)
  {
    std::cout<<"Line segments should have the same start and end coordinates!\n";
    return false;
  }

  return (a1.y < b1.y && a2.y > b2.y)|| (a1.y > b1.y && a2.y < b2.y);
}


Point get_floor_point(Point p, Point a1, Point a2, Point b1, Point b2)
{
  int x = floor(p.x), y;
  if(a1.y < b1.y)
  {
    y = get_val_at_coord_(x, a1, a2);
  }
  else
  {
    if(x < b1.x)
    {
      std::cout<<p.to_string()<<a1.to_string()<<a2.to_string()<<b1.to_string()<<b2.to_string()<<'\n';
    }
    y = get_val_at_coord_(x, b1, b2);
  }
  return Point(x, y);
}

Point get_ceil_point(Point p, Point a1, Point a2, Point b1, Point b2)
{
  int x = ceil(p.x), y;
  if(a2.y < b2.y)
  {
    y = get_val_at_coord_(x, a1, a2);
  }
  else
  {
    y = get_val_at_coord_(x, b1, b2);
  }
  return Point(x, y);
}

void add_intersection(std::vector<Point>& sol, Point a1, Point a2, Point b1, Point b2)
{
  Point intersection = get_intersection(a1, a2, b1, b2);
  Point floor_p = get_floor_point(intersection, a1, a2, b1, b2);
  Point ceil_p = get_ceil_point(intersection, a1, a2, b1, b2);
  // It can be the case that the intersection happens at the end of the segment because 
  // we take the ceiling of the coordinate. In that case we shoul not add it
  if(floor_p.x > a1.x)
  {
    add_point(sol, floor_p);
  }
  if(ceil_p.x < a2.x)
  {
    add_point(sol, ceil_p);
  }
}

std::vector<Point> get_cswm(std::vector<Point> &S, int h)
{
  if (!(h > 0))
  {
    std::cout << "Window size must me greater than 0!\n";
    return std::vector<Point>();
  }
  std::vector<Point> S_CSWM;
  std::deque<Point> L;
  add_point(S_CSWM, S[0]);
  add_point(L, Point(S[0].x - h + 1, S[0].y));
  add_point(L, S[0]);
  for (int i = 0; i < S.size() - 1; i++)
  {
    // on each iteration we compute the lower part of L and the segment defined by [S[i], S[i+1]]
    Point current = S[i], next = S[i + 1];
    int slope = (next.y - current.y)/(next.x - current.x);
    assert(current.x == L[0].x + (h-1));
    int val_at_next;
    if(L.back().x + (h-1) >= next.x){
      val_at_next = get_val_at_coord(next.x - (h-1), L);
    }
    else{
      val_at_next = L.back().y;
    }
    // Case 1
    if(val_at_next < next.y){
      int j = 0;
      while(j < L.size() && L[j].x + (h-1) <= next.x){
        add_point(S_CSWM, Point(L[j].x + (h-1), L[j].y));
        j++;
      }
      if(j < L.size() && j > 0 && L[j-1].x < next.x){
        int val = get_val_at_coord_(next.x - (h-1), L[j-1], L[j]);
        add_point(S_CSWM, Point(next.x, val));
      }
      // Truncate the back part that is higher than next point
      if (next.y > L.back().y)
      {
        L.push_back(next);
      }
      else
      {
        Point last_deleted = Point(0,0);
        while (!L.empty() && L.back().y > next.y)
        {
          last_deleted = L.back();
          L.pop_back();
        }
        // In this case we need to insert a point where the value intersects with the trajectory
        if(!L.empty() && L.back().y < next.y)
        {
          int x = get_coord_for_val(next.y, L.back(), last_deleted);
          add_point(L, Point(x, next.y));
        }
        add_point(L, next);
      }
      // Remove elements with x coord smaller than next.x - (h - 1)
      if(L.front().x < next.x - h + 1)
      {
        Point last_point = Point(0,0);
        while(!L.empty() && L.front().x <= next.x - (h - 1))
        {
          last_point = L.front();
          L.pop_front();
        }
        float val = get_val_at_coord_(next.x - h + 1, last_point, L[0]);
        // TODO add afunction similar to add_point but for the beggining
        L.push_front(Point(next.x - (h - 1), val));
      }
      else
      {
        L.push_front(Point(next.x - h + 1, next.y));
      }
    }
    else // Case 2
    {
      int j=0;
      int s_val_at_coord = get_val_at_coord_(L[0].x + (h-1), current, next);
      while(j < L.size() && L[j].y < s_val_at_coord){
        add_point(S_CSWM, Point(L[j].x + (h-1), L[j].y));
        j++;
        if(j < L.size()){
          s_val_at_coord = get_val_at_coord_(L[j].x + (h-1), current, next);
        }
      }
      if(j > 0){
        add_intersection(S_CSWM, current, next, Point(L[j-1].x + (h - 1), L[j-1].y), Point(L[j].x + (h - 1), L[j].y));
      }
      else{
        add_point(S_CSWM, current);
      }
      add_point(S_CSWM, next);
      std::deque<Point> new_L;
      add_point(new_L, Point(next.x - (h - 1), next.y));
      add_point(new_L, next);
      L = new_L;
    }
  }
  for(int i = 0; i < L.size(); i++)
  {
    if(L[i].x + h - 1 >= S.back().x && L[i].x + h - 1 <= S.back().x + h - 1)
    {
      add_point(S_CSWM, Point(L[i].x + h - 1, L[i].y));
    }
  }
  return S_CSWM;
}

// expects two trajectories and modifies them such that both have points at the same coordinates
void normalise_trajectories(std::vector<Point>& A, std::vector<Point>& B)
{
  if(A.begin()->x != B.begin()->x || A.back().x != B.back().x)
  {
    std::cout<<"Both trajectories should have the same start and end points in normalise!\n";
    return;
  }
  std::vector<Point> A_norm, B_norm;
  int i = 0, j = 0;
  while(i < A.size() && j < B.size())
  {
    if(A[i].x == B[j].x)
    {
      A_norm.push_back(A[i]);
      B_norm.push_back(B[j]);
      i++;
      j++;
    }
    else if(A[i].x < B[j].x)
    {
      A_norm.push_back(A[i]);
      float val = get_val_at_coord_(A[i].x, B[j-1], B[j]);
      B_norm.push_back(Point(A[i].x, val));
      i++;
    }
    else
    {
      B_norm.push_back(B[j]);
      float val = get_val_at_coord_(B[j].x, A[i-1], A[i]);
      A_norm.push_back(Point(B[j].x, val));
      j++;
    }
  }
  if(i != A.size() || j != B.size())
  {
    std::cout<<"Something is wrong with normalise!\n";
    return;
  }
  A = A_norm;
  B = B_norm;
  if(A.size() != B.size())
  {
    std::cout<<"Trajectories should have the same number or points now!\n";
  }
}


// Returns the lower part of two trajectories
std::vector<Point> get_lower_part(std::vector<Point> &A, std::vector<Point> &B)
{
  std::vector<Point> sol;
  if(A.begin()->x != B.begin()->x || A.back().x != B.back().x)
  {
    std::cout<<"Both trajectories should have the same start and end points!\n";
    return sol;
  }
  normalise_trajectories(A, B);
  // here A and B should have the same length and have the same x-coordinates 
  for(int i = 0; i < A.size() - 1; i++)
  {
    add_point(sol, min(A[i], B[i]));
    if(intersects(A[i], A[i+1], B[i], B[i+1]))
    {
      add_intersection(sol, A[i], A[i+1], B[i], B[i+1]);
    }
  }
  add_point(sol, min(A.back(), B.back()));  
  return sol;
}

void split_traj(std::vector<Point> L, std::vector<Point>& L1, std::vector<Point>& L2, int val)
{
  int k = 0;
  while(k < L.size() && L[k].x < val)
  {
    add_point(L1, L[k]);
    k++;
  }
  float y = get_val_at_coord_(val, L[k-1], L[k]);
  add_point(L1, Point(val, y));
  add_point(L2, Point(val, y));
  while(k < L.size())
  {
    add_point(L2, L[k]);
    k++;
  }
}

void propagate_2(int h, int w, const std::vector<Point> &LEFT_CSWM, const std::vector<Point> &TOP_CSWM, std::vector<Point>& LEFT_OUT, std::vector<Point>& TOP_OUT)
{
  if(h <= w)
  {
    std::vector<Point> L1, L2, T1, T2;

    split_traj(LEFT_CSWM, L1, L2, h);
    split_traj(TOP_CSWM, T1, T2, w);
    for(int k = 0; k < L1.size(); k++)
    {
      Point p = L1[k];
      add_point(LEFT_OUT, Point(p.x, p.y + p.x - 1));
    }
    for(int k = 0; k < L2.size();k++)
    {
      Point p = L2[k];
      add_point(LEFT_OUT, Point(p.x + w - h, p.y + w - 1));
    }
                
    
    for(int k = 0; k < T1.size(); k++)
    {
      Point p = T1[k];
      add_point(TOP_OUT, Point(p.x, p.y + h - 1));
    }

    for(int k = 0; k < T2.size(); k++)
    {
      Point p = T2[k];
      add_point(TOP_OUT, Point(p.x, p.y + (w + h - 1) - p.x));
    }
  }
  else
  {
    std::vector<Point> L1, L2, T1, T2;

    split_traj(LEFT_CSWM, L1, L2, w);
    split_traj(TOP_CSWM, T1, T2, w);
    for(int k = 0; k < L1.size(); k++)
    {
      Point p = L1[k];
      add_point(LEFT_OUT, Point(p.x, p.y + p.x - 1));
    }
    for(int k = 0; k < L2.size(); k++)
    {
      Point p = L2[k];
      add_point(LEFT_OUT, Point(p.x, p.y + w - 1));
    }

    for(int k = 0; k < T1.size(); k++)
    {
      Point p = T1[k];
      add_point(TOP_OUT, Point(p.x, p.y + h - 1));
    }
    for(int k = 0; k < T2.size(); k++)
    {
      Point p = T2[k];
      add_point(TOP_OUT, Point(p.x - w + h, p.y + (w + h - 1) - (p.x - w + h)));
    }
  }
}

void initialise_empty(border_type B[], int M, int N){
  for(int i = 0; i < M; i++){
    for(int j = 0; j < N; j++){
      B[i * N + j] = std::vector<Point>();
    }
  }
}

int get_rle_edit_dist(const rle_string &s0, const rle_string &s1)
{
  const int M = s0.size();
  const int N = s1.size();

  initialise_empty(OUT, 2, N);
  // Initialise input border
  init_input_border(LEFT, TOP, M, N, s0, s1);
  int current_row = 0,prev_row = 1;
  for (int i = 1; i < M; i++)
  {
    prev_row = current_row;
    current_row = 1 - current_row;
    for (int j = 1; j < N; j++)
    {
      OUT[current_row * N + j] = std::vector<Point>();
      int h = s0[i].len + 1;
      int w = s1[j].len + 1;
      // Retrieve input border for current block
      get_input_border(LEFT, TOP, OUT, i, j, dyn, s0, s1, M, N, current_row);
      if(s0[i].ch == s1[j].ch)
      {
        for(int k = 0; k < LEFT[i].size(); k++)
        {
          add_point(OUT[current_row * N + j], LEFT[i][k]);
        }
        for(int k = 0; k < TOP[j].size(); k++)
        {
          add_point(OUT[current_row * N + j], Point(TOP[j][k].x + h - 1, TOP[j][k].y));
        }
      }
      else
      {
        std::vector<Point> LEFT_CSWM, TOP_CSWM, LEFT_OUT, TOP_OUT;
        int window = std::min(h, w);
        LEFT_CSWM = get_cswm(LEFT[i], window);
        TOP_CSWM = get_cswm(TOP[j], window);
        // Propagate 2
        propagate_2(h, w, LEFT_CSWM, TOP_CSWM, LEFT_OUT, TOP_OUT);
        // Propagate 3
        OUT[current_row * N + j] = get_lower_part(LEFT_OUT, TOP_OUT);        
      }
      dyn[current_row * N + j] = get_val_at_coord(w, OUT[current_row * N + j]);
    }
  }

  return dyn[current_row * N  + (N - 1)];
}

int get_edit_dist(int M, int N, const std::string &s0, const std::string &s1)
{
  M += 1;
  N += 1;
  int current_row = 0,prev_row = 1;
  for (int i = 0; i < N; i++)
  {
    dyn[current_row * N + i] = i;
  }

  for (int i = 1; i < M; i++)
  {
    prev_row = current_row;
    current_row = 1 - current_row;
    dyn[current_row * N + 0] = i;
    for (int j = 1; j < N; j++)
    {
      dyn[current_row * N + j] = std::min(dyn[prev_row * N + (j - 1)] + (s0[i] != s1[j]), std::min(dyn[prev_row * N + j] + 1, dyn[current_row * N + (j - 1)] + 1));
    }
  }

  return dyn[current_row * N + (N - 1)];
}