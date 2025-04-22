/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:23:42 by adrianafern       #+#    #+#             */
/*   Updated: 2025/04/22 18:36:26 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data
{
	long			start_time;
	int				num_philos;
	int				time_eat;
	int				time_sleep;
	int				time_die;
	int				num_meals;
	struct s_philo	*philos;
	pthread_mutex_t	*forks;
	pthread_t		dead_checker;
	pthread_mutex_t	stdout_mut;
	pthread_mutex_t	meals_mut;
	pthread_mutex_t	death_mut;
	int				someone_died;
}					t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*fork1;
	pthread_mutex_t	*fork2;
	t_data			*info;
}					t_philo;

//	PHILO ROUTINES
void				*one_philo(void *arg);
void				*philo_life(void *arg);
void				*philo_death(void *arg);

//	PHILO UTILS
int					check_if_dead(t_data *info);
long				get_time(t_data *info);
void				print_message(t_philo *philo, char *str);
void				cut_sleep(long time_in_ms, t_data *info);
void				clean_resources(t_data *info);

//  BASIC UTILS
int					atoi_limit(const char *nptr);
int					ft_strlen(const char *str);
int					ft_isdigit(char *str);

#endif