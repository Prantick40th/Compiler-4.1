#include<stdio.h>
int main(void)
{
    FILE *p1;
    char c;

    p1 = fopen("datafile1.c", "r");

    if(!p1)
        printf("\nFile can't be opened!");
    else
    {
      	while((c = fgetc(p1)) != EOF)
      	{
        	if(c=='#')
            {
                while((c = fgetc(p1))!= '<');
                while((c = fgetc(p1))!= '>')
                    printf("%c", c);
                printf("\n");
            }
        }
    }
    fclose(p1);
    return 0;
}

