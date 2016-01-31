#include <iostream>
#include <string>
#include <stack>
#include <cassert>
#include <cctype>
#include "Map.h"

using namespace std;

bool isOperator(char c){
    return (c == '+' || c=='-' || c=='*' || c=='/');
}

int operate(int var1, int var2, char& o){
    switch (o) {
        case '+':
            return var1+var2;
        case '-':
            return var1-var2;
        case '*':
            return var1*var2;
        case '/':
            if (var2 == 0){
                o = 'p';
                return -1;
            } else{
                return var1/var2;
            }
        default:
            break;
    }
    return 0;
}

int evaluate(string infix, const Map& values, string& postfix, int& result){
    // Evaluates an integer arithmetic expression
    // Precondition: infix is an infix integer arithmetic
    //   expression consisting of single lower case letter operands,
    //   parentheses, and the operators +, -, *, /, with embedded blanks
    //   allowed for readability.
    // Postcondition: If infix is a syntactically valid infix integer
    //   expression whose only operands are single lower case letters
    //   (whether or not they appear in the values map), then postfix is
    //   set to the postfix form of the expression; otherwise postfix may
    //   or may not be changed, result is unchanged, and the function
    //   returns 1.  If infix is syntactically valid but contains at
    //   least one lower case letter operand that does not appear in the
    //   values map, then result is unchanged and the function returns 2.
    //   If infix is syntactically valid and all its lower case operand
    //   letters appear in the values map, then if evaluating the
    //   expression (using for each letter in the expression the value in
    //   the map that corresponds to it) attempts to divide by zero, then
    //   result is unchanged and the function returns 3; otherwise,
    //   result is set to the value of the expression and the function
    //   returns 0.

    //check initial conditions
    for(int i = 0; i<infix.length(); i++){
        if (!(islower(infix[i]) || isOperator(infix[i])|| infix[i]==' ')) {
            return 1;
        }
    }
    //check paranthases
    int l = 0;
    int r = 0;
    for(int i = 0; i<infix.length(); i++){
        if (infix[i]=='(')
            l++;
        if (infix[i]==')')
            r++;
        if (l!=r)
            return 1;
    }
    
    
    postfix = "";
    stack<char> opers;
    for (int i = 0; i < infix.length(); i++) {
        switch (infix[i]) {
            case ' ':
                break;
            case '(':
                opers.push(infix[i]);
                break;
            case ')':
                while (opers.top() != '('){
                    postfix = postfix + opers.top();
                    opers.pop();
                }
                opers.pop();
                break;
            case '+':
                while(!opers.empty() && opers.top()!= '('){
                    postfix = postfix + opers.top();
                    opers.pop();
                }
                opers.push('+');
                break;
            case '-':
                while(!opers.empty() && opers.top()!= '('){
                    postfix = postfix + opers.top();
                    opers.pop();
                }
                opers.push('-');
                break;
            case '*':
                while(!opers.empty() && opers.top()!= '(' && opers.top() != '+' && opers.top() != '-'){
                    postfix = postfix + opers.top();
                    opers.pop();
                }
                opers.push('*');
                break;
            case '/':
                while(!opers.empty() && opers.top()!= '(' && opers.top() != '+' && opers.top() != '-'){
                    postfix = postfix + opers.top();
                    opers.pop();
                }
                opers.push('/');
                break;
            default:
                
                postfix = postfix + infix[i];
                break;
        }
        
    }
    while(!opers.empty()){
        postfix = postfix + opers.top();
        opers.pop();
    }
    
    //collect value
    stack<int> nums;
    for (int i = 0; i<postfix.length(); i++){
        if(isOperator(postfix[i])){
            int var2 = nums.top();
            nums.pop();
            int var1 = nums.top();
            nums.pop();
            int ans = operate(var1, var2, postfix[i]);
            if (postfix[i] == 'p')
                return 3;
            nums.push(ans);
        } else{
            int val = 0;
            if(values.contains(postfix[i]))
                values.get(postfix[i], val);
            else
                return 2;
            nums.push(val);
        }
    }
    result = nums.top();
    return 0;

}

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
    //assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
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
    cout << evaluate("a+e", m, pf, answer) << endl;
    cout << pf <<endl;
    cout << answer << endl;
    
    
}
