#include <stdio.h>
#include "api_client.h"
#include <cjson/cJSON.h>

int main(void) {
    ApiClient* noah = init_api_client();
    cJSON *json = get_data(noah, "https://catfact.ninja/fact");
    char *fact = cJSON_GetObjectItem(json, "fact")->valuestring;
    printf("%s\n", fact);
   

    return 0;
}