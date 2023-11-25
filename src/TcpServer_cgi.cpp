#include "TcpServer.hpp"

void	TcpServer::_endCGI(Sockets *client)
{
		CGIprocess &cgi = client->cgi;

		if (cgi.isError())
		{
			if (cgi.response.empty())
				client->oRequest.setErrorMsg("Error with cgi");
			client->response = DefaultErrorPages::generate(500,
				client->oRequest.getErrorMsg());
		}
		else
		{
			cgi.addHeaders();
			client->response = cgi.response;
		}
		
		cgi.done = true;
		this->_remove_cgi(*client, 0);
		this->_remove_cgi(*client, 1);

		cgi.endCGI(false);
		client->CGIrun = false;
		client->resGen = true;

		//std::cout << "\033[35m[] Response ->\033[0m" << std::endl;
		//std::cout << "\033[2m" << client->response << "\033[0m" << std::endl;
}

void	TcpServer::_add_cgi(Sockets &client, unsigned int nb)
{
	CGIprocess	&cgi = client.cgi;

	cgi.size = sizeof(cgi.info);
	memset((void*)&cgi.info, 0, cgi.size);

	this->_CGIstreams[cgi.fds[nb]] = &client;
	cgi.clientFd = client.socket;

	if (nb == 0)
		cgi.event[0].events = EPOLLOUT;
	else
		cgi.event[1].events = EPOLLIN;

	cgi.event[nb].events |= EPOLLRDHUP;
	cgi.event[nb].events |= EPOLLPRI;

	cgi.event[nb].data.fd = cgi.fds[nb];


	if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, cgi.fds[nb],
				&cgi.event[nb]) == -1)
	{
		this->_CGIstreams.erase(cgi.fds[nb]);
        std::cerr << "Error while addind cgi to epoll" << std::endl;
		throw InternalError();
	}
}

void	TcpServer::_remove_cgi(Sockets &client, unsigned int nb)
{
	CGIprocess	&cgi = client.cgi;

	if (this->_CGIstreams.find(cgi.fds[nb]) == this->_CGIstreams.end())
		return ;

	this->_CGIstreams.erase(cgi.fds[nb]);

	if ((cgi.c == false) && (epoll_ctl(this->_epfd, EPOLL_CTL_DEL, cgi.fds[nb],
					&cgi.event[nb]) == -1))
	{
		std::cerr << "Error while removing cgi from epoll" << std::endl;
		throw InternalError();
	}
}
