#include<iostream>
#include<stdio.h>
#include<string.h>
using namespace std;

void exp(void);
void term(void);
void fact(void);
void id(void);
void num(void);
char str[100];
int f=0;
int i=0;
int l;

int main(void)
{
    cout<<"\nCFG:\n";
    cout<<"\t<Exp> -> <Term> + <Term> | <Term> - <Term> | <Term>\n";
    cout<<"\t<Term> -> <Factor> * <Factor> | <Factor> / <Factor> | <Factor>\n";
    cout<<"\t<Factor> -> (Exp) | id | num\n";
    cout<<"\tID -> a | b | c | d |e \n";
    cout<<"\tnum -> 0 | 1 | 2 | ... | 9\n";
    cout<<"Enter a string to Parse: ";
    cin>>str;

    l=strlen(str);
    if(l>=1)
        exp();
    else
        cout<<"\nInvalid String";
    if(l==i && f)
        cout<<"\nValid String";
    else
        cout<<"\nInvalid String";
    return 0;
}
void exp()
{
    term();
    if(f && (str[i]=='+'||str[i]=='-'))
    {
        i++;
        term();
    }
}
void term()
{
    fact();
    if(f && (str[i]=='*'||str[i]=='/'))
    {
        i++;
        fact();
    }
}
void fact()
{
    if(i<l && str[i]=='(')
    {
        i++;
        f=1;
        exp();
        if(f && str[i]==')')
            i++;
        else
        {
            f=0;
            return;
        }
    }
    else
    {
        if(i<l && (str[i]=='0'||str[i]=='1'||str[i]=='2'||str[i]=='3'||str[i]=='4'||
                   str[i]=='5'||str[i]=='6'||str[i]=='7'||str[i]=='8'||str[i]=='9'))
        {
            i++;
            f=1;
            return;
        }
        else if(i<l && (str[i]=='a'||str[i]=='b'||str[i]=='c'||str[i]=='d'||str[i]=='e'))
        {
           i++;
           f=1;
           return;
        }
        else
        {
            f=0;
            return;
        }
    }
}
