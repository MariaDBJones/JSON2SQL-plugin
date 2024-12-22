#include "common.h"

#define DELETELOG 1

char* handle_delete_request(const char *url) {
// initialize the JSON answer
    cJSON *json = cJSON_CreateObject();
    char *answerget;
    char version[64]="v1";
    char resource[64]="";
    char schema[64]="";
    char object[64]="";
    char column[64]="";
    char value[64]="";
    char *query=NULL;
    unsigned int action = 0;
    MYSQL *cnx;
// 2 = sql needed
// 1 = direct answere
// 0 = exec not possible

// extracting tokens from url
    int tokens = sscanf(url, "/%63[^/]/%63[^/]/%63[^/]/%63[^/]/%63[^/]/%63[^/]", version, resource, schema, object, column, value);
// end token extraction

int isresourcent=0;
if (strchr(resource, '\0') != NULL) {
// String is null-terminated
    isresourcent=1;
}
    cJSON_AddStringToObject(json, "status", "HANDLER");
    cJSON_AddStringToObject(json, "method", "DELETE");
    cJSON_AddStringToObject(json, "url", url);
    cJSON_AddNumberToObject(json, "tokens", tokens);

// analyzing toeksn to match patterns and resources
// 4 possibilites :
// - tokens = 2 && resource in (ping, healthcheck, status)
// - tokens = 4 && resource = tables
// - tokens = 6 && resource = tables
// - bad request format
switch(tokens) {
    case 4:
#if DELETELOG == 1
      cJSON_AddStringToObject(json, "version", version);
      cJSON_AddStringToObject(json, "resource", resource);
      cJSON_AddStringToObject(json, "schema", schema);
      cJSON_AddStringToObject(json, "table", object);
      cJSON_AddNumberToObject(json, "sizeof resource",sizeof(resource));
      cJSON_AddNumberToObject(json, "strchr resource null is not null", isresourcent);
      cJSON_AddStringToObject(json, "action",   "QUERY");
#endif // GETLOG
      cJSON_AddStringToObject(json, "message",  "developped later");
      cJSON_AddNumberToObject(json, "httpcode", HTTP_OK);
    break; // end tokens=4
   case 6:
#if DELETELOG == 1
      cJSON_AddStringToObject(json, "version", version);
      cJSON_AddStringToObject(json, "resource", resource);
      cJSON_AddStringToObject(json, "schema", schema);
      cJSON_AddStringToObject(json, "table", object);
      cJSON_AddStringToObject(json, "column", column);
      cJSON_AddStringToObject(json, "value", value);
      cJSON_AddNumberToObject(json, "strlen resource",strlen(resource));
      cJSON_AddNumberToObject(json, "sizeof resrouce",sizeof(resource));
      cJSON_AddNumberToObject(json, "strchr resource null is not null", isresourcent);
      cJSON_AddStringToObject(json, "action",   "QUERY");
#endif // DELETELOG
      char tmp[1]="";
      int length = snprintf(tmp, 1, "DELETE FROM %s.%s WHERE %s = '%s'", schema, object, column, value);
      query=malloc(length+1);
      snprintf(query, length+1 , "DELETE FROM %s.%s WHERE %s = '%s'", schema, object, column, value);
#if DELETELOG == 1
      cJSON_AddNumberToObject(json, "query length", length);
      cJSON_AddNumberToObject(json, "final query length", strlen(query));
      cJSON_AddStringToObject(json, "query", query);
#endif
    break; // end tokens=6
    default:
      cJSON_AddStringToObject(json, "action", "ERROR");
      cJSON_AddStringToObject(json, "error", "Invalid DELETE request");
      cJSON_AddNumberToObject(json, "httpcode", HTTP_BAD_REQUEST);
      answerget = cJSON_PrintUnformatted(json);
      cJSON_Delete(json);
      free(query);
      return answerget;
}
// end decision making

log_message("query time");
    if (query != NULL) {
// assigned
        cJSON_AddStringToObject(json, "todo", query);
log_message("cnx time");
// we establish internal local connexion
        cnx = mysql_init(NULL);
if (!cnx) {
log_message("cnx init failed");
          cJSON_AddStringToObject(json, "cnx status", "CONNECTION failed");
          cJSON_AddNumberToObject(json, "mariadbcode", mysql_errno(cnx));
          cJSON_AddNumberToObject(json, "httpcode", HTTP_INTERNAL_SERVER_ERROR);
          // clean exit procedure
          char *json_string = cJSON_PrintUnformatted(json);
          cJSON_Delete(json);
          return json_string; // Caller is responsible for freeing this memory
}
log_message("cnx init ok");
//        if (mysql_real_connect_local(cnx) == NULL) {
          if (mysql_real_connect(cnx, "localhost", NULL, NULL, NULL, 3306, NULL, 0) == NULL ) {
log_message("cnx failed");
          cJSON_AddStringToObject(json, "cnx status", "CONNECTION failed");
          cJSON_AddNumberToObject(json, "mariadbcode", mysql_errno(cnx));
          cJSON_AddNumberToObject(json, "httpcode", HTTP_INTERNAL_SERVER_ERROR);
          // clean exit procedure
          char *json_string = cJSON_PrintUnformatted(json);
          cJSON_Delete(json);
          return json_string; // Caller is responsible for freeing this memory
        } else {
log_message("cnx ok");
          cJSON_AddStringToObject(json, "cnx status", "CONNECTION OK");
          cJSON_AddNumberToObject(json, "mariadbcode", mysql_errno(cnx));
          cJSON_AddNumberToObject(json, "httpcode", HTTP_OK);
          mysql_close(cnx);
        }
    }
#if DELETELOG == 1
else {
//not assigned
log_message("no query");
        cJSON_AddStringToObject(json, "todo", "nothing");
    }
#endif // GETLOG

// housekeeping
    answerget = cJSON_PrintUnformatted(json);
    cJSON_Delete(json);
    free(query);
//end housekeeping

return answerget;
} // end function
