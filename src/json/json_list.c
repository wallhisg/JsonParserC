#include <json/json_list.h>
#include <driver/buffer_alloc.h>

void json_list_init()
{

}

JsonList *create_json_list(size_t id, JsonValues *value)
{
    JsonList *newList = (JsonList *)buffer_malloc(sizeof(JsonList));
    if(newList == NULL) {
        error("Not enough mem for newList");
    }
    else {
        newList->id = id;
        newList->value = value;
    }
    
    return newList;
}

void display_json_list(JsonList* jsonList)
{
    printf("JSON LIST INFO: %lu\r\n", jsonList->id);
    JsonValues *cursor = jsonList->value;
    
    while(cursor != NULL)
    {
        printf("\tTYPE: %d, \tNAME: %s, \tVALUE: %s\r\n", 
               cursor->type, cursor->name, cursor->value);
        cursor = cursor->next;
    }
}

void destroy_json_list(JsonList *jsonList)
{
    if(jsonList != NULL)
    {
        remove_json_values(jsonList->value);
        buffer_free(jsonList);
    }
}

JsonValues *create_json_value(JsonType type, char *name, 
                              char *value, JsonValues *next)
{
    JsonValues *newValue = (JsonValues *)buffer_malloc(sizeof(JsonValues));
    if(newValue == NULL) {
        error("Not enough mem for newValue");
    }
    else {
        newValue->type = type;
        newValue->name = name;
        newValue->value = value;
        newValue->next = next;
    }
        
    return newValue;
}

JsonValues * insert_json_value(JsonValues *head, JsonType type,
                               char* name, char* value)
{
    if(head != NULL)
    {
        JsonValues *cursor = head;
        
        while(cursor->next != NULL)
            cursor = cursor->next;
        
        JsonValues *newValue = create_json_value(type, name, value, NULL);
        cursor->next = newValue;
    }
    else {
        printf("HEAD: %p\r\n", head);
        head = create_json_value(type, name, value, NULL);
    }
    
    return head;
}

JsonValues *insert_json_value_by_pointer(JsonValues* head, JsonValues *next)
{
    if(head != NULL)
    {
        JsonValues *cursor = head;

        while(cursor->next != NULL)
            cursor = cursor->next;

        cursor->next = next;
    }
    else {
        printf("HEAD: %p\r\n", head);
        head = create_json_value(next->type, next->name, next->value, NULL);
    }

    return head;
}

void display_json_values(JsonValues* head)
{
    printf("DISPLAY_JSON_VALUE.\r\n");
    JsonValues *cursor = head;
    size_t i = 0;
    while(cursor != NULL)
    {
        printf("i: %lu - ", i++);
        printf("\tTYPE: %d \tNAME: %s \tVALUE: %s\r\n",
               cursor->type, cursor->name, cursor->value);
        
        cursor = cursor->next;
    }
}


void remove_json_values(JsonValues* head)
{
    JsonValues *cursor, *old;
    
    if(head != NULL)
    {
        cursor = head->next;
        head->next = NULL;
        while(cursor != NULL)
        {
            old = cursor->next;
            destroy_json_value(cursor);
            cursor = old;
        }
        destroy_json_value(head);
    }
}

void destroy_json_value(JsonValues* value)
{
    buffer_free(value->name);
    buffer_free(value->value);
    buffer_free(value);
}
