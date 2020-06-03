/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sako <sako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/02 14:39:57 by sako              #+#    #+#             */
/*   Updated: 2020/06/03 16:11:56 by sako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int main(int ac, char **av)
{
	int i;

	sleep_seconds = 0;
	num_philo = 5;
	food_limit = 5;
	if (ac == 4)
	{
		num_philo = atoi(av[1]);
		sleep_seconds = atoi(av[2]);
		food_limit = atoi(av[3]);
	}
	num_can_eat = num_philo - 1;
	food_temp = food_limit;

	chopstick = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * num_philo);
	philo = (pthread_t *)malloc(sizeof(pthread_t) * num_philo);
	pthread_mutex_init (&food_lock, NULL);
	pthread_mutex_init (&num_can_eat_lock, NULL);
	for (i = 0; i < num_philo; i++)
		pthread_mutex_init (&chopstick[i], NULL);
	for (i = 0; i < num_philo; i++)
		pthread_create (&philo[i], NULL, philosopher, (void *)i);


	for (i = 0; i < num_philo; i++)
		pthread_join (philo[i], NULL);
	return (0);
}

void *philosopher (void *num)
{
	int id;
	int i;
	int left_chopstick;
	int right_chopstick;
	int f;

	id = (int)num;
	printf ("Philosopher %d is done thinking and now ready to eat. \n", id);
	right_chopstick = id;
	left_chopstick = id + 1;

	// Wrap around the forks.
	if (left_chopstick == num_philo)
		left_chopstick = 0;

		while (f = food_on_table())
		{
			get_token ();

			grab_chopstick (id, right_chopstick, "right");
			grab_chopstick (id, left_chopstick, "left");

			printf ("Philosopher %d: eating\n", id);
			usleep (DELAY * (food_temp - f + 1));
			down_chopsticks (left_chopstick, right_chopstick);
			return_token ();
		}

		printf ("Philosopher %d is done eating. \n", id);
		return (NULL);
}

int food_on_table ()
{
	int myfood;

	pthread_mutex_lock (&food_lock);
	if (food_limit > 0)
		food_limit--;
	myfood = food_limit;
	pthread_mutex_unlock (&food_lock);
	return (myfood);
}

void grab_chopstick (int phil, int c, char *hand)
{
	pthread_mutex_lock (&chopstick[c]);
	printf ("Philosopher %d: got %s chopstick %d\n", phil, hand, c);
}

void down_chopsticks (int c1, int c2)
{
	pthread_mutex_unlock (&chopstick[c1]);
	pthread_mutex_unlock (&chopstick[c2]);
}

int get_token ()
{
	int successful;

	successful = 0;
	while (!successful)
	{
		pthread_mutex_lock (&num_can_eat_lock);
		if (num_can_eat > 0)
		{
			num_can_eat--;
			successful = 1;
		}
		else
			successful = 0;
		pthread_mutex_unlock (&num_can_eat_lock);
	}
}

void return_token ()
{
	pthread_mutex_lock (&num_can_eat_lock);
	num_can_eat++;
	pthread_mutex_unlock (&num_can_eat_lock);
}