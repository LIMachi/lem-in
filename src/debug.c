/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/25 04:51:25 by hmartzol          #+#    #+#             */
/*   Updated: 2018/01/26 03:32:32 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lem_in.h>

inline void	debug(t_env_lem_in *env)
{
	size_t	i;
	t_llist	*lltmp;
	t_room	*rtmp;
	t_link	*ltmp;

	ft_printf("fd: %d, nb_ant: %d\n", env->fd, env->nb_ant);
	i = -1;
	while (++i < env->table.size)
		if ((lltmp = env->table.data[i]))
			while (lltmp)
			{
				rtmp = (t_room*)lltmp->data;
				ft_printf(
					"room: '%s'(%p)(%p), flags: %d, passes: %d, x: %d, y: %d\n",
					lltmp->label, lltmp, rtmp, rtmp->flags, rtmp->passes,
					rtmp->x, rtmp->y);
				ltmp = rtmp->links;
				while (ltmp)
				{
					ft_printf("\t-> '%s'(%p)(%p)\n", ltmp->target->label,
						ltmp->target, (t_room*)lltmp->data);
					ltmp = ltmp->next;
				}
				lltmp = lltmp->next;
			}
}
