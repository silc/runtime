/*

  silcxml.c

  Author: Pekka Riikonen <priikone@silcnet.org>

  Copyright (C) 2008 Pekka Riikonen

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; version 2 of the License.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

*/

#include <silcruntime.h>

/* XML parser context */
struct SilcXMLParserStruct {
  void *parser;			       /* Parser implementation */
  SilcXMLParserHandlerStruct handler;  /* Handler */
  void *context;		       /* User context */
  SilcXMLParamsStruct params;	       /* Parser parameters */
};

#ifdef HAVE_EXPAT_H

#include <expat.h>

/* Map expat error to silc_errno */

static SilcResult silc_xml_expat_error(XML_Parser parser)
{
  enum XML_Error error = XML_GetErrorCode(parser);

  switch (error) {
  case XML_ERROR_NONE:
    return SILC_OK;
  case XML_ERROR_NO_MEMORY:
    return SILC_ERR_OUT_OF_MEMORY;
  case XML_ERROR_UNKNOWN_ENCODING:
  case XML_ERROR_INCORRECT_ENCODING:
    return SILC_ERR_BAD_ENCODING;
  case XML_ERROR_ABORTED:
    return SILC_ERR_ABORTED;
  default:
    return SILC_ERR_SYNTAX;
  }
}

/* Return error string */

static const char *silc_xml_get_error(SilcXMLParser parser)
{
  return XML_ErrorString(XML_GetErrorCode(parser->parser));
}

/* Start element */

static void silc_xml_expat_start_element(void *userData,
					 const XML_Char *name,
					 const XML_Char **atts)
{
  SilcXMLParser parser = userData;
  SilcHashTable t = NULL;
  int i;

  if (atts && atts[0]) {
    t = silc_hash_table_alloc(NULL, 0, silc_hash_utf8_string, NULL,
			      silc_hash_utf8_compare, NULL,
			      NULL, NULL, TRUE);
    if (!t) {
      XML_StopParser(parser->parser, FALSE);
      return;
    }

    for (i = 0; atts[i]; i += 2)
      silc_hash_table_add(t, (void *)atts[i], (void *)atts[i + 1]);
  }

  if (parser->handler.start_element)
    parser->handler.start_element(parser, name, t, parser->context);

  if (t)
    silc_hash_table_free(t);
}

/* End element */

static void silc_xml_expat_end_element(void *userData,
				       const XML_Char *name)
{
  SilcXMLParser parser = userData;

  if (parser->handler.end_element)
    parser->handler.end_element(parser, name, parser->context);
}

/* Characters */

static void silc_xml_expat_data(void *userData,
				const XML_Char *s,
				int len)

{
  SilcXMLParser parser = userData;

  if (parser->handler.data)
    parser->handler.data(parser, (const unsigned char *)s,
			 (SilcUInt32)len, parser->context);
}

/* Processing instruction */

static void silc_xml_expat_pi(void *userData,
			      const XML_Char *target,
			      const XML_Char *data)
{
  SilcXMLParser parser = userData;

  if (parser->handler.pi)
    parser->handler.pi(parser, target, data, parser->context);
}

/* Create parser */

SilcXMLParser silc_xml_parser_create(SilcXMLParams params,
				     SilcXMLParserHandler handler,
				     void *context)
{
  SilcXMLParser parser;
  XML_Parser ep;

  parser = silc_calloc(1, sizeof(*parser));
  if (!parser)
    return NULL;

  SILC_LOG_DEBUG(("Allcoated XML parser %p", parser));

  if (params)
    parser->params = *params;
  if (handler)
    parser->handler = *handler;
  parser->context = context;

  /* Allocate expat parser */
  if (parser->params.no_namespace)
    ep = XML_ParserCreate("UTF-8");
  else
    ep = XML_ParserCreateNS("UTF-8", '\0');

  if (!ep) {
    silc_set_errno(SILC_ERR_OUT_OF_MEMORY);
    silc_free(ep);
    return NULL;
  }

  parser->parser = ep;

  /* Set callbacks */
  XML_SetUserData(ep, parser);
  XML_SetElementHandler(ep, silc_xml_expat_start_element,
			silc_xml_expat_end_element);
  XML_SetCharacterDataHandler(ep, silc_xml_expat_data);
  XML_SetProcessingInstructionHandler(ep, silc_xml_expat_pi);

  return parser;
}

/* Free parser */

void silc_xml_parser_free(SilcXMLParser parser)
{
  if (!parser)
    return;

  SILC_LOG_DEBUG(("Free XML parser %p", parser));

  if (parser->parser)
    XML_ParserFree(parser->parser);
  silc_free(parser);
}

/* Parse */

SilcBool silc_xml_parse(SilcXMLParser parser,
			const unsigned char *data,
			SilcUInt32 data_len)
{
  int ret;

  SILC_LOG_DEBUG(("Parse XML data with parser %p", parser));

  if (!parser || !data) {
    silc_set_errno(SILC_ERR_INVALID_ARGUMENT);
    return FALSE;
  }

  /* Parse */
  ret = XML_Parse(parser->parser, (const char *)data, (int)data_len, 1);
  if (!ret) {
    silc_set_errno_reason(silc_xml_expat_error(parser->parser),
			  silc_xml_get_error(parser));
    return FALSE;
  }

  return TRUE;
}

/* Parse file */

SilcBool silc_xml_parse_file(SilcXMLParser parser,
			     const char *filename)
{
  unsigned char *data;
  SilcUInt32 data_len;
  SilcBool ret;

  if (!filename) {
    silc_set_errno(SILC_ERR_INVALID_ARGUMENT);
    return FALSE;
  }

  SILC_LOG_DEBUG(("Parse XML file '%s' with parser %p", filename, parser));

  data = silc_file_readfile(filename, &data_len, NULL);
  if (!data)
    return FALSE;

  ret = silc_xml_parse(parser, data, data_len);
  if (!ret) {
    silc_free(data);
    silc_set_errno_reason(silc_xml_expat_error(parser->parser),
			  silc_xml_get_error(parser));
    silc_set_errno_location(filename,
			    XML_GetCurrentLineNumber(parser->parser),
			    XML_GetCurrentColumnNumber(parser->parser));
    return FALSE;
  }

  silc_free(data);

  return ret;
}

/* Get attribute */

const char *silc_xml_get_attribute(SilcXMLParser parser,
				   SilcHashTable attributes,
				   const char *name)
{
  char *val;

  if (!attributes)
    return NULL;

  if (!silc_hash_table_find(attributes, (void *)name, NULL, (void *)&val))
    return NULL;

  return val;
}

/* Return current location */

void silc_xml_current_location(SilcXMLParser parser,
			       SilcUInt32 *current_line,
			       SilcUInt32 *current_column)
{
  if (current_line)
    *current_line = XML_GetCurrentLineNumber(parser->parser);
  if (current_column)
    *current_column = XML_GetCurrentColumnNumber(parser->parser);
}

#endif /* HAVE_EXPAT_H */
