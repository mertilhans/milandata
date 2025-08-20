/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husarpka <husarpka@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 18:36:02 by merilhan          #+#    #+#             */
/*   Updated: 2025/08/20 22:35:25 by husarpka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <signal.h>

void	handle_parent_sigint(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	set_last_exit_status(130);
}

void	handle_parent_sigquit(int signum)
{
	(void)signum;
	write(1, "Quit (core dumped)\n", 19);
	set_last_exit_status(131);
}

void	setup_parent_execution_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	// Parent process için SIGINT handler
	sa_int.sa_handler = handle_parent_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);

	// Parent process için SIGQUIT handler  
	sa_quit.sa_handler = handle_parent_sigquit;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_child_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	// Child process için default signal handler'ları
	sa_int.sa_handler = SIG_DFL;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);

	sa_quit.sa_handler = SIG_DFL;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}
