#include "shell.h"


int	is_valid_char(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9') || c == '_');
}


static char	*handle_special_vars(char *str, int *i)
{
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(get_last_exit_status()));
	}
	if (str[*i] == '$')
	{
		(*i)++;
		return (ft_itoa(getpid()));
	}
	return (NULL);
}

static char	*find_and_expand_var(char *str, int *i, t_env *env_list)
{
	char	*var_name;
	char	*expanded_val;
	int		start_i;
	int		len;

	start_i = *i;
	while (str[*i] && (isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	len = *i - start_i;
	if (len <= 0)
		return (ft_strdup(""));
	var_name = gb_malloc(len + 1);
	if (!var_name)
		return (ft_strdup(""));
	ft_memcpy(var_name, str + start_i, len);
	var_name[len] = '\0';
	expanded_val = find_env_value(env_list, var_name); //
	gc_free(var_name);
	if (expanded_val)
		return (ft_strdup(expanded_val));
	return (ft_strdup(""));
}

char	*handle_dollar(char *str, int *i, t_env *env_list)
{
	char	*expanded;

	(*i)++;
	if (!str[*i])
		return (ft_strdup("$"));
	expanded = handle_special_vars(str, i);
	if (expanded)
		return (expanded);
	if (isalpha(str[*i]) || str[*i] == '_')
		return (find_and_expand_var(str, i, env_list));
	return (ft_strdup("$"));
}