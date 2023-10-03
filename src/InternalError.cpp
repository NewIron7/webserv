/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InternalError.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 13:14:43 by hboissel          #+#    #+#             */
/*   Updated: 2023/10/03 10:54:49 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "InternalError.hpp"

const char* InternalError::what() const throw()
{
	return (strerror(errno));
}
