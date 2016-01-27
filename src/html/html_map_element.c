/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 Bo Yang <struggleyb.nku@gmail.com>
 * Copyright 2014 Rupinder Singh Khokhar<rsk1coder99@gmail.com>
 */
#include <assert.h>
#include <stdlib.h>

#include <dom/html/html_map_element.h>

#include "html/html_document.h"
#include "html/html_map_element.h"

#include "html/html_collection.h"

#include "core/node.h"
#include "core/attr.h"
#include "utils/utils.h"

static struct dom_element_protected_vtable _protect_vtable = {
	{
		DOM_NODE_PROTECT_VTABLE_HTML_MAP_ELEMENT
	},
	DOM_HTML_MAP_ELEMENT_PROTECT_VTABLE
};

/**
 * Create a dom_html_map_element object
 *
 * \param doc  The document object
 * \param ele  The returned element object
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception _dom_html_map_element_create(struct dom_html_document *doc,
		dom_string *namespace, dom_string *prefix,
		struct dom_html_map_element **ele)
{
	struct dom_node_internal *node;

	*ele = malloc(sizeof(dom_html_map_element));
	if (*ele == NULL)
		return DOM_NO_MEM_ERR;

	/* Set up vtables */
	node = (struct dom_node_internal *) *ele;
	node->base.vtable = &_dom_html_element_vtable;
	node->vtable = &_protect_vtable;

	return _dom_html_map_element_initialise(doc, namespace, prefix, *ele);
}

/**
 * Initialise a dom_html_map_element object
 *
 * \param doc  The document object
 * \param ele  The dom_html_map_element object
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception _dom_html_map_element_initialise(struct dom_html_document *doc,
		dom_string *namespace, dom_string *prefix,
		struct dom_html_map_element *ele)
{
	return _dom_html_element_initialise(doc, &ele->base,
			doc->elements[DOM_HTML_ELEMENT_TYPE_MAP],
			namespace, prefix);
}

/**
 * Finalise a dom_html_map_element object
 *
 * \param ele  The dom_html_map_element object
 */
void _dom_html_map_element_finalise(struct dom_html_map_element *ele)
{
	_dom_html_element_finalise(&ele->base);
}

/**
 * Destroy a dom_html_map_element object
 *
 * \param ele  The dom_html_map_element object
 */
void _dom_html_map_element_destroy(struct dom_html_map_element *ele)
{
	_dom_html_map_element_finalise(ele);
	free(ele);
}


/*------------------------------------------------------------------------*/
/* The protected virtual functions */

/* The virtual function used to parse attribute value, see src/core/element.c
 * for detail */
dom_exception _dom_html_map_element_parse_attribute(dom_element *ele,
		dom_string *name, dom_string *value,
		dom_string **parsed)
{
	UNUSED(ele);
	UNUSED(name);

	dom_string_ref(value);
	*parsed = value;

	return DOM_NO_ERR;
}

/* The virtual destroy function, see src/core/node.c for detail */
void _dom_virtual_html_map_element_destroy(dom_node_internal *node)
{
	_dom_html_map_element_destroy((struct dom_html_map_element *) node);
}

/* The virtual copy function, see src/core/node.c for detail */
dom_exception _dom_html_map_element_copy(dom_node_internal *old,
		                dom_node_internal **copy)
{
	return _dom_html_element_copy(old, copy);
}

/*-----------------------------------------------------------------------*/
/* API functions */

#define SIMPLE_GET(attr)						\
	dom_exception dom_html_map_element_get_##attr(		\
		dom_html_map_element *element,			\
		dom_string **attr)					\
	{								\
		dom_exception ret;					\
		dom_string *_memo_##attr;				\
									\
		_memo_##attr =						\
			((struct dom_html_document *)			\
			 ((struct dom_node_internal *)element)->owner)->\
			memoised[hds_##attr];				\
									\
		ret = dom_element_get_attribute(element, _memo_##attr, attr); \
									\
		return ret;						\
	}
#define SIMPLE_SET(attr)						\
dom_exception dom_html_map_element_set_##attr(			\
		dom_html_map_element *element,			\
		dom_string *attr)					\
	{								\
		dom_exception ret;					\
		dom_string *_memo_##attr;				\
									\
		_memo_##attr =						\
			((struct dom_html_document *)			\
			 ((struct dom_node_internal *)element)->owner)->\
			memoised[hds_##attr];				\
									\
		ret = dom_element_set_attribute(element, _memo_##attr, attr); \
									\
		return ret;						\
	}

#define SIMPLE_GET_SET(attr) SIMPLE_GET(attr) SIMPLE_SET(attr)

SIMPLE_GET_SET(name);

/* The callback function for  _dom_html_collection_create*/
bool callback(struct dom_node_internal *node, void *ctx)
{
	if(node->type == DOM_ELEMENT_NODE &&
			dom_string_caseless_isequal(node->name,
				((dom_html_document *)ctx)->elements[DOM_HTML_ELEMENT_TYPE_AREA]))
	{
		return true;
	}

	return false;
}

/**
 * Get the areas property
 *
 * \param ele		The dom_html_map_element object
 * \param areas		The returned dom_html_collection object
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */

dom_exception dom_html_map_element_get_areas(
		dom_html_map_element *ele, dom_html_collection **areas)
{
	dom_html_document *doc
		= (dom_html_document *) ((dom_node_internal *) ele)->owner;
	
	/*doc is passed as a parameter to callback to avoid repeated calculations */
	return _dom_html_collection_create(doc, (dom_node_internal *) ele,
			callback, (void *) doc, areas);
}
