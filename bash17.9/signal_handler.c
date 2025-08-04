/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 00:00:00 by mertilhan13       #+#    #+#             */
/*   Updated: 2025/08/02 06:48:00 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <signal.h>

volatile sig_atomic_t g_signal_received = 0;

void handle_sigint(int signum)
{
    (void)signum;
    g_signal_received = SIGINT;
    
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void setup_interactive_signals(void)
{
    struct sigaction sa_int;
    
    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL);
}

void setup_child_signals(void)
{
    signal(SIGINT, SIG_DFL); 
    signal(SIGQUIT, SIG_DFL); 
}

void check_and_handle_signal(int *last_exit_status)
{
    if(g_signal_received == SIGINT)
    {
        g_signal_received = 0;
        *last_exit_status = 130; 
    }
}
