#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#include "multi_proc.c"
#include "multi_thread.c"


struct option longopts[] = {
	{ "mode", required_argument, NULL, 'm' },
	{ "port", required_argument, NULL, 'p' },
};

int scan_int(char *s, int *n) {
	if (sscanf(s, "%d", n) < 1) 
		return -1;
	return 0;
}

int parse_args(int argc, char *argv[], struct config *cfg) {
	char c;
	while ((c = getopt_long(argc, argv, "m:p:", longopts, NULL)) != -1) {
		switch (c) {
		case 'm':
			if (scan_int(optarg, &cfg->mode) == -1)
				return -1;
			break;
		case 'p':
			if (scan_int(optarg, &cfg->port) == -1)
				return -1;
			break;
		default:
			return -1;
		}
	}
	return 0;
}

void print_usage() {
	printf("usage: ./a.out port\n");
}

/*
 * arg0 func name (default)
 * arg1 socket port
 */
int main(int argc, char *argv[]) {
	struct config cfg;

	if (-1 == parse_args(argc, argv, &cfg)) {
		print_usage();
		exit(1);
	}

	switch (cfg.mode) {
	case 1:
		run_multi_proc(&cfg);
		break;
	case 2:
		run_multi_thread(&cfg);
		break;
	default:
		perror("mode error");
		exit(1);
	}
}
