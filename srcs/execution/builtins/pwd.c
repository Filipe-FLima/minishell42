/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yulpark <yulpark@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 15:31:20 by yuleumpark        #+#    #+#             */
/*   Updated: 2025/04/18 19:27:06 by yulpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

//char *getcwd(char *buf, size_t size);

t_exec_error pwd(t_main_data *data)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		clean_all_data_exit(data, 1);
	printf("%s\n", pwd);
	free(pwd);
	return (SUCCEED);
}

//int main(void)
//{
//	char **args;
//	args = malloc(5 * sizeof(char*));
//	args[0] = "pwd";
//	pwd(args);
//	free(args);
//	return (0);
//}
