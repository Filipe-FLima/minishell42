/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flima <flima@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:08:13 by filipe            #+#    #+#             */
/*   Updated: 2025/03/28 21:47:06 by flima            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZATION_H
# define TOKENIZATION_H

# include "minishell.h"

# define METACHARS " \t\n|<>$\"\'&"

//split the cmd line into token using these syntax
typedef enum e_syntax
{
	SPACE_CHAR,
	TAB_CHAR,// \t (tab) 
	NEW_LINE,// \n (Newline, indicating end of command or separator)
	PIPE,// | (Pipe)
	LESS,// < (Output redirection)
	GREAT,// > (Input redirection)
	VARIABLE,// $ (Variable expansion)
	D_QUOTE,// "" (Double quotes for string)
	S_QUOTE,// '' (Single quotes for literal string)
	AND,// & (AND operator)
	WORD,// CMD, CMD_ARG or generic string value
	APPEND,// >> (Append to output)
	HEREDOC,// << (Here-document)
}	t_syntax;

typedef enum e_parsing_err //modificar error codes
{
	SUCCESS,
	ERROR_MEM_ALLOC,
	UNCLOSED_QUOTE,
	ERROR_AND_OPERATOR,
	ERROR_CMD_SUBSTITUTION,
	ERROR_MTCH_END,
	ERROR_PIPE_END,
	ERROR_UNEXPEC_PIPE,
	ERROR_UNEXPEC_LESS,
	ERROR_UNEXPEC_GREAT,
	ERROR_UNEXPEC_APPEND,
	ERROR_UNEXPEC_HERDOC,
	ERROR_UNEXPEC_PARENTH,
}	t_pars_err;

typedef enum e_redir_type
{
	REDIR,
	IN,
	HERE_D,
	STDOUT,
	APP
}	t_redir_type;

typedef struct s_env_var
{
	char				*variable;
	struct s_env_var	*next;
}						t_env_var;

typedef struct s_redir
{
	t_redir_type	redir_type;
	int				fd;
	char			*file;
}					t_redir;

typedef struct s_command
{
	char				**args;
	t_redir				*redir;
	int					last_in;
	int					last_out;
	struct s_command	*next;
}						t_command;

typedef struct s_token
{
	char			*value;
	enum e_syntax	type;
	struct s_token	*next;
}					t_token;

//Jump table tokenization 

//Tokanization
t_pars_err	tokenize_input(t_main_data *data, char	*input);
t_token		*add_new_token(void);
void		add_token_back(t_token **head, t_token *new);
t_token		*last_token(t_token *list);
t_syntax	get_token_type(char cha);

//environment variables functions
t_env_var	*add_new_var(void);
void		add_var_back(t_env_var **head, t_env_var *new);
void		duplicate_env_var(t_main_data *data, char **env);
char	*find_environment_var(t_main_data *data, t_env_var *envp, \
	const char *var_name);

//lexer functions
typedef t_pars_err	(*t_lex_functions)(char *str, size_t *i, t_syntax *type);
t_pars_err	process_single_quotes(char *input, size_t *i, t_syntax *tok_type);
t_pars_err	process_double_quotes(char *input, size_t *i, t_syntax *tok_type);
t_pars_err	process_word_n_spaces(char *input, size_t *i, t_syntax *tok_type);
t_pars_err	process_pipe(char *input, size_t *i, t_syntax *tok_type);
t_pars_err	process_great_n_herdoc(char *input, size_t *i, t_syntax *tok_type);
t_pars_err	process_less_n_append(char *input, size_t *i, t_syntax *tok_type);
t_pars_err	process_env_var(char *input, size_t *i, t_syntax *tok_type);

//syntax functions
typedef t_pars_err	(*t_syntax_check)(t_token *previous, t_token *current);
t_pars_err	syntax(t_main_data *data);
t_token		*skip_blank_nodes_n_get_next(t_token *tokens, int flag);
t_pars_err	syntax_pipe(t_token *previous, t_token *current);
t_pars_err	syntax_great(t_token *previous, t_token *current);
t_pars_err	syntax_append(t_token *previous, t_token *current);
t_pars_err	syntax_less(t_token *previous, t_token *current);
t_pars_err	syntax_heredoc(t_token *previous, t_token *current);

//append functions
t_pars_err	merge_tokens_n_rm_blank_tokens(t_main_data *data);
void		remove_next_token(t_token *current);

// heredoc functions
t_pars_err	capture_heredocs(t_main_data *data);
void		hered_err_exit(t_main_data *data, t_exit_code status, char *msg);
void		heredoc_reading(t_main_data *data, char *file_name, char *delim);
//debugging
void		debugging(t_main_data *data);

//free functions
void		init_data(t_main_data *data);
void		free_tokens(t_token *tokens);
void		clean_temp_data(t_main_data *data);
void		free_env_vars(t_env_var *head);
void		clean_all_data_exit(t_main_data *data, int out_status);

//error handlers
// typedef void (*t_handle_error)(t_main_data *data, t_pars_err status);
void		status_error(t_main_data *data, t_pars_err status);
void		error_msg(char *msg);
void		status_error_syntax(t_main_data *data, t_pars_err status);
void		assign_error_table_msg(char **table);

#endif
