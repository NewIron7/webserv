/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EndProgram.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 13:09:28 by hboissel          #+#    #+#             */
/*   Updated: 2023/10/03 10:30:06 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef ENDPROGRAM_HPP
# define ENDPROGRAM_HPP

# include <exception>

class EndProgram : public std::exception
{
		public:
					virtual const char* what() const throw();
};

#endif
