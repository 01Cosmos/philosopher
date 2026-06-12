
#include "philo.h"

bool	is_dead(t_philo *philo)
{
	long long	since_last_meal;

	pthread_mutex_lock(&philo->sim->death_mutex);
	if (philo->sim->someone_died)
		return (pthread_mutex_unlock(&philo->sim->death_mutex), true);
	pthread_mutex_unlock(&philo->sim->death_mutex);
	pthread_mutex_lock(&philo->sim->meal_check_mutex);
	since_last_meal = get_curr_time() - philo->last_meal_time;
	pthread_mutex_unlock(&philo->sim->meal_check_mutex);
	if (since_last_meal > philo->sim->time_to_die)
	{
		pthread_mutex_lock(&philo->sim->death_mutex);
		philo->sim->someone_died = true;
		pthread_mutex_unlock(&philo->sim->death_mutex);
		print_action(philo, "died");
		return (true);
	}
	return (false);
}

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->left_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(&philo->left_fork);
		print_action(philo, "has taken a fork");
	}
}

void	eat(t_philo *philo)
{
	take_forks(philo);
	print_action(philo, "is eating");
	pthread_mutex_lock(&philo->sim->meal_check_mutex);
	philo->last_meal_time = get_curr_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->sim->meal_check_mutex);
	sleeping(philo, philo->sim->time_to_eat);
	put_forks(philo);
}

void	p_sleep(t_philo *philo)
{
	print_action(philo, "is sleeping");
	sleeping(philo, philo->sim->time_to_sleep);
}

void	think(t_philo *philo)
{
	print_action(philo, "is thinking");
}
