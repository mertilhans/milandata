/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husarpka <husarpka@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:23:33 by husarpka          #+#    #+#             */
/*   Updated: 2025/08/19 12:28:21 by husarpka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <stdlib.h>

t_export	**get_export_list(void)
{
	static t_export	*g_export_list = NULL;

	return (&g_export_list);
}

void	unset_export_value(t_export **export_list, const char *key)
{
	t_export	*current;
	t_export	*prev;

	current = *export_list;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*export_list = current->next;
			return ;
		}
		prev = current;
		current = current->next;
	}
}

t_export	*init_export_from_env(t_env *env_list)
{
	t_export	*export_list;
	t_env		*current;

	export_list = NULL;
	current = env_list;
	while (current)
	{
		set_export_value(&export_list, current->key, current->value);
		current = current->next;
	}
	return (export_list);
}
