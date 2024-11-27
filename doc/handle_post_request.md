# Translate API POST requests into INSERT statements  
  
* POST /v1/tables/SCHEMA/TABLE {json body} → INSERT INTO SCHEMA.TABLE (COLs...] VALUES (vals..), (vals..)  

## RESULT  
* If failure
 {
  "source": "tables",
  "schema": "schema_name",
  "table": "table_name",
  ...
  "status": " ",
  "rows": 0
} 

* If OK  
{
  "source": "tables",
  "schema": "schema_name",
  "table": "table_name",
  "status": "OK",
  "rows": n
}
