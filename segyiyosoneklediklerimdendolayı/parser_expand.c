#include "shell.h"

void	expand_argv_capacity(t_parser *cmd, int *argc)
{
	char	**new_argv;
	int		i;

	cmd->argv_cap *= 2;
	new_argv = gb_malloc(sizeof(char *) * cmd->argv_cap);
	if (!new_argv)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (i < *argc)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	gc_free(cmd->argv);
	cmd->argv = new_argv;
}

void	cleanup_split_args(char **split_args)
{
	int	j;

	if (!split_args)
		return ;
	j = 0;
	while (split_args[j])
	{
		gc_free(split_args[j]);
		j++;
	}
	gc_free(split_args);
}

void	add_split_args(t_parser *cmd, int *argc, char **split_args)
{
	int	j;

	j = 0;
	while (split_args && split_args[j])
	{
		if (*argc + 1 >= cmd->argv_cap)
			expand_argv_capacity(cmd, argc);
		cmd->argv[*argc] = ft_strdup(split_args[j]);
		if (!cmd->argv[*argc])
		{
			perror("strdup failed for split arg");
			exit(EXIT_FAILURE);
		}
		(*argc)++;
		j++;
	}
}

void	handle_split_expansion(t_parser *cmd, int *argc, char *o, char *e)
{
	char	**split_args;

	(void)o;
	split_args = split_expanded_string(e);
	add_split_args(cmd, argc, split_args);
	cleanup_split_args(split_args);
	if (e)
		gc_free(e);
}

void	handle_normal_expansion(t_parser *cmd, int *argc, t_token *t, char *e)
{
	if (e)
		cmd->argv[*argc] = e;
	else
	{
		cmd->argv[*argc] = ft_strdup(t->value);
		if (!cmd->argv[*argc])
		{
			perror("strdup failed for argv element");
			exit(EXIT_FAILURE);
		}
	}
	(*argc)++;
}