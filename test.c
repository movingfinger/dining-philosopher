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
	long time_basic;
	t_philosophers *t_philo;

	if (ac == 6)
	{
		num_philo = atoi(av[1]);
		die_seconds = atoi(av[2]);
		eat_seconds = atoi(av[3]);
		sleep_seconds = atoi(av[4]);
		food_limit = atoi(av[5]);
	}
	else if (ac == 5)
	{
		num_philo = atoi(av[1]);
		die_seconds = atoi(av[2]);
		eat_seconds = atoi(av[3]);
		sleep_seconds = atoi(av[4]);
		food_limit = -1;
	}
	num_can_eat = num_philo - 1;
	food_temp = food_limit;
	time_basic = 0;

	printf("Number of philosopher is %lld\n", num_philo);
	printf("Die time is %ds\n", die_seconds);
	printf("Eat time is %ds\n", eat_seconds);
	printf("Sleep time is %ds\n", sleep_seconds);
	printf("food limit is %d\n", food_limit);

	m_fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * num_philo);
	t_philo = (t_philosophers *)malloc(sizeof(t_philosophers) * num_philo);
	pthread_mutex_init (&food_lock, NULL);
	pthread_mutex_init (&num_can_eat_lock, NULL);

	for (i = 0; i < num_philo; i++)
	{
		pthread_mutex_init (&m_fork[i], NULL);
		Spawn(&t_philo[i], &time_basic, &m_fork, i);
	}
	//printf("Spawn done\n");

	//for (i = 0; i < num_philo; i++)
	//	pthread_join (t_philo[i].thread_philo, NULL);
	for (i = 0; i < num_philo; i++)
	{
		pthread_join (t_philo[i].thread_philo, NULL);
		pthread_join (t_philo[i].thread_philo_time, NULL);
	}
	//printf("All done!\n");
	return (0);
}

int food_on_table (t_philosophers *philo)
{
	pthread_mutex_lock (&food_lock);
	printf("Philosopher %d food left %d\n", philo->pos, philo->eat_count);
	philo->eat_count--;
	pthread_mutex_unlock (&food_lock);
	return (philo->eat_count);
}

void *philosopher (void *arg)
{
	t_philosophers *philo;
	int i;
	int left_fork;
	int right_fork;

	philo = arg;
	right_fork = philo->pos;
	left_fork = philo->pos + 1;
	i = 0;

	// Wrap around the forks.
	if (left_fork == num_philo)
		left_fork = 0;
	//pthread_create(&philo->thread_philo, NULL, life_cycle, philo);
	pthread_create(&philo->thread_philo_time, NULL, life_cycle, philo);
	while (i = food_on_table(philo))
	{
		philo->state = 1;
		printf("Philosoher %d is thinkg\n", philo->pos);
		usleep(10);
		philo->state = 0;
		printf ("Philosopher %d is eating. \n", philo->pos);
		eat(philo, left_fork, right_fork);
		pthread_create(&philo->thread_philo_time, NULL, life_cycle, philo);
		philo->state = 2;
		printf("Philosopher %d is sleeping now\n", philo->pos);
		usleep(sleep_seconds * 1000);
		if (!i)
		{
			printf("Philosopher %d died\n", philo->pos);
			exit (0);
		}
	}

	if (food_limit == 0)
	{
		printf("No food left!\n");
		exit (0);
	}

	pthread_exit(0);
	return (NULL);
}

void Spawn(t_philosophers *philo, long *time, pthread_mutex_t **m_fork, int i)
{
	static int death;

	philo->pos = i;
	philo->eat_count = food_limit;
	philo->state = 1;
	philo->death = &death;
	philo->time = time;

	pthread_create(&philo->thread_philo, NULL, philosopher, philo);
}

void eat(t_philosophers *philo, int left, int right)
{
	int id;

	id = philo->pos;
	philo->state = 2;
	get_token ();

	grab_fork (id, right);
	grab_fork (id, left);

	printf ("Philosopher %d: eating\n", id);
	usleep (eat_seconds * 1000);
	down_forks (left, right);
	return_token ();
}

void grab_fork (int phil, int c)
{
	pthread_mutex_lock (&m_fork[c]);
}

void down_forks (int c1, int c2)
{
	pthread_mutex_unlock (&m_fork[c1]);
	pthread_mutex_unlock (&m_fork[c2]);
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

void *time_ct(void *var)
{
	struct timeval time;
	struct timeval time2;
	long *elapsed;

	elapsed = var;
	gettimeofday(&time, NULL);
	while (1)
	{
		gettimeofday(&time2, NULL);
		*elapsed = (((time2.tv_sec - time.tv_sec) * 1000 + ((time2.tv_usec - time.tv_usec))));
		if (*elapsed < 0)
			*elapsed *= -1;
	}
}

void *life_cycle(void *arg)
{
	t_philosophers *philo;
	struct timeval time;
	struct timeval time2;
	long long elapsed;

	philo = arg;
	gettimeofday(&time, NULL);
	elapsed = 0;
	while (1)
	{
		gettimeofday(&time2, NULL);
		elapsed = (((time2.tv_sec - time.tv_sec) * 1000 + ((time2.tv_usec - time.tv_usec))));
		
		// if timestamp over time to die, then die.
		if (die_seconds * 1000 <= elapsed)
		{			
			philo->current = *philo->time;
			philo->state = 3;
			printf("Die second is %d and elapsed is %llu. Philosopher %d died\n", die_seconds, elapsed, philo->pos);
			exit (0);
		}
		if (philo->state == 0)	// if eating state
		{
			//printf("status changed to eating\n");
			pthread_exit(NULL);
		}
	}
}