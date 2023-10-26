#include <iostream>
#include "Server.hpp"

void	load_config(const char *);

static ServerLocation	init_location1(void)
{
	ServerLocation	ret;

	ret.setLimit_except(POST | GET);
	ret.setProxy_pass("http://redirect.url");
	ret.setAlias("/alias/home");
	ret.setAutoindex(true);
	ret.setIndex("./indexer.html");
	ret.addCgi(".hugo", "bin/hu.go");
	ret.addCgi(".dams", "bin/damien");
	ret.setLocation("/loc/");
	ret.addUpload_store(".upload", "./Downloads");
	return ret;
}

static ServerLocation	init_location2(void)
{
	ServerLocation	ret;

	ret.setLimit_except(POST | GET | DELETE);
	ret.setProxy_pass("http://redirect2.url");
	ret.setAlias("/alias2/home");
	ret.setAutoindex(false);
	ret.setIndex("./indexer2.html");
	ret.addCgi(".hugo", "./cgi2.exe");
	ret.setLocation("/loc2/");
	ret.addUpload_store(".dams", "./Downloads2/");
	return ret;
}

static void	init_server(Server&	test)
{
	test.setPort(80);
	test.setHost("1.2.3.4");
	test.addServer_name("localhost");
	test.addServer_name("mydomain.com");
	test.setDefault();
	test.addErr_code(404, "/errors/404.html");
	test.setBody_size(2048);
	test.addLocation(init_location1());
	test.addLocation(init_location2());
}

static void	print_server(const Server& server)
{
	std::cout << "Server details" << std::endl << std::endl
		<< "port : " << server.getPort() << std::endl
		<< "host : " << server.getHost() << std::endl
		<< "server_name : ";
	std::cout << server.getServer_names()[0];
	for (size_t i = 1; i < server.getServer_names().size(); i++)
	{
		std::cout << ", " << server.getServer_names()[i];
	}
	std::cout << std::endl
		<< "default : " << server.getDefault() << std::endl
		<< "err_codes : ";
	std::map<int, std::string> err_codes = server.getErr_codes();
	for (std::map<int, std::string>::const_iterator it = err_codes.begin(); it != err_codes.end(); ++it)
		std::cout << it->first << " = " << it->second << "; ";
	std::cout << std::endl
		<< "body_size : " << server.getBody_size() << std::endl
		<< "locations : " << std::endl;
	for (size_t i = 0; i < server.getLocations().size(); i++)
	{
		ServerLocation	tmp = server.getLocations()[i];
		std::cout << "\t" << tmp.getLocation() << std::endl
			<< "\t- limit_except : GET(" << ((tmp.getLimit_except() & GET) != 0)
			<< "), POST(" << ((tmp.getLimit_except() & POST) != 0)
			<< "), DELETE(" << ((tmp.getLimit_except() & DELETE) != 0) << ")" << std::endl
			<< "\t- proxy_pass : " << tmp.getProxy_pass() << std::endl
			<< "\t- alias : " << tmp.getAlias() << std::endl
			<< "\t- autoindex : " << tmp.getAutoindex() << std::endl
			<< "\t- index : " << tmp.getIndex() << std::endl
			<< "\t- cgi : ";
		std::map<std::string, std::string> cgi = tmp.getCgi();
		for (std::map<std::string, std::string>::const_iterator it = cgi.begin(); it != cgi.end(); ++it)
			std::cout << it->first << " = " << it->second << "; ";
		std::cout << std::endl << "\t- upload_store : ";
		std::map<std::string, std::string> upload_store = tmp.getUpload_store();
		for (std::map<std::string, std::string>::const_iterator it = upload_store.begin(); it != upload_store.end(); ++it)
			std::cout << it->first << " = " << it->second << "; ";
		std::cout << std::endl;
	}
}

int	main(int argc, char *argv[])
{
	Server	test;

	if (argc == 1)
	{
		init_server(test);
		print_server(test);
	}
	else
		load_config(argv[1]);
}
