/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:45:11 by yulpark           #+#    #+#             */
/*   Updated: 2025/04/01 23:49:53 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../execution.h"

// add the part that checks with the environment variables
int echo(char **args, t_env_var *envp)
{
	int n;
	int i;

	n = 0;
	i = 1;
	if (args[1][0] == '-' && args[1][1] == 'n' && args[1][2] == '\0')
	{
		n = -1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		i++;
		if (args[i])
			ft_putchar_fd(' ', 1);
	}
	if (n == 0)
		ft_putchar_fd('\n', 1);
	return (0);
}

//int main(void)
//{
//	char **args;
//	args = malloc(5 * sizeof(char*));
//	args[0] = "echo";
//	args[1] = "-n";
//	args[2] = "test";
//	args[3] = "test";
//	args[4] = NULL;
//	echo(args);
//	free(args);
//	return (0);
//}

