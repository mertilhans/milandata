/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: husarpka <husarpka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 03:35:53 by husarpka          #+#    #+#             */
/*   Updated: 2025/07/03 03:35:54 by husarpka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"



void	tokenizer_free(t_tokenizer *tokenizer)
{
	if(!tokenizer)
		return;
	if(tokenizer->input)
		free(tokenizer->input);
	free(tokenizer);
}
void	lexer_advance(t_tokenizer *tokenizer)
{
	if (tokenizer->pos < tokenizer->len - 1)
    {
        tokenizer->pos++;
        tokenizer->current = tokenizer->input[tokenizer->pos];
    }
    else
    {
        tokenizer->current = '\0';
    }
}
char lexer_peek(t_tokenizer *tokenizer)
{
	 if (tokenizer->pos + 1 < tokenizer->len)
        return (tokenizer->input[tokenizer->pos + 1]);
    return ('\0');
}
int	ft_ispace(char c)
{
	return(c == ' ' || c == '\t' || c == '\n' || c == '\r' );
}
void	ft_skip_space(t_tokenizer *tokenizer)
{
	while(tokenizer->current && ft_ispace(tokenizer->current))
		lexer_advance(tokenizer);
}

