#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"lib/keywords.h"
#include"lib/LEXEME.h"
#include"lib/Symbol_Table_Generate.h"
#include"lib/parsing.h"

ErrorInfo allErrors[1000];
char input[1000000] ;
char all_code_in_line[1000][1000] ;
char allVal_token[1000][1000] ;
char allVal_token_type[1000][1000] ;

int token_map_line[1000];
int allVal_token_size = 0;
int lineCount = 0;
int numberOfError = 0;

char stackParen[10000][5] ;
int stackParenHead = 0;

int ifCount= 0;

int errorCheck = 0;

void fileRead(char *fileName){
    FILE *fptr;
    char c;
    int i = 0;
    if ((fptr = fopen(fileName, "r")) == NULL) {
        printf("Error! opening file");
    }else{
        while((c = fgetc(fptr)) != EOF){
            input[i] = c;
            i++;
        }
    }
	fclose(fptr);
}

void fileWrite(char *fileName , char sentence[]){
    FILE *fptr;
    fptr = fopen(fileName, "w");

    if (fptr == NULL) {
        printf("Error! opening file");
        exit(1);
    }
    fprintf(fptr, "%s", sentence);
    fclose(fptr);
}

void fileAppend(char *fileName , char sentence[]){
    FILE *fptr;
    fptr = fopen(fileName, "a");

    if (fptr == NULL) {
        printf("Error! opening file");
        exit(1);
    }
    fprintf(fptr, "%s", sentence);
    fclose(fptr);
}

void printOutput(char str[]){
    printf("%s\n",str);
}


void commentRemove(){
    char output[1000000];
    int j = 0;
    for(int i = 0; i < strlen(input) ; i++ ){
        if(input[i] == '/' && input[i+1] == '*' ){
            int multiStart = 1;
            while(multiStart){
                i++;
                if(input[i] == '*' &&  input[i+1] == '/'){
                    multiStart = 0;
                    i++;
                }

            }
        }
        else if(input[i] == '/' && input[i+1] == '/'){
            int singleStart = 1;
            while(singleStart){
                i++;
                if(input[i] == '\n'){
                    singleStart = 0;
                }
            }
        }
        else{
            output[j] = input[i];
            j++;
        }

    }
    output[j] = '\0';
    strcpy(input,output);
}


void trim(char str[]){
    char output[10000];
    int length = strlen(str);
    if(length <= 0)
        return ;

    int j = 0;
    int start  = 0;
    for(int i = 0; str[i] == ' ' ;i++)
        start++;

    for(int i = start ; i < strlen(str) ; i++ ){

        if(str[i] == ' '  && str[i+1] == ' '){
            int space = 1;
            while(space){
                i++;
                if(str[i+1] != ' '){
                    space = 0;
                    output[j] = ' ';
                    j++;
                }
            }
        }else{
            output[j] = str[i];
            j++;
        }
    }
    output[j] = '\0';
    if(output[strlen(output)-1] == ' ')
        output[strlen(output)-1] = '\0';


    int i = 0;
    for(; i<strlen(output) ;i++){
        str[i] = output[i];
    }

    str[i] = '\0';
}


void convertInLine(){
    char buffer[500];
    for(int i = 0,j=0 ; i < strlen(input) ;i++){
        buffer[j++] = input[i];
        if(input[i] == '\n'){
            buffer[j] = '\0';
            trim(buffer);
            strcpy(all_code_in_line[lineCount] , buffer );
            lineCount++;
            j = 0;
        }
    }
}


void convertString(){
    char output[1000000];
    int j = 0;
     for(int i = 0; i < strlen(input) ;i++){
        if(isSeperator(input[i]) || isOperator(input[i]) || isParen(input[i])){
            output[j++] = ' ';
            output[j++] = input[i];
            output[j++] = ' ';
        }
        else{
            output[j++] = input[i];
        }
     }
     output[j] = '\0';
     strcpy(input , output);
}


char* check_type(char str[]){
    if(isSeperator(str[0])){
        return "sp";
     }
     else if(isParen(str[0])){
            return "par";
     }
     else if(isOperator(str[0])){
          return "op";
     }
     else{
        trim(str);
        if(isKeyword(str)){
            return "kw";
        }
        else if(isNumber(str)){
            return "nm";
        }
        else if(isValidIdentifier(str)){
            return "id";
        }
     }
     return "unknown";

}

void parse_token()
{
    for(int i = 0 ; i <= lineCount; i++){
        char buffer[500];
        for(int j = 0,k = 0; j < strlen(all_code_in_line[i]) ;j++){
            buffer[k++] = all_code_in_line[i][j];
            if(all_code_in_line[i][j] == ' '){
                buffer[k] = '\0';

                char *ty = check_type(buffer);
                trim(buffer);
                strcpy(allVal_token[allVal_token_size],buffer);
                strcpy(allVal_token_type[allVal_token_size],ty);
                token_map_line[allVal_token_size] = i;
                allVal_token_size++;
                k = 0;
            }
        }

    }
}

void Error_calculation(int currentLine , char type[],  char errorVal[],  char messages[]) {

    if(numberOfError == 0){
        allErrors[numberOfError].linenumber = currentLine;
        strcpy(allErrors[numberOfError].errortype,type);
        strcpy(allErrors[numberOfError].errorvalue,errorVal);
        strcpy(allErrors[numberOfError].errorMess,messages);
        numberOfError++;
    }
    else{
        if(allErrors[numberOfError-1].linenumber != currentLine|| strcmp(allErrors[numberOfError-1].errortype, type) != 0 || strcmp(allErrors[numberOfError-1].errorvalue, errorVal) != 0 ){
            allErrors[numberOfError].linenumber = currentLine;
            strcpy(allErrors[numberOfError].errortype,type);
            strcpy(allErrors[numberOfError].errorvalue,errorVal);
            strcpy(allErrors[numberOfError].errorMess,messages);
            numberOfError++;
        }
    }
}

void srcCode()
{
    printf("------------------------------------------------------------------------------\n");
    printf("Please Enter The Input. If you want to stop input please EOF or CTRL+Z: \n");
    printf("------------------------------------------------------------------------------\n");
    char str[1000000];
    int i = 0;

    for( i = 0 ; scanf("%c",&str[i]) != EOF ;i++);

    fileWrite("input.c",str);
}


int main()
{

    srcCode();

    fileRead("input.c");
    printf("\n");
    printOutput(input);

    commentRemove(input);
    convertString();
    convertInLine();

    parse_token();

    printf("\n\n");
    int crntVal = 0;

    printf("\n\n\n\n-------------Single Line Code-------------\n\n");
    for(int i = 0 ; i <= allVal_token_size; i++){
         printf("%s ",allVal_token[i]);
    }
    printf("\n\n");

    printf("\n\n\n\n-------------With Token Value-------------\n\n");
    int currntLine = 0;
    for(int i = 0 ; i <= allVal_token_size; i++){
         if(currntLine != token_map_line[i]){
            printf("\n\n");
            printf("%d    ",token_map_line[i]);
         }
         currntLine = token_map_line[i];
         printf("%s %s ",allVal_token_type[i], allVal_token[i]);

    }
    printf("\n\n");
    for(int i = 0 ; i <= allVal_token_size; i++){
        if( strcmp(allVal_token[i], "for") == 0 && strcmp(allVal_token[i+1], "(") == 0){
            i+=2;
            int numOfSemi = 0;
            while(strcmp(allVal_token[i], ")") != 0 ){

                if(strcmp(allVal_token[i], ";") == 0){
                    numOfSemi++;
                }
                i++;
            }
            if(numOfSemi != 2){
                Error_calculation(token_map_line[i] , "loop" ,"for", "check the loop syntax");
            }
            i+=2;
            continue;
        }

        if(allVal_token[i][0]=='+' && allVal_token[i+1][0]=='+'){
               i+=2;
               continue;

            }
        if(allVal_token[i][0]=='=' && allVal_token[i+1][0]=='='){
               i+=2;
               continue;

            }
        if(allVal_token[i][0]=='-' && allVal_token[i+1][0]=='-'){
               i+=2;
               continue;

            }
        if(allVal_token[i][0]=='<' && allVal_token[i+1][0]=='='){
               i+=2;
               continue;

            }
        if(allVal_token[i][0]=='>' && allVal_token[i+1][0]=='='){
               i+=2;
               continue;

            }
        if( strcmp(allVal_token[i], "if") == 0){
            ifCount++;
        }

        if(strcmp(allVal_token[i], "else") == 0 ){
            if(ifCount<=0){
                Error_calculation(token_map_line[i] , allVal_token_type[i] ,allVal_token[i], "Unmatched else");
            }else{
                ifCount--;
            }
        }


        if(isParen(allVal_token[i][0])){

            if(allVal_token[i][0] == '(' || allVal_token[i][0] == '{' || allVal_token[i][0] == '['){
                 strcpy(stackParen[stackParenHead], allVal_token[i]);
                 stackParenHead +=1;
            }
            else{
                int crna_stacl = stackParenHead-1;
                if(allVal_token[i][0] == '}' && stackParen[crna_stacl][0] != '{'){
                    Error_calculation(token_map_line[i] , allVal_token_type[i] ,allVal_token[i], "Unbalanced Bracket");
                }
                else if(allVal_token[i][0] == ')' && stackParen[crna_stacl][0] != '('){
                    Error_calculation(token_map_line[i] , allVal_token_type[i] ,allVal_token[i], "Unbalanced Bracket");
                }
                else if(allVal_token[i][0] == ']' && stackParen[crna_stacl][0] != '['){
                    Error_calculation(token_map_line[i] , allVal_token_type[i] ,allVal_token[i], "Unbalanced Bracket");
                }
                else{
                    stackParenHead -= 1;
                }
            }
        }
         else if(strcmp(allVal_token_type[i], allVal_token_type[i-1] ) == 0){
                Error_calculation(token_map_line[i] , allVal_token_type[i] ,allVal_token[i], "Duplicate token");
        }
        else if(strcmp(allVal_token[i], allVal_token[i-1] ) == 0){
                Error_calculation(token_map_line[i] , allVal_token_type[i] ,allVal_token[i], "Duplicate token");
        }

    }

    while(stackParenHead >= 1){
        Error_calculation(lineCount-1 ,"par" ,stackParen[stackParenHead--], "Unbalanced Bracket");
    }

    char reportFileName[50] = "Report_170204030.txt";

    printf("\n\n\n\n-------------The source code with line number-------------\n\n");
    fileWrite(reportFileName,"\n-------------The source code with line number-------------\n\n");
    for(int i = 0 ; i <= lineCount; i++){
        char codeReport[1000];
        sprintf(codeReport, "%d ", i+1);

        strcat(codeReport , all_code_in_line[i]);
        codeReport[strlen(codeReport)] = '\0';
        printf("%d    %s\n",i+1,all_code_in_line[i]);
        fileAppend(reportFileName,codeReport);
    }


    printf("\n\n\n\n-------------Headers-------------\n\n");
    fileAppend(reportFileName,"\n\n\n\n-------------Header-------------\n\n");
    char header[10000] ;
    for(int i = 0,numOfHeader = 0; i <= allVal_token_size; i++){
        if(strcmp(allVal_token[i], "#include") == 0 ){
            if(strstr(header, allVal_token[i+2]) == 0 ){
                strcat(header, allVal_token[i+2]);
                strcat(header, ", ");
            }
        }
    }
    header[strlen(header)-2] = '\0';
    fileAppend(reportFileName,header);
    printf("%s\n",header);


    printf("\n\n\n\n-------------Identifiers-------------\n\n");
    fileAppend(reportFileName,"\n\n\n\n-------------Identifiers-------------\n\n");
    char Identifiers[10000] ;
    for(int i = 0 ; i <= allVal_token_size; i++){
        if( strcmp(allVal_token_type[i], "id") == 0  ){
            if(strstr(Identifiers, allVal_token[i]) == 0 ){
                strcat(Identifiers, allVal_token[i]);
                strcat(Identifiers, ", ");
            }
        }
    }
    Identifiers[strlen(Identifiers)-2] = '\0';
    fileAppend(reportFileName,Identifiers);
    printf("%s\n",Identifiers);

    printf("\n\n\n\n-------------Keywords-------------\n\n");
    fileAppend(reportFileName,"\n\n\n\n-------------Keywords-------------\n\n");
    char Keywords[10000] ;
    for(int i = 0 ; i <= allVal_token_size; i++){
        if( strcmp(allVal_token_type[i], "kw") == 0  ){
            if(strstr(Keywords, allVal_token[i]) == 0 ){
                strcat(Keywords, allVal_token[i]);
                strcat(Keywords, ", ");
            }
        }
    }
    Keywords[strlen(Keywords)-2] = '\0';
    fileAppend(reportFileName,Keywords);
    printf("%s\n",Keywords);

    printf("\n\n\n\n-------------Functions-------------\n\n");
    fileAppend(reportFileName,"\n\n\n\n-------------Functions-------------\n\n");
    char Functions[10000] ;
    for(int i = 0 ; i <= allVal_token_size; i++){
        if( strcmp(allVal_token_type[i], "id") == 0  && allVal_token[i+1][0] == '('){
            if(strstr(Functions, allVal_token[i]) == 0 ){
                strcat(Functions, allVal_token[i]);
                strcat(Functions, ", ");
            }
        }
    }
    Functions[strlen(Functions)-2] = '\0';
    fileAppend(reportFileName,Functions);
    printf("%s\n",Functions);

        printf("\n\n\n\n-------------Errors-------------\n\n");
    fileAppend(reportFileName,"\n\n\n\n-------------Error-------------\n\n");
    for(int i = 0 ; i < numberOfError; i++){
        char errorReport[1000];
        sprintf(errorReport, "Line No. %d: | Error type %s & error: %s -> %s \n", allErrors[i].linenumber, allErrors[i].errortype , allErrors[i].errorvalue , allErrors[i].errorMess );
        errorReport[strlen(errorReport)] = '\0';
        fileAppend(reportFileName,errorReport);
        printf("%s\n", errorReport);
    }

    if(numberOfError==0){
        char ch;
        char sourceFileName[30] = "input.c";
        char tempFileName[30] = "LEX_01.txt";
        char targetFileName[30] = "LEX_O2.txt";
        FILE *source, *target, *temp;

        // Lexeme separator
        source = fopen(sourceFileName, "r");
        temp = fopen(tempFileName, "w");
        lexemeSeparator(source, temp, sourceFileName, tempFileName);

        //  Lexeme Categorizer
        temp = fopen(tempFileName, "r");
        target = fopen(targetFileName, "w");
        lexemeTokenization(temp, target, tempFileName, targetFileName);

        //  printing in console
        printf("\n\n");
        printf("\n\n\n\n-------------LEXICAL ANALYSIS-------------\n\n");
        target = fopen (targetFileName,"r");
        while( (ch = fgetc(target)) != EOF){
            printf("%c",ch);
        }
        fclose(target);

        printf("\n\n");
        printf("\n\n\n\n-------------SYMBOL TABLE-------------\n\n");
        strcpy(scope1, "global");
        identifierSepLex();
        storeAll();
        scopeDetect();
        symbolTableGenerate();
        display();
    }
    printf("\n-------------------END OF COMPILATION----------------");
    printf("\n\n");

    printf("\n-------------------CHECK CFG PARSING----------------");

    char option;

    printf("\n\n");

    printf("\nDo you want to continue CFG? {Y|N) ");
    scanf("%c", &option);
    printf("\n\n");

    if (option ==  'y' || option == 'Y')
    {
        char str[10000];
        printf("Enter a string: ");
        scanf("%s",str);

        int j = 0;
        for(int i = 0; i < strlen(str); i++)
            if(str[i] != ' ') inputString[j++] = str[i];
        inputString[j] = '\0';
        lenOfSt = strlen(inputString);
        stat();
        if(!flag) printf("Invalid syntex\n");
        else printf("All okay!!\n");
    }
    else if (option == 'n' || option == 'N')
    {
        return 0;
    }
    else
    {
        printf("Invlid selection.");
    }

}
