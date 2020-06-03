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

pthread_mutex_t *chopstick;
pthread_t *philo;
pthread_mutex_t food_lock;
pthread_mutex_t num_can_eat_lock;

long long num_philo;
long long num_can_eat;
int sleep_seconds;
int food_limit;
int food_temp;

void *philosopher (void *id);
void grab_chopstick (int, int, char *);
void down_chopsticks (int, int);

int food_on_table ();
int get_token ();
void return_token ();

#endif