#include "Server.hpp"

static ServerLocation	init_location1(void)
{
	ServerLocation	ret;

	ret.setLimit_except(POST | GET);
	ret.setProxy_pass("http://redirect.url");
	ret.setAlias("/alias/home");
	ret.setAutoindex(true);
	ret.setIndex("./indexer.html");
	ret.setCgi("./cgi.exe");
	ret.setLocation("/loc/");
	ret.setUpload_store("./Downloads");
	return ret;
}

static ServerLocation	init_location2(void)
{
	ServerLocation	ret;

	ret.setLimit_except(POST | GET);
	ret.setProxy_pass("http://redirect2.url");
	ret.setAlias("/alias2/home");
	ret.setAutoindex(false);
	ret.setIndex("./indexer2.html");
	ret.setCgi("./cgi2.exe");
	ret.setLocation("/loc2/");
	ret.setUpload_store("./Downloads2/");
	return ret;
}

static void	init_server(Server&	test)
{
	test.setPort(80);
	test.setHost("1.2.3.4");
	test.addServer_name("localhost");
	test.addServer_name("mydomain.com");
	test.setDefault(true);
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
	for (int i = 0; i < server.getServer_name().size(); i++)
	{
		std::cout << server.getServer_name()[i];
	}
}

int	main(void)
{
	Server	test;

	init_server(test);
	Server.setPort(80);
	print_server(test);
}
