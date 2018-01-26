/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lem_in.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/21 18:44:31 by hmartzol          #+#    #+#             */
/*   Updated: 2018/01/26 02:56:11 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEM_IN_H
# define LEM_IN_H

# include <libft.h>

typedef struct		s_link
{
	t_llist			*target;
	struct s_link	*next;
}					t_link;

typedef struct		s_room
{
	int				flags;
	int				passes;
	int				x;
	int				y;
	t_link			*links;
}					t_room;

typedef struct		s_path
{
	size_t			length;
	char			**labels;
	int				*ants;
	struct s_path	*next;
}					t_path;

typedef struct		s_env_lem_in
{
	int				fd;
	int				nb_ant;
	int				verbose;
	t_hashtable		table;
	t_path			*path;
	t_llist			*start;
	t_llist			*end;
}					t_env_lem_in;

# define START		1
# define END		2
# define BLACKLIST	4
# define PRINTER	8

void				parser(t_env_lem_in *env);
void				debug(t_env_lem_in *env);
void				printer(t_env_lem_in *env);
void				print_path_usage(t_env_lem_in *env);
void				bfs(t_env_lem_in *env);
void				error(int code, t_env_lem_in *env, char *format, ...);

#endif
