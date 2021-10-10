#ifndef parsing_H

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<stdbool.h>

void factor();
void term();
void stat();
void smple_expn();
void relop();

char inputString[10000];
int lenOfSt;
int lookheadIndex = 0;
bool flag = false;
bool isIF = false;

// check ID
bool isValidID(char t)
{
    if( t == 'a' || t == 'b'|| t == 'c'|| t == 'd'|| t == 'e' ) return true;
    return false;
}

bool isValidNum(char t)
{
    if( t == '1' || t == '2'|| t == '3'|| t == '4'|| t == '5'|| t == '6' || t == '7'|| t == '8' || t == '9')return true;
    return false;
}


bool isID()
{
    if(isValidID(inputString[lookheadIndex]) && inputString[lookheadIndex+1] == '=')
    {
        return true;
    }
    return false;
}

void factor()
{
    if(inputString[lookheadIndex] == 'e'
            && inputString[lookheadIndex+1] == 'l'
            && inputString[lookheadIndex+2] == 's'
            && inputString[lookheadIndex+3] == 'e' )
    {
        return;
    }

    //-----------
    if(inputString[lookheadIndex] == 'a'
            || inputString[lookheadIndex] == 'b'
            || inputString[lookheadIndex] == 'c'
            || inputString[lookheadIndex] == 'd'
            || inputString[lookheadIndex] == 'e')
    {
        lookheadIndex++;
        flag = true;
        return;
    }
    else
    {
        flag = false;
    }
    if(inputString[lookheadIndex] == '0'
            || inputString[lookheadIndex] == '1'
            || inputString[lookheadIndex] == '2'
            || inputString[lookheadIndex] == '3'
            || inputString[lookheadIndex] == '4'
            ||inputString[lookheadIndex] == '5'
            || inputString[lookheadIndex] == '6'
            || inputString[lookheadIndex] == '7'
            || inputString[lookheadIndex] == '8'
            || inputString[lookheadIndex] == '9')
    {
        lookheadIndex++;
        flag = true;
        return;
    }
    else
    {
        flag = false;
    }

    if(inputString[lookheadIndex] == '(')
    {
        lookheadIndex++;
        flag = true;
        smple_expn();
        if(inputString[lookheadIndex] == ')')
        {
            lookheadIndex++;
            flag = true;
            return;
        }
        else
        {
            flag = false;
            return;
        }
    }
}

void term()
{
    factor();
    if( flag)
    {
        if(inputString[lookheadIndex] == '*' ||inputString[lookheadIndex] == '/')
        {
            lookheadIndex++;
            flag = true;
            if(lookheadIndex == lenOfSt) flag = false;
            if(flag) factor();
        }
    }
}

void smple_expn()
{
    term();
    if(flag)
    {
        if(inputString[lookheadIndex] == '+' ||inputString[lookheadIndex] == '-')
        {
            lookheadIndex++;
            flag = true;

            if(lookheadIndex == lenOfSt) flag = false;
            if(flag) term();
        }
    }
}

void relop()
{
    // ==
    if(inputString[lookheadIndex] == '=')
    {
        lookheadIndex++;
        flag = true;
        if(inputString[lookheadIndex] == '=')
        {
            lookheadIndex++;
            flag = true;
            return;
        }
        else
        {
            flag = false;
        }
    }
    else if(inputString[lookheadIndex] == '!')
    {
        lookheadIndex++;
        flag = true;
        if(inputString[lookheadIndex] == '=')
        {
            lookheadIndex++;
            flag = true;
            return;
        }
        else
        {
            flag = false;
        }
    }
    else if(inputString[lookheadIndex] == '>')
    {
        lookheadIndex++;
        flag = true;
        if(inputString[lookheadIndex] == '=')
        {
            lookheadIndex++;
            flag = true;
            return;
        }
    }
    else if(inputString[lookheadIndex] == '<')
    {
        lookheadIndex++;
        flag = true;
        if(inputString[lookheadIndex] == '=')
        {
            lookheadIndex++;
            flag = true;
        }
        return;
    }
    if(!isValidID(inputString[lookheadIndex]) && !isValidNum(inputString[lookheadIndex])) flag = false;
    if(lookheadIndex == lenOfSt) flag = false;
}
void extn1()
{
    if(inputString[lookheadIndex] == 'e'
            && inputString[lookheadIndex+1] == 'l'
            && inputString[lookheadIndex+2] == 's'
            && inputString[lookheadIndex+3] == 'e' )
    {
        lookheadIndex += 4;
        flag = true;
        stat();
    }
}

void extn()
{
    if(lookheadIndex == lenOfSt) return; // epsilon
    relop();
    if(flag) smple_expn();
}


void expn()
{
    smple_expn();
    if(flag)
    {
        extn();
    }
}


void asgn_stat()
{
    expn();
}

void dscn_stat()
{
    if(inputString[lookheadIndex] == '(')
    {
        lookheadIndex++;
        flag = true;
        expn();
        if(inputString[lookheadIndex] == ')')
        {
            lookheadIndex++;
            flag = true;
            stat();
            if(flag)
            {
                extn1();
            }
        }
        else
        {
            flag = false;
        }
    }
    else
    {
        flag = false;
    }
}
// ############ END DSCN ############

// ############ LOOP STAT ############
void loop_stat()
{
    // for(a = 5; a != 9; a = a+1) b = a
    if(inputString[lookheadIndex] == 'f'
            && inputString[lookheadIndex+1] == 'o'
            && inputString[lookheadIndex+2] == 'r')
    {
        lookheadIndex += 3;
        flag = true;
        if(inputString[lookheadIndex] == '(')
        {
            lookheadIndex++;
            flag = true;
            if(isID())
            {
                lookheadIndex += 2;
                flag = true;
                asgn_stat();
                if(inputString[lookheadIndex] == ';')
                {
                    lookheadIndex++;
                    flag = true;
                    expn();
                    if(inputString[lookheadIndex] == ';')
                    {
                        lookheadIndex++;
                        flag = true;
                    }
                    else
                    {
                        flag = false;
                    }
                    if(flag)
                    {
                        if(isID())
                        {
                            lookheadIndex += 2;
                            flag = true;
                            asgn_stat();
                            if(inputString[lookheadIndex] == ')')
                            {
                                lookheadIndex++;
                                flag = true;
                                if(lookheadIndex < lenOfSt - 1)
                                    stat();
                                else
                                {
                                    flag = false;
                                    return;
                                }
                            }
                            else
                            {
                                flag = false;
                            }
                        }
                    }
                }
                else
                {
                    flag = false;
                }
            }
        }
        else
        {
            flag = false;
        }
    }

    //while(a>5) a = a + 5
    if((inputString[lookheadIndex] == 'w'
            && inputString[lookheadIndex+1] == 'h'
            && inputString[lookheadIndex+2] == 'i'
            && inputString[lookheadIndex+3] == 'l'
            && inputString[lookheadIndex+4] == 'e'))
    {

        lookheadIndex += 5;
        flag = true;
        if(inputString[lookheadIndex] == '(')
        {
            lookheadIndex++;
            flag = true;
            expn();

            if(flag)
            {
                if(inputString[lookheadIndex] == ')')
                {
                    lookheadIndex++;
                    flag = true;
                    if(lookheadIndex == lenOfSt) flag = false;
                    stat();
                }else{
                    flag = false;
                }
            }
        }
    }
}
// ############ END LOOP ############

void stat()
{
    // asgn_stat
    if(isID())
    {
        lookheadIndex += 2;
        flag = true;
        asgn_stat();
    }

    // dscn_stat
    if(inputString[lookheadIndex] == 'i' && inputString[lookheadIndex+1] == 'f')
    {
        lookheadIndex += 2;
        flag = true;
        if(flag) dscn_stat();

    }
    //loop_stat
    if((inputString[lookheadIndex] == 'f' && inputString[lookheadIndex+1] == 'o' && inputString[lookheadIndex+2] == 'r')
            || (inputString[lookheadIndex] == 'w' && inputString[lookheadIndex+1] == 'h' && inputString[lookheadIndex+2] == 'i'&& inputString[lookheadIndex+3] == 'l'&& inputString[lookheadIndex+4] == 'e'))
    {
        loop_stat();
    }
}

#endif
