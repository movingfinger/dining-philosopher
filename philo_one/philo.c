/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sako <sako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/02 14:39:57 by sako              #+#    #+#             */
/*   Updated: 2020/06/09 00:01:12 by sako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int main(int ac, char **av)
{
	int i;
	long time_basic;
	pthread_t t_time;
	t_philosophers *t_philo;

	set_param(av, ac);
	time_basic = 0;
	m_fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * num_philo);
	t_philo = (t_philosophers *)malloc(sizeof(t_philosophers) * num_philo);
	timestamp = (long long *)malloc(sizeof(long long) * num_philo);
	pthread_mutex_init (&food_lock, NULL);
	pthread_mutex_init (&num_can_eat_lock, NULL);

	pthread_create(&t_time, NULL, time_ct, &time_basic);
	for (i = 0; i < num_philo; i++)
	{
		pthread_mutex_init (&m_fork[i], NULL);
		Spawn(&t_philo[i], &time_basic, i);
	}	
	for (i = 0; i < num_philo; i++)
	{
		pthread_join (t_philo[i].thread_philo, NULL);
		pthread_join (t_philo[i].thread_philo_time, NULL);
	}
	for (i = 0; i < num_philo; i++)
		pthread_mutex_destroy (&m_fork[i]);
	for (i = 0; i < num_philo; i++)
		if (t_philo[i].eat_count)
			 check_food += t_philo[i].eat_count;
	if (!check_food)
		ft_putendl("All philosophers are happy now!");
	return (0);
}