/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routines.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianafernandez <adrianafernandez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 12:39:45 by adrianafern       #+#    #+#             */
/*   Updated: 2025/04/22 22:00:56 by adrianafern      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*one_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->fork1);
	print_message(philo, "has taken a fork");
	cut_sleep(philo->info->time_die, philo->info);
	pthread_mutex_unlock(philo->fork1);
	print_message(philo, "died");
	return (NULL);
}

void	print_and_flag_death(t_data *info, int i)
{
	pthread_mutex_lock(&info->death_mut);
	pthread_mutex_lock(&info->stdout_mut);
	info->someone_died = 1;
	printf("%ld   %d died\n", get_time(info), info->philos[i].id);
	pthread_mutex_unlock(&info->stdout_mut);
	pthread_mutex_unlock(&info->death_mut);
}

void	*philo_death(void *arg)
{
	t_data	*info;
	int		i;

	info = (t_data *)arg;
	i = 0;
	while (1)
	{
		pthread_mutex_lock(&info->meals_mut);
		if (get_time(info) - info->philos[i].last_meal_time > info->time_die)
		{
			if (info->philos[i].meals_eaten < info->num_meals
				|| info->num_meals == -1)
				print_and_flag_death(info, i);
			pthread_mutex_unlock(&info->meals_mut);
			return (NULL);
		}
		pthread_mutex_unlock(&info->meals_mut);
		i++;
		if (i == info->num_philos)
			i = 0;
	}
}

int	philo_eat(t_philo *philo)
{
	if (check_if_dead(philo->info))
		return (1);
	pthread_mutex_lock(philo->fork1);
	print_message(philo, "has taken a fork");
	pthread_mutex_lock(philo->fork2);
	print_message(philo, "has taken a fork");
	pthread_mutex_lock(&philo->info->meals_mut);
	philo->last_meal_time = get_time(philo->info);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->info->meals_mut);
	print_message(philo, "is eating");
	cut_sleep(philo->info->time_eat, philo->info);
	pthread_mutex_unlock(philo->fork1);
	pthread_mutex_unlock(philo->fork2);
	return (0);
}

void	*philo_life(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 != 0)
		cut_sleep(philo->info->time_eat, philo->info);
	while (1)
	{
		pthread_mutex_lock(&philo->info->meals_mut);
		if (check_if_dead(philo->info)
			|| ((philo->meals_eaten >= philo->info->num_meals)
				&& philo->info->num_meals != -1))
		{
			pthread_mutex_unlock(&philo->info->meals_mut);
			break ;
		}
		pthread_mutex_unlock(&philo->info->meals_mut);
		if (philo_eat(philo) == 1)
			break;
		print_message(philo, "is sleeping");
		cut_sleep(philo->info->time_sleep, philo->info);
		print_message(philo, "is thinking");
	}
	return (NULL);
}
