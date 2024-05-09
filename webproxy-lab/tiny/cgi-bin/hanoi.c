/*
 * hanoi.c - a minimal CGI program that adds two numbers together
 */
/* $begin hanoi */
#include "csapp.h"

// char arg1[MAXLINE];
char content[MAXLINE];

void hanoi(int n, int from, int via, int to) {
  if (n == 1) {
    sprintf(content, "%s\n<p>move %d to %d</p>", content, from, to);
    return;
  }
  if (n > 8) {
    sprintf(content, "%s\n<p>too many lines to print!</p>", content);
    return;
  }
  hanoi(n-1, from, to, via);
  hanoi(1, from, via, to);
  hanoi(n-1, via, from, to);
}

int main(void) {
  char *buf;
  int n1= 0;

  if ((buf = getenv("QUERY_STRING")) != NULL) {
    sscanf(buf, "arg1=%d", &n1);
  }

  sprintf(content, "QUERY_SRTING=%s", buf);
  sprintf(content, "<p>Welcome to HANOI WORLD</p>");
  hanoi(n1, 1,2,3);
  sprintf(content, "%s\n<p>Thanks for visiting!</p>\r\n", content);

  printf("connection: close\r\n");
  printf("Content-length: %d\r\n", (int)strlen(content));
  printf("Content-type: text/html\r\n\r\n");
  printf("%s", content);
  fflush(stdout);
  
  exit(0);
}
/* $end hanoi */
