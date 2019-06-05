/* webserver.c */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cnaiapi.h>

#if defined(LINUX) || defined(SOLARIS)
#include <sys/time.h>
#endif

/*use this library to open default browser on windows*/

#if defined(WIN32) 
#include <windows.h>
#endif


#define BUFFSIZE	256
#define SERVER_NAME	"CNAI Demo Web Server"

#define ERROR_400	"<head></head><body><html><h1>Error 400</h1><p>Th\
e server couldn't understand your request.</html></body>\n"

#define ERROR_404	"<head></head><body><html><h1>Error 404</h1><p>Do\
cument not found.</html></body>\n"

#define HOME_PAGE	"<head><meta charset='UTF-8'></head><body><html>Αποθηκεύθηκε: %s\
<br />Ο ενδιαφέρων σύνδεσμος είναι: <a href= %s\
>αυτός</a></html></body>\n"

int	recvln(connection, char *, int);
void	send_head(connection, int, int);
void retrieveLink(FILE *fp, char url[]);
void file_Check(FILE *fp);

/*-----------------------------------------------------------------------
 *
 * Program: webserver
 * Purpose: serve hard-coded webpages to web clients
 * Usage:   webserver <appnum>
 *
 *-----------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{	
	FILE *fp;//Initiallize a variable for file
	char getHtml[2048];
  	char url[256];

	connection	conn;
	int		n;
	char		buff[BUFFSIZE], cmd[16], path[64], vers[16];
	char		*timestr;
#if defined(LINUX) || defined(SOLARIS)
	struct timeval	tv;
#elif defined(WIN32)
	time_t          tv;
#endif

	if (argc != 2) {
		(void) fprintf(stderr, "usage: %s <appnum>\n", argv[0]);
		exit(1);
	}
	//Dowloading and saving html source code from http://users.uom.gr/~pfoul/net_test1.html to net_test1.html
	//Also printing download progress and connection status on console  
	(void) snprintf(getHtml,2048,"wget -c %s","http://users.uom.gr/~pfoul/net_test1.html");
	system(getHtml);

	//Opening net_test1.html file for reading
	fp = fopen("net_test1.html","r");
	file_Check(fp);

	//Retrieving video address
	(void) retrieveLink(fp, url);
  	fclose(fp);

	while(1) {

		/* wait for contact from a client on specified appnum */

		conn = await_contact((appnum) atoi(argv[1]));
		if (conn < 0)
			exit(1);

		/* read and parse the request line */

		n = recvln(conn, buff, BUFFSIZE);
		sscanf(buff, "%s %s %s", cmd, path, vers);

		/* skip all headers - read until we get \r\n alone */

		while((n = recvln(conn, buff, BUFFSIZE)) > 0) {
			if (n == 2 && buff[0] == '\r' && buff[1] == '\n')
				break;
		}

		/* check for unexpected end of file */

		if (n < 1) {
			(void) send_eof(conn);
			continue;
		}

		fp= fopen("video.html","w");
		file_Check(fp);
		
		/* check for a request that we cannot understand */

		if (strcmp(cmd, "GET") || (strcmp(vers, "HTTP/1.0") &&
					   strcmp(vers, "HTTP/1.1"))) {
			send_head(conn, 400, strlen(ERROR_400));
			(void) send(conn, ERROR_400, strlen(ERROR_400),0);
			fputs(ERROR_400, fp);
			(void) send_eof(conn);
			continue;
		}

		/* send the requested web page or a "not found" error on browser */

		if (strcmp(path, "/") == 0) {
#if defined(LINUX) || defined(SOLARIS)
			gettimeofday(&tv, NULL);
			timestr = ctime(&tv.tv_sec);
#elif defined(WIN32)
			time(&tv);
			timestr = ctime(&tv);
#endif
			send_head(conn, 200, strlen(HOME_PAGE));
			(void) sprintf(buff, HOME_PAGE, timestr,url);
			(void) send(conn, buff, strlen(buff),0);
			fputs(buff, fp);
		} else { /* not found */
			send_head(conn, 404, strlen(ERROR_404));
			(void) send(conn, ERROR_404, strlen(ERROR_404),0);
			fputs(ERROR_404, fp);
			
		}
		fclose(fp);	
		(void) send_eof(conn);
	}
}

/*-----------------------------------------------------------------------
 * send_head - send an HTTP 1.0 header with given status and content-len
 *-----------------------------------------------------------------------
 */
void send_head(connection conn, int stat, int len)
{
	char	*statstr, buff[BUFFSIZE];

	/* convert the status code to a string */

	switch(stat) {
	case 200:
		statstr = "OK";
		break;
	case 400:
		statstr = "Bad Request";
		break;
	case 404:
		statstr = "Not Found";
		break;
	default:
		statstr = "Unknown";
		break;
	}
	
	/*
	 * send an HTTP/1.0 response with Server, Content-Length,
	 * and Content-Type headers.
	 */

	(void) sprintf(buff, "HTTP/1.0 %d %s\r\n", stat, statstr);
	(void) send(conn, buff, strlen(buff), 0);

	(void) sprintf(buff, "Server: %s\r\n", SERVER_NAME);
	(void) send(conn, buff, strlen(buff), 0);

	(void) sprintf(buff, "Content-Length: %d\r\n", len);
	(void) send(conn, buff, strlen(buff), 0);

	(void) sprintf(buff, "Content-Type: text/html\r\n");
	(void) send(conn, buff, strlen(buff), 0);

	(void) sprintf(buff, "\r\n");
	(void) send(conn, buff, strlen(buff), 0);
}

//This function is used to check if file has been created. 
//If not prints an error message and exits.
void file_Check(FILE *fp){
	if(fp == NULL){
      printf("Error while opening the file\n");
      exit(1);
  }
}

//This function is used to retrieve the video's url from net_test1.html
//It stops when full url(http://users.uom.gr/~pfoul/test_vid.mp4) is retrieved.
void retrieveLink(FILE *fp, char url[]){
  int first_br = 0;
  int isLink = 0;
  int ready_to_start = 0;
  char ch;
  int i = 0;

  while((ch = fgetc(fp))!= EOF){

    if(ch == 'a' && first_br == 1){
      isLink = 1;
    }
    else if(first_br = 1){
      first_br = 0;
    }

    if(ch == '>' && ready_to_start == 1 && isLink == 1)
    {
      isLink = 0;
      ready_to_start = 0;
      break;
    }

    if(ready_to_start == 1 && isLink == 1)
    {
      if(ch!= '"'){
        url[i] = ch;
        i++;
      }
    }

    if(ch == '<'){
      first_br = 1;
    }

    if(ch == '='&& isLink == 1){
      ready_to_start = 1;
    }
  }
}
