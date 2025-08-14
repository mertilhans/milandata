#include "shell.h"

void	builder_init(t_builder *builder, int initial_cap)
{
	builder->cap = initial_cap;
	if (builder->cap <= 0)
		builder->cap = 16;
	builder->str = gb_malloc(builder->cap);
	if (!builder->str)
	{
		builder->cap = 0;
		return ;
	}
	builder->str[0] = '\0';
	builder->len = 0;
}

int	builder_push_char(t_builder *builder, char c)
{
	char	*new_str;

	if (builder->len + 1 >= builder->cap)
	{
		builder->cap *= 2;
		new_str = gb_malloc(builder->cap);
		if (!new_str)
			return (0);
		ft_memcpy(new_str, builder->str, builder->len);
		gc_free(builder->str);
		builder->str = new_str;
	}
	builder->str[builder->len] = c;
	builder->len++;
	builder->str[builder->len] = '\0';
	return (1);
}

int	builder_append_str(t_builder *builder, char *to_add)
{
	int		add_len;
	char	*new_str;

	add_len = ft_strlen(to_add);
	if (builder->len + add_len >= builder->cap)
	{
		builder->cap = builder->len + add_len + 1;
		new_str = gb_malloc(builder->cap);
		if (!new_str)
			return (0);
		ft_memcpy(new_str, builder->str, builder->len);
		gc_free(builder->str);
		builder->str = new_str;
	}
	ft_memcpy(builder->str + builder->len, to_add, add_len);
	builder->len += add_len;
	builder->str[builder->len] = '\0';
	return (1);
}