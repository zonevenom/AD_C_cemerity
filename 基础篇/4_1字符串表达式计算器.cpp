#include <iostream>
#include<cstring>

using namespace std;

/*运算符优先级排序*/
int priority(char op){
    int value=0;
    if(op=='*'||op=='/'){
        value = 2;
    }else if(op=='+'||op=='-'){
        value = 1;
    }
    return value;
}

/*加减乘除运算*/
int apply(int a,int b,char c){
    switch (c)
    {
    case '+': return a+b;
    case '-': return a-b;
    case '*': return a*b;
    case '/': return a/b;
    }
    return 0;
}

/*字符串运算*/
int completestr(char* str){
    int len = strlen(str);
    int value[10]; //操作数
    char ops[10];  //运算符
    //这里运用的是静态数组在计算机的内存地址是连续的特性
    int v=0,o=0;//操作数和运算符数组的下标
    for(int i=0;i<len;i++){
        if(str[i] == ' ') continue;
        if(str[i]>='0'&&str[i]<='9'){ //处理数字
            int temp =0;
            while (i<len &&str[i]>='0'&&str[i]<='9'){
                temp = temp *10 + (str[i]-'0');
                i++;
            }
            value[v++]=temp;
            i--;
        }else if(str[i] == '('){ //处理左括号
            ops[o++] = str[i];
        }else if(str[i] == ')'){ //处理右括号
            while(o>0 && ops[o-1] !='('){
                int b = value[--v];
                int a = value[--v];
                char op = ops[--o];
                value[v++]=apply(a,b,op);
            }
            o--; //去除左括号
        }else { //运算符优先级运算
            while(o>0 && priority(ops[o-1])>=priority(str[i])){
                int b = value[--v];
                int a = value[--v];
                char op = ops[--o];
                value[v++]=apply(a,b,op);
            }
            ops[o++] = str[i];
        }
    }
    while(o>0){
        int b = value[--v];
        int a = value[--v];
        char op = ops[--o];
        value[v++]=apply(a,b,op);
    }
    return value[0];
}

int main(){
    char* str;
    cin.getline(str,1000);
    cout<<completestr(str)<<endl;
    return 0;
}
