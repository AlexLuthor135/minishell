/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alappas <alappas@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 16:32:01 by alappas           #+#    #+#             */
/*   Updated: 2023/12/13 16:32:03 by alappas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	find_redir(t_token **head)
{
	t_token	*current;
	t_token	*tmp;

	current = *head;
	while (current != NULL)
	{
		tmp = current;
		if (tmp->type == T_RED_INP && tmp->next->type == T_RED_INP)
			find_delim(tmp);
		else if (tmp->type == T_RED_OUT && tmp->next->type == T_RED_OUT)
			find_append(tmp);
		current = current->next;
	}
}

void	find_delim(t_token *current)
{
	t_token	*tmp;
	t_token	*tmp2;

	tmp = current;
	if (tmp->type == T_RED_INP && tmp->next->type == T_RED_INP)
	{
		tmp->type = T_DELIM;
		ft_strdel(&tmp->word);
		tmp->word = ft_strdup("<<");
		tmp2 = tmp->next;
		tmp->next = tmp->next->next;
		tmp->next->prev = tmp;
		ft_strdel(&tmp2->word);
		free(tmp2);
	}
}

void	find_append(t_token *current)
{
	t_token	*tmp;
	t_token	*tmp2;

	tmp = current;
	if (tmp->type == T_RED_OUT && tmp->next->type == T_RED_OUT)
	{
		tmp->type = T_APPEND;
		ft_strdel(&tmp->word);
		tmp->word = ft_strdup(">>");
		tmp2 = tmp->next;
		tmp->next = tmp->next->next;
		tmp->next->prev = tmp;
		ft_strdel(&tmp2->word);
		free(tmp2);
	}
}
