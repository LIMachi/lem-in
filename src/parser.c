/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/25 04:01:31 by hmartzol          #+#    #+#             */
/*   Updated: 2018/01/29 04:00:56 by hmartzol         ###   ########.fr       */
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

inline static void	new_node(t_env_lem_in *e, char *l, char *t,
							int *mask)
{
	t_room	*room;
	int		x;
	int		y;

	*t++ = '\0';
	if (ft_hashtable_get(&e->table, l))
		error(1, e, "duplicate room name '%s'\n", l);
	x = ft_strtoll(t, &t, 0);
	if (*t++ != ' ' || (!ft_isdigit(*t) && *t != '-' && *t != '+'))
		error(2, e, "invalid coordinates seperator for room '%s'\n", l);
	y = ft_strtoll(t, &t, 0);
	if (*t != '\n' && *t != '\0')
		error(3, e, "excess characters after coordinates for room '%s'\n", l);
	if ((room = ft_malloc(sizeof(t_room))) == NULL)
		error(4, e, "failed to alocate memory for room '%s'\n", l);
	*room = (t_room){.flags = *mask, .passes = 0, .x = x, .y = y, .links =
		NULL};
	if (ft_hashtable_insert(&e->table, l, room, sizeof(t_room)) ==
			(size_t)-1)
		error(5, e, "failed to add room '%s' to the table\n", l);
	if (*mask == START)
		e->start = ft_hashtable_get(&e->table, l);
	if (*mask == END)
		e->end = ft_hashtable_get(&e->table, l);
	*mask = 0;
}

inline static int	duplicate_link(t_env_lem_in *e, char *l, char *t)
{
	if (e->ignore_double_link)
		return (0);
	error(8, e, "duplicate link between room '%s' and '%s'\n", l, t);
	return (0);
}

inline static int	new_link(t_env_lem_in *e, char *l, char *t)
{
	t_llist	*r1;
	t_llist	*r2;
	t_link	*tmp;

	*t++ = '\0';
	if ((r1 = (t_llist*)ft_hashtable_get(&e->table, l)) == NULL)
		error(6, e, "failed to locate room '%s' in the table\n", l);
	if ((r2 = (t_llist*)ft_hashtable_get(&e->table, t)) == NULL)
		error(7, e, "failed to locate room '%s' in the table\n", t);
	if (r1 == r2)
		error(17, e, "trying to link room '%s' to itself\n", l);
	tmp = ((t_room*)r1->data)->links;
	while (tmp && tmp->target != r2)
		tmp = tmp->next;
	if (tmp)
		return (duplicate_link(e, l, t));
	if ((tmp = ft_malloc(sizeof(t_link))) == NULL)
		error(9, e, "failed to alocate link between '%s' and '%s'\n", l, t);
	*tmp = (t_link){.target = r2, .next = ((t_room*)r1->data)->links};
	((t_room*)r1->data)->links = tmp;
	if ((tmp = ft_malloc(sizeof(t_link))) == NULL)
		error(10, e, "failed to alocate link between '%s' and '%s'\n", t, l);
	*tmp = (t_link){.target = r1, .next = ((t_room*)r2->data)->links};
	((t_room*)r2->data)->links = tmp;
	return (0);
}

inline static void	end(t_env_lem_in *env, char *line)
{
	ft_free(line);
	if (env->start == NULL)
		error(16, env, "starting room is undefined\n");
	if (env->end == NULL)
		error(15, env, "ending room is undefined\n");
	if (env->nb_ant < 1)
		error(18, env, "not ants\n");
}

inline void			parser(t_env_lem_in *e, int mask, char *line)
{
	char	*t;

	while (get_next_line(e->fd, &line) > 0 && *line != 'L' && *line != '\0')
	{
		if (!e->clean_output)
			ft_printf("%s\n", line);
		if (*line == '#')
		{
			if ((!ft_strcmp(line + 1, "#start") && ((mask |= START) & END)) ||
					(!ft_strcmp(line + 1, "#end") && ((mask |= END) & START)))
				error(14, e, "command ##start and ##end next to each other\n");
		}
		else if ((t = ft_strchr(line, ' ')) && t != line)
			new_node(e, line, t, &mask);
		else if ((t = ft_strchr(line, '-')) && t != line)
			new_link(e, line, t);
		else if (e->nb_ant || (e->nb_ant = ft_strtoll(line, &t, 0)) < 1 ||
				line == t || *t != '\0')
			error(13, e, "invalid line: '%s'\n", line);
		ft_memdel((void**)&line);
	}
	end(e, line);
}
