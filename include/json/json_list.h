#ifndef JSON_LIST_H
#define JSON_LIST_H

#include <system/system.h>
#include <json/json.h>

typedef struct JsonValues{
    JsonType type;
    char *name;
    char *value;
    struct JsonValues *next;
} JsonValues;

typedef struct {
    size_t id;
    JsonValues *value;
} JsonList;

void json_list_init();

JsonList *create_json_list(size_t id, JsonValues *value);
void destroy_json_list(JsonList *jsonList);
void display_json_list(JsonList *jsonList);

JsonValues *create_json_value(JsonType type, char *name, 
                              char *value, JsonValues *next);
JsonValues *insert_json_value(JsonValues* head, JsonType type,
                               char* name, char* value);
JsonValues *insert_json_value_by_pointer(JsonValues* head, JsonValues *next);
void display_json_values(JsonValues *head);
void remove_json_values(JsonValues *head);
void destroy_json_value(JsonValues *value);


#endif  //  JSON_LIST_H
