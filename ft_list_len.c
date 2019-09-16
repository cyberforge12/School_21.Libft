#include "libft.h"

size_t      ft_list_len(t_list *alst)
{
    size_t len;

    len = 0;
    if (!alst)
        return (NULL);
    while (alst)
    {
        len++;
        alst = alst->next;
    }
    return (len);
}