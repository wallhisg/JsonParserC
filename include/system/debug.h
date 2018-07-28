#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#define DEBUG_MESSAGE   0
#define debug_message(msg) \
            { if (DEBUG_MESSAGE) printf("--%s--\r\n", msg);}

#define DEBUG           1
#define debug(...) \
            do { if (DEBUG) fprintf(stdout, __VA_ARGS__); } while (0)
                
#define DEBUG_JSON      0
#define debug_json(...) \
            do { if (DEBUG_JSON) fprintf(stdout, __VA_ARGS__); } while (0)   
                
#define DEBUG_JSON_PARSER      1
#define debug_json_parser(...) \
            do { if (DEBUG_JSON_PARSER) fprintf(stdout, __VA_ARGS__);} while (0)                 
                
#endif //   DEBUG_H
