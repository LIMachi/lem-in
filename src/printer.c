/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/25 05:26:54 by hmartzol          #+#    #+#             */
/*   Updated: 2018/01/30 06:11:29 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lem_in.h>

inline static void	printer0(t_env_lem_in *env)
{
	size_t	i;
	t_llist	*lltmp;
	t_link	*ltmp;

	i = -1;
	while (++i < env->table.size)
		if ((lltmp = env->table.data[i]))
			while (lltmp)
			{
				((t_room*)lltmp->data)->flags |= PRINTER;
				ltmp = ((t_room*)lltmp->data)->links;
				while (ltmp)
				{
					if (!(((t_room*)ltmp->target->data)->flags & PRINTER))
						ft_printf("%s-%s\n", lltmp->label, ltmp->target->label);
					ltmp = ltmp->next;
				}
				lltmp = lltmp->next;
			}
}

inline void			printer(t_env_lem_in *env)
{
	size_t	i;
	t_llist	*lltmp;
	t_room	*rtmp;

	ft_printf("%d\n", env->nb_ant);
	i = -1;
	while (++i < env->table.size)
		if ((lltmp = env->table.data[i]))
			while (lltmp)
			{
				rtmp = (t_room*)lltmp->data;
				rtmp->flags &= ~PRINTER;
				if (rtmp->flags & START)
					ft_printf("##start\n");
				if (rtmp->flags & END)
					ft_printf("##end\n");
				ft_printf("%s %d %d\n", lltmp->label, rtmp->x, rtmp->y);
				lltmp = lltmp->next;
			}
	printer0(env);
	ft_printf("\n");
}

inline void			print_paths_found(t_env_lem_in *env)
{
	t_path	*path;
	size_t	i;

	path = env->path;
	while (path)
	{
		i = -1;
		ft_printf("'%s'->", env->start->label);
		while (++i < path->length - 1)
			ft_printf("'%s'->", path->labels[i]);
		ft_printf("'%s'\n", path->labels[i]);
		path = path->next;
	}
}

/*
** 2 test->truc->thing == L1-truc\nL1-thing L2-truc\nL2-thing
** ft_printf("L%d-%s", ant, room_label)
*/

static inline int	update_path(long long ant, t_path *path)
{
	int			out;
	size_t		i;

	i = path->length;
	out = 0;
	while (--i)
		if ((path->ants[i] = path->ants[i - 1]))
		{
			ft_printf("%sL%d-%s", &"\0 "[out], path->ants[i], path->labels[i]);
			out = 1;
		}
	if ((path->ants[0] = ant))
	{
		ft_printf("%sL%d-%s", &"\0 "[out], ant, path->labels[0]);
		out = 1;
	}
	return (out);
}

inline void			print_path_usage(t_env_lem_in *env)
{
	t_path		*path;
	long long	waiting_ant;
	int			run;

	waiting_ant = 1;
	run = 1;
	while ((run || waiting_ant <= env->nb_ant) && (path = env->path) &&
			!(run = 0))
		while (path)
			if ((run |= update_path(waiting_ant <= env->nb_ant ?
					waiting_ant++ : 0, path)))
				ft_printf("%c", (path = path->next) ? ' ' : '\n');
			else
				path = path->next;
}
