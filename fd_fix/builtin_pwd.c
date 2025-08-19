/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husarpka <husarpka@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 05:48:36 by merilhan          #+#    #+#             */
/*   Updated: 2025/08/19 15:50:16 by husarpka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <stdlib.h>

static void	handle_pwd_write_error(void)
{
	ft_putendl_fd("write", 2);
	set_last_exit_status(1);
}

static void	print_pwd(const char *cwd)
{
	if (write(STDOUT_FILENO, cwd, ft_strlen(cwd)) == -1)
		handle_pwd_write_error();
	else if (write(STDOUT_FILENO, "\n", 1) == -1)
		handle_pwd_write_error();
	else
		set_last_exit_status(0);
}

void	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		print_pwd(cwd);
		free(cwd);
	}
	else
	{
		ft_putendl_fd("pwd", 2);
		set_last_exit_status(1);
	}
}
