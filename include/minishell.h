/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alappas <alappas@student.42wolfsburg.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 21:09:35 by vkozlova          #+#    #+#             */
/*   Updated: 2024/05/15 20:26:11 by alappas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <sys/stat.h>
# include <stdlib.h>
# include <termios.h>
# include <dirent.h> 
# include <sys/wait.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <termcap.h>
# include <unistd.h>
# include <stdint.h>
# include <limits.h>
# include <errno.h>
# include <stdbool.h>
# include "../libft/include/libft.h"

typedef enum e_token_type
{
	T_WORD = 1,
	T_NEWLINE,
	T_RED_INP,
	T_RED_OUT,
	T_APPEND,
	T_PIPE,
	T_DELIM,
}	t_token_type;

typedef struct s_pipe_list
{
	int					fd[2];
	struct s_pipe_list	*next;
	struct s_pipe_list	*prev;
}	t_pipe_list;

typedef struct s_envir
{
	char			*var_name;
	char			*var_value;
	struct s_envir	*next;
	struct s_envir	*prev;
}	t_envir;

typedef struct s_token
{
	t_token_type		type;
	char				*word;
	struct s_token		*next;
	struct s_token		*prev;
}	t_token;

typedef struct s_redir
{
	t_token			*redir_token;
	t_token			*redir_word;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd_list
{
	char				*value;
	char				**args_array;
	t_redir				*redir_list;
	t_redir				*in;
	t_redir				*out;
	int					fd_in;
	int					redir_status;
	struct s_cmd_list	*next;
	struct s_cmd_list	*prev;
}				t_cmd_list;

typedef struct s_data
{
	struct s_token	*token_list;
	t_envir			*env_list;
	t_envir			*export_list;
	int				exit_status;
	char			*input_minishell;
	char			*input_line;
	char			**env_array;
	char			**new_env_array;
	char			**path;
	t_cmd_list		*list;
	int				in;
	int				out;
}				t_data;

/* Exit functions */
void		exit_shell_no_free(char *message, int exit_code, t_data *data);
void		exit_shell_no_mes(int exit_code, t_data *data);
void		exit_shell(char *message, int exit_code, t_data *data);

/* Free and init functions */
void		start_loop(t_data *data);
void		save_envir(t_data *data, char **env_str);
char		**new_env_array(t_data *data);
void		init_data(t_data **data, char **envp);
void		reset_data(t_data *data);
void		free_pipe(t_pipe_list *pipes);
void		free_data(t_data *data);
void		free_2darray(char **array);
void		ft_envclear(t_envir **env_list);
void		free_list(t_cmd_list **list);
void		incr_shell_lvl(t_data *data);
void		shlvl_helper(t_data *data, t_envir *env_list,
				int level, t_envir *head);

/* Parsing functions */
int			lexical_analysis(t_data *data, char *input);
char		*find_command_path(t_data *data, t_cmd_list *list);
char		*find_executable_path(char **paths, char *cmd);

/* Signal handling */
int			handle_d(t_data *data, char *line);
void		handle_c(int signo);
void		handle_signal(void);
void		handle_sig_child(int signo);

/* Environment list functions */
t_envir		*create_env_list(char **envp);
void		print_envir(t_envir	*envir);
void		print_export(t_envir *envir);
t_envir		*swap_env(t_envir *first, t_envir *second);
t_envir		*create_export_list(char **envp);
t_envir		*sort_export(t_envir **export);
int			check_export_exist(t_data *data);
int			check_env_exist(t_data *data);
void		create_new_export(t_data *data, char **arg);

/* Environment list utils */
t_envir		*ft_envlast(t_envir *lst);
t_envir		*ft_envfirst(t_envir *lst);
t_envir		*ft_envnew(char **env_line);
void		ft_envadd_back(t_envir **env_list, t_envir *new);
t_envir		*find_env_node(t_envir *env_list, char *var_name);
char		*find_env_var(t_data *data, char *var_name);
char		*trim_input_env(char *input);
t_envir		*ft_envdelone(t_envir *env_list);
int			ft_lilen(t_envir *env_list);

/* Quotes */
int			odd_quote(char *str);
int			special_chars(char *str);
int			in_quotes(char *s, int pos);
int			in_single_quotes(char *s, int pos);

/* Tokens */

void		tokenise(t_data *data, char *str);
void		free_tokens(t_token **begin);
t_token		*create_token(t_data *data, int i, int len);
t_token		*create_arg_token(t_data *data, char *word, enum e_token_type type);
void		add_token(t_token **token, t_token *new);
void		add_redir_token(t_redir **head, t_redir *redir);
t_redir		*create_redir_token(t_token *redir_token, t_token *word);
int			token_analysis(t_data *data);
void		set_token_type(t_token *token);
void		print_tokens(t_data *data);
void		find_append(t_token *current);
void		find_delim(t_token *current);
void		find_redir(t_token **head);

/* Error handling */
int			check_delim(t_token *token);
int			check_append(t_token *token);
char		*check_first_token(char *str, int *i);
char		*check_token(char *str, int *i);
int			syntax_errors(t_token *token, t_data *data);
int			check_red(t_token *token);
int			check_pipe(t_token *token);

/* List handling functions */
void		init_list_data(t_data *data);
t_cmd_list	*create_node(t_token **start, t_token *current, t_cmd_list *list);
t_cmd_list	*create_last_node(t_token **start, t_cmd_list *list);
t_cmd_list	*get_last_cmd(t_cmd_list *list);

/* Builtin functions */
int			is_builtin(t_cmd_list *list);
int			call_builtin_func(t_data *data, t_cmd_list *list);
void		echo(t_cmd_list *list);
void		env(t_data *data);
void		unset(t_envir **env_list, t_data *data, int value);
void		export(t_envir **env_list, t_envir **export_list,
				t_cmd_list *list, t_data *data);
void		pwd(void);
void		cd(t_data *data, t_cmd_list *list);
void		exit_builtin(t_data *data, t_cmd_list *list);

/* Builtin functions utils*/

void		cd_home(t_data *data, t_envir *env_list);
void		cd_folder(t_data *data, t_envir *env_list, char *pwd);

/* Pipe handling functions */

void		exec_pipe(t_data *data, t_cmd_list *list);
void		exec_pipes(t_data *data);
t_pipe_list	*create_pipes(t_cmd_list *list);
t_pipe_list	*get_last_pipe(t_pipe_list *pipes);
void		redir_input_to_pipe(int fd, t_data *data);
void		redir_output_to_pipe(int fd, t_data *data);
void		exec_last_cmd(t_data *data, t_cmd_list *list);
void		exec_cmd(t_data *data, t_cmd_list *list);

/* Heredoc/Delim handling functions */

void		delim(char *name, t_data *data, t_cmd_list *list);
void		read_input_delim(char *name, int stdin, int stdout, int fd);
void		manage_delim(t_cmd_list *list, t_data *data);

/* Redirection handling functions */
int			manage_redir(t_cmd_list *list, t_data *data);
void		redir_input(t_cmd_list *list, t_data *data);
void		redir_output(t_redir *redir, t_data *data);
void		redir_input_output(t_cmd_list *list, t_data *data);
int			set_redir_status(t_cmd_list *list);
void		return_in_out(t_data *data);
void		close_fds(t_pipe_list **first_pipe,
				t_pipe_list *in, t_pipe_list *out);

/* String handling functions */

char		*replace_dollars(t_data *data, char *str);
char		*trim_input(char *input);
void		process_input(char *input, char *str, int *i, int *j);
char		*put_str_to_str(char *dest, char *src, t_data *data);
void		put_to_stderr(char *dest, char *src, t_data *data, int err);
void		put_to_stderr_and_exit(char *dest, char *src,
				t_data *data, int err);
void		ft_putstr_fd(char *s, int fd);
char		*ft_strstr(const char *haystack, const char *needle);
int			is_chr_str(char c, char *str);
char		**ft_singlesplit(char const *str, char c);

/* Builtin utils*/

void		check_env(t_data *data, t_envir **env_list, char **arg);
void		check_export(t_data *data, t_envir **export_list,
				t_envir **head_export, char **arg);
void		check_export_null(t_envir *head_export, t_envir **env_list,
				t_envir **export_list, char **arg);
int			unset_helper(char *list_arg, int value, t_data *data);
int			export_helper(char *list_arg, t_data *data);

void		fix_quotes(t_data *data);

extern int	g_signal;

#endif