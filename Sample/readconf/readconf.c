#include <stdio.h>
#include <glib-2.0/glib.h>
#include "readconf.h"

config_t *pconf;

int sampleAdd()
{
  FILE *fp;
  gchar *pdata;
  gsize len;
  
  /* pconf->setting.interval = 300; */
  /* pconf->setting.keywordfile = "/etc/sls.key"; */
  /* pconf->skype.user = "username"; */
  
  /* g_key_file_set_integer(keyfile, GROUP_SETTING, KEY_INTERVAL, pconf->setting.interval); */
  /* g_key_file_set_string(keyfile, GROUP_SETTING, KEY_KEYWORDFILE, pconf->setting.keywordfile); */
  /* g_key_file_set_string(keyfile, GROUP_SKYPE, KEY_USER, pconf->skype.user); */
  
  /* if(!(pdata = g_key_file_to_data(keyfile, &len, &error))) */
  /* 	perror("g_key_file_to_data"); */
  
  /* if(!(fp = fopen(conffile, "w"))) */
  /* 	perror("fopen"); */
  
  /* if(fwrite(pdata, len, 1, fp) < 1) */
  /* 	perror("fwrite"); */
  
  /* fclose(fp); */
  return 0;
}

int getSetting()
{
  /* pconf->setting.interval = (int)g_key_file_get_integer(keyfile, GROUP_SETTING, KEY_INTERVAL, NULL); */
  /* pconf->setting.keywordfile = (char *)g_key_file_get_string(keyfile, GROUP_SETTING, KEY_KEYWORDFILE, NULL); */
  /* pconf->skype.user = g_key_file_get_string(keyfile, GROUP_SKYPE, KEY_USER, NULL); */
  return 0;
}

config_t *readconf(char *conffile)
{
  GKeyFile *keyfile;
  GError *error = NULL;

  pconf = g_slice_new(config_t);
  keyfile = g_key_file_new();

  if(!g_key_file_load_from_file(keyfile, conffile, G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS, &error))
    {
      perror(conffile);
      sampleAdd();
      return NULL;
    }

  getSetting();
  g_key_file_free(keyfile);

  return pconf;
}
