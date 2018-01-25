/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/25 04:10:31 by hmartzol          #+#    #+#             */
/*   Updated: 2018/01/25 05:42:51 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lem_in.h>

/*
** cas:
** première ligne lue: un int simple, le nombre de fourmis
** nom de salle: ne commence pas par # ou L, ne comporte pas de -, est suivi
** d un ' ' et est situé en début de ligne/après un -
** (bonus possible pour les noms de salles: les entourer de "" pour inclure des
** charactères interdit)
** position: serie de 2 ints arès un nom de salle séparés par un ' '
** commentaire: commence par '#'
** commande: commence par "##", est lié a la ligne suivante
** commandes invalides = commentaires, pas besoins de ligne suivante
** si ligne invalide ou erreur, stop parsing et commencer execution
** message d erreur "ERROR\n"
*/

inline static void	new_node(t_env_lem_in *env, char *line, char *t,
							int *last_line)
{
	t_room	*room;
	int		x;
	int		y;

	*t++ = '\0';
	if (ft_hashtable_get(&env->table, line))
		exit(1 & ft_printf("ERROR\n"));
	x = ft_strtoll(t, &t, 10);
	if (*t++ != ' ' || (!ft_isdigit(*t) && *t != '-' && *t != '+'))
		exit(2 & ft_printf("ERROR\n"));
	y = ft_strtoll(t, &t, 10);
	if (*t != '\n' && *t != '\0')
		exit(3 & ft_printf("ERROR\n"));
	if ((room = ft_malloc(sizeof(t_room))) == NULL)
		exit(4 & ft_printf("ERROR\n"));
	*room = (t_room){.flags = *last_line, .passes = 0, .x = x, .y = y, .links =
		NULL};
	*last_line = 0;
	if (ft_hashtable_insert(&env->table, line, room, sizeof(t_room)) ==
			(size_t)-1)
		exit(5 & ft_printf("ERROR\n"));
}

inline static void	new_link(t_env_lem_in *env, char *line, char *t)
{
	t_llist	*r1;
	t_llist	*r2;
	t_link	*tmp;

	*t++ = '\0';
	if ((r1 = (t_llist*)ft_hashtable_get(&env->table, line)) == NULL)
		exit(6 & ft_printf("ERROR\n"));
	if ((r2 = (t_llist*)ft_hashtable_get(&env->table, t)) == NULL)
		exit(7 & ft_printf("ERROR\n"));
	tmp = ((t_room*)r1->data)->links;
	while (tmp && tmp->target != r2)
		tmp = tmp->next;
	if (tmp)
		exit(8 & ft_printf("ERROR\n"));
	if ((tmp = ft_malloc(sizeof(t_link))) == NULL)
		exit(9 & ft_printf("ERROR\n"));
	*tmp = (t_link){.target = r2, .next = ((t_room*)r1->data)->links};
	((t_room*)r1->data)->links = tmp;
	if ((tmp = ft_malloc(sizeof(t_link))) == NULL)
		exit(10 & ft_printf("ERROR\n"));
	*tmp = (t_link){.target = r1, .next = ((t_room*)r2->data)->links};
	((t_room*)r2->data)->links = tmp;
}

inline static void	init(t_env_lem_in *env, char **line)
{
	char	*t;

	*line = NULL;
	if ((env->table = ft_hashtable(256, NULL)).data == NULL)
		exit(11 & ft_printf("ERROR\n"));
	if (get_next_line(env->fd, line) > 0)
		if ((env->nb_ant = ft_strtoll(*line, &t, 10)) < 1 || t == *line || *t)
			exit(12 & ft_printf("ERROR\n"));
	ft_memdel((void**)line);
}

void				parser(t_env_lem_in *env)
{
	char	*line;
	char	*t;
	int		last_line;

	init(env, &line);
	last_line = 0;
	while (get_next_line(env->fd, &line) > 0)
	{
		if (*line == '\n')
			continue ;
		else if (*line == 'L' || *line == '\0')
			break ;
		else if (*line == '#')
			if (!ft_strcmp(line + 1, "#start"))
				last_line |= 1;
			else
				last_line |= 2 * !ft_strcmp(line + 1, "#end");
		else if ((t = ft_strchr(line, ' ')))
			new_node(env, line, t, &last_line);
		else if ((t = ft_strchr(line, '-')))
			new_link(env, line, t);
		else
			exit(13 & ft_printf("ERROR\n"));
		ft_memdel((void**)&line);
	}
	ft_free(line);
}
