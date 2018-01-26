/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 01:27:37 by hmartzol          #+#    #+#             */
/*   Updated: 2018/01/26 02:37:59 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lem_in.h>

static int				recursive_fill(t_room *room, int depth)
{
	t_link	*tmp;
	int		max_depth;
	int		i;

	if (room == NULL || room->flags & (BLACKLIST | END) || room->passes)
		return (depth);
	room->passes = ++depth;
	max_depth = depth;
	tmp = room->links;
	while (tmp)
	{
		i = recursive_fill((t_room*)tmp->target->data, depth);
		if (i > max_depth)
			max_depth = i;
		tmp = tmp->next;
	}
	return (max_depth);
}

static inline t_path	*extract_path(t_env_lem_in *env)
{
	int	d;

	d = recursive_fill(env->start->data, 0);
	ft_printf("depth: %d\n", d);
	return (NULL);
}

inline void				bfs(t_env_lem_in *env)
{
	(void)extract_path(env);
}
