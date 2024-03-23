#!/bin/sh

gcc my_server.c -o my_server.fcgi -lfcgi
spawn-fcgi -p 8080 my_server.fcgi
nginx -g "daemon off;"
systemctl start nginx
nginx -s reload