/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:36:02 by merilhan          #+#    #+#             */
/*   Updated: 2025/08/18 18:36:03 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	setup_parent_execution_signals(void)
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_child_signals(void)
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
