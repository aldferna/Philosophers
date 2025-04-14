/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianafernandez <adrianafernandez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:23:42 by adrianafern       #+#    #+#             */
/*   Updated: 2025/04/14 19:33:45 by adrianafern      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h> // pthread_create/_detach/_join, mutexes(init,destroy,un-lock)
# include <stdio.h>
# include <stdlib.h>   // malloc,free, exit
# include <string.h>   // memset
# include <sys/time.h> // gettimeofday
# include <unistd.h>   // write, usleep

typedef struct s_data
{
	long	start_time;
	int		num_philos;
	int     time_eat; // tiempo que ocupa los tenedores
	int		time_sleep;
	int		time_die; // tiempo para empezar a comer desde ultima comida/inicio programa
	int		num_meals;
	struct s_philo *philos; //array t_philos
	pthread_mutex_t *forks; //array mutex forks
	pthread_t dead_checker; //hilo monitor
	pthread_mutex_t stdout_mut; //para no colapsar stdout con printf
	pthread_mutex_t meals_death_mut; //para que el monitor lea ok
	//pthread_mutex_t death_mut;
	int someone_died;
}			t_data;

typedef struct s_philo
{
	int		id;
	int meals_eaten;
	long last_meal_time;
	pthread_t thread;
	pthread_mutex_t *fork1; //puntero al array que tengo en data
	pthread_mutex_t *fork2;
	t_data	*info;
}			t_philo;

// comer -> dormir -> pensar

//	PROJECT UTILS
long get_time(t_data *info);
void print_message(t_philo *philo, char *str);

//  BASIC UTILS 
int			atoi_limit(const char *nptr);
int			ft_strlen(char *str);
int			ft_isdigit(char *str);

#endif