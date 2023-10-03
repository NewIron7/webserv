/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InternalError.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 13:09:28 by hboissel          #+#    #+#             */
/*   Updated: 2023/10/03 10:30:06 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef INTERNALERROR_HPP
# define INTERNALERROR_HPP

# include <exception>
# include <cerrno>
# include <cstring>

class InternalError : public std::exception
{
		public:
					virtual const char* what() const throw();
};

#endif
