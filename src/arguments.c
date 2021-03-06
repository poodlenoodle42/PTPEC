#include "arguments.h"
#include <getopt.h>    
#include <string.h>
#include <stdlib.h>
#include "common.h"
#include <unistd.h>

static const char help_message[] = "PTPEC - Peer to peer encrypted chat client\n\n"
"Commandline options:\n"
"\t -n / --nickname <Nickname> \t Your Nickname in the conversasion\n"
"\t -p / --port <Port> \t \t The port used to connect to others and listend on for others to connect to you\n"
"\t -a / --address <ip address> \t Address of first peer to connect to. If omitted new session is started\n"
"\t --password <pwd> \t \t Password to protect the conversaion, if omitted you are promted for the password. \n"
"\t -h / --help \t \t \t Show this help message"
;

void check_arg(Arguments* args);

Arguments* parse_arguments(int argc, char* argv[]){
    static Arguments args;
    int c;
    memset(&args,0,sizeof(Arguments));
    args.address_info.sin_family = AF_INET;
    int help = 0;
    opterr = 0;

    const static struct option long_options[] = {
        {"address", required_argument, 0, 'a'},
        {"port", required_argument, 0, 'p'},
        {"nickname",required_argument,0,'n'},
        {"password",required_argument,0,100},
        {"help",no_argument,0,'h'},
        {0,0,0,0}
    };
    int option_index;
    while ((c = getopt_long(argc, argv, "a:p:n:p:", long_options,&option_index)) != -1){
        switch (c)
        {
        case 'a':
            args.address = 1;
            args.address_info.sin_addr.s_addr = inet_addr(optarg); 
            break;
        case 'p':
            args.port = 1;
            args.address_info.sin_port = htons((uint16_t)atoi(optarg));
            break;
        case 'n':
            args.nickname = 1;
            args.nickname_str = malloc(strlen(optarg) + 1);
            strcpy(args.nickname_str,optarg);
            break;
        case 'h':
            help = 1;
            break;
        case 100:
            args.pwd = 1;
            args.pwd_str = malloc(strlen(optarg) + 1);
            strcpy(args.pwd_str,optarg);
            break;
        case '?':
            if (optopt == 'a' || optopt == 'p' || optopt == 'n')
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else
                fprintf (stderr, "Option -%c not known.\n", optopt);
        default:
            break;
        }
    }
    if(help){
        puts(help_message);
        exit(0);
    }
    check_arg(&args);
    args.address_info.sin_family = AF_INET;
    return &args;
}

void check_arg(Arguments* args){
    if(args->nickname == 0){
        fputs("Specify a nickname with -n <Nickname>\n",stderr);
        exit(1);
    }
    if(args->pwd == 0){
        args->pwd_str = getpass("Enter the session password: ");
        args->pwd = 1;
    }
    if(args->address_info.sin_port == 0){
        fputs("Port could not parsed\n",stderr);
        exit(1);
    }
}