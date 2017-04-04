#include <stdio.h>

#include <string.h>     /* strerror() */
#include <errno.h>      /* errno */

#include <fcntl.h>      /* open() */
#include <unistd.h>     /* close() */
#include <sys/ioctl.h>  /* ioctl() */

#include <linux/input.h>    /* EVIOCGVERSION ++ */

#define EV_BUF_SIZE 16

int main(int argc, char *argv[]) {
    int fd, sz;
    unsigned i;

    /* A few examples of information to gather */
    unsigned version;
    unsigned short id[4];  /* or use struct input_id */
    char name[256] = "N/A";

    struct input_event ev[EV_BUF_SIZE]; /* Read up to N events at a time */

    if (argc < 2) {
        fprintf(stderr,
            "Usage: %s /dev/input/eventN\n"
            "Where X = input device number\n",
            argv[0]
        );
        return EINVAL;
    }

    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        fprintf(stderr,
            "ERR %d:\n"
            "Unable to open `%s'\n"
            "%s\n",
            errno, argv[1], strerror(errno)
        );
    }

    /* Error check here as well. */
    ioctl(fd, EVIOCGVERSION, &version);
    ioctl(fd, EVIOCGID, id); 
    ioctl(fd, EVIOCGNAME(sizeof(name)), name);

    fprintf(stderr,
        "Name      : %s\n"
        "Version   : %d.%d.%d\n"
        "ID        : Bus=%04x Vendor=%04x Product=%04x Version=%04x\n"
        "----------\n"
        ,
        name,

        version >> 16,
        (version >> 8) & 0xff,
        version & 0xff,

        id[ID_BUS],
        id[ID_VENDOR],
        id[ID_PRODUCT],
        id[ID_VERSION]
    );

    /* Loop. Read event file and parse result. */
    for (;;) {
        sz = read(fd, ev, sizeof(struct input_event) * EV_BUF_SIZE);

        if (sz < (int) sizeof(struct input_event)) {
            fprintf(stderr,
                "ERR %d:\n"
                "Reading of `%s' failed\n"
                "%s\n",
                errno, argv[1], strerror(errno)
            );
            goto fine;
        }

        /* Implement code to translate type, code and value */
        for (i = 0; i < sz / sizeof(struct input_event); ++i) {
            fprintf(stderr,
                "%ld.%06ld: "
                "type=%02x "
                "code=%02x "
                "value=%02x\n",
                ev[i].time.tv_sec,
                ev[i].time.tv_usec,
                ev[i].type,
                ev[i].code,
                ev[i].value
            );
            #include <stdlib.h>
            if (ev[i].type == 0x1 && ev[i].value == 0x0) {
                if (ev[i].code == 0x66) { printf("Home button pressed\n"); system("mpc toggle >/dev/null 2>&1"); }
                if (ev[i].code == 0x21) { printf("f button pressed\n"); system("mpc crossfade 5 >/dev/null 2>&1"); }
                if (ev[i].code == 0x26) { printf("l button pressed\n"); system("mpc random off && mpc repeat off && mpc single off >/dev/null 2>&1"); }
                if (ev[i].code == 0x1f) { printf("s button pressed\n"); system("mpc random on && mpc repeat off && mpc single off >/dev/null 2>&1"); }
                if (ev[i].code == 0x01) { printf("Esc button pressed\n"); system("mpc random off && mpc repeat on && mpc single on >/dev/null 2>&1"); }
                if (ev[i].code == 0x67) { printf("Up button pressed\n"); system("mpc volume +5 >/dev/null 2>&1"); }
                if (ev[i].code == 0x69) { printf("Left button pressed\n"); system("mpc prev >/dev/null 2>&1"); }
                if (ev[i].code == 0x6a) { printf("Right button pressed\n"); system("mpc next >/dev/null 2>&1"); }
                if (ev[i].code == 0x6c) { printf("Down button pressed\n"); system("mpc volume -5 >/dev/null 2>&1"); }
                if (ev[i].code == 0x1c) { printf("Enter button pressed\n"); system("mpc seek 00:00:00 >/dev/null 2>&1"); }
                if (ev[i].code == 0x73) { printf("Volume Up button pressed\n"); system("mpc volume +5 >/dev/null 2>&1"); }
                if (ev[i].code == 0x72) { printf("Volume Down button pressed\n"); system("mpc volume -5 >/dev/null 2>&1"); }
                if (ev[i].code == 0x71) { printf("Mute button pressed\n"); system("mpc pause >/dev/null 2>&1"); }
                if (ev[i].code == 0x68) { printf("Page Up button pressed\n"); system("mpc next >/dev/null 2>&1"); }
                if (ev[i].code == 0x6d) { printf("Page Down button pressed\n"); system("mpc prev >/dev/null 2>&1"); }
                if (ev[i].code == 0xa8) { printf("Rewind button pressed\n"); system("mpc seek -5% >/dev/null 2>&1"); }
                if (ev[i].code == 0xa4) { printf("Play/Pause button pressed\n"); system("mpc toggle >/dev/null 2>&1"); }
                if (ev[i].code == 0xd0) { printf("Fast Forward button pressed\n"); system("mpc seek +5% >/dev/null 2>&1"); }
                if (ev[i].code == 0xa5) { printf("Previous button pressed\n"); system("mpc prev >/dev/null 2>&1"); }
                if (ev[i].code == 0xa6) { printf("Stop button pressed\n"); system("mpc stop >/dev/null 2>&1"); }
                if (ev[i].code == 0xa3) { printf("Next button pressed\n"); system("mpc next >/dev/null 2>&1"); }
            }
        }
    }

    fine:
        close(fd);

    return errno;
}


