/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianafernandez <adrianafernandez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:18:12 by adrianafern       #+#    #+#             */
/*   Updated: 2025/04/22 22:01:57 by adrianafern      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_threads_join(t_data *info)
{
	int				i;
	struct timeval	time;

	gettimeofday(&time, NULL);
	info->start_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	i = 0;
	while (i < info->num_philos)
	{
		pthread_mutex_lock(&info->meals_mut);
		info->philos[i].last_meal_time = get_time(info);
		pthread_mutex_unlock(&info->meals_mut);
		pthread_create(&info->philos[i].thread, NULL, philo_life,
			&info->philos[i]);
		i++;
	}
	pthread_create(&info->dead_checker, NULL, philo_death, info);
	i = 0;
	while (i < info->num_philos)
	{
		pthread_join(info->philos[i].thread, NULL);
		i++;
	}
	pthread_join(info->dead_checker, NULL);
}

void	init_forks_and_philos(t_data *info)
{
	int		i;
	t_philo	philo;

	info->forks = malloc(info->num_philos * sizeof(pthread_mutex_t));
	info->philos = malloc(info->num_philos * sizeof(t_philo));
	i = 0;
	while (i < info->num_philos)
	{
		pthread_mutex_init(&info->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&info->stdout_mut, NULL);
	pthread_mutex_init(&info->meals_mut, NULL);
	pthread_mutex_init(&info->death_mut, NULL);
	i = 0;
	while (i < info->num_philos)
	{
		memset(&philo, 0, sizeof(t_philo));
		philo.id = i + 1;
		philo.fork1 = &info->forks[i];
		philo.fork2 = &info->forks[(i + 1) % info->num_philos];
		philo.info = info;
		info->philos[i] = philo;
		i++;
	}
}

int	check_for_exit_signal(t_data info)
{
	if (info.num_philos == -5 || info.time_die == -5 || info.time_eat == -5
		|| info.time_sleep == -5 || info.num_meals == -5)
		return (0);
	return (1);
}

int	init_data(t_data *info, int argc, char **argv)
{
	int	i;

	if (argc != 5 && argc != 6)
		return (printf("philos - t die - t eat - t sleep - [meals]\n"), 0);
	i = 1;
	while (argv[i])
	{
		if (!ft_isdigit(argv[i]) || !argv[i])
			return (printf("The program needs numerical arguments\n"), 0);
		i++;
	}
	memset(info, 0, sizeof(t_data));
	info->num_philos = atoi_limit(argv[1]);
	if (info->num_philos == 0)
		return (printf("The program needs at least 1 philosopher\n"), 0);
	info->time_die = atoi_limit(argv[2]);
	info->time_eat = atoi_limit(argv[3]);
	info->time_sleep = atoi_limit(argv[4]);
	info->num_meals = -1;
	if (argc == 6)
		info->num_meals = atoi_limit(argv[5]);
	if (check_for_exit_signal(*info) == 0)
		return (0);
	init_forks_and_philos(info);
	return (1);
}

int	main(int argc, char **argv)
{
	t_data			info;
	struct timeval	time;

	if (init_data(&info, argc, argv) == 0)
		return (free(info.forks), free(info.philos), 1);
	if (info.num_philos == 1)
	{
		gettimeofday(&time, NULL);
		info.start_time = ((time.tv_sec * 1000) + (time.tv_usec / 1000));
		pthread_create(&info.philos[0].thread, NULL, one_philo,
			&info.philos[0]);
		pthread_join(info.philos[0].thread, NULL);
	}
	else
		init_threads_join(&info);
	clean_resources(&info);
}

//norminette y carpeta!

//   463  ./philo 3 310 104 103 2  (el 3o en comer muere)
// 0 1 come
// 104 2 come
// 208 3 come (207 1 libre)
// 312 1 come (1 muere xq fork queda libre en el 312 > 310)
// 416 2 come
// 520 3 come

//   466  ./philo 3 310 103 103 15  (comen todos OK) 103 * 3 = 309 < 310

//   467  ./philo 3 310 103 103 (infinito)

//   477  ./philo 3 411 103 103 2 (wc -l 6)
//   479  ./philo 3 412 103 103 20 (wc -l 60)
//   481  ./philo 20 411 103 103 20 (wc -l 400)

//   484  ./philo 39 810 200 200 (infinito)