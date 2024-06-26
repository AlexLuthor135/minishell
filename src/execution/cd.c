/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alappas <alappas@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 16:32:18 by alappas           #+#    #+#             */
/*   Updated: 2024/05/15 20:09:20 by alappas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	no_dir_error(int err, char *dir, t_data *data)
{
	char	*str;

	str = ft_strjoin("cd: ", dir);
	perror(str);
	free(str);
	data->exit_status = err;
}

void	cd_no_arg(t_data *data)
{
	t_envir	*home;

	home = find_env_node(data->env_list, "HOME");
	if (home == NULL)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		data->exit_status = 1;
	}
	else if (chdir(home->var_value) == -1)
		no_dir_error(errno, home->var_value, data);
	else
	{
		cd_home(data, data->env_list);
		cd_home(data, data->export_list);
	}
}

void	cd(t_data *data, t_cmd_list *list)
{
	char	*pwd;

	pwd = NULL;
	pwd = getcwd(pwd, 1);
	if (!(list->args_array[1]))
		cd_no_arg(data);
	else if (chdir(list->args_array[1]) == -1)
		no_dir_error(errno, list->args_array[1], data);
	else
	{
		cd_folder(data, data->env_list, pwd);
		cd_folder(data, data->export_list, pwd);
	}
	free(pwd);
}

void	cd_home(t_data *data, t_envir *env_list)
{
	t_envir	*head;

	head = env_list;
	if (find_env_node(env_list, "OLDPWD"))
	{
		env_list = find_env_node(env_list, "OLDPWD");
		free(env_list->var_value);
		env_list->var_value = ft_strdup(find_env_var(data, "PWD"));
		env_list = head;
	}
	if (chdir(find_env_var(data, "HOME")) == -1)
		no_dir_error(errno, find_env_var(data, "HOME"), data);
	if (find_env_node(env_list, "PWD"))
	{
		env_list = find_env_node(env_list, "PWD");
		free(env_list->var_value);
		env_list->var_value = ft_strdup(find_env_var(data, "HOME"));
		env_list = head;
	}
}

void	cd_folder(t_data *data, t_envir *env_list, char *pwd)
{
	t_envir	*head;
	char	*cur_pwd;

	head = env_list;
	cur_pwd = NULL;
	if (find_env_node(data->env_list, "OLDPWD"))
	{
		env_list = find_env_node(env_list, "OLDPWD");
		free(env_list->var_value);
		env_list->var_value = ft_strdup(pwd);
		env_list = head;
	}
	if (find_env_node(data->env_list, "PWD"))
	{
		env_list = find_env_node(env_list, "PWD");
		free(env_list->var_value);
		cur_pwd = getcwd(cur_pwd, 1);
		env_list->var_value = cur_pwd;
		env_list = head;
	}
}
