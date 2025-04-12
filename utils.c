/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianafernandez <adrianafernandez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:01:55 by adrianafern       #+#    #+#             */
/*   Updated: 2025/04/12 20:17:36 by adrianafern      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if ((str[i] < '0' || str[i] > '9') && ((str[i] != ' ') && (str[i] != '-'
					&& str[i] != '+')))
			return (0);
		i++;
	}
	return (1);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

static int	is_space(int *i, const char *nptr)
{
	while (nptr[*i] != '\0' && (nptr[*i] == ' ' || nptr[*i] == '\n'
			|| nptr[*i] == '\t' || nptr[*i] == '\f' || nptr[*i] == '\v'
			|| nptr[*i] == '\r'))
		(*i)++;
	if (nptr[*i] == '\0')
		return (1);
	return (0);
}

static int	is_sign(int *i, const char *nptr)
{
	int	sign;

	sign = 1;
	while (nptr[*i] && (nptr[*i] == '-' || nptr[*i] == '+'))
	{
		if (nptr[*i] == '-')
			sign = -1;
		(*i)++;
	}
	if (nptr[*i] == '\0')
		return (-1);
	return (sign);
}

int	atoi_limit(const char *nptr)
{
	int		i;
	int		sign;
	long	number;

	i = 0;
	if (is_space(&i, nptr) == 1)
	{
		printf("philos - t die - t eat - t sleep - [meals]\n");
		exit(1);
	}
	sign = is_sign(&i, nptr);
	number = 0;
	while (nptr[i] != '\0' && (nptr[i] >= '0' && nptr[i] <= '9'))
	{
		number = number * 10 + (nptr[i] - 48);
		i++;
	}
	number *= sign;
	if (number > 2147483647 || number < 0)
	{
		printf("philos - t die - t eat - t sleep - [meals]\n");
		exit(1);
	}
	return ((int)number);
}
