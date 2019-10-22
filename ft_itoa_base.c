#include "libft.h"

static unsigned long long	ft_itoa_negative(int n, int base, int *flag)
{
	unsigned long long	number;

	if (n < 0)
	{
		if (base == 10)
		{
			*flag = 1;
			number = -n;
		}
		else
			number = 0xFFFFFFFF + n + 1;
	}
	else
		number = n;
	return (number);
}

char						*ft_itoa_base(int n, int base)
{
	char				*str;
	char				tab[16] = {"0123456789abcdef"};
	unsigned long long	number;
	int 				o;
	size_t 				size;
	unsigned long long	tmp;
	int 				flag;

	flag = 0;
	size = 0;
	tmp = number = ft_itoa_negative(n, base, &flag);
	while (tmp /= base)
		size++;
	if (base < 2 || base > 16)
		return (0);
	size += flag + 1;
	if (!number)
		return ("0");
	else if (!(str = ft_strnew(size)))
		return (NULL);
	while (number)
	{
		o = number % base;
		str[--size] = tab[o];
		number /= base;
	}
	if (flag && base == 10)
		str[0] = '-';
	return (str);
}
