#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(void)
{
    FILE *p1, *p2;
    char c, lna[10];
    int ln = 1, x;

    p1 = fopen("datafile1.c", "r");
    p2 = fopen("dfwithLine.txt", "w");

    if(!p1)
        printf("\nFile can't be opened!");
    else
    {
      	while((c = fgetc(p1)) != EOF)
      	{
            itoa(ln, lna, 10);
            for(x=0; x < strlen(lna); x++)
                fputc(lna[x], p2);
            fputc(':', p2);
            while(c!='\n')
            {
                fputc(c, p2);
                c = fgetc(p1);
            }
            fputc('\n', p2);
            ln++;
        }
    }
    fclose(p1);
    fclose(p2);

    printf("Code With Line Number:\n\n");
    p2 = fopen("dfwithLine.txt", "r");
    while((c = fgetc(p2))!= EOF)
        printf("%c", c);
    fclose(p2);

    return 0;
}

