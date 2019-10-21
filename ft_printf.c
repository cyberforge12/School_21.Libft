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
	while (++i < 10)
	{
		if (**str == types[i])
		{
			params->conv = **str;
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

void 	do_format(t_params *params, va_list args)
{
	char *buf;
	char *tmp;
	int len;

	if (params->conv == '%')
		params->ret = ft_strdup("%");
	if (params->conv == 's')
		params->ret = ft_strdup(va_arg(args, char*));
	if (params->conv == 'd')
	{
		if (params->length[0] == 'l')
			params->ret = ft_strdup(ft_itoa(va_arg(args, long)));
		else
			params->ret = ft_strdup(ft_itoa(va_arg(args, int)));
	}
	if ((len = ft_strlen(params->ret)) < params->width)
	{
		params->width -= len;
		buf = ft_strnew(params->width);
		ft_memset(buf, ' ', params->width);
		tmp = params->ret;
		params->ret = ft_strjoin(buf, params->ret);
		free(tmp);
		free(buf);
	}
}


int		ft_printf(const char *str, ...)
{
	va_list		args;
	char 		flags[5] = {"#0- +"};
	char 		decimal[11] = {"0123456789."};
	char 		length[3] = {"hlL"};
	char 		conv[11] = "cspdiouxXf%";
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
			while (1)
			{
				if (ft_strchr(flags, *str))
					check_flags(&str, &params);
				else if (ft_strchr(decimal, *str))
					check_decimal(&str, &params);
				else if (ft_strchr(length, *str))
					check_length(&str, &params);
				else if (ft_strchr(conv, *str))
					check_conversion(&str, &params);
				else
					break;
			}
			do_format(&params, args);
			ft_putstr(params.ret);
			count += ft_strlen(params.ret);
		}
	}
	va_end(args);
	return (count);
}
