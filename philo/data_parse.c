
#include "philo.h"

int	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

int	str_is_digit(char *s)
{
	int	i;

	if (!s || !s[0])
		return (0);
	if (s[0] == '-')
		return (0);
	if (s[0] == '+')
		i = 1;
	else
		i = 0;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!is_digit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_atoi(char *str, int *overflow)
{
	int		sign;
	long	res;

	sign = 1;
	res = 0;
	*overflow = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (is_digit(*str))
	{
		if (res > (LONG_MAX - (*str - '0')) / 10)
			return (*overflow = 1, 0);
		res = res * 10 + (*str - '0');
		str++;
	}
	res *= sign;
	if (res > INT_MAX || res < INT_MIN)
		return (*overflow = 1, 0);
	return ((int)res);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	parse_args(int ac, char **av, t_sim *sim)
{
	int	i;
	int	overflow;

	overflow = 0;
	if (ac != 5 && ac != 6)
		return (printf("Invalid args\n"), 0);
	i = 0;
	while (++i < ac)
	{
		if (!str_is_digit(av[i]))
			return (printf("Args should be positive number\n"), 0);
	}
	sim->num_philos = ft_atoi(av[1], &overflow);
	sim->time_to_die = ft_atoi(av[2], &overflow);
	sim->time_to_eat = ft_atoi(av[3], &overflow);
	sim->time_to_sleep = ft_atoi(av[4], &overflow);
	if (ac == 6)
		sim->num_meals = ft_atoi(av[5], &overflow);
	else
		sim->num_meals = -1;
	if (overflow || sim->num_philos <= 0 || sim->num_philos > MAX_PHILO_NUM
		|| sim->time_to_die <= 0 || sim->time_to_eat <= 0
		|| sim->time_to_sleep <= 0)
		return (printf("Invalid numeric values\n"), 0);
	return (1);
}
