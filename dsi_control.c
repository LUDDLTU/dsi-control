// https://github.com/tzlion/gblinkdx/pull/3/files
// https://en.wikipedia.org/wiki/Parallel_port 0x378
// https://www.freebsd.org/cgi/man.cgi?query=outb&apropos=0&sektion=0&manpath=FreeBSD+11.1-RELEASE+and+Ports&arch=default&format=html
// http://as6edriver.sourceforge.net/Parallel-Port-Programming-HOWTO/accessing.html
// https://wiki.xp-el.com/_media/ohm_law_lathund.png
// http://sitelec.org/cours/abati/dsi.htm

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <machine/cpufunc.h>
#include <machine/sysarch.h>


int set_dim(int dim) {
	int delay = 833;
	int port  = 0x378;

	printf("Dim level: %d\n", dim);

	// Startbit
	//printf("1 ");
	usleep( (useconds_t)(delay) );
	outb((unsigned short)port, 1);

	// Data
	for (int i = 7; i >= 0; i--) {
		usleep( (useconds_t)(delay) );
		if (dim & (1<<i)) {
			//printf("10 ");
			outb((unsigned short)port, 1);
			usleep( (useconds_t)(delay) );
			outb((unsigned short)port, 0);
		} else {
			//printf("01 ");
			outb((unsigned short)port, 0);
			usleep( (useconds_t)(delay) );
			outb((unsigned short)port, 1);
		}
	}

	// Stopbit
	//printf("0000\n");
	usleep((useconds_t)(delay));
	outb((unsigned short)port, 0);
	usleep((useconds_t)(delay));
	outb((unsigned short)port, 0);
	usleep((useconds_t)(delay));
	outb((unsigned short)port, 0);
	usleep((useconds_t)(delay));
	outb((unsigned short)port, 0);

	return 0;
}

/*--------------------------------------------------------------------------*/
int main(int ac,char *av[]){
	int		fdio, fdcm, val, ret, port, clock, countdir;
	unsigned char	dimval, buf[1024];
	unsigned char	dimval_last;
	double		dimfloat;
	time_t		ti;

	port     = 0x378;
	ret      = 0;
	dimfloat = 0;
	dimval   = 0;
	clock    = 0;
	countdir = 0;

	printf("%u: %s\n", __LINE__, __FILE__);

	if( ac <2  ){
		printf("Arg?\n");
		exit(2);
	}

	val = atoi(av[1]);

	if(  (fdio=open("/dev/io",O_RDWR)) != -1  ){
		for(int i = 0; i < 10; i++){
			set_dim(val);
			sleep(1);
		}

		close(fdio);
	} else {
		printf("Can't open because %s\n",strerror(errno)); fflush(stdout);
		ret = 2;
	}

	return ret;
}

/*--------------------------------------------------------------------------*/
