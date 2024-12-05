//include
#include "global-json-api.h"

static char* handle_patch_request(const char *url, const char *upload_data, size_t *upload_data_size) {
// initialize the JSON answer
    cJSON *json = cJSON_CreateObject(); 
// local variables
    char schema[64];  
    char table[64];
    char colname[64];
    char colvalue[64];
// check request format, parameter extraction & statement exec
  if (sscanf(url, "/v1/tables/%64[^/]/%64s", schema, table, colname, colvalue) == 4) {  
    // initializing the variables 
    char query[512]="";
// Here query database
    snprintf(query, sizeof(query), "'UPDATE %s.%s'", schema, table);
// looping over request body to extract 
// CODE TO BE WRITTEN
      
// we establish internal local connexion
        MYSQL *connection = mysql_init(NULL);
        if (mysql_real_connect_local(connection) == NULL) { 
          fprintf(stderr, "mysql_init/mysql_real_connect_local failed\n");
          cJSON_AddStringToObject(json, "status", "CONNECTION failed");
          cJSON_AddNumberToObject(json, "mariadbcode", mysql_errno(connection));
          cJSON_AddNumberToObject(json, "httpcode", HTTP_INTERNAL_SERVER_ERROR);
          // clean exit procedure         
          char *json_string = cJSON_PrintUnformatted(json);
          cJSON_Delete(json);
          return json_string; // Caller is responsible for freeing this memory
        }
// execute the query      
        if (mysql_real_query(connection, STRING_WITH_LEN(query))) {
          fprintf(stderr, "mysql_query() failed\n");
          cJSON_AddStringToObject(json, "status", "QUERY failed");
          cJSON_AddNumberToObject(json, "mariadbcode", mysql_errno(connection));
          cJSON_AddNumberToObject(json, "httpcode", HTTP_INTERNAL_SERVER_ERROR);
          // clean exit procedure         
          char *json_string = cJSON_PrintUnformatted(json);
          mysql_close(connection);
          cJSON_Delete(json);
          return json_string; // Caller is responsible for freeing this memory
        } else 
        {
// resulset to json translation
           cJSON_AddStringToObject(json, "status", "OK");
           cJSON_AddNumberToObject(json, "rows", mysql_affected_rows(resultset));
          cJSON_AddNumberToObject(json, "mariadbcode", mysql_errno(connection));
          cJSON_AddNumberToObject(json, "httpcode", HTTP_OK);
// clean exit procedure w/ housekeeping
          char *json_string = cJSON_PrintUnformatted(json);
          cJSON_Delete(json);
          mysql_close(connection);
          mysql_free_result(resultset);
          return json_string; // Caller is responsible for freeing this memory;
        }
    }
// request format is KO
  cJSON_AddStringToObject(json, "error", "Invalid POST request");
  cJSON_AddNumberToObject(json, "code", HTTP_BAD_REQUEST);
// clean exit procedure w/ housekeeping
  char *json_string = cJSON_PrintUnformatted(json);
  cJSON_Delete(json);
  return json_string; // Caller is responsible for freeing this memory    
  }
