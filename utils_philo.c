/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianafernandez <adrianafernandez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:11:27 by adrianafern       #+#    #+#             */
/*   Updated: 2025/04/14 18:24:40 by adrianafern      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* void print_message(t_philo *philo, char *str) //con los mutex
{
	pthread_mutex_lock(&philo->info->stdout_mut);
	printf("%ld   %d %s\n", get_time(philo->info), philo->id, str);
	pthread_mutex_unlock(&philo->info->stdout_mut);
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
	milisecs_passed = ((seconds * 1000) + (microsecs / 1000)) - info->start_time;
	return(milisecs_passed);
} */
