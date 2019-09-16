#include "libft.h"

void    ft_clear_list(t_list *lst)
{
    t_list *temp;

    while (lst)
    {
        temp = lst->next;
        ft_memdel((void **)&lst);
        lst = temp;
    }
}