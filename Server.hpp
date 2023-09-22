#ifndef SERVER_HPP
# define SERVER_HPP

class	Server
{
	private:
		int				_port;
		int				_host;
		std::string*	_servers_names;
		bool			_default;
		std::map<int, std::string>*	_err_codes;
		int				_body_size;
		ServerLocation*	_locations;

	public:
		Server();
		~Server();
		Server(const Server& old);
		Server&	operator=(const Server& old);

		int				getPort();
		int				getHost();
		std::string*	getServers_names();
		bool			getDefault();
		std::map<int, std::string>*	getErr_codes();
		int				getBody_size();
		ServerLocation*	getLocations();

		void	setPort(int port);
		void	setHost(int host);
		void	setServers_names(std::string* servers_names);
		void	setDefault(bool dfl);
		void	setErr_codes(std::map<int, std::string>* err_codes);
		void	setBody_size(int body_size);
		void	setLocations(ServerLocations* locations);
}

#endif
