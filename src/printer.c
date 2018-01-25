/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/25 05:26:54 by hmartzol          #+#    #+#             */
/*   Updated: 2018/01/25 05:39:21 by hmartzol         ###   ########.fr       */
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
		{
			((t_room*)lltmp->data)->flags |= PRINTER;
			ltmp = ((t_room*)lltmp->data)->links;
			while (ltmp)
			{
				if (!(((t_room*)ltmp->target->data)->flags & PRINTER))
					ft_printf("%s-%s\n", lltmp->label, ltmp->target->label);
				ltmp = ltmp->next;
			}
		}
}

void				printer(t_env_lem_in *env)
{
	size_t	i;
	t_llist	*lltmp;
	t_room	*rtmp;

	ft_printf("%d\n", env->nb_ant);
	i = -1;
	while (++i < env->table.size)
		if ((lltmp = env->table.data[i]))
		{
			rtmp = (t_room*)lltmp->data;
			rtmp->flags &= ~PRINTER;
			if (rtmp->flags & START)
				ft_printf("##start\n");
			if (rtmp->flags & END)
				ft_printf("##end\n");
			ft_printf("%s %d %d\n", lltmp->label, rtmp->x, rtmp->y);
		}
	printer0(env);
}