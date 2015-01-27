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
#include "collection.h"
#include "debug.h"
#include "document.h"

namespace mongoodm {

const char* Collection::GetName() const
{
	assert(raw_collection_ != NULL);
    return mongoc_collection_get_name(raw_collection_);
}

bool Collection::Rename(const char *new_db_name, const char *new_collection_name, bool drop_target_before_rename)
{
	assert(raw_collection_ != NULL && new_db_name != NULL && new_collection_name != NULL);

    bson_error_t error;
    bool retflag = mongoc_collection_rename(raw_collection_, new_db_name, new_collection_name, drop_target_before_rename, &error);
    if (!retflag) {
        LOG_BSON_ERROR("Collection.Rename", error);
    }
    return retflag;
}

bool Collection::Stats(const bson_t *options, bson_t *reply)
{
	assert(raw_collection_ != NULL);

    bson_error_t error;
    bool retflag = mongoc_collection_stats(raw_collection_, options, reply, &error);
    if (!retflag) {
        LOG_BSON_ERROR("Collection.Stats", error);
    }
    return retflag;
}

bool Collection::Stats(const std::string &options_str, std::string &reply_str)
{
	assert(raw_collection_ != NULL);

    bson_error_t error;
    bson_t *options = NULL;
    if (options_str.size() > 0) {
        options = bson_new_from_json((const uint8_t*)options_str.c_str(), options_str.size(), &error);
        if (NULL == options) {
            LOG_BSON_ERROR("Collection.Stats", error);
            return false;
        }
    }
    bson_t reply = BSON_INITIALIZER;
    bool retflag = Stats(options, &reply);
    if (retflag) {
        size_t length = 0;
        char *str = bson_as_json(&reply, &length);
        reply_str.assign(str, length);
        bson_free(str);
    }
    if (options != NULL) {
        bson_destroy(options);
    }
    bson_destroy(&reply);
    return retflag;
}

bool Collection::Validate(const bson_t *options, bson_t *reply)
{
	assert(raw_collection_ != NULL && reply != NULL);

    bson_error_t error;
    bool retflag = mongoc_collection_validate(raw_collection_, options, reply, &error);
    if (!retflag) {
        LOG_BSON_ERROR("Collection.Validate", error);
    }
    return retflag;
}

bool Collection::Validate(bool full, bool scandata, bson_t *reply)
{
	assert(raw_collection_ != NULL && reply != NULL);

    bson_t *options = BCON_NEW("full", BCON_BOOL(full), "scandata", BCON_BOOL(scandata));
    bool retflag = Validate(options, reply);
    bson_destroy(options);
    return retflag;
}

bool Collection::Validate(bool full, bool scandata, std::string &reply_str)
{
	assert(raw_collection_ != NULL);

    bson_t reply = BSON_INITIALIZER;
    bool retflag = Validate(full, scandata, &reply);
    if (retflag) {
        size_t length = 0;
        char *str = bson_as_json(&reply, &length);
        reply_str.assign(str, length);
        bson_free(str);
    }
    bson_destroy(&reply);
    return retflag;
}

std::string Collection::GetLastError() const
{
	assert(raw_collection_ != NULL);

	std::string error_str;
	const bson_t *error = mongoc_collection_get_last_error(raw_collection_);
	if (error != NULL) {
		size_t length = 0;
		char *str = bson_as_json(error, &length);
		error_str.assign(str, length);
		bson_free(str);
	}
	return error_str;
}

bool Collection::ExecuteSimpleCommand(
        const bson_t *command,
        const mongoc_read_prefs_t *read_prefs,
        bson_t *reply)
{
	assert(raw_collection_ != NULL && command != NULL && reply != NULL);

	bson_error_t error;
    bool retflag = mongoc_collection_command_simple(raw_collection_, command, read_prefs, reply, &error);
    if (!retflag) {
		LOG_BSON_ERROR("Collection.ExecuteSimpleCommand", error);
	}
    return retflag;
}

bool Collection::ExecuteSimpleCommand(
        const std::string &command_str,
        const mongoc_read_prefs_t *read_prefs,
        std::string &reply_str)
{
	assert(raw_collection_ != NULL && command_str.size() > 0);

    bson_error_t error;
    bson_t *command = bson_new_from_json((const uint8_t*)command_str.c_str(), command_str.size(), &error);
    if (NULL == command) {
        LOG_BSON_ERROR("Collection.ExecuteSimpleCommand", error);
        return false;
    }
    bson_t reply = BSON_INITIALIZER;
    bool retflag = ExecuteSimpleCommand(command, read_prefs, &reply);
    if (retflag) {
        size_t length = 0;
        char *str = bson_as_json(&reply, &length);
        reply_str.assign(str, length);
        bson_free(str);
    }
    bson_destroy(command);
    bson_destroy(&reply);
    return retflag;
}

bool Collection::Drop()
{
	assert(raw_collection_ != NULL);

	bson_error_t error;
	bool retflag = mongoc_collection_drop(raw_collection_, &error);
	if (!retflag) {
		LOG_BSON_ERROR("Collection.Drop", error);
	}
	return retflag;
}

bool Collection::CreateIndex(const bson_t *keys, const mongoc_index_opt_t *opt)
{
	assert(raw_collection_ != NULL && keys != NULL);

	bson_error_t error;
	bool retflag = mongoc_collection_create_index(raw_collection_, keys, opt, &error);
	if (!retflag) {
		LOG_BSON_ERROR("Collection.CreateIndex", error);
	}
	return retflag;
}

bool Collection::CreateIndex(const std::string &keys_str, const mongoc_index_opt_t *opt)
{
	assert(raw_collection_ != NULL && keys_str.size() > 0);

	bson_error_t error;
	bson_t *keys = bson_new_from_json((const uint8_t*)keys_str.c_str(), keys_str.size(), &error);
	if (NULL == keys) {
		LOG_BSON_ERROR("Collection.CreateIndex", error);
		return false;
	}
	bool retflag = CreateIndex(keys, opt);
	bson_destroy(keys);
	return retflag;
}

bool Collection::DropIndex(const char *index_name)
{
	assert(raw_collection_ != NULL && index_name != NULL);

	bson_error_t error;
	bool retflag = mongoc_collection_drop_index(raw_collection_, index_name, &error);
	if (!retflag) {
		LOG_BSON_ERROR("Collection.DropIndex", error);
	}
	return retflag;
}

int64_t Collection::Count(
        mongoc_query_flags_t flags, 
        const bson_t *query, 
        int64_t skip/* = 0*/, 
        int64_t limit/* = 0*/, 
        const mongoc_read_prefs_t *read_prefs/* = NULL*/)
{
    assert(raw_collection_ != NULL && query != NULL);

    bson_error_t error;
    bool retflag = mongoc_collection_count(raw_collection_, flags, query, skip, limit, read_prefs, &error);
    if (!retflag) {
        LOG_BSON_ERROR("Collection.Count", error);
    }
    return retflag;
}

int64_t Collection::Count(
        mongoc_query_flags_t flags, 
        const std::string &query_str, 
        int64_t skip/* = 0*/, 
        int64_t limit/* = 0*/, 
        const mongoc_read_prefs_t *read_prefs/* = NULL*/)
{
    assert(raw_collection_ != NULL && query_str.size() > 0);

    bson_error_t error;
    bson_t *query = bson_new_from_json((const uint8_t*)query_str.c_str(), query_str.size(), &error);
    if (NULL == query) {
        LOG_BSON_ERROR("Collection.Count", error);
        return false;
    }
    int64_t count = Count(flags, query, skip, limit, read_prefs);
    bson_destroy(query);
    return count;
}

int Collection::FindRawDocuments(
		std::vector<const bson_t*> &results, 
        bson_t *query, 
        bson_t *ret_fields/* = NULL*/, 
        unsigned int offset/* = 0*/,
        unsigned int limit/* = 0*/,
        unsigned int batch_size/* = 100*/)
{
    assert(raw_collection_ != NULL && query != NULL);

    mongoc_cursor_t *cursor = mongoc_collection_find(raw_collection_, MONGOC_QUERY_NONE, offset, limit, batch_size, query, ret_fields, NULL);
    const bson_t *d = NULL;
    while (mongoc_cursor_more(cursor) && mongoc_cursor_next(cursor, &d)) {
        results.push_back(bson_copy(d));
    }
    int retcode = results.size();
	bson_error_t error;
    if (mongoc_cursor_error (cursor, &error)) {
		LOG_BSON_ERROR("Collection.FindRawDocuments", error);
        retcode = -1;
    }
    mongoc_cursor_destroy(cursor);
    return retcode;

}

int Collection::FindRawDocuments(
		std::vector<std::string> &results, 
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
			LOG_BSON_ERROR("Collection.FindRawDocuments", error);
            return -1;
        }
    }
    else {
        query = bson_new();
    }
    bson_t *fields = bson_new_from_json((const uint8_t*)ret_fields_str.c_str(), ret_fields_str.size(), &error);
    if (ret_fields_str.size() > 0 && NULL == fields) {
        bson_destroy(query);
		LOG_BSON_ERROR("Collection.FindRawDocuments", error);
        return -1;
    }

    mongoc_cursor_t *cursor = mongoc_collection_find(raw_collection_, MONGOC_QUERY_NONE, offset, limit, batch_size, query, fields, NULL);
    const bson_t *d = NULL;
    while (mongoc_cursor_more(cursor) && mongoc_cursor_next(cursor, &d)) {
        size_t len = 0;
        char *str = bson_as_json(d, &len);
        std::cout << str << std::endl;
        results.push_back(std::string(str, len));
        bson_free(str);
    }
    int retcode = results.size();
    if (mongoc_cursor_error (cursor, &error)) {
		LOG_BSON_ERROR("Collection.FindRawDocuments", error);
        retcode = -1;
    }
    mongoc_cursor_destroy(cursor);

    bson_destroy(query);
    if (fields != NULL) {
        bson_destroy(fields);
    }
    return retcode;
}

bool Collection::FindAndModify(
        const bson_t *query, 
        const bson_t *sort, 
        const bson_t *update, 
        const bson_t *fields, 
        bool _remove, 
        bool upsert, 
        bool _new, 
        bson_t *reply)
{
    assert(raw_collection_ != NULL && query != NULL);

    bson_error_t error;
    bool retflag = mongoc_collection_find_and_modify(raw_collection_, query, sort, update, fields, _remove, upsert, _new, reply, &error);
    if (!retflag) {
        LOG_BSON_ERROR("Collection.FindAndModify", error);
    }
    return retflag;
}

bool Collection::FindAndModify(
        const char *query_str, 
        const char *sort_str, 
        const char *update_str, 
        const char *fields_str, 
        bool _remove, 
        bool upsert, 
        bool _new, 
        std::string *reply_str)
{
    assert(raw_collection_ != NULL && query_str != NULL);

    bson_error_t error;
    bson_t *query = bson_new_from_json((const uint8_t*)query_str, -1, &error);
    if (NULL == query) {
        LOG_BSON_ERROR("Collection.FindAndModify", error);
        return false;
    }
    bson_t *sort = (NULL == sort_str ? NULL : bson_new_from_json((const uint8_t*)sort_str, -1, &error));
    if (sort_str != NULL && NULL == sort) {
        LOG_BSON_ERROR("Collection.FindAndModify", error);
        bson_destroy(query);
        return false;
    }
    bson_t *update = (NULL == update_str ? NULL : bson_new_from_json((const uint8_t*)update_str, -1, &error));
    if (update_str != NULL && NULL == update) {
        LOG_BSON_ERROR("Collection.FindAndModify", error);
        bson_destroy(query);
        if (sort != NULL) {
            bson_destroy(sort);
        }
        return false;
    }
    bson_t *fields = (NULL == fields_str ? NULL : bson_new_from_json((const uint8_t*)fields_str, -1, &error));
    if (fields_str != NULL && NULL == fields) {
        LOG_BSON_ERROR("Collection.FindAndModify", error);
        bson_destroy(query);
        if (sort != NULL) {
            bson_destroy(sort);
        }
        if (update != NULL) {
            bson_destroy(update);
        }
        return false;
    }
    bson_t *reply = (NULL == reply_str ? NULL : bson_new());
    bool retflag = FindAndModify(query, sort, update, fields, _remove, upsert, _new, reply);
    if (reply != NULL) {
        size_t length = 0;
        char *str = bson_as_json(reply, &length);
        reply_str->assign(str, length);
        bson_free(str);
    }
    bson_destroy(query);
    if (sort != NULL) {
        bson_destroy(sort);
    }
    if (update != NULL) {
        bson_destroy(update);
    }
    if (fields != NULL) {
        bson_destroy(fields);
    }
    if (reply != NULL) {
        bson_destroy(reply);
    }
    return retflag;
}

bool Collection::InsertDocument(const Document *doc)
{
    assert(raw_collection_ != NULL && doc != NULL);

	bson_error_t error;
    bson_t b_doc = BSON_INITIALIZER;
    if (!doc->ToBson(&b_doc)) {
        return false;
    }
    bool retflag = mongoc_collection_insert(raw_collection_, MONGOC_INSERT_NONE, &b_doc, NULL, &error);
	if (!retflag) {
		LOG_BSON_ERROR("Collection.InsertDocument", error);
	}
    bson_destroy(&b_doc);
    return retflag;
}

bool Collection::InsertBulkDocuments(const std::vector<const Document *> & docs)
{
    assert(raw_collection_ != NULL && docs.size() > 0);

	bson_error_t error;
    mongoc_bulk_operation_t *bulk_op = mongoc_collection_create_bulk_operation(raw_collection_, true, NULL);
    for (std::vector<const Document *>::const_iterator it = docs.begin(); it != docs.end(); ++it) {
        InsertDocument(*it);
    }
    bson_t *reply = bson_new();
    bool retflag = (mongoc_bulk_operation_execute(bulk_op, reply, &error) == docs.size());
	if (!retflag) {
		LOG_BSON_ERROR("Collection.InsertBulkDocuments", error);
	}
    bson_destroy(reply);
    mongoc_bulk_operation_destroy(bulk_op);
    return retflag;
}

bool Collection::Update(
        mongoc_update_flags_t flags,
        const bson_t *selector,
        const bson_t *update, 
        const mongoc_write_concern_t *write_concern/* = NULL*/)
{
    assert(raw_collection_ != NULL && selector != NULL && update != NULL);

	bson_error_t error;
    bool retflag = mongoc_collection_update(raw_collection_, flags, selector, update, write_concern, &error);
	if (!retflag) {
		LOG_BSON_ERROR("Collection.Update", error);
	}
    return retflag;
}

bool Collection::Update(
        mongoc_update_flags_t flags,
        const std::string &selector_str,
        const std::string &update_str, 
        const mongoc_write_concern_t *write_concern/* = NULL*/)
{
    assert(raw_collection_ != NULL && selector_str.size() > 0 && update_str.size() > 0);

	bson_error_t error;
    bson_t *selector = bson_new_from_json((const uint8_t*)selector_str.c_str(), selector_str.size(), &error);
    if (NULL == selector) {
		LOG_BSON_ERROR("Collection.Update", error);
        return false;
    }
    bson_t *update = bson_new_from_json((const uint8_t*)update_str.c_str(), update_str.size(), &error);
    if (NULL == update) {
        bson_destroy(selector);
		LOG_BSON_ERROR("Collection.Update", error);
        return false;
    }
    bool retflag = Update(flags, selector, update, write_concern);
    bson_destroy(selector);
    bson_destroy(update);
    return retflag;
}

bool Collection::UpdateDocument(
        mongoc_update_flags_t flags,
        const Document *doc, 
        const mongoc_write_concern_t *write_concern/* = NULL*/)
{
    assert(raw_collection_ != NULL && doc != NULL);

    const Field *pkey = doc->GetIdField();
    assert(pkey != NULL);
    bson_t selector = BSON_INITIALIZER;
    if (!pkey->BuildBson(&selector)) {
        bson_destroy(&selector);
        return false;
    }
    bson_t update = BSON_INITIALIZER;
    if (!doc->ToBson(&update)) {
        bson_destroy(&selector);
        bson_destroy(&update);
        return false;
    }
    bool retflag = Update(flags, &selector, &update, write_concern);
    bson_destroy(&selector);
    bson_destroy(&update);
    return retflag;
}

bool Collection::Save(
        const bson_t *doc,
        const mongoc_write_concern_t *write_concern/* = NULL*/)
{
    assert(raw_collection_ != NULL && doc != NULL);

    bson_error_t error;
    bool retflag = mongoc_collection_save(raw_collection_, doc, write_concern, &error);
    if (!retflag) {
        LOG_BSON_ERROR("Collection.Save", error);
    }
    return retflag;
}

bool Collection::SaveDocument(
        const Document *doc,
        const mongoc_write_concern_t *write_concern/* = NULL*/)
{
    assert(raw_collection_ != NULL && doc != NULL);

    bson_t b = BSON_INITIALIZER;
    if (!doc->ToBson(&b)) {
        bson_destroy(&b);
        return false;
    }
    bool retflag = Save(&b, write_concern);
    bson_destroy(&b);
    return retflag;
}

bool Collection::Remove(
        mongoc_remove_flags_t flags,
        const bson_t *selector,
        const mongoc_write_concern_t *write_concern/* = NULL*/)
{
    assert(raw_collection_ != NULL && selector != NULL);

    bson_error_t error;
    bool retflag = mongoc_collection_remove(raw_collection_, flags, selector, write_concern, &error);
    if (!retflag) {
        LOG_BSON_ERROR("Collection.Remove", error);
    }
    return retflag;
}

bool Collection::Remove(
        mongoc_remove_flags_t flags,
        const std::string &selector_str,
        const mongoc_write_concern_t *write_concern/* = NULL*/)
{
    assert(raw_collection_ != NULL && selector_str.size() > 0);

    bson_error_t error;
    bson_t *selector = bson_new_from_json((const uint8_t*)selector_str.c_str(), selector_str.size(), &error);
    if (NULL == selector) {
        LOG_BSON_ERROR("Collection.Remove", error);
        return false;
    }
    bool retflag = Remove(flags, selector, write_concern);
    bson_destroy(selector);
    return retflag;
}

}  // namespace mongoodm

