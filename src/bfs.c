/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bfs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 01:27:37 by hmartzol          #+#    #+#             */
/*   Updated: 2018/01/30 06:18:34 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lem_in.h>

static size_t			recursive_fill(t_room *room, size_t depth)
{
	t_link	*tmp;
	size_t	max_depth;
	size_t	i;

	if (room == NULL || room->flags & (BLACKLIST | END) ||
			(room->passes && room->passes < depth + 1))
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

static inline void		clean_rooms(t_env_lem_in *env)
{
	size_t	i;
	t_llist	*lltmp;

	i = -1;
	while (++i < env->table.size)
		if ((lltmp = env->table.data[i]))
			while (lltmp)
			{
				((t_room*)lltmp->data)->passes = 0;
				lltmp = lltmp->next;
			}
}

static inline t_llist	*initial_room(t_env_lem_in *env, size_t *d)
{
	t_room	*tmp;
	t_link	*link;
	t_llist	*out;

	clean_rooms(env);
	if (!(*d = recursive_fill(env->start->data, 0)))
		return (NULL);
	link = ((t_room*)env->end->data)->links;
	out = NULL;
	while (link)
	{
		if (*d >= (tmp = (t_room*)link->target->data)->passes &&
			!(tmp->flags & BLACKLIST))
		{
			*d = tmp->passes;
			out = link->target;
		}
		link = link->next;
	}
	if (out)
		((t_room*)out->data)->flags |= BLACKLIST;
	if (!*d)
		return (NULL);
	return (out);
}

static inline t_path	*extract_path(t_env_lem_in *env)
{
	size_t	d;
	t_llist	*room;
	t_path	*out;
	t_link	*link;

	if ((room = initial_room(env, &d)) == NULL)
		return (NULL);
	if (((out = ft_malloc(sizeof(t_path))) == NULL) ||
		((out->labels = ft_malloc(sizeof(char*) * d)) == NULL) ||
		((out->ants = ft_memalloc(sizeof(int*) * d)) == NULL))
		error(20, env, "failed to alocate paths\n");
	*out = (t_path){.length = d--, .labels = out->labels, .ants = out->ants,
		.next = NULL};
	if (d > 0)
		out->labels[d - 1] = room->label;
	while ((long)--d > 0)
	{
		link = ((t_room*)room->data)->links;
		while (link && ((t_room*)link->target->data)->passes != d + 1)
			link = link->next;
		out->labels[d - 1] = link->target->label;
		room = link->target;
		((t_room*)room->data)->flags |= BLACKLIST;
	}
	return (out);
}

inline void				bfs(t_env_lem_in *env)
{
	t_path	*path;

	while ((path = extract_path(env)))
	{
		path->next = env->path;
		path->labels[path->length - 1] = env->end->label;
		env->path = path;
		if (env->debug)
			debug(env);
	}
	if (!env->path)
		error(21, env, "no valid path from '%s' to '%s' was found\n",
			env->start->label, env->end->label);
}
