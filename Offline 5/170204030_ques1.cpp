#include<iostream>
#include<stdio.h>
#include<string.h>
using namespace std;
void A(void);
void X(void);
char str[100];
int f=0;
int i=0;
int l;

int main(void)
{
    cout<<"\nCFG:\n";
    cout<<"\tA -> aXd\n";
    cout<<"\tX -> bbX\n";
    cout<<"\tX -> bcX\n";
    cout<<"\tX -> dx\n";
    cout<<"Enter a string to Parse: ";
    cin>>str;




    l=strlen(str);
    if(l>=1)
        A();
    else
        cout<<"\nInvalid String";
    if(l==i && f)
        cout<<"\nValid String";
    else
        cout<<"\nInvalid String";
    return 0;
}

void A()
{
    if(str[i]=='a')
    {
        i++;
        X();

        if(f && str[i]=='d')
        {
            f=1;
            return;
        }

    }
    else
    {
        f=0;
        return;
    }
}

void X()
{
    if((l-1)==i)
    {
        f=1;
        i++;
        return;
    }
    else
    {
        if (str[i] == 'b' || str[i] == 'd')
        {
            i++;
            if(str[i]=='b'||str[i]=='c')
            {
                i++;
                X();
            }
        }

        else
        {
            f=0;
            return;
        }
    }
}
