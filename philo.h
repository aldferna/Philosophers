/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:23:42 by adrianafern       #+#    #+#             */
/*   Updated: 2025/04/09 17:50:19 by aldferna         ###   ########.fr       */
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
	int		num_philos;
	int     time_eat; // tiempo que ocupa los tenedores
	int		time_sleep;
	int		time_die;
	// tiempo para empezar a comer desde ultima comida/inicio programa
	int		num_meals;
}			t_data;

typedef struct s_philo
{
	int		id;
	t_data	*info;

}			t_philo;

// comer -> dormir -> pensar

//  UTILS
int			atoi_limit(const char *nptr);
int			ft_strlen(char *str);
int			ft_isdigit(char *str);

#endif