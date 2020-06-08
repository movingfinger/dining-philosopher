/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sako <sako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/08 18:18:01 by sako              #+#    #+#             */
/*   Updated: 2020/06/08 21:11:58 by sako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void error_check(char **arg)
{
	if (ft_atol(arg[1]) > 100)
		ft_print_error(ERR_NUMMAX);
	for (int i = 2; arg[i]; i++)
	{
		if (arg[i][0] == '-')
			ft_print_error(ERR_NEG);
		if (ft_atol(arg[i]) > __INT_MAX__)
			ft_print_error(ERR_INTMAX);
		for (int j = 0; arg[i][j]; j++)
		{
			if (arg[i][j] < '0' || arg[i][j] > '9')
				ft_print_error(ERR_NONUM);
		}
	}
}

void set_param(char **av, int ac)
{
	
	if (ac > 6 || ac < 5)
		ft_print_error(ERR_ARG);
	else if(ac == 5)
		food_limit = -1;
	else if (ac == 6)
		food_limit = ft_atol(av[5]);
	error_check(av);
	num_philo = ft_atol(av[1]);
	die_seconds = ft_atol(av[2]);
	eat_seconds = ft_atol(av[3]);
	sleep_seconds = ft_atol(av[4]);
	
	num_can_eat = num_philo - 1;
	ft_display_param("Number of philosopher is", num_philo, "ms");
	ft_display_param("Die time is", die_seconds, "ms");
	ft_display_param("Eat time is", eat_seconds, "ms");
	ft_display_param("Sleep time is", sleep_seconds, "ms");
	if (food_limit == -1)
		ft_putendl("food is unlimited");
	else
		ft_display_param("food limit is", food_limit, "");
}