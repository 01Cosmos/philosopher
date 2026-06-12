
#include "philo.h"

int	init_mutexe(t_sim *sim)
{
	sim->philos = malloc(sizeof(t_philo) * sim->num_philos);
	if (!sim->philos)
		return (0);
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
	{
		free(sim->philos);
		return (printf("error mutex"), 0);
	}
	if (pthread_mutex_init(&sim->death_mutex, NULL) != 0)
	{
		free(sim->philos);
		pthread_mutex_destroy(&sim->print_mutex);
		return (printf("error mutex"), 0);
	}
	if (pthread_mutex_init(&sim->meal_check_mutex, NULL) != 0)
	{
		free(sim->philos);
		pthread_mutex_destroy(&sim->print_mutex);
		pthread_mutex_destroy(&sim->death_mutex);
		return (printf("error mutex"), 0);
	}
	sim->someone_died = false;
	sim->start_time = get_curr_time();
	return (1);
}

int	init_data(t_sim *sim)
{
	int	i;

	if (!init_mutexe(sim))
		return (printf("error malloc"), 0);
	i = -1;
	while (++i < sim->num_philos)
	{
		(1) && (sim->philos[i].id = i + 1, sim->philos[i].meals_eaten = 0);
		sim->philos[i].last_meal_time = sim->start_time;
		sim->philos[i].sim = sim;
		if (pthread_mutex_init(&sim->philos[i].left_fork, NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&sim->philos[i].left_fork);
			pthread_mutex_destroy(&sim->meal_check_mutex);
			pthread_mutex_destroy(&sim->death_mutex);
			pthread_mutex_destroy(&sim->print_mutex);
			return (free(sim->philos), printf("error mutex"), 0);
		}
	}
	i = -1;
	while (++i < sim->num_philos)
		sim->philos[i].right_fork = &sim->philos[(i + 1)
			% sim->num_philos].left_fork;
	return (1);
}

void	*philo_routine(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (p->sim->num_philos == 1)
	{
		pthread_mutex_lock(&p->left_fork);
		print_action(p, "has taken a fork");
		pthread_mutex_lock(&p->sim->death_mutex);
		p->sim->someone_died = true;
		pthread_mutex_unlock(&p->sim->death_mutex);
		sleeping(p, p->sim->time_to_die);
		print_action(p, "died");
		return (pthread_mutex_unlock(&p->left_fork), NULL);
	}
	if (p->id % 2 == 0)
		usleep(100);
	while (!is_dead(p) && !eaten_enough(p))
	{
		eat(p);
		if (eaten_enough(p) || is_dead(p))
			break ;
		p_sleep(p);
		think(p);
	}
	return (NULL);
}

int	create_threads(t_sim *sim)
{
	pthread_t	monitor;
	int			i;

	i = -1;
	while (++i < sim->num_philos)
	{
		if (pthread_create(&sim->philos[i].thread_id, NULL,
				philo_routine, &sim->philos[i]) != 0)
			return (0);
	}
	if (pthread_create(&monitor, NULL, monitor_routine, sim) != 0)
		return (0);
	i = -1;
	while (++i < sim->num_philos)
		pthread_join(sim->philos[i].thread_id, NULL);
	pthread_join(monitor, NULL);
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;
	bool	all_done;

	sim = (t_sim *)arg;
	while (1)
	{
		all_done = true;
		i = -1;
		while (++i < sim->num_philos)
		{
			if (is_dead(&sim->philos[i]))
				return (NULL);
			if (!eaten_enough(&sim->philos[i]))
				all_done = false;
		}
		if (sim->num_meals != -1 && all_done)
		{
			pthread_mutex_lock(&sim->death_mutex);
			sim->someone_died = true;
			return (pthread_mutex_unlock(&sim->death_mutex), NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
