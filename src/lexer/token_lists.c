/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lists.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alappas <alappas@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 16:31:44 by alappas           #+#    #+#             */
/*   Updated: 2023/12/13 16:31:47 by alappas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_token(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!new || !head)
		return ;
	if (*head)
	{
		tmp = *head;
		while (tmp->next)
		{
			tmp->next->prev = tmp;
			tmp = tmp->next;
		}
		tmp->next = new;
		new->prev = tmp;
		new->next = NULL;
	}
	else
	{
		*head = new;
		new->prev = NULL;
		new->next = NULL;
	}
}

t_redir	*create_redir_token(t_token *redir_token, t_token *word)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	redir->redir_token = redir_token;
	redir->redir_word = word;
	return (redir);
}

void	add_redir_token(t_redir **head, t_redir *redir)
{
	t_redir	*tmp;

	if (!redir || !head)
		return ;
	if (*head)
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = redir;
		redir->next = NULL;
	}
	else
	{
		*head = redir;
		redir->next = NULL;
	}
}
