#include "shell.h"
static char	*realloc_res(char *res, int *res_len, int *res_cap)
{
	char	*new_res;

	if (*res_len + 1 >= *res_cap)
	{
		*res_cap = *res_len + 2;
		new_res = gb_malloc(*res_cap);
		if (!new_res)
			return (res);
		if (res)
		{
			ft_memcpy(new_res, res, *res_len);
			gc_free(res);
		}
		else
			new_res[0] = '\0';
		res = new_res;
	}
	return (res);
}

char	*push_char_res(char *res, char c, int *res_len, int *res_cap)
{
	res = realloc_res(res, res_len, res_cap);
	res[*res_len] = c;
	(*res_len)++;
	res[*res_len] = '\0';
	return (res);
}
static void	expansion_loop(char *str, t_builder *b, t_env *env_list)
{
	int		i;
	char	*expanded;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			expanded = handle_dollar(str, &i, env_list);
			if (expanded)
			{
				builder_append_str(b, expanded);
				gc_free(expanded);
			}
		}
		else
		{
			builder_push_char(b, str[i]);
			i++;
		}
	}
}

char	*expand_variables(char *str, t_env *env_list)
{
	t_builder	builder;

	if (!str)
		return (NULL);
	builder_init(&builder, ft_strlen(str) + 1);
	if (!builder.str)
		return (NULL);
	expansion_loop(str, &builder, env_list);
	return (builder.str);
}

char	*handle_simple(const char *str, int *i, t_env *env_list)
{
	int		len;
	char	*var_name;
	t_env	*env_var;
	char	*result;

	(*i)++;
	len = 0;
	while (is_valid_char(str[*i + len]))
		len++;
	if (len == 0)
		return (ft_strdup(""));
	var_name = gb_malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_memcpy(var_name, str + *i, len);
	var_name[len] = '\0';
	*i += len;
	env_var = find_env(env_list, var_name);
	if (env_var == NULL || env_var->value == NULL)
		result = ft_strdup("");
	else
		result = ft_strdup(env_var->value);
	gc_free(var_name);
	return (result);
}
