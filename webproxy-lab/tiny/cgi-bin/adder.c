/*
 * adder.c - a minimal CGI program that adds two numbers together
 */
/* $begin adder */
#include "csapp.h"

int main(void) {
  char *buf, *p;
  // char arg1[MAXLINE], arg2[MAXLINE];
  char content[MAXLINE];
  int n1= 0 , n2= 0;

  if ((buf = getenv("QUERY_STRING")) != NULL) {
    p = strchr(buf, '&');
    *p = '\0';
    // strcpy(arg1, buf);
    // strcpy(arg2, p+1);
    // n1 = atoi(arg1);
    // n2 = atoi(arg2);
    sscanf(buf, "arg1=%d", &n1);
    sscanf(p + 1, "arg2=%d", &n2);
  }

  sprintf(content, "QUERY_SRTING=%s", buf);
  sprintf(content, "<p>Welcome to add.com</p>");
  sprintf(content, "%s\n<p>your answer is: %d + %d = %d</p>\r\n", content, n1,n2,n1+n2);
  sprintf(content, "%s<p>Thanks for visiting!</p>\r\n", content);

  printf("connection: close\r\n");
  printf("Content-length: %d\r\n", (int)strlen(content));
  printf("Content-type: text/html\r\n\r\n");
  printf("%s", content);
  fflush(stdout);
  
  exit(0);
}
/* $end adder */
