#ifndef SERVERLOCATION_HPP
# define SERVERLOCATION_HPP

# include <string>
# include <map>

# define POST 1
# define GET 2
# define DELETE 4

class	ServerLocation
{
	private:
		std::string		_location;
		int				_limit_except;
		std::string		_proxy_pass;
		std::string		_alias;
		bool			_autoindex;
		std::string		_index;
		std::map<std::string, std::string>	_cgi;
		std::map<std::string, std::string>	_upload_store;

	public:
		ServerLocation();
		~ServerLocation();
		ServerLocation(const ServerLocation& old);
		ServerLocation& operator=(const ServerLocation& old);

		std::string		getLocation() const;
		int				getLimit_except() const;
		std::string		getProxy_pass() const;
		std::string		getAlias() const;
		bool			getAutoindex() const;
		std::string		getIndex() const;
		std::map<std::string, std::string>	getCgi() const;
		std::map<std::string, std::string>	getUpload_store() const;

		void	setLocation(std::string location);
		void	setLimit_except(int limit_except);
		void	setProxy_pass(std::string proxy_pass);
		void	setAlias(std::string alias);
		void	setAutoindex(bool autoindex);
		void	setIndex(std::string index);
		void	addCgi(std::string ext, std::string exe);
		void	addUpload_store(std::string ext, std::string dir);
};
		
#endif
