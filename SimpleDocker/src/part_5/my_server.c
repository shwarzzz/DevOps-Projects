#include <fcgi_stdio.h>
#include <stdlib.h>

int main() {
  while (FCGI_Accept() >= 0) {
    printf(
        "Content-Type: text/html\r\n"
        "\r\n");
    printf(
        "<html><body>"
        "<h1>Hello World!</h1>"
        "</body></html>\n");
  }
  return 0;
}