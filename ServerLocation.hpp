#ifndef SERVERLOCATION_HPP
# define SERVERLOCATION_HPP

# define POST 1
# define GET 2
# define DELETE 4

class	SeverLocation
{
	private:
		int				_limit_except;
		std::string		_proxy_pass;
		std::string		_alias;
		bool			_autoindex;
		std::string		_index;
		std::string		_cgi;
		std::string		_location;
		std::string		_upload_store;

	public:
		ServerLocation();
		~ServerLocation();
		ServerLocation(const ServerLocation& old);
		ServerLocation& operator=(const ServerLocation& old);

		int				getLimit_except();
		std::string		getProxy_pass();
		std::string		getAlias();
		bool			getAutoindex();
		std::string		getIndex();
		std::string		getCgi();
		std::string		getLocation();
		std::string		getUpload_store();

		void	setLimit_except(int limit_except);
		void	setProxy_pass(std::string proxy_pass);
		void	setAlias(std::string alias);
		void	setAutoindex(bool autoindex);
		void	setIndex(std::string index);
		void	setCgi(std::string cgi);
		void	setLocation(std::string location);
		void	setUpload_store(std::string upload_store);
}
		
#endif
