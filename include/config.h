#ifndef CONFIG_H
#define CONFIG_H

struct arguments {
    char *p;
    char *i;
    char *s;
    int daemon;
};

int config_parse(int argc, char **argv, struct arguments *arguments);

#endif