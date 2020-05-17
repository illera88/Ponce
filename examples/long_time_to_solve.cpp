/* Run with 32 a's as first argument
*/
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc != 2)
        return -1;
    int len = strlen(argv[1]);
    int sum = 0;

    for (int i = 0; i < len; i++)
    {
        sum += argv[1][i];
    }
    //For example 32 a's
    if (sum == 97 * 32)
        printf("Win\n");
    else
        printf("Fail\n");
    return 0;
}
