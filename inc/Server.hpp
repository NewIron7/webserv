#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <vector>
# include <map>
# include "ServerLocation.hpp"

class	Server
{
	private:
		int				_port;
		std::string		_host;
		std::vector<std::string> _server_names;
		bool			_default;
		std::map<int, std::string>	_err_codes;
		int				_body_size;
		std::vector<ServerLocation>	_locations;

	public:
		Server();
		~Server();
		Server(const Server& old);
		Server&	operator=(const Server& old);

		int				getPort() const;
		std::string		getHost() const;
		std::vector<std::string>	getServer_names() const;
		bool			getDefault() const;
		std::map<int, std::string>	getErr_codes() const;
		int				getBody_size() const;
		std::vector<ServerLocation>	getLocations() const;

		void	setPort(int port);
		void	setHost(std::string host);
		void	addServer_name(std::string server_name);
		void	setDefault();
		void	addErr_code(int code, std::string file);
		void	setBody_size(int body_size);
		void	addLocation(ServerLocation locations);
};

#endif
