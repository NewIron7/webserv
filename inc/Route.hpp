/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hboissel <hboissel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 11:38:33 by hboissel          #+#    #+#             */
/*   Updated: 2023/11/13 11:49:05 by hboissel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef ROUTE_HPP
# define ROUTE_HPP

# include <string>
# include <vector>

class Route
{
	public:
		Route(void);
		~Route(void);

		std::vector<std::string>	methods;
		std::string					redirection;
		std::string					location;
		bool						directoryListing;
		std::string					root;
		std::string					cgiPath;
		std::string					cgiExtension;
		bool						uploadedFile;
		std::string					uploadPath;

	private:
}

#endif
