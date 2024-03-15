#!/bin/sh

gcc my_server.c -o my_server.fcgi -lfcgi
service nginx start
nginx -s reload
spawn-fcgi -p 8080 -n my_server.fcgi