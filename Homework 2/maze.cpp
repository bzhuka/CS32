//
//  main.cpp
//  maze3
//
//  Created by Bradley Zhu on 2/5/16.
//  Copyright © 2016 Bradley Zhu. All rights reserved.
//
/*
#include <iostream>
#include <string>

using namespace std;
*/
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
    if (sr == er && sc == ec) {
        return true;
    }
    maze[sr][sc] = 'x';
    if (maze[sr-1][sc] == '.') {//north
        if(pathExists(maze, nRows, nCols, sr-1, sc, er, ec)) {
            return true;
        }
    }
    if (maze[sr][sc-1] == '.') {//east
        if(pathExists(maze, nRows, nCols, sr, sc-1, er, ec)) {
            return true;
        }
    }
    if (maze[sr+1][sc] == '.') {//south
        if(pathExists(maze, nRows, nCols, sr+1, sc, er, ec)) {
            return true;
        }
    }
    if (maze[sr][sc+1] == '.') {//west
        if(pathExists(maze, nRows, nCols, sr, sc+1, er, ec)) {
            return true;
        }
    }
    return false;
}
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise
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
}
*/