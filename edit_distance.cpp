#include <iostream>
#include "edit_distance.hpp"

typedef std::vector<std::vector<std::vector<Point> > > border_type;
typedef std::vector<rle::RLE_run> rle_string;

int min(int a, int b, int c)
{
  return std::min(a, std::min(b, c));
}

std::string traj_to_string(std::vector<Point> A)
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

int get_edit_dist(const int M, const int N, const std::string &s0, const std::string &s1)
{
  int dyn[M + 1][N + 1];

  for (int i = 0; i <= M; i++)
  {
    dyn[i][0] = i;
  }

  for (int i = 0; i <= N; i++)
  {
    dyn[0][i] = i;
  }

  for (int i = 1; i <= M; i++)
  {
    for (int j = 1; j <= N; j++)
    {
      dyn[i][j] = min(dyn[i - 1][j - 1] + (s0[i] != s1[j]), dyn[i - 1][j] + 1, dyn[i][j - 1] + 1);
    }
  }

  return dyn[M][N];
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
  if((points[l].y - points[l-1].y)*(p.x - points[l-1].x) == (p.y - points[l-1].y) * (points[l].x - points[l-1].x))
  {
    points.pop_back();
  }
  points.push_back(p);
}

void init_input_border(border_type &LEFT, border_type &TOP, int M, int N, rle_string s0, rle_string s1)
{
  int char_count = 0;
  for (int i = 1; i < M; i++)
  {
    LEFT[i][1].push_back(Point(1, s0[i].len + char_count));
    LEFT[i][1].push_back(Point(s0[i].len + 1, char_count));
    char_count += s0[i].len;
  }

  char_count = 0;
  for (int j = 1; j < N; j++)
  {
    TOP[1][j].push_back(Point(1, char_count));
    TOP[1][j].push_back(Point(s1[j].len + 1, s1[j].len + char_count));
    char_count += s1[j].len;
  }
}

void get_input_border(border_type &LEFT, border_type &TOP, border_type OUT, int i, int j, std::vector<std::vector<int > > dyn, rle_string s0, rle_string s1)
{
  if (LEFT[i][j].empty())
  {
    // This is the first point
    if (j <= 1)
    {
      std::cout << "Column should be greater than 1!\n";
      return;
    }

    add_point(LEFT[i][j], Point(0, dyn[i][j - 1]));
    // width and height of block to the left
    int w = s1[j - 1].len;
    int h = s0[i].len;
    for (int k = 0; k < w + h + 1; k++)
    {
      // These points are part of the left input border
      // The very last point should also be carried over here, TODO: check
      if (OUT[i][j - 1][k].x > w)
      {
        int index = k - w;
        int value = OUT[i][j - 1][k].y;
        add_point(LEFT[i][j], Point(index, value));
      }
    }
  }

  if (TOP[i][j].empty())
  {
    if (i <= 1)
    {
      std::cout << "Row index should be greater than 1!\n";
      return;
    }
    // Width and height of block above
    int w = s1[j].len;
    int h = s0[i - 1].len;
    // The first point should be added here automatically, TODO: check
    for (int k = 0; k < w; k++)
    {
      add_point(TOP[i][j], OUT[i - 1][j][k]);
    }
    // Adding the very last point
    add_point(TOP[i][j], Point(w, dyn[i - 1][j]));
  }
}

int get_val_at_coord(int coord, std::vector<Point> points)
{
  if (coord < points[0].x || coord > points.back().x)
  {
    std::cout << "Coord should not be outside of the range of points given!\n";
    return 0;
  }

  int left = 0, right = points.size() - 1, middle;
  // Leftmost binary search
  while (left < right)
  {
    middle = (left + right) / 2;
    if (points[middle].x < coord)
      left = middle + 1;
    else
      right = middle;
  }

  if (points[left].x == coord)
  {
    return points[left].y;
  }

  int x1 = points[left].x, y1 = points[left].y, x2 = points[left + 1].x, y2 = points[left + 1].y;
  return y1 + ((y2 - y1) / (x2 - x1)) * (coord - x1);
}

int get_coord_for_val(int val, std::vector<Point> points)
{
  if (val < points.begin()->y || val > points.back().y)
  {
    std::cout << "Value should not be outside of the range of the points given!\n";
    return 0;
  }

  int left = 0, right = points.size() - 1, middle;
  // Leftmost binary search
  while (left < right)
  {
    middle = (left + right) / 2;
    if (points[middle].y < val)
      left = middle + 1;
    else
      right = middle;
  }

  Point p = points[left];
  if (p.y == val)
  {
    return p.x;
  }
  int x1 = points[left].x, y1 = points[left].y, x2 = points[left + 1].x, y2 = points[left + 1].y;
  if (y2 == y1)
  {
    std::cout << "Slope should not be zero!\n";
    return 0;
  }
  return x1 + ((val - y1) / (y2 - y1)) * (x2 - x1);
}

Point get_intersection(std::vector<Point> points, Point p1, Point p2, int offset)
{
  if (p1.y == p2.y)
  {
    return Point(get_coord_for_val(p1.y, points) + offset, p1.y);
  }
  int slope = -1;
  if (slope != (p2.y - p1.y) / (p2.x - p1.x))
  {
    std::cout << "Stope is wrong!\n";
    return Point(0,0);
  }
  // move points left
  p1.x -= offset;
  p2.x -= offset;
  int val_of_line;
  for (int i = 0; i < points.size() - 1; i++)
  {
    val_of_line = p1.y + slope * (points[i].x - p1.x);
    if (points[i].y == val_of_line)
    {
      return Point(points[i].x + offset, val_of_line);
    }
    if (points[i + 1].y > val_of_line)
    {
      return Point(get_coord_for_val(val_of_line, points) + offset, val_of_line);
    }
  }
  val_of_line = p2.y;
  if (points.back().y == val_of_line)
  {
    return Point(points.back().x + offset, val_of_line);
  }
  std::cout << "Something not right in get_intersection_coord. Intersection not found!\n";
  return Point(0,0);
}

std::vector<Point> get_cswm(std::vector<Point> S, int h)
{
  if (!(h > 0))
  {
    std::cout << "Window size must me greater than 0!\n";
    return std::vector<Point>();
  }
  std::vector<Point> S_CSWM, L;
  add_point(S_CSWM, S[0]);
  add_point(L, Point(S[0].x - h + 1, S[0].y));
  add_point(L, S[0]);
  for (int i = 0; i < S.size() - 1; i++)
  {
    Point current = S[i], next = S[i + 1];
    int slope = (next.y - current.y)/(next.x - current.x);

    bool intersection_found = false;
    bool next_point_encountered = false;
    for(int k = 0; k < L.size(); k++)
    {
      Point p = L[k];
      // We reached point i+1 and we need to stop
      if(p.x + (h-1) > next.x)
        break;
      int s_val_at_curr_point = current.y + slope * (p.x + (h-1) - current.x);
      if(p.y <= s_val_at_curr_point)
      {
        add_point(S_CSWM, Point(p.x + (h-1), p.y));
      }
      else
      {
        add_point(S_CSWM, Point(p.x + (h-1), s_val_at_curr_point));
        intersection_found = true;
        break;
      }
    }
    if(intersection_found)
    {
      add_point(S_CSWM, next);
      std::vector<Point> new_L;
      add_point(new_L, Point(next.x - (h - 1), next.y));
      add_point(new_L, next);
      L = new_L;
    } 
    else
    {
      // Truncate the back part that is higher than next point
      if (next.y > L.back().y)
      {
        L.push_back(next);
      }
      else
      {
        int x = get_coord_for_val(next.y, L);
        while (L.back().x >= x)
        {
          L.pop_back();
        }
        add_point(L, Point(x, next.y));
        add_point(L, next);
      }
      // Remove elements with x coord smaller than next.x - (h - 1)
      Point last_point = Point(0,0);
      while(!L.empty() && L[0].x <= next.x - (h - 1))
      {
        last_point = L[0];
        L.erase(L.begin());
      }
      int val = last_point.y + (L[0].y - last_point.y)/(L[0].x - last_point.x) * (next.x - (h - 1) - last_point.x);
      L.insert(L.begin(), Point(next.x - (h - 1), val));
    }
  }

  for(int i = 0; i < L.size(); i++)
  {
    if(L[i].x + h - 1 > S.back().x && L[i].x + h - 1 <= S.back().x + h - 1)
    {
      add_point(S_CSWM, Point(L[i].x + h - 1, L[i].y));
    }
  }
  return S_CSWM;
}
// Returns the lower part of two trajectories
std::vector<Point> get_lower_part(std::vector<Point> A, std::vector<Point> B)
{
  std::vector<Point> sol;
  if(A.begin()->x != B.begin()->x || A.back().x != B.back().x)
  {
    std::cout<<"Both trajectories should have the same start and end points!\n";
    return sol;
  }
  
  
  return A;
}

void propagate_2(int h, int w, std::vector<Point> LEFT_CSWM, std::vector<Point> TOP_CSWM, std::vector<Point>& LEFT_OUT, std::vector<Point>& TOP_OUT)
{
  if(h <= w)
  {
    for(int k = 0; k < LEFT_CSWM.size(); k++)
    {
      Point p = LEFT_CSWM[k];
      if(p.x < h)
      {
        add_point(LEFT_OUT, Point(p.x, p.y + p.x - 1));
      }
      else
      {
        add_point(LEFT_OUT, Point(p.x + w - h, p.y + w - 1));
      }          
    }
    for(int k = 0; k < TOP_CSWM.size(); k++)
    {
      Point p = TOP_CSWM[k];
      if(p.x < w)
      {
        add_point(TOP_OUT, Point(p.x, p.y + h - 1));
      }
      else
      {
        add_point(TOP_OUT, Point(p.x, p.y + (w + h - 1) - p.x));
      }
    }
  }
  else
  {
    for(int k = 0; k < LEFT_CSWM.size(); k++)
    {
      Point p = LEFT_CSWM[k];
      if(p.x < w)
      {
        add_point(LEFT_OUT, Point(p.x, p.y + p.x - 1));
      }
      else
      {
        add_point(LEFT_OUT, Point(p.x, p.y + w - 1));
      }
    }
    for(int k = 0; k < TOP_CSWM.size(); k++)
    {
      Point p = TOP_CSWM[k];
      if(p.x < w)
      {
        add_point(TOP_OUT, Point(p.x, p.y + h - 1));
      }
      else
      {
        add_point(TOP_OUT, Point(p.x + w - h, p.y + (w + h - 1) - p.x));
      } 
    }
  }
}

int get_rle_edit_dist(rle_string s0, rle_string s1)
{
  const int M = s0.size();
  const int N = s1.size();
  std::vector< std::vector< int > > dyn(M, std::vector<int>(N));
  // The following are a graphical representation for the Input (split into LEFT and TOP)
  // and Output border for each block
  border_type LEFT(M, std::vector<std::vector< Point > >(N));
  border_type TOP(M, std::vector<std::vector< Point > >(N));
  border_type OUT(M, std::vector<std::vector< Point > >(N));

  // Initialise input border
  init_input_border(LEFT, TOP, M, N, s0, s1);
  for (int i = 1; i < M; i++)
  {
    for (int j = 1; j < N; j++)
    {
      // Retrieve input border for current block
      get_input_border(LEFT, TOP, OUT, i, j, dyn, s0, s1);

      int h = s0[i].len + 1;
      int w = s1[j].len + 1;
      std::vector<Point> LEFT_CSWM, TOP_CSWM, LEFT_OUT, TOP_OUT;
      int window = std::min(h, w);
      LEFT_CSWM = get_cswm(LEFT[i][j], window);
      TOP_CSWM = get_cswm(TOP[i][j], window);
      // Propagate 2
      propagate_2(h, w, LEFT_CSWM, TOP_CSWM, LEFT_OUT, TOP_OUT);
      // Propagate 3
      OUT[i][j] = get_lower_part(LEFT_OUT, TOP_OUT);
      dyn[i][j] = get_val_at_coord(w-1, OUT[i][j]);
    }
  }

  return dyn[M - 1][N - 1];
}