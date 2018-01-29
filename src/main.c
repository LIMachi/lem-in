/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/21 18:44:54 by hmartzol          #+#    #+#             */
/*   Updated: 2018/01/29 04:00:28 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lem_in.h>
#include <ft_getopt.h>

inline static void	help(char *name)
{
	ft_printf("\nusage: %s [-h --help] [-i --input <path>] [-v --verbose] [-p -"
	"-path] [-h --hide-comments] [-l --ignore-double-link]\n"
	"-h --help:               show this help\n"
	"-i --input <path>:       specify a file from which to load the rooms\n"
	"-v --verbose:            print additional information\n"
	"-p --path:               print all the path found from ##start to ##end\n"
	"-u --hide-comments:      remove all comments and invalid commands\n"
	"-l --ignore-double-link: ignore identical links instead of throwing an err"
	"or\n", name);
	exit(0);
}

inline static void	read_opts(int argc, char **argv, t_env_lem_in *e)
{
	t_getopt_env		ge;
	size_t				r;
	const t_getopt_opt	longopts[] = {{'h', "help", 0, 0}, {'i', "input", 1, 0},
		{'v', "verbose", 0, 0}, {'p', "path", 0, 0},
		{'u', "hide-comments", 0, 0}, {'l', "ignore-double-link", 0, 0}};

	ge = ft_getopt_env("hvpuli:", longopts);
	while ((r = ft_getopt(argc, argv, &ge)) != -1)
		if (r == 'h')
			help(argv[0]);
		else if (r == 'v')
			e->verbose = 1;
		else if (r == 'p')
			e->print_paths = 1;
		else if (r == 'u')
			e->clean_output = 1;
		else if (r == 'l')
			e->ignore_double_link = 1;
		else if (r == 'i' && -1 == (e->fd = open(ge.optarg, O_RDONLY)))
			exit(0 & ft_printf("can't open file: %s\n", ge.optarg));
}

int					main(int argc, char **argv)
{
	static t_env_lem_in	env = {.fd = 0, .nb_ant = 0, .verbose = 0, .end = NULL,

	.print_paths = 0, .table = {0, 0, 0}, .path = NULL, .start = NULL,
	.clean_output = 0, .ignore_double_link = 0};
	ft_error(ERROR_SILENT_ON, NULL);
	read_opts(argc, argv, &env);
	if ((env.table = ft_hashtable(25600, NULL)).data == NULL)
		error(11, &env, "failed to alocate the table of rooms\n");
	parser(&env, 0, NULL);
	bfs(&env);
	if (env.clean_output)
		printer(&env);
	if (env.print_paths)
		print_paths_found(&env);
	print_path_usage(&env);
}
