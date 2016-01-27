/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 Bo Yang <struggleyb.nku@gmail.com>
 * Copyright 2014 Rupinder Singh Khokhar <rsk1coder99@gmail.com>
 */

#include <assert.h>
#include <stdlib.h>

#include <dom/html/html_image_element.h>

#include "html/html_document.h"
#include "html/html_image_element.h"

#include "core/node.h"
#include "core/attr.h"
#include "utils/utils.h"

static struct dom_element_protected_vtable _protect_vtable = {
	{
		DOM_NODE_PROTECT_VTABLE_HTML_IMAGE_ELEMENT
	},
	DOM_HTML_IMAGE_ELEMENT_PROTECT_VTABLE
};

/**
 * Create a dom_html_image_element object
 *
 * \param doc  The document object
 * \param ele  The returned element object
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception _dom_html_image_element_create(struct dom_html_document *doc,
		dom_string *namespace, dom_string *prefix,
		struct dom_html_image_element **ele)
{
	struct dom_node_internal *node;

	*ele = malloc(sizeof(dom_html_image_element));
	if (*ele == NULL)
		return DOM_NO_MEM_ERR;

	/* Set up vtables */
	node = (struct dom_node_internal *) *ele;
	node->base.vtable = &_dom_html_element_vtable;
	node->vtable = &_protect_vtable;

	return _dom_html_image_element_initialise(doc, namespace, prefix, *ele);
}

/**
 * Initialise a dom_html_image_element object
 *
 * \param doc  The document object
 * \param ele  The dom_html_image_element object
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception _dom_html_image_element_initialise(struct dom_html_document *doc,
		dom_string *namespace, dom_string *prefix,
		struct dom_html_image_element *ele)
{
	return _dom_html_element_initialise(doc, &ele->base,
			doc->elements[DOM_HTML_ELEMENT_TYPE_IMG],
			namespace, prefix);
}

/**
 * Finalise a dom_html_image_element object
 *
 * \param ele  The dom_html_image_element object
 */
void _dom_html_image_element_finalise(struct dom_html_image_element *ele)
{
	_dom_html_element_finalise(&ele->base);
}

/**
 * Destroy a dom_html_image_element object
 *
 * \param ele  The dom_html_image_element object
 */
void _dom_html_image_element_destroy(struct dom_html_image_element *ele)
{
	_dom_html_image_element_finalise(ele);
	free(ele);
}

/*------------------------------------------------------------------------*/
/* The protected virtual functions */

/* The virtual function used to parse attribute value, see src/core/element.c
 * for detail */
dom_exception _dom_html_image_element_parse_attribute(dom_element *ele,
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
void _dom_virtual_html_image_element_destroy(dom_node_internal *node)
{
	_dom_html_image_element_destroy((struct dom_html_image_element *) node);
}

/* The virtual copy function, see src/core/node.c for detail */
dom_exception _dom_html_image_element_copy(dom_node_internal *old,
		dom_node_internal **copy)
{
	return _dom_html_element_copy(old, copy);
}

/*-----------------------------------------------------------------------*/
/* API functions */

#define SIMPLE_GET(attr)						\
	dom_exception dom_html_image_element_get_##attr(		\
		dom_html_image_element *element,			\
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
dom_exception dom_html_image_element_set_##attr(			\
		dom_html_image_element *element,			\
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
SIMPLE_GET_SET(align);
SIMPLE_GET_SET(alt);
SIMPLE_GET_SET(border);
SIMPLE_GET_SET(long_desc);
SIMPLE_GET_SET(src);
SIMPLE_GET_SET(use_map);

dom_exception dom_html_image_element_get_height(
	dom_html_image_element *image, dom_ulong *height)
{
	return dom_html_element_get_dom_ulong_property(&image->base, "height",
			SLEN("height"), height);
}

dom_exception dom_html_image_element_set_height(
	dom_html_image_element *image, dom_ulong height)
{
	return dom_html_element_set_dom_ulong_property(&image->base, "height",
			SLEN("height"), height);
}

dom_exception dom_html_image_element_get_hspace(
	dom_html_image_element *image, dom_ulong *hspace)
{
	return dom_html_element_get_dom_ulong_property(&image->base, "hspace",
			SLEN("hspace"), hspace);
}

dom_exception dom_html_image_element_set_hspace(
	dom_html_image_element *image, dom_ulong hspace)
{
	return dom_html_element_set_dom_ulong_property(&image->base, "hspace",
			SLEN("hspace"), hspace);
}

dom_exception dom_html_image_element_get_vspace(
	dom_html_image_element *image, dom_ulong *vspace)
{
	return dom_html_element_get_dom_ulong_property(&image->base, "vspace",
			SLEN("cspace"), vspace);
}

dom_exception dom_html_image_element_set_vspace(
	dom_html_image_element *image, dom_ulong vspace)
{
	return dom_html_element_set_dom_ulong_property(&image->base, "vspace",
			SLEN("vsapce"), vspace);
}

dom_exception dom_html_image_element_get_width(
	dom_html_image_element *image, dom_ulong *width)
{
	return dom_html_element_get_dom_ulong_property(&image->base, "width",
			SLEN("width"), width);
}

dom_exception dom_html_image_element_set_width(
	dom_html_image_element *image, dom_ulong width)
{
	return dom_html_element_set_dom_ulong_property(&image->base, "width",
			SLEN("width"), width);
}

/**
 * Get the is_map property
 *
 * \param ele		The dom_html_image_element object
 * \param is_map	The status
 * \return 		DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception dom_html_image_element_get_is_map(dom_html_image_element *ele,
				                bool *is_map)
{
	return dom_html_element_get_bool_property(&ele->base, "ismap",
			SLEN("ismap"), is_map);
}

/**
 * Set the is_map property
 *
 * \param ele		The dom_html_image_element object
 * \param is_map	The status
 * \return 		DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception dom_html_image_element_set_is_map(dom_html_image_element *ele,
				                bool is_map)
{
	return dom_html_element_set_bool_property(&ele->base, "ismap",
			SLEN("ismap"), is_map);
}

