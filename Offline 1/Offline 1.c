#include <stdio.h>
FILE *fp, *fp2;

int main(void)
{
    char c;

    int count = 0;
    fp = fopen ("input.c","r") ;
    fp2 = fopen ("output.txt","w");
    char retType[4][6] ={"int", "Char", "float" , "long"};



    while((c = fgetc(fp)) !=EOF)
    {

        char d;
        if((c=="("))
        {
            while((d=fgetc(fp))!=EOF)
            {
                if(d != ")"){
                fputc(d,fp2);
                }

                if(d == ")"){
                    count++;
                    return;}
            }
        }

    }

    fp2 = fopen("fp2","r");
    while((c=fgetc(fp2))!=EOF)
        printf("%c",c);

    printf("%d",count);

    fclose(fp2);

    return 0;
}
