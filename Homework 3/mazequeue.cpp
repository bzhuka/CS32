//
//  mazequeue.cpp
//  mazequeue
//
//  Created by Bradley Zhu on 1/30/16.
//  Copyright © 2016 Bradley Zhu. All rights reserved.
//

#include <queue>
#include <string>
#include <iostream>

using namespace std;

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

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
    // Return true if there is a path from (sr,sc) to (er,ec)
    // through the maze; return false otherwise
    queue<Coord> cstack;
    
    cstack.push(Coord(sr, sc));
    maze[sr][sc] = 'C';
    while (!cstack.empty()) {
        Coord a = cstack.front();
        //cout << "(" << a.r() << "," << a.c() << ")" << endl;
        cstack.pop();
        if(a.r()==er && a.c()==ec) {
            return true;
        }
        if(maze[a.r()-1][a.c()] == '.') {
            cstack.push(Coord(a.r()-1, a.c()));
            maze[a.r()-1][a.c()] = 'C';
        }
        if(maze[a.r()][a.c()+1] == '.') {
            cstack.push(Coord(a.r(), a.c()+1));
            maze[a.r()][a.c()+1] = 'C';
        }
        if(maze[a.r()+1][a.c()] == '.') {
            cstack.push(Coord(a.r()+1, a.c()));
            maze[a.r()+1][a.c()] = 'C';
        }
        if(maze[a.r()][a.c()-1] == '.') {
            cstack.push(Coord(a.r(), a.c()-1));
            maze[a.r()][a.c()-1] = 'C';
        }
    }
    return false;
}
/*
int main()
{
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
}*/