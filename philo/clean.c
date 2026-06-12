
#include "philo.h"

void	clean_all(t_sim *sim)
{
	int	i;

	i = -1;
	while (++i < sim->num_philos)
		pthread_mutex_destroy(&sim->philos[i].left_fork);
	pthread_mutex_destroy(&sim->print_mutex);
	pthread_mutex_destroy(&sim->death_mutex);
	pthread_mutex_destroy(&sim->meal_check_mutex);
	free(sim->philos);
}

long long	get_curr_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000);
}

void	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(&philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}
