#!/usr/bin/env python3

import cgi
import cgitb

# Enable CGI traceback for debugging purposes
cgitb.enable()

# Set content type to HTML
print("Content-type: text/html\r\n\r\n")

# Start HTML content
print("<!DOCTYPE html>\r\n")
print("<html>\r\n")
print("<head>\r\n")
print("<title>CGI Python Script - GET Parameters</title>\r\n")
print("</head>\r\n")
print("<body>\r\n")
print("<h1>GET Parameters</h1>\r\n")

# Get GET parameters
form = cgi.FieldStorage()
params = {}
if form:
    for key in form.keys():
        params[key] = form.getvalue(key)
        print(f"<p>{key} = {params[key]}</p>\r\n")

print("</body>\r\n")
print("</html>\r\n")