#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <sstream>
#include <cmath>
#include <cctype>

using namespace std;

// =====================
// VALUE SYSTEM
// =====================
struct Value {
    double num = 0;
    string str = "";
    vector<Value> arr;
    int type = 0; // 0=num,1=str,2=array
};

// =====================
// BYTECODE
// =====================
enum Op {
    PUSH_NUM, PUSH_STR,
    LOAD, STORE, DEL,
    ADD, SUB, MUL, DIV,
    PRINT,
    JMP, JMPF,
    MAKE_ARR, ARR_GET,
    CALL, RET,
    INPUT,
    SQRT, FORCE, ENERGY, VELOCITY
};

struct Instr {
    Op op;
    string arg;
};

// =====================
// VM
// =====================
map<string,Value> globals;
vector<Instr> code;
map<string,int> funcs;

stack<Value> st;

struct Frame {
    int ret;
    map<string,Value> locals;
};

stack<Frame> callStack;

// ---------------------
Value getVar(string n){
    if(!callStack.empty() &&
       callStack.top().locals.count(n))
        return callStack.top().locals[n];
    return globals[n];
}

void setVar(string n, Value v){
    if(!callStack.empty())
        callStack.top().locals[n]=v;
    else globals[n]=v;
}

// =====================
// VM EXECUTION
// =====================
void run(){

    int ip=0;

    while(ip < code.size()){

        auto &i = code[ip];

        switch(i.op){

        case PUSH_NUM:{
            Value v; v.type=0; v.num=stod(i.arg);
            st.push(v); break;
        }

        case PUSH_STR:{
            Value v; v.type=1; v.str=i.arg;
            st.push(v); break;
        }

        case LOAD: st.push(getVar(i.arg)); break;

        case STORE:{
            auto v=st.top(); st.pop();
            setVar(i.arg,v); break;
        }

        case DEL:
            globals.erase(i.arg);
            break;

        case ADD:{
            auto b=st.top(); st.pop();
            auto a=st.top(); st.pop();
            Value r;
            if(a.type==1 || b.type==1){
                r.type=1; r.str=a.str + b.str;
            } else {
                r.type=0; r.num=a.num + b.num;
            }
            st.push(r); break;
        }

        case SUB:{
            auto b=st.top(); st.pop();
            auto a=st.top(); st.pop();
            Value r; r.type=0; r.num=a.num-b.num;
            st.push(r); break;
        }

        case MUL:{
            auto b=st.top(); st.pop();
            auto a=st.top(); st.pop();
            Value r; r.type=0; r.num=a.num*b.num;
            st.push(r); break;
        }

        case DIV:{
            auto b=st.top(); st.pop();
            auto a=st.top(); st.pop();
            Value r; r.type=0; r.num=a.num/b.num;
            st.push(r); break;
        }

        case PRINT:{
            auto v=st.top(); st.pop();
            if(v.type==0) cout<<v.num<<endl;
            if(v.type==1) cout<<v.str<<endl;
            if(v.type==2) cout<<"[array size "<<v.arr.size()<<"]"<<endl;
            break;
        }

        case JMP: ip=stoi(i.arg); continue;

        case JMPF:{
            auto v=st.top(); st.pop();
            if(v.num==0){ ip=stoi(i.arg); continue; }
            break;
        }

        case MAKE_ARR:{
            int n=stoi(i.arg);
            Value v; v.type=2;
            for(int j=0;j<n;j++){
                v.arr.push_back(st.top());
                st.pop();
            }
            st.push(v); break;
        }

        case ARR_GET:{
            auto idx=st.top(); st.pop();
            auto arr=st.top(); st.pop();
            st.push(arr.arr[(int)idx.num]);
            break;
        }

        case CALL:{
            Frame f; f.ret=ip+1;

