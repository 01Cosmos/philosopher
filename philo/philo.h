
#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <pthread.h>
# include <limits.h>
# include <sys/time.h>

# define MAX_PHILO_NUM 200

typedef struct s_sim	t_sim;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_t		thread_id;
	pthread_mutex_t	left_fork;
	pthread_mutex_t	*right_fork;
	t_sim			*sim;
}	t_philo;

typedef struct s_sim
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_meals;
	bool			someone_died;
	long long		start_time;
	t_philo			*philos;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	meal_check_mutex;
}	t_sim;

//-----parsing

int			is_digit(char c);
int			str_is_digit(char *s);
int			ft_atoi(char *str, int *overflow);
long long	get_curr_time(void);
int			ft_strcmp(const char *s1, const char *s2);

//-----actions

bool		is_dead(t_philo *philo);
void		take_forks(t_philo *philo);
void		eat(t_philo *philo);
void		p_sleep(t_philo *philo);
void		think(t_philo *philo);
void		sleeping(t_philo *philo, long long duration);

//-----clean

void		clean_all(t_sim *sim);

//---data

int			init_data(t_sim *sim);

//-----gl
void		put_forks(t_philo *philo);
void		print_action(t_philo *philo, char *action);
void		*philo_routine(void *arg);
void		*monitor_routine(void *arg);
bool		eaten_enough(t_philo *philo);
int			parse_args(int ac, char **av, t_sim *sim);
int			create_threads(t_sim *sim);
long long	get_curr_time(void);

#endif