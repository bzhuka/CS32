//
//  main.cpp
//  Eval
//
//  Created by Bradley Zhu on 2/1/16.
//  Copyright © 2016 Bradley Zhu. All rights reserved.
//

#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

bool isOp(char ch);

bool isPar(char ch);

bool isValid(string infix);

bool prec(char ch, char top);

int apply(char ch, int op, int op2);

string removeSpaces(string infix);

int evaluate(string infix, const Map& values, string& postfix, int& result) {
    infix = removeSpaces(infix);
    if (!isValid(infix)) {
        return 1;
    }
    
    stack<char> cstack;
    postfix = "";
    for (int i = 0 ; i < infix.length(); i++) {
        if (isalpha(infix[i])) {
            postfix = postfix + infix[i];
        } else if (infix[i] == '(') {
            cstack.push(infix[i]);
        } else if (infix[i] == ')') {
            while (cstack.top() != '(') {
                postfix = postfix + cstack.top();
                cstack.pop();
            }
            cstack.pop();
        } else if (isOp(infix[i])) {
            while ((!cstack.empty()) && (cstack.top() != '(') && prec(infix[i], cstack.top())) {
                postfix = postfix + cstack.top();
                cstack.pop();
            }
            cstack.push(infix[i]);
        }
    }
    while (!cstack.empty()) {
        postfix = postfix + cstack.top();
        cstack.pop();
    }
    
    for (int i = 0; i < infix.length(); i++) {
        if (isalpha(infix[i])) {
            if(!values.contains(infix[i])) {
                return 2;
            }
        }
    }
    
    stack<int> istack;
    int val;
    int operand;
    int operand2;
    for (int i = 0; i < postfix.length(); i++) {
        if(isalpha(postfix[i])) {
            values.get(postfix[i], val);
            istack.push(val);
        } else {
            operand2 = istack.top();
            istack.pop();
            operand = istack.top();
            istack.pop();
            if(postfix[i] == '/' && operand2 == 0) {
                return 3;
            }
            istack.push(apply(postfix[i], operand, operand2));
        }
    }
    result = istack.top();
    return 0;
}

string removeSpaces(string infix) {
    string result;
    for (int i = 0; i < infix.length(); i++) {
        if (infix[i] != ' ') {
            result = result + infix[i];
        }
    }
    return result;
}

bool isValid(string infix) {
    bool hasL = false;
    bool lastal = false;
    bool lastop = true;
    int nump = 0;
    if(!(isalpha(infix[0]) || isPar(infix[0]))) {
        return false;
    }
    for (int i = 0; i < infix.length(); i++) {
        if (isalpha(infix[i])) {
            if (isupper(infix[i])) {
                return false;
            }
            hasL = true;
            if (lastal) {
                return false;
            }
            lastal = true;
            lastop = false;
        } else if (isOp(infix[i])) {
            if (lastop) {
                return false;
            }
            lastal = false;
            lastop = true;
        } else if (infix[i] == '('){
            if(i == infix.length()-1 ){
                return false;
            } else if (infix[i+1] == ')') {
                return false;
            }
            if (lastal) {
                return false;
            }
            nump++;
        } else if (infix[i] == ')') {
            if (lastop) {
                return false;
            }
            nump--;
        } else {
            return false;
        }
    }
    if (!isalpha(infix[infix.length()-1]) && !isPar(infix[infix.length()-1])) {
        return false;
    }
    if(nump != 0) {
        return false;
    }
    return hasL;
}

bool prec(char ch, char top) {
    if (ch == '*' || ch == '/') {
        if (top == '+' || top == '-') {
            return false;
        }
    }
    return true;
}

bool isOp(char ch) {
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
        return true;
    }
    return false;
}

bool isPar(char ch) {
    if (ch == '(' || ch == ')') {
        return true;
    }
    return false;
}

int apply(char ch, int op, int op2) {
    if (ch == '+') {
        return op + op2;
    } else if (ch == '-') {
        return op-op2;
    } else if (ch == '*') {
        return op * op2;
    } else {
        return op / op2;
    }
}
/*
int main()
{
    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
    int  vals[] = {  3,  -9,   6,   2,   4,   1  };
    Map m;
    for (int k = 0; vars[k] != '#'; k++)
        m.insert(vars[k], vals[k]);
    string pf;
    int answer;
    assert(evaluate("a+ e", m, pf, answer) == 0  &&
           pf == "ae+"  &&  answer == -6);
    answer = 999;
    assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
    // unary operators not allowed:
    assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a*b", m, pf, answer) == 2  &&
           pf == "ab*"  &&  answer == 999);
    assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0  &&
           pf == "yoau-*+"  &&  answer == -1);
    answer = 999;
    assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
           pf == "oyy-/"  &&  answer == 999);
    assert(evaluate(" a  ", m, pf, answer) == 0  &&
           pf == "a"  &&  answer == 3);
    assert(evaluate("((a))", m, pf, answer) == 0  &&
           pf == "a"  &&  answer == 3);
    cout << "Passed all tests" << endl;
}*/