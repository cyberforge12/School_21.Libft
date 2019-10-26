#include "libft.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h> //REMOVE!!!

static int		ft_error(const char *caller_name)
{
	ft_putstr("\nsome error in ");
	ft_putstr(caller_name);
	ft_putstr("\n");
	exit (0);
}

static void		check_decimal(const char **str, t_params *params)
{
	int		i;
	int 	is_precision;

	is_precision = 0;
	i = 0;
	if (**str == '.')
	{
		is_precision = 1;
		(*str)++;
	}
	if (**str && **str >= '0' && **str <= '9')
	{
		i = ft_atoi(*str);
		while (**str && **str >= '0' && **str <= '9')
			(*str)++;
	}
	if (**str == '$')
	{
		params->arg_num = i;
		(*str)++;
	}
	if (is_precision)
		params->precision = i;
	else
		params->width = i;
}

static void 	check_flags(const char **str, t_params *params)
{
	while (1)
	{
		if (**str == ' ')
			params->space = 1;
		else if (**str == '#')
			params->alt = 1;
		else if (**str == '0')
			params->pad = '0';
		else if (**str == '-')
			params->left = 1;
		else if (**str == '+')
			params->plus = 1;
		else
			break;
		(*str)++;
	}
}

static void 	check_length(const char **str, t_params *params)
{
	char 	length_flag[3] = {"hlL"};
	int 	i;

	i = -1;
	while (++i < 3)
	{
		if (**str == length_flag[i])
		{
			params->length[0] = **str;
			if (**str == 'h' && **(str + 1) == 'h')
			{
				params->length[1] = **(str + 1);
				(*str)++;
			}
			else if (**str == 'l' && *(*str + 1) == 'l')
			{
				params->length[1] = **(str + 1);
				(*str)++;
			}
			else
				(*str)++;
		}
	}
}

static void 	check_conversion(const char **str, t_params *params)
{
	char 		types[11] = "cspdiouxXf%";
	int 	i;

	i = -1;
	while (++i < 11)
	{
		if (**str == types[i])
		{
			params->conv = **str;
			if (**str == 'o' || **str == 'O')
				params->base = 8;
			else if (**str == 'd' || **str == 'i' || **str == 'u'
				||**str == 'D')
				params->base = 10;
			else if (**str == 'x' || **str == 'X')
				params->base = 16;
			else if (**str == 'C')
			{
				params->length[0] = 'l';
				params->length[1] = 0;
				params->conv = 'c';
			}
			else if (**str == 'S')
			{
				params->length[0] = 'l';
				params->length[1] = 0;
				params->conv = 's';
			}
			else if (**str == 'p')
			{
				params->alt = 1;
				params->length[0] = 'l';
				params->length[1] = 0;
				params->conv = 'x';
			}
			(*str)++;
			break ;
		}
	}
}

void	default_params(t_params *params)
{
	params->precision = -1;
	params->length[0] = 0;
	params->length[1] = 0;
	params->width = 0;
	params->left = 0;
	params->pad = ' ';
	params->plus = 0;
	params->alt = 0;
	params->arg_num = 0;
	params->space = 0;
	params->conv = 0;
	params->ret = NULL;

}

void 	do_format_width(t_params *params)
{
	int		len;
	char	*buf;
	char	*tmp;

	if ((len = ft_strlen(params->ret)) < params->width)
	{
		params->width -= len;
		buf = ft_strnew(params->width);
		ft_memset(buf, params->pad, params->width);
		tmp = params->ret;
		if (params->left)
			params->ret = ft_strjoin(params->ret, buf);
		else
			params->ret = ft_strjoin(buf, params->ret);
		free(tmp);
		free(buf);
	}
}

void 	do_format(t_params *params, va_list args)
{
	char		*tmp;
	long long	i;

	if (params->conv == '%')
		params->ret = ft_strdup("%");
	if (params->conv == 'c')
	{
		params->ret = ft_strnew(1);
		params->ret[0] = va_arg(args, int);
	}
	if (params->conv == 's')
		params->ret = ft_strdup(va_arg(args, char*));
	if (params->conv == 'd' || params->conv == 'i' || params->conv == 'o'
		|| params->conv == 'x' || params->conv == 'X')
	{
		i = va_arg(args, long long);
		params->ret = ft_itoa_base(i, params->base);
		if (params->conv == 'X')
		{
			tmp = params->ret;
			while (*tmp)
			{
				if (*tmp >= 'a' && *tmp <= 'f')
				{
					*tmp += 'A' - 'a';
					tmp++;
				}
			}
		}
		if (i > 0 && params->plus)
		{
			tmp = params->ret;
			params->ret = ft_strjoin("+", params->ret);
			free(tmp);
		}
	}
	do_format_width(params);
}


int		ft_printf(const char *str, ...)
{
	va_list		args;
	char 		flags[6] = "#0- +";
	char 		decimal[12] = "0123456789.";
	char 		length[4] = "hlL";
	char 		conv[12] = "cspdiouxXf%";
	int			count;
	t_params	params;

	va_start(args, str);
	count = 0;
	if (!str)
		ft_error(__func__);
	while (*str)
	{
		if (*str != '%')
		{
			ft_putchar(*str++);
			count++;
		}
		else
		{
			str++;
			default_params(&params);
			while (*str)
			{
				if (ft_strchr(flags, *str))
					check_flags(&str, &params);
				else if (ft_strchr(decimal, *str))
					check_decimal(&str, &params);
				else if (ft_strchr(length, *str))
					check_length(&str, &params);
				else if (ft_strchr(conv, *str))
				{
					check_conversion(&str, &params);
					break;
				}
			}
			do_format(&params, args);
			ft_putstr(params.ret);
			count += ft_strlen(params.ret);
			free(params.ret);
		}
	}
	va_end(args);
	return (count);
}
