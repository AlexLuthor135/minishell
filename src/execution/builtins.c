/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alappas <alappas@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 16:32:07 by alappas           #+#    #+#             */
/*   Updated: 2024/05/15 20:19:00 by alappas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_builtin(t_cmd_list *list)
{
	if (list && list->value)
	{
		if (ft_strcmp(list->value, "echo") == 0)
			return (1);
		else if (ft_strcmp(list->value, "cd") == 0)
			return (1);
		else if (ft_strcmp(list->value, "pwd") == 0)
			return (1);
		else if (ft_strcmp(list->value, "export") == 0)
			return (1);
		else if (ft_strcmp(list->value, "unset") == 0)
			return (1);
		else if (ft_strcmp(list->value, "env") == 0)
			return (1);
		else if (ft_strcmp(list->value, "exit") == 0)
			return (1);
	}
	return (0);
}

int	call_builtin_func(t_data *data, t_cmd_list *list)
{
	if (is_builtin(list))
	{
		if (ft_strcmp(list->value, "echo") == 0)
			echo(list);
		else if (ft_strcmp(list->value, "cd") == 0)
			cd(data, list);
		else if (ft_strcmp(list->value, "pwd") == 0)
			pwd();
		else if (ft_strcmp(list->value, "export") == 0)
			export(&(data->env_list), &(data->export_list), list, data);
		else if (ft_strcmp(list->value, "unset") == 0)
		{
			unset(&(data->env_list), data, 0);
			unset(&(data->export_list), data, 1);
		}
		else if (ft_strcmp(list->value, "env") == 0)
			env(data);
		else if (ft_strcmp(list->value, "exit") == 0)
			exit_builtin(data, list);
		return (1);
	}
	return (0);
}

void	env(t_data *data)
{
	print_envir(data->env_list);
}

void	pwd(void)
{
	char	*cwd;

	cwd = NULL;
	cwd = getcwd(cwd, 1);
	printf("%s\n", cwd);
	free(cwd);
}
