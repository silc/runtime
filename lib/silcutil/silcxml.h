/*

  silcxml.h

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

/****h* silcutil/XML Interface
 *
 * DESCRIPTION
 *
 * XML parser interface provides simple stream based interface for parsing
 * XML data and files.
 *
 * EXAMPLE
 *
 * SilcXMLParser parser;
 *
 * // Create XML parser
 * parser = silc_xml_parser_create(NULL, &handler, ctx);
 *
 * // Parse XML file
 * if (!silc_xml_parse_file(parser, filename)) {
 *   silc_errno_location(NULL, &cur_line, NULL);
 *   fatal("Error %s:%d: %s", filename, cur_line, silc_errno_reason());
 * }
 *
 * // Free parser
 * silc_xml_parser_free(parser);
 *
 ***/

#ifndef SILCXML_H
#define SILCXML_H

/****s* silcutil/SilcXMLParser
 *
 * NAME
 *
 *    typedef struct SilcXMLParserStruct *SilcXMLParser;
 *
 * DESCRIPTION
 *
 *    The XLM parser context allocated by silc_xml_parser_create.  It is
 *    freed by calling silc_xml_parser_free.
 *
 ***/
typedef struct SilcXMLParserStruct *SilcXMLParser;

/****s* silcutil/SilcXMLParserHandler
 *
 * NAME
 *
 *    typedef struct SilcXMLParserHandlerObject { ... }
 *                   SilcXMLParserHandler, SilcXMLParserHandlerStruct;
 *
 * DESCRIPTION
 *
 *    The XML parser handler function callbacks are declared in this
 *    structure.  The structure is given as argument to the
 *    silc_xml_parser_create.
 *
 * SOURCE
 */
typedef struct SilcXMLParserHandlerObject {
  /* Called at the start of an XML element.  The `name' is the element name.
     The `attributes' is the element attributes or NULL if there were no
     attributes.  The `attributes' may be enumerated using the SilcHashTable
     API.  The silc_xml_get_attribute can be used to retrieve the attribute
     values from the `attributes' by their name. */
  void (*start_element)(SilcXMLParser parser,
			const char *name,
			SilcHashTable attributes,
			void *context);

  /* Called and the end of an XML element.  The `name' is the element name. */
  void (*end_element)(SilcXMLParser parser,
		      const char *name,
		      void *context);

  /* Called to deliver the characters or whatever data is in the element. */
  void (*data)(SilcXMLParser parser,
	       const unsigned char *data,
	       SilcUInt32 data_len,
	       void *context);

  /* Called to deliver a processing instruction.  The `target' is the first
     word in the processing instruction.  The `data' is the rest of the
     characters in it skipping all whitespace after the initial word.  This
     callback may be NULL if it is not needed. */
  void (*pi)(SilcXMLParser parser,
	     const char *target,
	     const char *data,
	     void *context);
} *SilcXMLParserHandler, SilcXMLParserHandlerStruct;
/***/

/****s* silcutil/SilcXMLParams
 *
 * NAME
 *
 *    typedef struct SilcXMLParamsObject { ... }
 *                                *SilcXMLParams, SilcXMLParamsStruct;
 *
 * DESCRIPTION
 *
 *    The XML parser parameters that can be give as argument to the
 *    silc_xml_parser_create.
 *
 * SOURCE
 */
typedef struct SilcXMLParamsObject {
  /* Do not process XML namespaces. */
  SilcBool no_namespace;
} *SilcXMLParams, SilcXMLParamsStruct;
/***/

/****f* silcutil/silc_xml_parser_create
 *
 * SYNOPSIS
 *
 *    SilcXMLParser silc_xml_parser_create(SilcXMLParams params,
 *                                         SilcXMLParserHandler handler,
 *                                         void *context);
 *
 * DESCRIPTION
 *
 *    Create XML parser and return in.  The `handler' contains the callback
 *    functions to be called while parsing XML data.  The `context' is
 *    delivered to each callback function.  The `params' define parser
 *    parameters, and may be NULL.  The parser parses XML data with UTF-8
 *    encoding.  All characters delivered to callbacks are in UTF-8 encoding.
 *
 ***/
SilcXMLParser silc_xml_parser_create(SilcXMLParams params,
				     SilcXMLParserHandler handler,
				     void *context);

/****f* silcutil/silc_xml_parser_free
 *
 * SYNOPSIS
 *
 *    void silc_xml_parser_free(SilcXMLParser parser);
 *
 * DESCRIPTION
 *
 *    Free's XML parser.
 *
 ***/
void silc_xml_parser_free(SilcXMLParser parser);

/****f* silcutil/silc_xml_parse
 *
 * SYNOPSIS
 *
 *    SilcBool silc_xml_parse(SilcXMLParser parser,
 *                            const unsigned char *data,
 *                            SilcUInt32 data_len);
 *
 * DESCRIPTION
 *
 *    Parse XML data `data' of length of `data_len' bytes.  Returns TRUE
 *    after the data has been parsed.  The handler callback functions set for
 *    `parser' will be called while parsing the XML data.
 *
 *    Returns FALSE and set silc_errno and silc_errno_reason if error
 *    occurs.
 *
 ***/
SilcBool silc_xml_parse(SilcXMLParser parser,
			const unsigned char *data,
			SilcUInt32 data_len);

/****f* silcutil/silc_xml_parse_file
 *
 * SYNOPSIS
 *
 *    SilcBool silc_xml_parse_file(SilcXMLParser parser,
 *                                 const char *filename);
 *
 * DESCRIPTION
 *
 *    Parse XML file indicated by `filename'.  Returns TRUE after the file
 *    has been parsed.  The handler callback functions set for `parser' will
 *    be called while parsing the XML file.
 *
 *    Returns FALSE and set silc_errno and silc_errno_reason if error
 *    occurs.  The silc_errno_location can be used to retrieve the exact
 *    location in the file where the error occurred.
 *
 ***/
SilcBool silc_xml_parse_file(SilcXMLParser parser,
			     const char *filename);

/****f* silcutil/silc_xml_get_attribute
 *
 * SYNOPSIS
 *
 *    const char *silc_xml_get_attribute(SilcXMLParser parser,
 *                                       SilcHashTable attributes,
 *                                       const char *name);
 *
 * DESCRIPTION
 *
 *    Returns the value of the attributes namaed `name' or NULL if no such
 *    attribute exist in the hash table of `attributes'.
 *
 ***/
const char *silc_xml_get_attribute(SilcXMLParser parser,
				   SilcHashTable attributes,
				   const char *name);

/****f* silcutil/silc_xml_current_location
 *
 * SYNOPSIS
 *
 *    void silc_xml_current_location(SilcXMLParser parser,
 *                                   SilcUInt32 *current_line,
 *                                   SilcUInt32 *current_column);
 *
 * DESCRIPTION
 *
 *    Return the current location of the parsed XML data.  The current line
 *    number and columns can be returned.  This may be used also when an
 *    error occurs but it is preferred to use silc_errno_location in case
 *    of error.
 *
 ***/
void silc_xml_current_location(SilcXMLParser parser,
			       SilcUInt32 *current_line,
			       SilcUInt32 *current_column);

#endif /* SILCXML_H */
