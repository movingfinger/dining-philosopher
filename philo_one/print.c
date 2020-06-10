/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sako <sako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/08 17:39:41 by sako              #+#    #+#             */
/*   Updated: 2020/06/08 23:11:07 by sako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void ft_putstr(const char *str)
{
	for (int i = 0; str[i]; i++)
		write(1, &str[i], 1);
}
void ft_putendl(const char *str)
{
	ft_putstr(str);
	ft_putstr("\n");
}

void ft_print_error(const char *str)
{
	ft_putendl(str);
	exit (0);
}

void ft_display_param(const char *str, long long num, const char *unit)
{
	ft_putstr(str);
	ft_putstr(" ");
	ft_putstr(ft_ltoa_base(num, 10));
	ft_putendl(unit);
}

void set_str(t_philosophers *philo)
{
	philo->len = ft_strlen(ft_ltoa_base(timestamp[philo->pos], 10)) + ft_strlen(ft_ltoa_base((long long)philo->pos, 10)) + 20;
	if (philo->state == 0)
		philo->len += 9;
	else if (philo->state == 1)
		philo->len += 11;
	else if (philo->state == 2)
		philo->len += 11;
	else if (philo->state == 3)
		philo->len += 20;
	else if (philo->state == 4)
		philo->len += 24;
	else if (philo->state == 5)
		philo->len += 17;
	else if (philo->state == 6)
		philo->len += 18;
	else if (philo->state == 7)
		philo->len += 10;
	philo->print_str = (char *)malloc(sizeof(char) * 2);
	memset(philo->print_str,'\0', 2);
}

void concat_str(t_philosophers *philo)
{
	char *time;
	char *id;
	
	set_str(philo);
	time = ft_ltoa_base(timestamp[philo->pos], 10);
	id = ft_ltoa_base(philo->pos, 10);
	ft_strlcat(philo->print_str, time, philo->len);
	ft_strlcat(philo->print_str, " ms - Philosopher ", philo->len);
	ft_strlcat(philo->print_str, id, philo->len);
	if (philo->state == 0)
		ft_strlcat(philo->print_str, "is eating\n", philo->len);
	else if (philo->state == 1)
		ft_strlcat(philo->print_str, "is thinking\n", philo->len);
	else if (philo->state == 2)
		ft_strlcat(philo->print_str, "is sleeping\n", philo->len);
	else if (philo->state == 3)
		ft_strlcat(philo->print_str, "died due to timeover\n", philo->len);
	else if (philo->state == 4)
		ft_strlcat(philo->print_str, "died due to lack of food\n", philo->len);
	else if (philo->state == 4)
		ft_strlcat(philo->print_str, "food left infinite\n", philo->len);
	ft_putstr(philo->print_str);
	free(philo->print_str);
}