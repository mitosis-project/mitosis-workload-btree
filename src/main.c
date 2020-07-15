/**
 * MIT License
 * Copyright (c) 2020 Mitosis-Project
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <inttypes.h>
#include <limits.h>
#include <errno.h>

#include <string.h>
#include <fcntl.h>     /* open */
#include <unistd.h>    /* exit */
#include <sys/ioctl.h> /* ioctl */
#include <sys/mman.h>
#include <sys/time.h>
#include <numa.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>

#ifdef _OPENMP
#    include <omp.h>
#endif

#include "config.h"

FILE *opt_file_out = NULL;  ///< standard outpu

extern int real_main(int argc, char *argv[]);

void signalhandler(int sig)
{
    fprintf(opt_file_out, "<sig>Signal %i caught!</sig>\n", sig);

    FILE *fd3 = fopen(CONFIG_SHM_FILE_NAME ".done", "w");

    if (fd3 == NULL) {
        fprintf(stderr, "ERROR: could not create the shared memory file descriptor\n");
        exit(-1);
    }

    usleep(250);

    fprintf(opt_file_out, "</benchmark>\n");

    exit(0);
}


int main(int argc, char *argv[])
{
    struct timeval tstart, tend;
    gettimeofday(&tstart, NULL);

    for (int i = 0; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");

    /* check if NUMA is available, otherwise we don't know how to allocate memory */
    if (numa_available() == -1) {
        fprintf(stderr, "ERROR: Numa not available on this machine.\n");
        return -1;
    }

    opt_file_out = stdout;
    int c;
    while ((c = getopt(argc, argv, "o:h")) != -1) {
        switch (c) {
        case '-':
            break;
        case 'h':
            printf("usage: %s [-o FILE]\n", argv[0]);
            return 0;
        case 'o':
            opt_file_out = fopen(optarg, "a");
            if (opt_file_out == NULL) {
                fprintf(stderr, "Could not open the file '%s' switching to stdout\n", optarg);
                opt_file_out = stdout;
            }
            break;
        case '?':
            switch (optopt) {
            case 'o':
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                return -1;
            default:
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                return -1;
            }
        }
    }

    int prog_argc = 0;
    char **prog_argv = NULL;

    prog_argv = &argv[0];
    prog_argc = argc;

    optind = 1;

    for (int i = 0; i < argc; i++) {
        if (strcmp("--", argv[i]) == 0) {
            argv[i] = argv[0];
            prog_argv = &argv[i];
            prog_argc = argc - i;
            break;
        }
    }

    /* start with output */
    fprintf(opt_file_out, "<benchmark exec=\"%s\">\n", argv[0]);

    fprintf(opt_file_out, "<config>\n");
#ifdef _OPENMP
    fprintf(opt_file_out, "  <openmp>on</openmp>");
#else
    fprintf(opt_file_out, "  <openmp>off</openmp>");
#endif
    fprintf(opt_file_out, "</config>\n");

    /* setting the bind policy */
    numa_set_strict(1);
    numa_set_bind_policy(1);

    struct sigaction sigact;
    sigset_t block_set;

    sigfillset(&block_set);
    sigdelset(&block_set, SIGUSR1);

    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = 0;
    sigact.sa_handler = signalhandler;
    sigaction(SIGUSR1, &sigact, NULL);

    fprintf(opt_file_out, "<run>\n");
    real_main(prog_argc, prog_argv);
    fprintf(opt_file_out, "</run>\n");

    gettimeofday(&tend, NULL);
    printf("Total time: %zu.%03zu\n", tend.tv_sec - tstart.tv_sec,
           (tend.tv_usec - tstart.tv_usec) / 1000);

    fprintf(opt_file_out, "</benchmark>\n");
    return 0;
}
