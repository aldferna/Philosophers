/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldferna <aldferna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:18:12 by adrianafern       #+#    #+#             */
/*   Updated: 2025/04/17 20:12:54 by aldferna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int check_if_dead(t_data *info)
{
	int death;

	pthread_mutex_lock(&info->death_mut);
	death = info->someone_died;
	pthread_mutex_unlock(&info->death_mut);
	return (death);
}

void print_message(t_philo *philo, char *str)
{
	if (check_if_dead(philo->info))
		return;
	pthread_mutex_lock(&philo->info->stdout_mut);
	printf("%ld   %d %s\n", get_time(philo->info), philo->id, str);
	pthread_mutex_unlock(&philo->info->stdout_mut);
}

long get_time(t_data *info)
{
	struct timeval time;
	long milisecs_passed;

	gettimeofday(&time, NULL);
	milisecs_passed = ((time.tv_sec * 1000) + (time.tv_usec / 1000)) - info->start_time;
	return(milisecs_passed);
}

void cut_sleep(long time_in_ms, t_data *info) //puede fallar si la espera es muy larga
{
    long waiting_started;

	waiting_started = get_time(info);
    while (get_time(info) - waiting_started < time_in_ms) //y si time_in_ms es menor
        usleep(50);
}

void clean_resources(t_data *info)
{
    int i;
    
	i = 0;
	// if (info->num_philos == 1)
	// {
	// 	pthread_mutex_destroy(&info->philos[0].fork1);
	// 	return;
	// } //?????
	while (i < info->num_philos)
	{
		pthread_mutex_destroy(&info->forks[i]);
		//free(&info->philos[i]);
		i++;
	}
    pthread_mutex_destroy(&info->stdout_mut);
    pthread_mutex_destroy(&info->meals_mut);
	pthread_mutex_destroy(&info->death_mut);
    free(info->forks);
    free(info->philos);
}

void *philo_death(void *arg)
{
	t_data *info;
	int i;

	info = (t_data *)arg;
	i = 0;
	while(1) //!info->someone_died (!info->someone_died && i < info->num_philos)
	{
		//usleep(500);
		if (check_if_dead(info))
			return NULL;
		pthread_mutex_lock(&info->meals_mut);
		if (get_time(info) - info->philos[i].last_meal_time > info->time_die) //+ tiempo sin comer que time_die
		{
			pthread_mutex_lock(&info->death_mut);
			pthread_mutex_lock(&info->stdout_mut);
			info->someone_died = 1;
			printf("%ld   %d died\n", get_time(info), info->philos[i].id);
			pthread_mutex_unlock(&info->stdout_mut);
			pthread_mutex_unlock(&info->death_mut);
			pthread_mutex_unlock(&info->meals_mut);
			//print_message(&info->philos[i], "died ines");
			return NULL;
		}
		pthread_mutex_unlock(&info->meals_mut);
		i++;
		if (i == info->num_philos)
			i = 0;
		//usleep(500); //cambiar
	}
	return NULL;
}

void	*philo_life(void *arg)
{
	t_philo *philo;
	
	philo = (t_philo *)arg;
/* 	pthread_mutex_lock(&philo->info->meals_death_mut);
	philo->last_meal_time = get_time(philo->info);
	pthread_mutex_unlock(&philo->info->meals_death_mut); */
	if (philo->id % 2 != 0)
		cut_sleep(philo->info->time_eat, philo->info);
	while (1)
	{
		pthread_mutex_lock(&philo->info->meals_mut);
		if (check_if_dead(philo->info) || ((philo->meals_eaten >= philo->info->num_meals) && philo->info->num_meals != -1))
		{
			pthread_mutex_unlock(&philo->info->meals_mut);
            break;
		}
		pthread_mutex_unlock(&philo->info->meals_mut);
		

		//usleep(philo->id * 100);
		
		if (philo->id % 2 == 0)
		{
			if (check_if_dead(philo->info))
				break;
			pthread_mutex_lock(philo->fork1); //esto que pasa si esta ocupado por otro? espera
			print_message(philo, "has taken a fork");
			pthread_mutex_lock(philo->fork2);
			print_message(philo, "has taken a fork");
		}
		else
		{
			if (check_if_dead(philo->info))
				break;
			//cut_sleep(philo->info->time_eat, philo->info);
			pthread_mutex_lock(philo->fork2);
			print_message(philo, "has taken a fork");
			pthread_mutex_lock(philo->fork1);
			print_message(philo, "has taken a fork");
		}
		
		// if (check_if_dead(philo->info))
		// 	break;
		// pthread_mutex_lock(philo->fork1);
        // print_message(philo, "has taken a fork");
        // pthread_mutex_lock(philo->fork2);
        // print_message(philo, "has taken a fork");
		
		pthread_mutex_lock(&philo->info->meals_mut);
		philo->last_meal_time = get_time(philo->info);
		philo->meals_eaten++;
		pthread_mutex_unlock(&philo->info->meals_mut);
		print_message(philo, "is eating");
		cut_sleep(philo->info->time_eat, philo->info);
		if (philo->id % 2 == 0)
		{
			//print_message(philo, "has soltar a fork 21 ines");	
			pthread_mutex_unlock(philo->fork1);
			pthread_mutex_unlock(philo->fork2);
		}
		else
		{
			//print_message(philo, "has soltar a fork 12 ines");	
			pthread_mutex_unlock(philo->fork2);
			pthread_mutex_unlock(philo->fork1);
		}
		print_message(philo, "is sleeping");
		cut_sleep(philo->info->time_sleep, philo->info);
		print_message(philo, "is thinking");
		// if (philo->id % 2 == 0)
		// 	usleep(1);
	}
	return NULL;
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
		pthread_mutex_lock(&info->meals_mut);  //va a philo_life?
        info->philos[i].last_meal_time = get_time(info);
        pthread_mutex_unlock(&info->meals_mut);
		pthread_create(&info->philos[i].thread, NULL, philo_life, &info->philos[i]); //ahí sí va &thread porque estás llenando la variable, pthread_join(...) usa el valor
		i++;
	}
	pthread_create(&info->dead_checker, NULL, philo_death, info); //monitorea los que existen
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
	philo.id = i + 1;
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
	pthread_mutex_init(&info->stdout_mut, NULL);
	pthread_mutex_init(&info->meals_mut, NULL);
	pthread_mutex_init(&info->death_mut, NULL);
	i = 0;
	while (i < info->num_philos)
	{
		memset(&philo, 0, sizeof(t_philo));
		philo.id = i + 1;
		philo.fork1 = &info->forks[i];
	//	printf("philo %d fork1 = %d\n", philo.id, i);
		philo.fork2 = &info->forks[(i + 1) % info->num_philos];
	//	printf("philo %d fork2 = %d\n", philo.id, (i + 1) % info->num_philos);
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
			return (printf("The program needs numerical arguments\n"), 0);
		i++;
	}
	memset(info, 0, sizeof(t_data));
	info->num_philos = atoi_limit(argv[1]); // limite y vacio
	if (info->num_philos == 0)
		return (printf("The program needs at least 1 philosopher\n"), 0);
	info->time_die = atoi_limit(argv[2]);
	info->time_eat = atoi_limit(argv[3]);
	info->time_sleep = atoi_limit(argv[4]);
	info->num_meals = -1;
	if (argc == 6)
		info->num_meals = atoi_limit(argv[5]);
	init_forks_and_philos(info);
	return (1);
}

void *one_philo(void *arg) //hay quee crear un thread por filo, tantos tenedores como hilos
{
	t_philo *philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->fork1);
	print_message(philo, "has taken a fork");
	cut_sleep(philo->info->time_die, philo->info);
	pthread_mutex_unlock(philo->fork1);
	print_message(philo, "has died");
	return NULL;
}

int	main(int argc, char **argv)
{
	t_data info;
	struct timeval time;

	if (init_data(&info, argc, argv) == 0)
	{
		//liberar memoria(?)
		exit(1);
	}
	if (info.num_philos == 1)
	{
		gettimeofday(&time, NULL);
		info.start_time = ((time.tv_sec * 1000) + (time.tv_usec / 1000));
		pthread_create(&info.philos[0].thread, NULL, one_philo, &info.philos[0]);
		pthread_join(info.philos[0].thread, NULL);
	}
	else
		init_threads_join(&info);
	clean_resources(&info);
}





//checkear leaks

//"verify if there is a mutex to prevent a philo from dying and starting eating at the same time"

//ok
//un philo
//a death delayed by more than 10 ms is unacceptable
//sigue imprimiendo una vez muere
//segfault si 0 philos
