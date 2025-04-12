/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianafernandez <adrianafernandez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:18:12 by adrianafern       #+#    #+#             */
/*   Updated: 2025/04/12 21:28:32 by adrianafern      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_life(void *arg)
{
	long time;

	t_philo *philo = (t_philo *)arg;
	//segun par o impart pones a pensar o no
	//lock
	printf("%lg   %d is eating\n", time, philo->id);
	usleep(philo->info->time_eat);
	//unlock

	//Los pares toman fork[i] y luego fork[i+1]
	//Los impares toman fork[i+1] y luego fork[i]
}

void init_thread(t_data *info)
{
	
}
//pthread_create(&philo.thread, NULL, routine_func, &info->philos[i]);

t_philo create_philo(t_data *info, int i)
{
	t_philo philo;

	memset(&philo, 0, sizeof(t_philo));
	philo.id = i;
	philo.meals_eaten = 0;
	philo.fork1 = &info->forks[i];
	philo.fork2 = &info->forks[(i + 1) % info->num_philos];
	philo.info = info;
	return(philo);
}

void init_forks_and_philos(t_data *info)
{
	int i;

	info->forks = malloc(info->num_philos * sizeof(pthread_mutex_t));
	info->philos = malloc(info->num_philos * sizeof(t_philo));
	i = 0;
	while (i < info->num_philos)
	{
		pthread_mutex_init(&info->forks[i], NULL);
		i++;
	}
	i = 0;
	while (i < info->num_philos)
	{
		info->philos[i] = create_philo(info, i);
		i++;
	}
}

int	init_data(t_data *info, int argc, char **argv)
{
	int	i;

	if (argc != 5 && argc != 6)
		return (printf("philos - t die - t eat - t sleep - [meals]\n"), 0);
	i = 1;
	while (argv[i])
	{
		if (ft_strlen(argv[i]) > 11) // limite
			return (write(2, "Error int out of limits\n", 24), 0);
		if (!ft_isdigit(argv[i]) || !argv[i]) // solo numeros
			return (printf("philos - t die - t eat - t sleep - [meals]\n"), 0);
		i++;
	}
	memset(info, 0, sizeof(t_data));
	info->num_philos = atoi_limit(argv[1]); // limite y vacio
	info->time_eat = atoi_limit(argv[2]);
	info->time_sleep = atoi_limit(argv[3]);
	info->time_die = atoi_limit(argv[4]);
	info->num_meals = -1;
	if (argc == 6)
		info->num_meals = atoi_limit(argv[5]);
	init_forks_and_philos(info);
	return (1);
}

int	main(int argc, char **argv)
{
	t_data info;

	if (init_data(&info, argc, argv) == 0)
		exit(1);
	init_thread(info);
}