#include <iostream>
#include <string>
#include <queue>

using namespace std;
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec){
    // Return true if there is a path from (sr,sc) to (er,ec)
    // through the maze; return false otherwise
    queue<Coord> list;
    Coord begin(sr, sc);
    Coord end(er, ec);
    list.push(begin);
    maze[sr][sc] = '-';
    
    while (!list.empty()) {
        Coord top = list.top();
        
        list.pop();
        if (top.r() == er && top.c() == ec)
            return true;
        if (maze[top.r()-1][top.c()] == '.'){ // check north
            maze[top.r()-1][top.c()] = '-';
            list.push(Coord(top.r()-1, top.c()));
        }
        if (maze[top.r()][top.c()+1] == '.'){ // check east
            maze[top.r()][top.c()+1] = '-';
            list.push(Coord(top.r(), top.c()+1));
        }
        if (maze[top.r()+1][top.c()] == '.'){ // check south
            maze[top.r()+1][top.c()] = '-';
            list.push(Coord(top.r()+1, top.c()));
        }
        if (maze[top.r()][top.c()-1] == '.'){ // check west
            maze[top.r()][top.c()-1] = '-';
            list.push(Coord(top.r(), top.c()-1));
        }
    }
    
    return false;
    
}

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};


int main(){
    string maze[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XX.X.XXXXX",
        "X..X.X...X",
        "X..X...X.X",
        "XXXX.XXX.X",
        "X.X....XXX",
        "X..XX.XX.X",
        "X...X....X",
        "XXXXXXXXXX"
    };
    
    if (pathExists(maze, 10,10, 6,4, 1,1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}