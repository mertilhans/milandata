/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_extra.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husarpka <husarpka@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 17:27:54 by merilhan          #+#    #+#             */
/*   Updated: 2025/08/19 12:24:46 by husarpka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	ft_tokenize_control(char *input)
{
	char	*current;

	current = input;
	while (*current)
	{
		if (*current == '|')
		{
			if (current == input || *(current + 1) == '\0')
				return (1);
		}
		current++;
	}
	return (0);
}
