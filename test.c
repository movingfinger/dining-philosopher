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

	if (ac == 4)
	{
		num_philo = atoi(av[1]);
		sleep_seconds = atoi(av[2]);
		food_limit = atoi(av[3]);
	}
	num_can_eat = num_philo - 1;
	food_temp = food_limit;
	time_basic = 0;

	m_fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * num_philo);
	philo = (pthread_t *)malloc(sizeof(pthread_t) * num_philo);
	pthread_mutex_init (&food_lock, NULL);
	pthread_mutex_init (&num_can_eat_lock, NULL);

	for (i = 0; i < num_philo; i++)
	{
		pthread_mutex_init (&m_fork[i], NULL);
		Spawn(&time_basic, &m_fork, i);
	}
	for (i = 0; i < num_philo; i++)
		pthread_join (philo[i], NULL);
	return (0);
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

void *philosopher (void *arg)
{
	t_philosophers *philo;
	pthread_t thread;
	int i;
	int left_fork;
	int right_fork;

	philo = arg;
	printf ("Philosopher %d is done thinking and now ready to eat. \n", philo->pos);
	right_fork = philo->pos;
	left_fork = philo->pos + 1;
	i = 0;

	// Wrap around the forks.
	if (left_fork == num_philo)
		left_fork = 0;
	pthread_create(&thread, NULL, life_cycle, philo);
	//while (i != food_limit)
	while (i = food_on_table())
	{
		eat(philo, left_fork, right_fork);
		pthread_create(&thread, NULL, life_cycle, philo);
		//if (food_limit > 0)
		//{
		//	if (++i >= food_limit)
		//	{
		//		printf("food count fulfilled\n");
		//		break;
		//	}
		//}
		usleep(sleep_seconds * 1000);
		philo->state = 1;
	}

	printf ("Philosopher %d is done eating. \n", philo->pos);

	if (++*philo->death >= num_philo)
	{
		printf("All philosophers died!\n");
		exit (0);
	}
	pthread_exit(0);
	return (NULL);
}

void Spawn(long *time, pthread_mutex_t **m_fork, int i)
{
	t_philosophers *philo;
	pthread_t thread;
	static int death;

	philo = (t_philosophers *)malloc(sizeof(t_philosophers));
	philo->pos = i;
	philo->eat_count = food_limit;
	philo->state = 1; //thinking
	philo->death = &death;
	philo->time = time;

	pthread_create(&thread, NULL, philosopher, philo);
}

void eat(t_philosophers *philo, int left, int right)
{
	int id;

	id = philo->pos;
	philo->state = 2;
	get_token ();

	grab_fork (id, right, "right");
	grab_fork (id, left, "left");

	printf ("Philosopher %d: eating\n", id);
	usleep (sleep_seconds * 1000);
	down_forks (left, right);
	return_token ();
}

//void eat(int id, int left, int right)
//{
//	get_token ();

//	grab_fork (id, right, "right");
//	grab_fork (id, left, "left");

//	printf ("Philosopher %d: eating\n", id);
//	usleep (sleep_seconds * 1000);
//	down_forks (left, right);
//	return_token ();
//}

//void *philosopher (void *num)
//{
//	int id;
//	int i;
//	int left_fork;
//	int right_fork;
//	int f;

//	id = (int)num;
//	printf ("Philosopher %d is done thinking and now ready to eat. \n", id);
//	right_fork = id;
//	left_fork = id + 1;

//	// Wrap around the forks.
//	if (left_fork == num_philo)
//		left_fork = 0;

//	while (f = food_on_table())
//	{
//		eat(id, left_fork, right_fork);
//	}

//	printf ("Philosopher %d is done eating. \n", id);
//	return (NULL);
//}

//int food_on_table ()
//{
//	int myfood;

//	pthread_mutex_lock (&food_lock);
//	if (food_limit > 0)
//		food_limit--;
//	myfood = food_limit;
//	pthread_mutex_unlock (&food_lock);
//	return (myfood);
//}

void grab_fork (int phil, int c, char *hand)
{
	pthread_mutex_lock (&m_fork[c]);
	printf ("Philosopher %d: has taken %s fork %d\n", phil, hand, c);
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
		*elapsed = (((time2.tv_sec - time.tv_sec) * 1000 + ((time2.tv_usec - time.tv_usec) / 1000)));
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
		elapsed = (((time2.tv_sec - time.tv_sec) * 1000 + ((time2.tv_usec - time.tv_usec) / 1000)));
		// if timestamp over time to die, then die.
		if (die_seconds <= elapsed)
		{
			philo->current = *philo->time;
			philo->state = 3;
			printf("Philosopher %d dies\n", philo->pos);
			exit (0);
		}
		if (philo->state == 0)	// if eating state
			pthread_exit(NULL);
	}
}