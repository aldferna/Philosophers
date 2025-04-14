/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianafernandez <adrianafernandez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:18:12 by adrianafern       #+#    #+#             */
/*   Updated: 2025/04/14 11:42:15 by adrianafern      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int philo_death()
{
	//un filósofo muere si pasa más tiempo sin comer que time_die
}

void print_message(t_philo *philo, char *str) //con los mutex
{
	pthread_mutex_lock(&philo->info->write_mut);
	printf("%lg   %d %s\n", get_time(philo->info), philo->id, str);
	pthread_mutex_unlock(&philo->info->write_mut);
}

long get_time(t_data *info)
{
	struct timeval time;
	long seconds;
	long microsecs;
	long milisecs_passed;

	gettimeofday(&time, NULL);
	seconds = time.tv_sec;
	microsecs = time.tv_usec;
	milisecs_passed = ((seconds * 1000) + (microsecs / 1000)) - info->time_start;
	return(milisecs_passed);
}

void	*philo_life(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	while (!philo_death() && (philo->meals_eaten < philo->info->num_meals))
	{
		print_message(philo, "is thinking");
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->fork1); //esto que pasa si esta ocupado por otro? espera
			print_message(philo, "has taken a fork");
			pthread_mutex_lock(philo->fork2);
			print_message(philo, "has taken a fork");
		}
		else
		{
			pthread_mutex_lock(philo->fork2);
			print_message(philo, "has taken a fork");
			pthread_mutex_lock(philo->fork1);
			print_message(philo, "has taken a fork");
		}
		pthread_mutex_lock(&philo->info->write_mut);
		philo->last_meal_time = get_time(philo->info); //mutex depende si philo_death lee
		pthread_mutex_unlock(&philo->info->write_mut);
		print_message(philo, "is eating");
		usleep(philo->info->time_eat); // usleep() usa microsegundos (?)
		philo->meals_eaten++; //mutex de esto depende si philo_death lee
		pthread_mutex_unlock(philo->fork1);
		pthread_mutex_unlock(philo->fork2);
		print_message(philo, "is sleeping");
		usleep(philo->info->time_sleep);
	}
}

void init_threads_join(t_data *info)
{
	int i;
	struct timeval time;

	gettimeofday(&time, NULL);
	info->start_time = (time.tv_sec * 1000) + (time.tv_usec / 1000); //inicializar cd empieza la simulacion
	i = 0;
	while (i < info->num_philos)
	{
		pthread_create(&info->philos[i].thread, NULL, philo_life, &info->philos[i]); //ahí sí va &thread porque estás llenando la variable, pthread_join(...) usa el valor
		i++;
	}
	pthread_create(&info.dead_checker, NULL, philo_death, &info->philos[i]); //monitorea los que existen
	i = 0;
	while(i < info->num_philos)
	{
		pthread_join(info->philos[i].thread, NULL); //espera
		i++;
	}
	pthread_join(info->dead_checker, NULL);
}

/* t_philo create_philo(t_data *info, int i)
{
	t_philo philo;

	memset(&philo, 0, sizeof(t_philo));
	philo.id = i;
	philo.fork1 = &info->forks[i];
	philo.fork2 = &info->forks[(i + 1) % info->num_philos];
	philo.info = info;
	return(philo);
} */

void init_forks_and_philos(t_data *info)
{
	int i;
	t_philo philo;

	info->forks = malloc(info->num_philos * sizeof(pthread_mutex_t));
	info->philos = malloc(info->num_philos * sizeof(t_philo));
	i = 0;
	while (i < info->num_philos)
	{
		pthread_mutex_init(&info->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&info->write_mut, NULL);
	i = 0;
	while (i < info->num_philos)
	{
		memset(&philo, 0, sizeof(t_philo));
		philo.id = i;
		philo.fork1 = &info->forks[i];
		philo.fork2 = &info->forks[(i + 1) % info->num_philos];
		philo.info = info;
		info->philos[i] = philo;
		//info->philos[i] = create_philo(info, i);
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
		exit(1); //liberar memoria
	init_threads_join(&info);
}