/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/21 18:44:54 by hmartzol          #+#    #+#             */
/*   Updated: 2018/01/26 03:30:46 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lem_in.h>
#include <ft_getopt.h>

inline static void	help(char *name)
{
	ft_printf("\nusage: %s [-h --help] [-i --input <path>] [-v --verbose]\n"
	"-h --help:           show this help\n"
	"-i --input <path>:   specify a file from which to load the rooms\n"
	"-v --verbose:        print additional information\n",
		name);
	exit(0);
}

inline static void	read_opts(int argc, char **argv, t_env_lem_in *e)
{
	t_getopt_env		ge;
	size_t				r;
	const t_getopt_opt	longopts[] = {{'h', "help", 0, 0},
									{'i', "input", 1, 0}};

	ge = ft_getopt_env("hi:v", longopts);
	while ((r = ft_getopt(argc, argv, &ge)) != -1)
		if (r == 'h')
			help(argv[0]);
		else if (r == 'v')
			e->verbose = 1;
		else if (r == 'i' && -1 == (e->fd = open(ge.optarg, O_RDONLY)))
			exit(0 & ft_printf("can't open file: %s\n", ge.optarg));
}

int					main(int argc, char **argv)
{
	static t_env_lem_in	env = {.fd = 0, .nb_ant = 0, .verbose = 0,

	.table = {0, 0, 0}, .path = NULL, .start = NULL, .end = NULL};
	ft_error(ERROR_SILENT_ON, NULL);
	read_opts(argc, argv, &env);
	parser(&env);
	bfs(&env);
	// printer(&env);
	debug(&env);
}
