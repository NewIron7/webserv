/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:38:33 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/16 06:36:18 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef ROUTE_HPP
# define ROUTE_HPP

# include <string>
# include <vector>
# include <iostream>

class Route
{
	public:
		Route(void);
		~Route(void);

		void printRoute() const;
		Route& operator=(const Route& other);

		std::vector<std::string>	methods;
		std::string					redirection;
		std::string					location;
		bool						directoryListing;
		bool						directoryAdding;
		std::string					root;
		std::string					cgiPath;
		std::string					cgiExtension;
		bool						uploadedFile;
		std::string					uploadPath;

		bool						empty;
		bool						dir;
	private:
};

#endif
