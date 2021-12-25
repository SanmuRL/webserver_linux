#include "config.h"

Config::Config()
{
    max_thread = 4;

    port = 9090;

    close_log = false;
}


void Config::parse_init(int argc, char** argv)
{
    int opt;
    const char *p = "p:n:c:o:d";
    while((opt = getopt(argc, argv, p)) != -1)
    {
        switch(opt)
        {
            case 'p':
            {
                port = atoi(optarg);
                break;
            }
            case 'n':
            {
                max_thread = atoi(optarg);
                break;
            }
            case 'c':
            {
                close_log = atoi(optarg);
                break;
            }
            case 'o':
            {
                OPT_LINGER = atoi(optarg);
                break;
            }
            case 'd':
            {
                use_deamon = true;
                break;
            }
            default: break;
        }
    }
}