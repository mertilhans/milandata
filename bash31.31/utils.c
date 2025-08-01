/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: merilhan <merilhan@42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 04:24:43 by husarpka          #+#    #+#             */
/*   Updated: 2025/08/01 02:17:36 by merilhan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int ft_atoi(char *str)
{
	int sign;
	int result;
	
	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}	
	while (*str >= '0' && *str <= '9')
	{	
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);

}

void ft_memcpy(char *s1,char *s2, int len)
{
    while(len--)
        *s1++=*s2++;
    *s1 = '\0';
}
int is_quote(char c)
{
    return (c == '\'' || c == '"');
}
int ft_strlen(char *s)
{
    int i = 0;
    while(s[i] && s[i++])
        ;
    return(i);
}

int ft_strcmp(const char *s1, const char *s2)
{
	int i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] == s2[i])
			i++;
		else 
			return (s1[i] - s2[i]);
	}
	return (s1[i] - s2[i]);
}
//void *gc_malloc(size_t size)
//{
//    void *ptr;
//    if (size == 0)
//        return (NULL);
//    ptr = malloc(size);
//    if (!ptr)
//        return (NULL);
//}

char *ft_strdup(char *str)
{
    char *res;
    char *result;

    res = gb_malloc(strlen(str) + 1);
    result = res;
    
    while(*str)
        *res++ = *str++;
    *res = '\0';
 return (result);   
}
//void add_node(void *value,size_t size)
//{
//    struct s_node *res = gc_malloc(sizeof(t_node));
//    if(!res)
//        return(NULL);
    
//}

