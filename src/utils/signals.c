/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alappas <alappas@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 21:09:36 by vkozlova          #+#    #+#             */
/*   Updated: 2024/05/15 20:27:24 by alappas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	g_signal;

void	handle_signal(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_c;
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	rl_catch_signals = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTSTP, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGCHLD, &sa, NULL);
}

void	handle_c(int signo)
{
	int	sig;

	if (signo == SIGCHLD)
		g_signal = SIGCHLD;
	else if (signo == SIGINT)
	{
		if (write(1, "\n", 1) == -1)
			exit(1);
		wait(NULL);
		sig = g_signal;
		g_signal = SIGINT;
		if (sig == SIGCHLD)
			return ;
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signal = 0;
	}
	else if (signo == SIGQUIT)
		wait(NULL);
}

int	handle_d(t_data *data, char *line)
{
	if (line == NULL)
	{
		ft_putstr_fd("exit\n", STDERR_FILENO);
		free_data(data);
		exit(0);
	}
	else if (ft_strlen(line) == 0)
	{
		ft_strdel(&line);
		return (1);
	}
	return (0);
}

void	handle_sig_child(int signo)
{
	if (signo == SIGINT)
	{
		g_signal = SIGINT;
		exit(1);
	}
}
