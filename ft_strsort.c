#include "libft.h"

void    ft_strsotr(char *str)
{
    size_t i;
    size_t j;

    i = 0;
    while (i < ft_strlen(str))
    {
        j = 0;
        while (j < ft_strlen(str))
        {
            if (str[i] < str[j])
                ft_swap(&str[i], &str[j]);
            j++;
        }
        i++;
    }
}

int     main(void)
{
    char str1[15] = "abcdefg";
    char str2[15] = "gfedcba";

    printf("str1 - %s\n", str1);
    printf("str2 - %s\n", str2);
}