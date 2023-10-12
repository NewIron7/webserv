#!/bin/bash

export REDIRECT_STATUS="200"
export REQUEST_METHOD="GET"
export QUERY_STRING="name=Aurelien&age=17"
export CONTENT_LENGTH=""
export HTTP_USER_AGENT="Mozilla/5.0"
export HTTP_REFERER="http://example.com"
export HTTP_ACCEPT_LANGUAGE="en-US,en;q=0.9"
export HTTP_ACCEPT_ENCODING="gzip, deflate, br"
export HTTP_CONNECTION="keep-alive"
export SERVER_NAME="example.com"
export SERVER_PORT="80"
export REMOTE_ADDR="192.168.1.1"
export REMOTE_PORT="12345"
export SERVER_PROTOCOL="HTTP/1.1"
export SCRIPT_NAME="/testGET.php"
export SCRIPT_FILENAME="/home/$USER/webserv/cgi-bin/testGET.php"
export DOCUMENT_ROOT="/var/www/html"
export GATEWAY_INTERFACE="CGI/1.1"

./php-cgi
