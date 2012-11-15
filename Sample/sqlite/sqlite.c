#include <sqlite3.h>
#include <stdio.h>
//#include <stdlib.h>
#include <stdarg.h>

sqlite3 *openSqlite(char *dbname)
{
  sqlite3 *db;
  if(sqlite3_open(dbname, &db) != SQLITE_OK)
    {
      sqlite3_errmsg(db);
      return NULL;
    }
  return db;
}

int cback(void *arg, int resultnm, char **result, char **column)
{
  int i;

  for(i = 0; i < resultnm; i++)
    {
      if(strcmp(*(column + i), "id") == 0)
	printf("id:%s ", *(result + i));
      if(strcmp(*(column + i), "name") == 0)
	printf("name:%s ", *(result + i));
      printf("\n");
    }


  return 0;
}

/* char *sqlgen(const cahr *format, ...) */
/* { */
/*   //  const char * sql = sqlite3_mprintf("select c.displayname, m.from_dispname, m.body_xml from Messages as m inner join Conversations as c on m.convo_id = c.id where author not like '%s' and body_xml like '%%%s%%' and timestamp > %d order by timestamp", pconf->skype.user, key, (int)past); */
/* } */

int runSql(sqlite3 *db, char *sql, int (*callback)(void *, int, char **, char **))
{
  char *errmesg;
  if(sqlite3_exec(db, "BEGIN", NULL, NULL, &errmesg) != SQLITE_OK)
    {
      sqlite3_errmsg(db);
      fprintf(stderr, "%s\n", errmesg);
      return -1;
    }

  if(sqlite3_exec(db, sql, callback, NULL, &errmesg) != SQLITE_OK)
    {
      sqlite3_errmsg(db);
      fprintf(stderr, "%s\n", errmesg);
      return -1;
    }

  if(sqlite3_exec(db, "COMMIT", NULL, NULL, &errmesg) != SQLITE_OK)
    {
      sqlite3_errmsg(db);
      fprintf(stderr, "%s\n", errmesg);
      return -1;
    }
  return 0;
}

#define checkArgs(c, v, n)			\
  if(c != n)					\
    {						\
      fprintf(stderr, "%s [database]\n", *v);	\
      return -1;				\
    }

int main(int argc, char **argv)
{
  sqlite3 *db;
  char *sql;

  checkArgs(argc, argv, 2);
  if((db = openSqlite(argv[1])) == NULL)
    {
      fprintf(stderr, "openSqlite");
      return -1;
    }

  runSql(db, "select * from testcode", cback);
  
  sqlite3_close(db);
  
  return 0;
}
