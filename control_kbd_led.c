#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <linux/input.h>
#include <fcntl.h>

int main( int argc, char **argv) {
	int fd, retval;
	struct input_event ev; /* the event */

	if( argc < 4 ) {
		fprintf(stderr, "Usage: %s event_file c|n|s 0|1\n\twhere led_num is 'c' for CAPS_LOCK, 'n' for NUM_LOCK and 's' for SCROLL_LOCK\n\t'1' to LED on, '0' to LED off.\n", argv[0]);
		return 1;
	}

	fd = open(argv[1], O_RDWR);
	if( fd == -1 ) {
		fprintf(stderr, "Error: cannot open file %s. System message is (%d) %s.\n", argv[1], errno, strerror(errno));
		return 2;
	}

	switch( (char) argv[2][0] ) {
		case 'c':	ev.code = LED_CAPSL;
					break;
		case 'n':	ev.code = LED_NUML;
					break;
		case 's':	ev.code = LED_SCROLLL;
					break;
		default:
					fprintf(stderr, "Error: unknown LED '%s'\n", argv[2]);
					close(fd);
					return 3;
	}

	switch( (char) argv[3][0] ) {
		case '0':	ev.value = 0;
					break;
		case '1':	ev.value = 1;
					break;
		default:
					fprintf(stderr, "Error: unknown LED state '%s'\n", argv[3]);
					close(fd);
					return 4;
	}

	ev.type = EV_LED;
	retval = write(fd, &ev, sizeof(struct input_event));

	if( retval == -1 ) {
		fprintf(stderr, "Error while writing in file %s. System message is (%d) %s.\n", argv[1], errno, strerror(errno));
		retval = 5;
	} else
		retval = 0;

	close(fd);
	return retval;
}
