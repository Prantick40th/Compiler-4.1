#ifndef LEXEME_H

#include <stdio.h>
#include <string.h>


void lexemeSeparator(FILE *source, FILE *target, char sourceFileName[30], char targetFileName[30]){
    char c;
    int lexemeStart = 0;
    char tempFileName[30] = "temp1.txt";
    FILE *temp;
    temp = fopen(tempFileName, "w");
    if(source == NULL || target == NULL || temp == NULL){
        printf("\nFile cannot be opened");
    }
    else{
        while( (c = fgetc(source)) != EOF ){
            if(c == ' '){
                fputc(c,temp);
                lexemeStart = 0;
                continue;
            }
            if(c != ' ' && lexemeStart == 0){
                lexemeStart = 1;
            }
            if(lexemeStart == 1){
                if(c == ';' || c == ',' || c == 39 || c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '(' || c == ')'){    //apostrophe ascii = 39
                    fputc(' ',temp);      //leading space of the lexeme
                    fputc(c,temp);
                    lexemeStart = 0;

                    char cc = fgetc(source);    //checking next character
                    if (cc == '+' || cc == '-' ){ //for  ++  --
                        fputc(cc,temp);
                        fputc(' ',temp);  //trailing space of the lexeme
                    }
                    else if (cc == ';'){  //semicolon after apostrophe
                        fputc(' ',temp);  //leading space of the lexeme
                        fputc(cc,temp);
                        fputc(' ',temp);  //trailing space of the lexeme
                    }
                    else{
                        fputc(' ',temp);  //leading space of the lexeme
                        fputc(cc,temp);
                    }
                    continue;
                }
                else if(c == '>' || c == '<' || c == '!' || c == '|' || c == '&' || c == '='){
                    fputc(' ',temp);      //leading space of the lexeme
                    fputc(c,temp);
                    lexemeStart = 0;

                    char cc = fgetc(source);    //checking next character
                    if (cc == '=' || cc == '|' || cc == '&'){   //for checking  >=  <=  !=  &&  ||
                        fputc(cc,temp);
                        fputc(' ',temp);  //trailing space of the lexeme
                    }
                    else if (cc == 39){     //apostrophe ASCII = 39
                        fputc(' ',temp);  //leading space of the lexeme
                        fputc(cc,temp);
                        fputc(' ',temp);  //trailing space of the lexeme
                    }
                    else{
                        fputc(' ',temp);  //leading space of the lexeme
                        fputc(cc,temp);
                    }
                    continue;
                }
                fputc(c,temp);
            }
        }
    }
    fputc(' ',temp);
    fclose(source);
    fclose(temp);

    //Removing more than one spaces
    int syntaxSpace = 0;
    int codeStart = 0;
    source = fopen(tempFileName, "r");

    while( (c = fgetc(source)) != EOF ){
        if(c == 32 && syntaxSpace == 0 && codeStart == 1){          //one space between syntaxes
            fputc(32,target);
            syntaxSpace = 1;
            continue;
        }
        else if(c == 32 && syntaxSpace == 1 && codeStart == 1){     //more than one space between syntaxes
            continue;
        }
        else if(c != 32){                                           //any character other than space
            fputc(c,target);
            codeStart = 1;
            syntaxSpace = 0;
        }
    }
    fclose(source);
    fclose(target);
    remove(tempFileName);
}


void lexemeTokenization(FILE *source, FILE *target, char sourceFileName[30], char targetFileName[30]){
    char c, lex[50];
    if(source == NULL || target == NULL){
        printf("\nFile cannot be opened");
    }
    else{
        int i = 0;
        while( (c = fgetc(source)) != EOF ){
            if(c == ' '){       //checking each lexeme separately and putting them in target file
                lex[i] = '\0';
                i=0;
                if(isSeparator1(lex)){
                    fputs("[sep ", target);
                    fputs(lex, target);
                    fputs("] ", target);
                }
                else if(isParenthesis1(lex)){
                    fputs("[par ", target);
                    fputs(lex, target);
                    fputs("] ", target);
                }
                else if(isOperator1(lex)){
                    fputs("[op ", target);
                    fputs(lex, target);
                    fputs("] ", target);
                }
                else if(isNumerical(lex)){
                    fputs("[num ", target);
                    fputs(lex, target);
                    fputs("] ", target);
                }
                else if(isKeyword1(lex)){
                    fputs("[kw ", target);
                    fputs(lex, target);
                    fputs("] ", target);
                }
                else if(isIdentifier(lex)){
                    fputs("[id ", target);
                    fputs(lex, target);
                    fputs("] ", target);
                }
                else {
                    fputs("[unkn ", target);
                    fputs(lex, target);
                    fputs("] ", target);
                }

                continue;
            }
            else{
                lex[i] = c;
                i++;
            }
        }
    }
    fclose(source);
    fclose(target);
}

int isNumerical(char *lex){
    int i, l, s;
    i=0;
    if(isdigit(lex[i])) {
        s=1; i++;
    }
    else if(lex[i]=='.') {
        s=2;
        i++;
    }
    else
        s=0;
    l=strlen(lex);
    if(s==1){
        for(;i<l;i++){
            if(isdigit(lex[i]))
                s=1;
            else if(lex[i]=='.') {
                s=2;
                i++;
                break;
            }
            else {
                s=0;
                break;
            }
        }
    }
    if(s==2){
        if(isdigit(lex[i])) {
            s=3; i++;
        }
        else
            s=0;
    }
    if(s==3){
        for(;i<l;i++) {
            if(isdigit(lex[i]))
                s=3;
            else {
                s=0;
                break;
            }
        }
    }
    if(s==3){
        s=1;
    }
    return s;
}


int isSeparator1(char *lex){
    int s = 0 ;

    if( strlen(lex) == 1 && (lex[0] == ';' || lex[0] == ',' || lex[0] == 39) ){
        s = 1;
    }
   return s;
}

int isParenthesis1(char *lex){
    int s = 0 ;

    if( strlen(lex) == 1 && (lex[0] == '(' || lex[0] == ')' || lex[0] == '{' || lex[0] == '}') ){
        s = 1;
    }
   return s;
}

int isOperator1(char *lex){
    int s = 0 ;

    if(strlen(lex) == 1){       //single operator
        if( lex[0] == '+' || lex[0] == '-' || lex[0] == '*' || lex[0] == '/' || lex[0] == '%' || lex[0] == '=' ||
           lex[0] == '<' || lex[0] == '>' || lex[0] == '!' || lex[0] == '&' || lex[0] == '|' || lex[0] == '~' || lex[0] == '^' ){
            s = 1;
        }
    }
    if(strlen(lex) == 2){       //double operator
        if( (lex[0] == '+' && lex[1] == '+') || (lex[0] == '-' && lex[1] == '-') || (lex[0] == '=' && lex[1] == '=') || (lex[0] == '!' && lex[1] == '=') ||
           (lex[0] == '|' && lex[1] == '|') || (lex[0] == '&' && lex[1] == '&') || (lex[0] == '>' && lex[1] == '=') || (lex[0] == '<' && lex[1] == '=') ){
            s = 1;
        }
    }
    return s;
}

int isIdentifier(char *lex){
    int s = 0 ;

    if((lex[0] >= 'a' && lex[0] <= 'z') || (lex[0] >= 'A' && lex[0] <= 'Z') || lex[0] == '_' ){
        s = 1;      //checking first character if it's valid or not (a-z) || (A-Z) || _
    }

    if (s==0)
        return s ;
    else{
        int i;
        for ( i = 1 ; i<strlen(lex); i++){
            if((lex[i] >= 'a' && lex[i] <= 'z') || (lex[i] >= 'A' && lex[i] <= 'Z') || lex[i] == '_' || (lex[i] >= '0' && lex[i] <= '9') ){
                continue;
            }
            else{
                s = 0;
            }
        }
    }
    return s ;
}


int isKeyword1(char *lex){
    int s = 0 ;

    if(lex[0] == 'i' && lex[1] == 'n' && lex[2] == 't' && lex[3] == '\0' ){
        s = 1;      //int
    }
    else if(lex[0] == 'c' && lex[1] == 'h' && lex[2] >= 'a' && lex[3] == 'r' && lex[4] == '\0' ){
        s = 1;      //char
    }
    else if(lex[0] == 'f' && lex[1] == 'l' && lex[2] >= 'o' && lex[3] == 'a' && lex[4] == 't' && lex[5] == '\0' ){
        s = 1;      //float
    }
    else if(lex[0] == 'd' && lex[1] == 'o' && lex[2] >= 'u' && lex[3] == 'b' && lex[4] == 'l' && lex[5] == 'e' && lex[6] == '\0' ){
        s = 1;      //double
    }
    else if(lex[0] == 'l' && lex[1] == 'o' && lex[2] >= 'n' && lex[3] == 'g' && lex[4] == '\0' ){
        s = 1;      //long
    }
    else if(lex[0] == 's' && lex[1] == 'h' && lex[2] >= 'o' && lex[3] == 'r' && lex[4] == 't' && lex[5] == '\0' ){
        s = 1;      //short
    }
    else if(lex[0] == 'w' && lex[1] == 'h' && lex[2] >= 'i' && lex[3] == 'l' && lex[4] == 'e' && lex[5] == '\0' ){
        s = 1;      //while
    }
    else if(lex[0] == 'f' && lex[1] == 'o' && lex[2] >= 'r' && lex[3] == '\0' ){
        s = 1;      //for
    }
    else if(lex[0] == 'd' && lex[1] == 'o' && lex[2] == '\0' ){
        s = 1;      //do
    }
    else if(lex[0] == 'i' && lex[1] == 'f' && lex[2] == '\0' ){
        s = 1;      //if
    }
    else if(lex[0] == 'e' && lex[1] == 'l' && lex[2] >= 's' && lex[3] == 'e' && lex[4] == '\0' ){
        s = 1;      //else
    }
    else if(lex[0] == 'v' && lex[1] == 'o' && lex[2] >= 'i' && lex[3] == 'd' && lex[4] == '\0' ){
        s = 1;      //void
    }
    else if(lex[0] == 's' && lex[1] == 't' && lex[2] >= 'a' && lex[3] == 't' && lex[4] == 'i' && lex[5] == 'c' && lex[6] == '\0' ){
        s = 1;      //static
    }
    else if(lex[0] == 'b' && lex[1] == 'r' && lex[2] >= 'e' && lex[3] == 'a' && lex[4] == 'k' && lex[5] == '\0' ){
        s = 1;      //break
    }
    else if(lex[0] == 'c' && lex[1] == 'o' && lex[2] >= 'n' && lex[3] == 't' && lex[4] == 'i' && lex[5] == 'n' && lex[6] == 'u' && lex[7] == 'e' && lex[8] == '\0' ){
        s = 1;      //continue
    }
    else if(lex[0] == 'd' && lex[1] == 'e' && lex[2] >= 'f' && lex[3] == 'a' && lex[4] == 'u' && lex[5] == 'l' && lex[6] == 't' && lex[7] == '\0' ){
        s = 1;      //default
    }
    else if(lex[0] == 'c' && lex[1] == 'o' && lex[2] >= 'n' && lex[3] == 's' && lex[4] == 't' && lex[5] == '\0' ){
        s = 1;      //const
    }
    else if(lex[0] == 's' && lex[1] == 'w' && lex[2] >= 'i' && lex[3] == 't' && lex[4] == 'c' && lex[5] == 'h' && lex[6] == '\0' ){
        s = 1;      //switch
    }
    else if(lex[0] == 'c' && lex[1] == 'a' && lex[2] >= 's' && lex[3] == 'e' && lex[4] == '\0' ){
        s = 1;      //case
    }
    else if(lex[0] == 's' && lex[1] == 'i' && lex[2] >= 'z' && lex[3] == 'e' && lex[4] == 'o' && lex[5] == 'f' && lex[6] == '\0' ){
        s = 1;      //sizeof
    }
    else if(lex[0] == 's' && lex[1] == 't' && lex[2] >= 'r' && lex[3] == 'u' && lex[4] == 'c' && lex[5] == 't' && lex[6] == '\0' ){
        s = 1;      //struct
    }
    else if(lex[0] == 's' && lex[1] == 'i' && lex[2] >= 'g' && lex[3] == 'n' && lex[4] == 'e' && lex[5] == 'd' && lex[6] == '\0' ){
        s = 1;      //signed
    }
    else if(lex[0] == 'u' && lex[1] == 'n' && lex[2] >= 's' && lex[3] == 'i' && lex[4] == 'g' && lex[5] == 'n' && lex[6] == 'e' && lex[7] == 'd' && lex[8] == '\0' ){
        s = 1;      //unsigned
    }
    else if(lex[0] == 'r' && lex[1] == 'e' && lex[2] >= 't' && lex[3] == 'u' && lex[4] == 'r' && lex[5] == 'n' && lex[6] == '\0' ){
        s = 1;      //return
    }

    return s ;
}

#endif
