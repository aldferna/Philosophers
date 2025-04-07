/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adrianafernandez <adrianafernandez@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:18:12 by adrianafern       #+#    #+#             */
/*   Updated: 2025/04/07 20:31:56 by adrianafern      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int init_data(t_data *info, int argc, char **argv)
{
    int i;

    if (argc != 5 && argc != 6)
        return(printf("num of philosp - time die - time eat - time sleep - [num meals]\n"), 0);
    i = 1;
    while (argv[i])
    {
        if (ft_strlen(argv[i]) > 11) //limite
            return(write(2, "Error int out of limits\n", 24), 0);
        if (!ft_isdigit(argv[i]) || !argv[i]) //solo numeros
            return(printf("nnnum of philosp - time die - time eat - time sleep - num meals\n"), 0);
        i++;
    }
    memset(info, 0, sizeof(t_data));
    info->num_philos = atoi_limit(argv[1]); //limite y vacio
    info->time_eat = atoi_limit(argv[2]);
    info->time_sleep = atoi_limit(argv[3]);
    info->time_die = atoi_limit(argv[4]);
    info->num_meals = -1;
    if (argc == 6)
        info->num_meals = atoi_limit(argv[5]);
    return (1);
}

int main(int argc, char **argv)
{
    t_data info;
    
    if (init_data(&info, argc, argv) == 0)
        exit(1);
}