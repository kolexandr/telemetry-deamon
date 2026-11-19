#include <stdlib.h>
#include <argp.h>
#include <stdio.h>
#include <config.h>
#include "ctype.h"

#include "daemon.h"
#include "log_utils.h"

static char doc[] = "Tuya daemon monitoring";

static char args_doc[] = "ARG1";

static struct argp_option options[] = {
    {"productId",       'p',    "VALUE",    0,  "Specify productId from tuya iot platform", 0},
    {"deviceId",        'i',    "VALUE",    0,  "Specify deviceId from tuya iot platform", 0},
    {"deviceSecret",    's',    "VALUE",    0,  "Specify deviceSecret from tuya iot platform", 0},
    {"deamon",          'd',    0,          0,  "Run the program in daemon mode", 0},
    {0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state){
    struct arguments *arguments = (struct arguments *)state->input;
    
    switch(key){
        case 'p':
            arguments->p = arg;
            break;
        case 'i':
            arguments->i = arg;
            break;
        case 's':
            arguments->s = arg;
            break;
        case 'd':
            arguments->daemon = 1;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static struct argp argp = {
    .options = options,
    .parser = parse_opt,
    .args_doc = args_doc,
    .doc = doc
};

static int is_empty_string(char *text){
    if (text == NULL || *text == '\0'){
        return 1;
    }

    
    while(*text){
        if (isspace((unsigned char)*text)){
            return 1;
        }
        text++;
    }

    return 0;
}

int config_parse(int argc, char **argv, struct arguments *arguments){

    if (argc == 1){
        argp_help(&argp, stderr, ARGP_HELP_USAGE, argv[0]);
        exit(EXIT_FAILURE);
    }

    arguments->p = NULL;
    arguments->i = NULL;
    arguments->s = NULL;
    
    arguments->daemon = 0;
    
    argp_parse(&argp, argc, argv, 0, 0, arguments);

    if (is_empty_string(arguments->p) == 1 || is_empty_string(arguments->i) == 1 || is_empty_string(arguments->s) == 1){
        log_critical_m("Argument/s can not be empty or include spaces");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}