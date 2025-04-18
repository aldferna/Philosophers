/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:11:27 by adrianafern       #+#    #+#             */
/*   Updated: 2025/04/18 15:43:25 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_if_dead(t_data *info)
{
	int	death;

	pthread_mutex_lock(&info->death_mut);
	death = info->someone_died;
	pthread_mutex_unlock(&info->death_mut);
	return (death);
}

void	print_message(t_philo *philo, char *str)
{
	if (check_if_dead(philo->info))
		return ;
	pthread_mutex_lock(&philo->info->stdout_mut);
	printf("%ld   %d %s\n", get_time(philo->info), philo->id, str);
	pthread_mutex_unlock(&philo->info->stdout_mut);
}

long	get_time(t_data *info)
{
	struct timeval	time;
	long			milisecs_passed;

	gettimeofday(&time, NULL);
	milisecs_passed = ((time.tv_sec * 1000) + (time.tv_usec / 1000))
		- info->start_time;
	return (milisecs_passed);
}

void	cut_sleep(long time_in_ms, t_data *info)
{
	long	waiting_started;

	waiting_started = get_time(info);
	while (get_time(info) - waiting_started < time_in_ms)
	{
		usleep(50);
		if (check_if_dead(info))
			break ;
	}
}

void	clean_resources(t_data *info)
{
	int	i;

	i = 0;
	while (i < info->num_philos)
	{
		pthread_mutex_destroy(&info->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&info->stdout_mut);
	pthread_mutex_destroy(&info->meals_mut);
	pthread_mutex_destroy(&info->death_mut);
	free(info->forks);
	free(info->philos);
}
