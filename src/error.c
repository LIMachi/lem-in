/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmartzol <hmartzol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/26 02:48:52 by hmartzol          #+#    #+#             */
/*   Updated: 2018/01/26 06:19:25 by hmartzol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lem_in.h>
#include <stdarg.h>

void	error(int code, t_env_lem_in *env, char *format, ...)
{
	va_list	va;

	if (!env->verbose)
	{
		ft_printf("ERROR\n");
		exit(code);
	}
	va_start(va, format);
	ft_dprintf(2, "Error: ");
	ft_vdprintf(2, format, va);
	va_end(va);
	exit(code);
}
