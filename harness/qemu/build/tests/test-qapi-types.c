/* AUTOMATICALLY GENERATED, DO NOT MODIFY */

/*
 * deallocation functions for schema-defined QAPI types
 *
 * Copyright IBM, Corp. 2011
 *
 * Authors:
 *  Anthony Liguori   <aliguori@us.ibm.com>
 *  Michael Roth      <mdroth@linux.vnet.ibm.com>
 *
 * This work is licensed under the terms of the GNU LGPL, version 2.1 or later.
 * See the COPYING.LIB file in the top-level directory.
 *
 */

#include "qapi/dealloc-visitor.h"
#include "test-qapi-types.h"
#include "test-qapi-visit.h"

const char *EnumOne_lookup[] = {
    "value1",
    "value2",
    "value3",
    NULL,
};

const char *UserDefUnionKind_lookup[] = {
    "a",
    "b",
    NULL,
};

const char *UserDefNativeListUnionKind_lookup[] = {
    "integer",
    "s8",
    "s16",
    "s32",
    "s64",
    "u8",
    "u16",
    "u32",
    "u64",
    "number",
    "boolean",
    "string",
    NULL,
};


void qapi_free_EnumOneList(EnumOneList * obj)
{
    QapiDeallocVisitor *md;
    Visitor *v;

    if (!obj) {
        return;
    }

    md = qapi_dealloc_visitor_new();
    v = qapi_dealloc_get_visitor(md);
    visit_type_EnumOneList(v, &obj, NULL, NULL);
    qapi_dealloc_visitor_cleanup(md);
}


void qapi_free_NestedEnumsOneList(NestedEnumsOneList * obj)
{
    QapiDeallocVisitor *md;
    Visitor *v;

    if (!obj) {
        return;
    }

    md = qapi_dealloc_visitor_new();
    v = qapi_dealloc_get_visitor(md);
    visit_type_NestedEnumsOneList(v, &obj, NULL, NULL);
    qapi_dealloc_visitor_cleanup(md);
}


void qapi_free_NestedEnumsOne(NestedEnumsOne * obj)
{
    QapiDeallocVisitor *md;
    Visitor *v;

    if (!obj) {
        return;
    }

    md = qapi_dealloc_visitor_new();
    v = qapi_dealloc_get_visitor(md);
    visit_type_NestedEnumsOne(v, &obj, NULL, NULL);
    qapi_dealloc_visitor_cleanup(md);
}


void qapi_free_UserDefOneList(UserDefOneList * obj)
{
    QapiDeallocVisitor *md;
    Visitor *v;

    if (!obj) {
        return;
    }

    md = qapi_dealloc_visitor_new();
    v = qapi_dealloc_get_visitor(md);
    visit_type_UserDefOneList(v, &obj, NULL, NULL);
    qapi_dealloc_visitor_cleanup(md);
}


void qapi_free_UserDefOne(UserDefOne * obj)
{
    QapiDeallocVisitor *md;
    Visitor *v;

    if (!obj) {
        return;
    }

    md = qapi_dealloc_visitor_new();
    v = qapi_dealloc_get_visitor(md);
    visit_type_UserDefOne(v, &obj, NULL, NULL);
    qapi_dealloc_visitor_cleanup(md);
}


void qapi_free_UserDefTwoList(UserDefTwoList * obj)
{
    QapiDeallocVisitor *md;
    Visitor *v;

    if (!obj) {
        return;
    }

    md = qapi_dealloc_visitor_new();
    v = qapi_dealloc_get_visitor(md);
    visit_type_UserDefTwoList(v, &obj, NULL, NULL);
    qapi_dealloc_visitor_cleanup(md);
}


void qapi_free_UserDefTwo(UserDefTwo * obj)
{
    QapiDeallocVisitor *md;
    Visitor *v;

    if (!obj) {
        return;
    }

    md = qapi_dealloc_visitor_new();
    v = qapi_dealloc_get_visitor(md);
    visit_type_UserDefTwo(v, &obj, NULL, NULL);
    qapi_dealloc_visitor_cleanup(md);
}


void qapi_free_UserDefNestedList(UserDefNestedList * obj)
{
    QapiDeallocVisitor *md;
    Visitor *v;

    if (!obj) {
        return;
    }

    md = qapi_dealloc_visitor_new();
    v = qapi_dealloc_get_visitor(md);
    visit_type_UserDefNestedList(v, &obj, NULL, NULL);
    qapi_dealloc_visitor_cleanup(md);
}


void qapi_free_UserDefNested(UserDefNested * obj)
{
    QapiDeallocVisitor *md;
    Visitor *v;

    if (!obj) {
        return;
    }

    md = qapi_dealloc_visitor_new();
    v = qapi_dealloc_get_visitor(md);
    visit_type_UserDefNested(v, &obj, NULL, NULL);
    qapi_dealloc_visitor_cleanup(md);
}


void qapi_free_UserDefAList(UserDefAList * obj)
{
    QapiDeallocVisitor *md;
    Visitor *v;

    if (!obj) {
        return;
    }

    md = qapi_dealloc_visitor_new();
    v = qapi_dealloc_get_visitor(md);
    visit_type_UserDefAList(v, &obj, NULL, NULL);
    qapi_dealloc_visitor_cleanup(md);
}


void qapi_free_UserDefA(UserDefA * obj)
{
    QapiDeallocVisitor *md;
    Visitor *v;

    if (!obj) {
        return;
    }

    md = qapi_dealloc_visitor_new();
    v = qapi_dealloc_get_visitor(md);
    visit_type_UserDefA(v, &obj, NULL, NULL);
    qapi_dealloc_visitor_cleanup(md);
}


void qapi_free_UserDefBList(UserDefBList * obj)
{
    QapiDeallocVisitor *md;
    Visitor *v;

    if (!obj) {
        return;
    }

    md = qapi_dealloc_visitor_new();
    v = qapi_dealloc_get_visitor(md);
    visit_type_UserDefBList(v, &obj, NULL, NULL);
    qapi_dealloc_visitor_cleanup(md);
}


void qapi_free_UserDefB(UserDefB * obj)
{
    QapiDeallocVisitor *md;
    Visitor *v;

    if (!obj) {
        return;
    }

    md = qapi_dealloc_visitor_new();
    v = qapi_dealloc_get_visitor(md);
    visit_type_UserDefB(v, &obj, NULL, NULL);
    qapi_dealloc_visitor_cleanup(md);
}


void qapi_free_UserDefUnionList(UserDefUnionList * obj)
{
    QapiDeallocVisitor *md;
    Visitor *v;

    if (!obj) {
        return;
    }

    md = qapi_dealloc_visitor_new();
    v = qapi_dealloc_get_visitor(md);
    visit_type_UserDefUnionList(v, &obj, NULL, NULL);
    qapi_dealloc_visitor_cleanup(md);
}


void qapi_free_UserDefUnion(UserDefUnion * obj)
{
    QapiDeallocVisitor *md;
    Visitor *v;

    if (!obj) {
        return;
    }

    md = qapi_dealloc_visitor_new();
    v = qapi_dealloc_get_visitor(md);
    visit_type_UserDefUnion(v, &obj, NULL, NULL);
    qapi_dealloc_visitor_cleanup(md);
}


void qapi_free_UserDefNativeListUnionList(UserDefNativeListUnionList * obj)
{
    QapiDeallocVisitor *md;
    Visitor *v;

    if (!obj) {
        return;
    }

    md = qapi_dealloc_visitor_new();
    v = qapi_dealloc_get_visitor(md);
    visit_type_UserDefNativeListUnionList(v, &obj, NULL, NULL);
    qapi_dealloc_visitor_cleanup(md);
}


void qapi_free_UserDefNativeListUnion(UserDefNativeListUnion * obj)
{
    QapiDeallocVisitor *md;
    Visitor *v;

    if (!obj) {
        return;
    }

    md = qapi_dealloc_visitor_new();
    v = qapi_dealloc_get_visitor(md);
    visit_type_UserDefNativeListUnion(v, &obj, NULL, NULL);
    qapi_dealloc_visitor_cleanup(md);
}


void qapi_free_UserDefOptionsList(UserDefOptionsList * obj)
{
    QapiDeallocVisitor *md;
    Visitor *v;

    if (!obj) {
        return;
    }

    md = qapi_dealloc_visitor_new();
    v = qapi_dealloc_get_visitor(md);
    visit_type_UserDefOptionsList(v, &obj, NULL, NULL);
    qapi_dealloc_visitor_cleanup(md);
}


void qapi_free_UserDefOptions(UserDefOptions * obj)
{
    QapiDeallocVisitor *md;
    Visitor *v;

    if (!obj) {
        return;
    }

    md = qapi_dealloc_visitor_new();
    v = qapi_dealloc_get_visitor(md);
    visit_type_UserDefOptions(v, &obj, NULL, NULL);
    qapi_dealloc_visitor_cleanup(md);
}

