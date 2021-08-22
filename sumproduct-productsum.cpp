/*
    This Program Use Lexical an Sintatical Analysis to validate an product sum or sum product expression.
*/

// How to Compile "g++ sumproduct-productsum.cpp -o exec -O2 -Wall -std=c++17"
// How to execute ".\exec"

#include <bits/stdc++.h>
using namespace std;

#define isLetter(symbol) (symbol >= 'A' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z')
void lexicalAnalysis(string expression, vector<string>& decomposition)
{
    /*
        Decompose the given expression in primitive elements(tokens).
        Save in decomposition.
    */
    string current;
    // Almost O(n)
    for(auto symb: expression)
    {
        if(isLetter(symb))
            current = symb;
        else if(symb == '\'' && current.size()!=0)
        {
            current += '\'';
            decomposition.push_back(current);
            current.clear();
        }
        else if(symb == '.' || symb == '+' || symb == '(' || symb == ')')
        {
            if(current.size()!=0) decomposition.push_back(current);
            current.clear();
            
            current = symb;
            decomposition.push_back(current);
            current.clear();
        }
        else if((symb == '\'' && current.size()==0) || symb != ' ')
        {
            decomposition.clear();
            return;
        }
    }
    if(current.size()!=0) decomposition.push_back(current);
}

queue<string> posfixTransform(vector<string>& lexical)
{
    map<string, int> precedence;
    precedence["."] = 2;
    precedence["+"] = 1;

    stack<string> opStack;
    queue<string> out;

    for(auto token: lexical)
    {
        if(token == "(")
            opStack.push("(");
        else if(token == "." || token == "+")
        {
            while(!opStack.empty() && opStack.top() != "(" and precedence[token] < precedence[opStack.top()])
            {
                out.push(opStack.top());
                opStack.pop();
            }
            opStack.push(token);
        }
        else if(token == ")")
        {
            while(!opStack.empty() && opStack.top()!="(")
            {
                out.push(opStack.top());
                opStack.pop();
            }
            if(!opStack.empty()) opStack.pop();
        }
        else
            out.push(token);
    }

    while(!opStack.empty())
    {
        out.push(opStack.top());
        opStack.pop();
    }

    return out;
}

vector<string> sintaticalAnalysis(queue<string> posfixQueue, char type)
{
    /*
        Create the final notation to a product sum or sum product typed.

        Types:
            s = "Sum Product"
            p = "Product Sum"
    */
    if(posfixQueue.size()==0)return {};

    stack<string> elements, expr;
    string current;
    string a,b,c;

    // O(n)
    while(!posfixQueue.empty())
    {
        c = posfixQueue.front();
        posfixQueue.pop();
        if(c != "." && c != "+")
            elements.push(c);
        else
        {
            a = elements.top();
            elements.pop();
            b = elements.top();
            elements.pop();

            if((type=='p'&&c==".") || (type=='s'&&c=="+"))
            {
                expr.push(a);
                elements.push(b);
                continue;
            }

            elements.push(a+","+b);
        }
    }

    while(!elements.empty())
    {
        expr.push(elements.top());
        elements.pop();
    }

    // O(n²log(n)) => Deverá ser otimizada
    vector<set<string>> sintatical;
    while(!expr.empty())
    {
        string current = "";
        set<string> element;
        for(auto symbol: expr.top())
        {
            if(symbol==',')
            {
                element.insert(current);
                current = "";
            }
            else current += symbol;
        }
        
        if(current != "")element.insert(current);
        sintatical.push_back(element);
        expr.pop();
    }

    vector<string> result;
    for(auto element:sintatical)
    {
        string term="";
        for(auto item:element)
            term+=item;
        result.push_back(term);
    }

    return result;
}

bool is_equal(vector<string> expr1, vector<string> expr2)
{
    if(expr1.size()!=expr2.size()) return false;

    unsigned char indices[expr1.size()];
    memset(indices, 0, expr1.size());

    // O(n²)
    for(long long unsigned int i = 0; i < expr1.size();++i)
    {
        bool found = false;
        for(long long unsigned int j = 0; j < expr2.size();++j)
        {
            if(expr1[i] == expr2[j] && indices[j] != 1)
            {
                indices[j] = 1;
                found = true;
                break;
            }
        }
        if(!found) return false;
    }

    return true;
}

int main()
{
    vector<string> out;
    lexicalAnalysis("A'.B.C+(A.B.C)+(D.E.F)", out);
    queue<string> posfix = posfixTransform(out);
    vector<string> p = sintaticalAnalysis(posfix, 's');

    vector<string> out2;
    lexicalAnalysis("(E.F.D)+(A.B)+C.B.A'", out2);
    queue<string>posfix2 = posfixTransform(out2);
    vector<string> s = sintaticalAnalysis(posfix2, 's');

    cout<<(is_equal(p,s)?"YES":"NO")<<"\n";
}