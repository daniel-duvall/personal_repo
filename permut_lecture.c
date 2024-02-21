// This program prints all the permutation of a string
#include <stdio.h>
#include <string.h>
/* swap function*/
void swap(char *a, char *b)
{
    char temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
/* This function prints the permutation of a string
This function takes three parameters:
String, starting index, and ending index*/
void permute(char *s, int l, int r)
{
    printf("permute has been called\n");
    int i;
    if (l == r)
        printf("i = r -> %s\n", s);
    else
    {
        for (i = l; i <= r; i++)
        {
            printf("l=%d; r=%d\n", l, r);
            swap(&s[l], &s[i]);
            printf("first swap -> %s\n", s);
            permute(s, l+1, r);
            swap(&s[l], &s[i]); //swap back
            printf("swap back -> %s\n", s);
            
        }
    }
}
int main()
{
    char str[] = "CAT";
    int n = strlen(str);
    printf("n=%d\n", n);
    permute(str, 0, n-1);
    return 0;
}
