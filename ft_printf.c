#include "libft.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h> //REMOVE!!!

void	check_decimal(const char **str, int *width, int *next_arg)
{
	int		i;

	i = 0;
	if (**str && **str >= '0' && **str <= '9')
		i = ft_atoi(*str);
	while (**str && **str >= '0' && **str <= '9')
		(*str)++;
	if (**str == '$')
		*next_arg = i;
	else
		*width = i;
}

char	*conversion(va_list args, int width, char *flag, char conv)
{
	int		count;
	char	*buf;
	char 	*ret;
	char 	*tmp;
	int 	len;

	count = 0;
	if (conv == 's')
		ret = ft_strdup(va_arg(args, char*));
	if (conv == 'd')
	{
		if (*flag == 'l')
			ret = ft_strdup(ft_itoa(va_arg(args, long)));
		else
			ret = ft_strdup(ft_itoa(va_arg(args, int)));
	}
	if (ret && *ret && ((len = ft_strlen(ret)) < width))
	{
		width -= len;
		buf = ft_strnew(width);
		ft_memset(buf, ' ', width);
		tmp = ret;
		ret = ft_strjoin(buf, ret);
		free(tmp);
		free(buf);
	}
//	if (*str == '%')
//	{
//		ft_putchar(*str++);
//		count++;
//	}
	return (ret);
}

void 	check_flags(const char **str, char *flag)
{
	char 	flags[8][1] = {"h", "l", "L", "#", "0", "-", "+", " "};
	int 	i;

	i = -1;
	while (++i < 8)
	{
		if (**str == flags[i][0])
		{
			flag[0] = **str;
			if (**str == 'h' && **(str + 1) == 'h')
			{
				flag[1] = **(str + 1);
				(*str)++;
			}
			else if (**str == 'l' && *(*str + 1) == 'l')
			{
				flag[1] = **(str + 1);
				(*str)++;
			}
			else
				(*str)++;
		}
	}
}

void 	check_conv(const char **str, char *conv)
{
	char 		convs[10] = "cspdiouxXf";
	int 	i;

	i = -1;
	while (++i < 10)
	{
		if (**str == convs[i])
		{
			conv[0] = **str;
			conv[1] = 0;
			(*str)++;
			break ;
		}

	}
}

int		ft_printf(const char *str, ...)
{
	va_list		args;
	char		flag[2] = {0, 0};
	int			count;
	int 		width;
	int 		next_arg;
	char 		conv[1] = {0};
	char 		*ret;

	width = 0;
	va_start(args, str);
	count = 0;
	while (*str)
	{
		if (*str == '%')
		{
			str++;
			check_decimal(&str, &width, &next_arg);
			check_flags(&str, flag);
			check_conv(&str, conv);
			ret = conversion(args, width, flag, conv[0]);
			ft_putstr(ret);
		}
		else
		{
			ft_putchar(*str++);
			count++;
		}
	}
	va_end(args);

	return (count);
}
