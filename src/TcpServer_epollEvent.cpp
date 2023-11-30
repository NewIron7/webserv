#include "TcpServer.hpp"

void	TcpServer::_processEPOLLOUT(struct epoll_event &ev)
{
	if (this->_CGIstreams.find(ev.data.fd) != this->_CGIstreams.end())
	{
		Sockets *client = this->_CGIstreams[ev.data.fd];
		CGIprocess &cgi = client->cgi;

		if (cgi.step == 0)
		{
			cgi.sendBody(client->oRequest);
			if (client->oRequest.getErrorCode() == 500)
			{
				this->_endCGI(client);
			}
			else
			{
				this->_remove_cgi(*client, 0);
                try
                {
                    this->_add_cgi(*client, 1);
                }
                catch(const std::exception& e)
                {
                    this->_endCGI(client);
                }
                
				
			}
		}

		return ;
	}
    else if (this->_streams.find(ev.data.fd) != this->_streams.end())
    {
        Sockets	&client = this->_streams[ev.data.fd];

        if (client.resSent || client.reqGot == false || client.CGIrun)
            return ;

        if (client.resGen == false)
        {
            //std::cout << "\033[35m[] Request ->\033[0m" << std::endl;
            // std::cout << "\033[2m" << client.request << "\033[0m" << std::endl;
            // std::cout << "\033[33m[+] Generating response\033[0m" << std::endl;

            client.process();
            if (client.CGIrun)
            {
                try
                {
                    if (client.cgi.step)
                        this->_add_cgi(client, 1);
                    else
                        this->_add_cgi(client, 0);
                }
                catch(const std::exception& e)
                {
                    this->_endCGI(&client);
                }
                return ;
            }

            client.resGen = true;
            client.request.clear();

            // std::cout << "\033[35m[] Response ->\033[0m" << std::endl;
            // std::cout << "\033[2m" << client.response << "\033[0m" << std::endl;
        }
        else
            client.request.clear();

        int	err = write(client.socket, client.response.c_str(),
                client.response.size());
        if (err == -1 && (err == 0 && client.response.empty() == false))
        {
            std::cerr << "\033[35m[#] Failed to sent response\033[0m" << std::endl;
        }
        else
        {
            client.resSent = true;
            //std::cout << "\033[32m[+] Response succesfully sent to client\033[0m" << std::endl;
        }
    }
}

void	TcpServer::_processEPOLLIN(struct epoll_event &ev)
{
	if ((this->_streams.find(ev.data.fd) != this->_streams.end())
			&& this->_streams[ev.data.fd].main)
	{
		Sockets &server = this->_streams[ev.data.fd];
        try
        {
            this->_add_client(server.socket);
            std::cout << "\033[32;1m[+] New connection from port " << server.port
			    << " on server [ " << server.socket << " ]\033[0m" << std::endl;
        }
        catch(const std::exception& e)
        {
            throw;
        }
	}
	else if (this->_CGIstreams.find(ev.data.fd) != this->_CGIstreams.end())
	{
		Sockets	*client = this->_CGIstreams[ev.data.fd];
		CGIprocess	&cgi = client->cgi;

		cgi.readResponse(client->oRequest);
		if (client->oRequest.getErrorCode() == 500)
		{
			this->_endCGI(client);
		}
	}
	else if (this->_streams.find(ev.data.fd) != this->_streams.end())
	{
		Sockets	&client = this->_streams[ev.data.fd];

		char	buf[BUFFER_SIZE];
		memset((void*)buf, 0, BUFFER_SIZE);

		int	bytesRead = read(client.socket, buf, BUFFER_SIZE);
		if (bytesRead == -1)
		{
			client.reqGot = false;
			std::cerr << "[#] Failed to read request" << std::endl;
		}
		else
		{
			client.request.insert(client.request.size(), buf, bytesRead);
			//std::cout << "\033[36m[+] " << bytesRead << " bytes read\033[0m" << std::endl;
			client.reqGot = true;
			client.resGen = false;
			client.resSent = false;
		}
	}
}

void	TcpServer::_processEPOLLERR(struct epoll_event &ev)
{
	if (this->_streams.find(ev.data.fd) != this->_streams.end())
	{
		Sockets &client = this->_streams[ev.data.fd];
		std::cout << "\033[31m[-] [ "
			<< client.socket << " ] error threrefore got disconnected\033[0m"
			<< std::endl;
        try
        {
           this->_remove_client(client);
        }
        catch(const std::exception& e)
        {
            std::cerr << "\033[41m" << e.what() << "\033[0m" << std::endl;
        }
        
		
	}
	else if (this->_CGIstreams.find(ev.data.fd) != this->_CGIstreams.end())
	{
		Sockets *client = this->_CGIstreams[ev.data.fd];
		this->_endCGI(client);
	}
}

void	TcpServer::_processEPOLLHUP(struct epoll_event &ev)
{
	if (this->_streams.find(ev.data.fd) != this->_streams.end())
	{
		Sockets &client = this->_streams[ev.data.fd];
		std::cout << "\033[31m[-] [ " << client.socket << " ] disconnected\033[0m"
			<< std::endl;
		try
        {
           this->_remove_client(client);
        }
        catch(const std::exception& e)
        {
            std::cerr << "\033[41m" << e.what() << "\033[0m" << std::endl;
        }
	}
	else if (this->_CGIstreams.find(ev.data.fd) != this->_CGIstreams.end())
	{
		Sockets *client = this->_CGIstreams[ev.data.fd];
		this->_endCGI(client);
	}
}

void	TcpServer::_processEvent(struct epoll_event &ev)
{
	if ((ev.events & EPOLLHUP) == EPOLLHUP
			|| (ev.events & EPOLLRDHUP) == EPOLLRDHUP)
		this->_processEPOLLHUP(ev);
	else if ((ev.events & EPOLLERR) == EPOLLERR)
		this->_processEPOLLERR(ev);
	else if ((ev.events & EPOLLIN) == EPOLLIN
			|| (ev.events & EPOLLPRI) == EPOLLPRI)
		this->_processEPOLLIN(ev);
	else if ((ev.events & EPOLLOUT) == EPOLLOUT)
		this->_processEPOLLOUT(ev);
}

