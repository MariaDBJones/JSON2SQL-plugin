# Translate API GET requests into read statements  
  
* GET /v1/tables/:schema/:table/:colname/:colvalue → SELECT * FROM SCHEMA.TABLE WHERE COLNAME = COLVALUE

* GET /v1/status/                                  → SHOW GLOBAL STATUS
  
* GET /v1/                                         → OK

## Function declaration

in [handle_get_request.h]() : static char* response handle_get_request(const char *url)  

## Source file

[handle_get_request.c]()

## RESULT  

### TABLES resource

* If 0 row found  
{
  "status": "NO DATA FOUND",
  "rows": 0,
  "mariadbcode": 0,
  "httpcode": 200
}

* If 1+ rows found  
{
  "status": "OK",
  "mariadbcode": 0,
  "httpcode": 200,
  "rows": 3,
  "data": [
    {
      "column1": "value1",
      "column2": "value2",
      ...
    },
    {
      "column1": "value1",
      "column2": "value2",
      ...
    },
    ...
  ]

}
### STATUS resource

### v1 resource
{
  "status": "OK",
  "httpcode": 200
}
