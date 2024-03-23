FROM nginx:latest
RUN apt update && apt install -y gcc libfcgi-dev spawn-fcgi
RUN mkdir /tomokoki_server
WORKDIR /tomokoki_server/
COPY ./my_server.c .
COPY ./nginx.conf /etc/nginx/nginx.conf
COPY ./run.sh .
CMD ["sh", "run.sh"]