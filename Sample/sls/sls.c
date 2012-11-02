#include <getopt.h>
#include <stdio.h> 
#include <stdlib.h>
#include <sqlite3.h>  
#include <string.h>
#include <time.h>

#include <glib.h>

#define GROUP_SETTING "setting"
#define KEY_INTERVAL "interval"
#define KEY_KEYWORDFILE "keywordfile"
#define GROUP_SKYPE "skype"
#define KEY_USER "user"

typedef struct {
  struct {
    gint interval;
    gchar *keywordfile;
  } setting;

  struct {
    gchar *user;
  } skype;
} config_t;

config_t *pconf;

int readconf(char *conffilename)
{  
  GKeyFile *keyfile;
  GError *error = NULL;
  gsize len;
  gchar *pdata;
  FILE *pconffile;

  pconf = g_slice_new(config_t);
  keyfile = g_key_file_new();

  if(!g_key_file_load_from_file(keyfile, conffilename, G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS, &error))
    {
      perror("g_key_file_load_from_file");

      pconf->setting.interval = 300;
      pconf->setting.keywordfile = "/etc/sls.key";
      pconf->skype.user = "username";

      g_key_file_set_integer(keyfile, GROUP_SETTING, KEY_INTERVAL, pconf->setting.interval);
      g_key_file_set_string(keyfile, GROUP_SETTING, KEY_KEYWORDFILE, pconf->setting.keywordfile);
      g_key_file_set_string(keyfile, GROUP_SKYPE, KEY_USER, pconf->skype.user);

      if(!(pdata = g_key_file_to_data(keyfile, &len, &error)))
	perror("g_key_file_to_data");

      if(!(pconffile = fopen(conffilename, "w")))
	perror("fopen");

      if(fwrite(pdata, len, 1, pconffile) < 1)
	perror("fwrite");

      fclose(pconffile);
      return -1;
    }

  pconf->setting.interval = g_key_file_get_integer(keyfile, GROUP_SETTING, KEY_INTERVAL, NULL);
  pconf->setting.keywordfile = g_key_file_get_string(keyfile, GROUP_SETTING, KEY_KEYWORDFILE, NULL);
  pconf->skype.user = g_key_file_get_string(keyfile, GROUP_SKYPE, KEY_USER, NULL);

  g_key_file_free(keyfile);

  return 0;
}

int callback(void *arg, int argc, char **argv, char **column)
{
  int i;

  for(i = 0; i < argc; i++)
    {
      if(strcmp(column[i], "from_displayname") && column[i][0] != '\0')
	printf("Group: %s\n", *(argv + i++));
      else
	printf("Group: Private\n");

      if(strcmp(column[i], "displayname") && column[i][0] != '\0')
	printf("> %s: ", *(argv + i++));

      printf("%s", *(argv + i));
    }

  printf("\n\n");

  return SQLITE_OK;
}

int usage(char *appname, struct option *lo, int len, const char **desc)
{
  int i;

  printf("%s [", appname);
  for(i = 0; i < len; printf("%c", lo[i].val), i++);
  printf("]\n");

  for(i = 0; i < len; i++)
    {
      printf(":: -%-2c --%-20s %s\n", lo[i].val, lo[i].name, desc[i]);
    }

  return 0;
}

int main(int argc, char **argv)
{
  const char *loginuser = getenv("USER");
  time_t past, timer = time(NULL);
  sqlite3 *db;
  char dbfile[50 + 1 + strlen(loginuser) + 20];
  char *sql;
  char *errmsg;
  char key[1024];
  struct tm *t = localtime(&timer);
  FILE *fp;

  if(readconf("/etc/sls.conf") < 0)
    {
      printf("add conffile");
      return -1;
    }

  t->tm_sec -= pconf->setting.interval;

  past = mktime(t);

  sprintf(dbfile, "/Users/%s/Library/Application Support/Skype/%s/main.db", loginuser, pconf->skype.user);
  sqlite3_open(dbfile, &db);

  if (!db)
    {
      perror("sqlite3_open");
      return -1;
    }

  if((fp = fopen(pconf->setting.keywordfile, "r")) == NULL)
    {
      fprintf(stderr, "can not open file %s\n", pconf->setting.keywordfile);
      return -1;
    }

  while(fgets(key, sizeof(key) / sizeof(key[0]), fp) != NULL)
    {
      printf("Key word: %s\n", key);

      (void)sqlite3_exec(db, "BEGIN", NULL, NULL, &errmsg); 
  
      sql = sqlite3_mprintf("select c.displayname, m.from_dispname, m.body_xml from Messages as m inner join Conversations as c on m.convo_id = c.id where author not like '%s' and body_xml like '%%%s%%' and timestamp > %d order by timestamp", pconf->skype.user, key, (int)past);
      (void)sqlite3_exec(db, sql, callback, NULL, &errmsg);
  
      sql = sqlite3_mprintf("select friendlyname from Chats where name not like '%%$%s%%' friendlyname like '%%%s%%'", pconf->skype.user, key);
      (void)sqlite3_exec(db, sql, callback, NULL, &errmsg);
  
      (void)sqlite3_exec(db, "COMMIT", NULL, NULL, &errmsg); 
    }

  sqlite3_close(db);
  
  return 0;
}
