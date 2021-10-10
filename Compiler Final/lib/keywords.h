#ifndef KEYWORDS_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NUMBER_OF_KEYWORDS 32

typedef struct ErrorInfo ErrorInfo;

struct ErrorInfo{
    int linenumber;
    char errortype[5];
    char errorvalue[100];
    char errorMess[100];
};

char keywords[NUMBER_OF_KEYWORDS][100] =
{
    "auto","break","case","char","const","continue","default",
    "do","double","else","enum","extern","float","for","goto",
    "if","int","long","register","return","short","signed",
    "sizeof","static","struct","switch","typedef","union",
    "unsigned","void","volatile","while"
};

int isKeyword(char str[]){
    int flag = 0;

    for(int i = 0; i < NUMBER_OF_KEYWORDS; i++){
		if(strcmp(keywords[i], str) == 0){
			flag = 1;
			break;
		}
	}

    return flag;
}

int isOperator(char ch)
{
    int flag = 0;
    if (ch == '+' || ch == '-' || ch == '*' ||ch == '/' || ch == '>' || ch == '<' ||ch == '=' ||ch == '!' || ch == '&' || ch == '|'){
        flag = 1;
    }
    return flag;
}

int isSeperator(char ch)
{
    int flag = 0;
    if (ch == ';' || ch == ',' || ch == '\'' ){
        flag = 1;
    }
    return flag;
}

int isAvoid(char ch)
{
    int flag = 1;
    if (ch >= 0 && ch <= 32 ){
        flag = 0;
    }
    return flag;
}

int isNumber(char str[])
{
    int flag = 1;
    int numberOfpoint = 0;
    char str1[strlen(str) + 1];
    strcpy(str1,str);

    for (int i = 0; i < strlen(str1); i++) {
        if(str1[i] >= 'A' && str1[i] <= 'Z') {
            str1[i] = str1[i] + 32;
        }
    }

    if(str1[0] == '0' && str1[1] == 'x'){
        for(int i = 2; i < strlen(str1); i++){
            if((str1[i] >= '0' && str1[i] <= '9') || (str1[i] >= 'a' && str1[i] <= 'f') ) {
                continue;
            }else{
                return 0;
            }
        }
        return 1;
    }
    for(int i = 0; i < strlen(str1); i++){
		if(isdigit(str1[i])){
            flag = 1;
            continue;
		}
		else if(str1[i] == '.'){
            numberOfpoint++;
		}else{
            flag = 0;
            break;

		}
	}
	if(numberOfpoint <=1 && flag == 1)
        return 1;
    else
        return 0;
}

int isValidIdentifier(char str[])
{
    if (!((str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[1] <= 'Z')|| str[0] == '_'))
        return 0;

    for (int i = 1; i < strlen(str); i++) {
        if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9') || str[i] == '_'))
            return 0;
    }

    return 1;
}


int isParen(char ch)
{
    int flag = 0;
    if (ch == '(' || ch == ')' || ch == '{'  || ch == '}' || ch == '[' || ch == ']' ){
        flag = 1;
    }
    return flag;
}

void removeSpace(char str[]){
    char output[strlen(str)+1];
    int j = 0;
    for(int i = 0; i < strlen(str) ;i++){
        if(str[i] != ' '){
            output[j] = str[i];
            j++;
        }
    }
    output[j] = '\0';
    strcpy(str , output);
}

#endif
