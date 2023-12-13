/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenising.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alappas <alappas@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 16:32:47 by alappas           #+#    #+#             */
/*   Updated: 2023/12/13 16:32:50 by alappas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// printing the tokens to debug
void	print_tokens(t_data *data)
{
	t_token	*tmp;

	tmp = data->token_list;
	while (tmp)
	{
		printf("\nword:%s:type:%d\n", tmp->word, tmp->type);
		tmp = tmp->next;
	}
}

t_token	*create_token(t_data *data, int i, int len)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		exit_shell_no_mes(1, data);
	new->word = ft_substr(data->input_line, i - len, len);
	new->type = T_WORD;
	return (new);
}

t_token	*create_arg_token(t_data *data, char *word, enum e_token_type type)
{
	t_token	*new;

	new = ft_calloc(1, sizeof(t_token));
	if (!new)
		exit_shell_no_mes(1, data);
	new->word = ft_strdup(word);
	new->type = type;
	return (new);
}
