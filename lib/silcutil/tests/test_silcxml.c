/* SILC XML tests */

#include "silcruntime.h"

SilcXMLParser parser;
SilcBool success = FALSE;

static void xml_start_element(SilcXMLParser parser,
			      const char *name,
			      SilcHashTable attributes,
			      void *context)
{
  fprintf(stderr, "<%s", name);

  if (attributes) {
    SilcHashTableList htl;
    char *att, *val;

    silc_hash_table_list(attributes, &htl);
    while (silc_hash_table_get(&htl, (void *)&att, (void *)&val))
      fprintf(stderr, " %s='%s'", att, val);

    silc_hash_table_list_reset(&htl);
  }
  fprintf(stderr, ">");
}

static void xml_end_element(SilcXMLParser parser,
			    const char *name,
			    void *context)
{
  fprintf(stderr, "</%s>", name);
}

static void xml_data(SilcXMLParser parser,
		     const unsigned char *data,
		     SilcUInt32 data_len,
		     void *context)
{
  silc_file_write(2, data, data_len);
}

static void xml_pi(SilcXMLParser parser,
		   const char *target,
		   const char *data,
		   void *context)
{
  fprintf(stderr, "%s %s", target, data);
}

static SilcXMLParserHandlerStruct handler =
{
  xml_start_element,
  xml_end_element,
  xml_data,
  xml_pi
};

int main(int argc, char **argv)
{
  SilcXMLParamsStruct params;
  SilcUInt32 cur_line;
  char *file;

  silc_runtime_init();

  if (argc != 2) {
    fprintf(stderr, "Usage: test_silcxml <filename>\n");
    goto err;
  }

  memset(&params, 0, sizeof(params));
  parser = silc_xml_parser_create(&params, &handler, NULL);
  if (!parser)
    goto err;

  if (!silc_xml_parse_file(parser, argv[1])) {
    silc_errno_location(&file, &cur_line, NULL);
    fprintf(stderr, "%s:%d: %s\n", file, cur_line, silc_errno_reason());
    goto err;
  }

  silc_xml_parser_free(parser);

  success = TRUE;

 err:
  SILC_LOG_DEBUG(("Testing was %s", success ? "SUCCESS" : "FAILURE"));
  fprintf(stderr, "Testing was %s\n", success ? "SUCCESS" : "FAILURE");

  silc_runtime_uninit();

  return !success;
}
