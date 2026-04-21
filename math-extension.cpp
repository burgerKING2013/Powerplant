GT, LT, EQ, GTE, LTE,
POW,
ABS,
DISTANCE,
GRAVITY,
SUM
case POW:{
    auto b=st.top(); st.pop();
    auto a=st.top(); st.pop();
    Value r; r.type=0;
    r.num = pow(a.num, b.num);
    st.push(r);
    break;
}
case ABS:{
    auto v=st.top(); st.pop();
    Value r; r.type=0;
    r.num = fabs(v.num);
    st.push(r);
    break;
}
case GT:{
    auto b=st.top(); st.pop();
    auto a=st.top(); st.pop();
    Value r; r.type=0;
    r.num = (a.num > b.num);
    st.push(r);
    break;
}

case LT:{
    auto b=st.top(); st.pop();
    auto a=st.top(); st.pop();
    Value r; r.type=0;
    r.num = (a.num < b.num);
    st.push(r);
    break;
}

case EQ:{
    auto b=st.top(); st.pop();
    auto a=st.top(); st.pop();
    Value r; r.type=0;
    r.num = (a.num == b.num);
    st.push(r);
    break;
}
case DISTANCE:{
    auto t=st.top(); st.pop();
    auto a=st.top(); st.pop();
    auto u=st.top(); st.pop();
    Value r;
    r.type=0;
    r.num = u.num*t.num + 0.5*a.num*t.num*t.num;
    st.push(r);
    break;
}
case GRAVITY:{
    auto r2=st.top(); st.pop();
    auto m2=st.top(); st.pop();
    auto m1=st.top(); st.pop();

    const double G = 6.674e-11;

    Value r;
    r.type=0;
    r.num = G * (m1.num * m2.num) / (r2.num * r2.num);
    st.push(r);
    break;
}
case SUM:{
    int b = stoi(i.arg);
    int a = st.top().num; st.pop();

    double s=0;
    for(int k=a;k<=b;k++) s+=k;

    Value r; r.type=0; r.num=s;
    st.push(r);
    break;
}
if(l[2]=="^"){
    pushVal(l[1]);
    pushVal(l[3]);
    code.push_back({POW,""});
    code.push_back({PRINT,""});
}
if(l[0]=="|"){
    pushVal(l[1]);
    code.push_back({ABS,""});
}
if(l[0]=="ε"){
if(l[0]=="ε"){
    pushVal(l[1]);
    pushVal(l[2]);

    if(l[3]==">") code.push_back({GT,""});
    if(l[3]=="<") code.push_back({LT,""});
    if(l[3]=="==") code.push_back({EQ,""});

    int j=code.size();
    code.push_back({JMPF,"0"});

    i++;
    while(lines[i][0]!="/ε"){
        compileLine(lines,i);
        i++;
    }

    code[j].arg = to_string(code.size());
}
if(l[0]=="Σ"){
    pushVal(l[1]);
    code.push_back({SUM,l[2]});
}
if(l[0]=="d"){
    pushVal(l[1]);
    pushVal(l[2]);
    pushVal(l[3]);
    code.push_back({DISTANCE,""});
}
if(l[0]=="g"){
    pushVal(l[1]);
    pushVal(l[2]);
    pushVal(l[3]);
    code.push_back({GRAVITY,""});
}
π 2 ^ 3
π | -5 |
π Σ 1 10
π d 10 2 3
π g 5 10 2
ε α β >
    π "bigger"
/ε
