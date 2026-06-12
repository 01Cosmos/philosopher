
#include "philo.h"

void	print_action(t_philo *philo, char *action)
{
	pthread_mutex_lock(&philo->sim->print_mutex);
	pthread_mutex_lock(&philo->sim->death_mutex);
	if (!philo->sim->someone_died || ft_strcmp(action, "died") == 0)
		printf("%lld %d %s\n", get_curr_time() - philo->sim->start_time,
			philo->id, action);
	pthread_mutex_unlock(&philo->sim->death_mutex);
	pthread_mutex_unlock(&philo->sim->print_mutex);
}

void	sleeping(t_philo *philo, long long duration)
{
	long long	start;

	start = get_curr_time();
	while (get_curr_time() - start < duration)
	{
		pthread_mutex_lock(&philo->sim->death_mutex);
		if (philo->sim->someone_died)
		{
			pthread_mutex_unlock(&philo->sim->death_mutex);
			return ;
		}
		pthread_mutex_unlock(&philo->sim->death_mutex);
		usleep(500);
	}
}

bool	eaten_enough(t_philo *philo)
{
	bool	enough;

	pthread_mutex_lock(&philo->sim->meal_check_mutex);
	enough = (philo->sim->num_meals != -1
			&& philo->meals_eaten >= philo->sim->num_meals);
	pthread_mutex_unlock(&philo->sim->meal_check_mutex);
	return (enough);
}

int	main(int ac, char **av)
{
	t_sim	sim;

	if (!parse_args(ac, av, &sim))
		return (1);
	if (!init_data(&sim))
		return (printf("Init error\n"), 1);
	if (!create_threads(&sim))
		return (printf("Thread error\n"), clean_all(&sim), 1);
	clean_all(&sim);
	return (0);
}
