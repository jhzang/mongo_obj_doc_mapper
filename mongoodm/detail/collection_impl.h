/**
 * Copyright (c) 2015 Junheng Zang(junheng.zang@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.  
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 */
#ifndef MONGOODM_DETAIL_COLLECTION_IMPL_H_
#define MONGOODM_DETAIL_COLLECTION_IMPL_H_

#include "../debug.h"

namespace mongoodm {

template <class T_Document>
int Collection::FindDocuments(
		std::vector<T_Document*> &results, 
        const std::string &query_str, 
        const std::string &ret_fields_str/* = ""*/, 
        unsigned int offset/* = 0*/, 
        unsigned int limit/* = 0*/,
        unsigned int batch_size/* = 100*/)
{
    assert(raw_collection_ != NULL);

	bson_error_t error;
    bson_t *query = NULL;
    if (query_str.size() > 0) {
        query = bson_new_from_json((const uint8_t*)query_str.c_str(), query_str.size(), &error);
        if (NULL == query) {
			LOG_BSON_ERROR("Collection.FindDocuments", error);
            return -1;
        }
    }
    else {
        query = bson_new();
    }
    bson_t *fields = bson_new_from_json((const uint8_t*)ret_fields_str.c_str(), ret_fields_str.size(), &error);
    if (ret_fields_str.size() > 0 && NULL == fields) {
        bson_destroy(query);
		LOG_BSON_ERROR("Collection.FindDocuments", error);
        return -1;
    }

    mongoc_cursor_t *cursor = mongoc_collection_find(raw_collection_, MONGOC_QUERY_NONE, offset, limit, batch_size, query, fields, NULL);
    int retcode = 0;
    const bson_t *d = NULL;
    while (mongoc_cursor_more(cursor) && mongoc_cursor_next(cursor, &d)) {
        size_t len = 0;
        char *str = bson_as_json(d, &len);
        LOG_TRACE(str);
        T_Document *doc = new T_Document();
        if (!doc->FromJsonString(str, len)) {
            LOG_ERROR("[Collection.FindDocuments] Parsing error" << std::endl);
            bson_free(str);
            retcode = -1;
            break;
        }
        results.push_back(doc);
        bson_free(str);
    }
    if (mongoc_cursor_error (cursor, &error)) {
		LOG_BSON_ERROR("Collection.FindDocuments", error);
        retcode = -1;
    }
    if (retcode != -1) {
        retcode = results.size();
    }
    mongoc_cursor_destroy(cursor);

    bson_destroy(query);
    if (fields != NULL) {
        bson_destroy(fields);
    }
    return retcode;
}

template <class T_Document>
bool Collection::FindOneDocument(
		T_Document &result, 
        const std::string &query_str, 
        const std::string &ret_fields_str/* = ""*/)
{
    std::vector<std::string> raw_results;
    int retcode = Find(raw_results, query_str, ret_fields_str, 0, 1, 1);
    if (retcode <= 0 || raw_results.empty()) {
        return false;
    }
    std::string &raw_str = raw_results[0];
    LOG_TRACE(raw_str);
    if (!result.FromJsonString(raw_str.c_str(), raw_str.size())) {
        LOG_ERROR("[Collection.FindOneDocument] Parsing error" << std::endl);
        return false;
    }
    return true;
}

}  // namespace mongoodm

#endif  // MONGOODM_DETAIL_COLLECTION_IMPL_H_

