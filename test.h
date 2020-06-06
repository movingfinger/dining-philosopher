/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sako <sako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/02 14:39:49 by sako              #+#    #+#             */
/*   Updated: 2020/06/03 16:12:12 by sako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
#define TEST_H

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <sys/time.h>

#define DELAY 5000

pthread_mutex_t *m_fork;			// mutex for fork
pthread_t *philo;					
pthread_mutex_t food_lock;
pthread_mutex_t num_can_eat_lock;	

long long num_philo = 4;			// number of philosophers
long long num_can_eat;				// num_philo - 1 for token system
int sleep_seconds = 5;				// time to sleep
int think_seconds = 5;				// time to sleep
int eat_seconds = 5;				// time to eat
int die_seconds = 5;				// time to die
int food_limit = 5;					// number of times each philosopher must eat. If -1, it means not defined.
int food_temp;						// counter for food left.

/*
** state 0: eating
** state 1: thinking
** state 2: sleeping
** state 3: died
*/

typedef struct	s_philosophers
{
	int			pos;
	int			eat_count;
	int			state;
	long		current;
	long		*time;
	int			*death;
}				t_philosophers;

//void *philosopher (void *id);
void *philosopher (void *arg);
void grab_fork (int, int, char *);
void down_forks (int, int);

int food_on_table ();
int get_token ();
void return_token ();
//void eat(int id, int left, int right);
void eat(t_philosophers *philo, int left, int right);
void *life_cycle(void *arg);
void *time_ct(void *var);
void Spawn(long *time, pthread_mutex_t **m_fork, int i);

#endif