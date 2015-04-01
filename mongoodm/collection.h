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
#ifndef MONGOODM_COLLECTION_H_
#define MONGOODM_COLLECTION_H_

#include <mongoc.h>

namespace mongoodm {

class Document;

class Collection
{
public:
	Collection(mongoc_collection_t *raw_collection)
		: raw_collection_(raw_collection)
	{
	}
	virtual ~Collection()
	{
		if (raw_collection_ != NULL) {
			mongoc_collection_destroy(raw_collection_);
		}
	}

	mongoc_collection_t* GetRaw() { return raw_collection_; }
    const char* GetName() const;
    bool Rename(const char *new_db_name, const char *new_collection_name, bool drop_target_before_rename);
    bool Stats(const bson_t *options, bson_t *reply);
    bool Stats(const std::string &options_str, std::string &reply_str);
    bool Validate(const bson_t *options, bson_t *reply);
    bool Validate(bool full, bool scandata, bson_t *reply);
    bool Validate(bool full, bool scandata, std::string &reply_str);
	std::string GetLastError() const;
	bool Drop();

    bool ExecuteSimpleCommand(
            const bson_t *command,
            const mongoc_read_prefs_t *read_prefs,
            bson_t *reply);
    bool ExecuteSimpleCommand(
            const std::string &command_str,
            const mongoc_read_prefs_t *read_prefs,
            std::string &reply_str);
	bool CreateIndex(const bson_t *keys, const mongoc_index_opt_t *opt);
	bool CreateIndex(const std::string &keys_str, const mongoc_index_opt_t *opt);
	bool DropIndex(const char *index_name);

    int64_t Count(
            const bson_t *query, 
            int64_t skip = 0, 
            int64_t limit = 0, 
            mongoc_query_flags_t flags = MONGOC_QUERY_NONE, 
            const mongoc_read_prefs_t *read_prefs = NULL);
    int64_t Count(
            const std::string &query_str, 
            int64_t skip = 0, 
            int64_t limit = 0, 
            mongoc_query_flags_t flags = MONGOC_QUERY_NONE, 
            const mongoc_read_prefs_t *read_prefs = NULL);

    bool Exists(
            const bson_t *query, 
            mongoc_query_flags_t flags = MONGOC_QUERY_NONE, 
            const mongoc_read_prefs_t *read_prefs = NULL)
    {
        return Count(query, 0, 1, flags, read_prefs) > 0;
    }
    bool Exists(
            const std::string &query_str, 
            mongoc_query_flags_t flags = MONGOC_QUERY_NONE, 
            const mongoc_read_prefs_t *read_prefs = NULL)
    {
        return Count(query_str, 0, 1, flags, read_prefs) > 0;
    }

    int Find(
            std::vector<const bson_t*> &results, 
            bson_t *query, 
            bson_t *ret_fields = NULL, 
            unsigned int offset = 0,
            unsigned int limit = 0,
            unsigned int batch_size = 100);
    inline bool FindOne(
            const bson_t *&result, 
            bson_t *query, 
            bson_t *ret_fields = NULL)
    {
        std::vector<const bson_t*> results;
        if (Find(results, query, ret_fields, 0, 0, 1) > 0) {
            result = results[0];
            return true;
        }
        return false;
    }

    int Find(
            std::vector<std::string> &results, 
            const std::string &query_str, 
            const std::string &ret_fields_str = "", 
            unsigned int offset = 0,
            unsigned int limit = 0,
            unsigned int batch_size = 100);
    inline bool FindOne(
            std::string &result, 
            const std::string &query_str, 
            const std::string &ret_fields_str = "")
    {
        std::vector<std::string> results;
        if (Find(results, query_str, ret_fields_str, 0, 0, 1) > 0) {
            result = results[0];
            return true;
        }
        return false;
    }

    template <class T_Document>
    int FindDocuments(
            std::vector<T_Document*> &results, 
            const std::string &query_str, 
            const std::string &ret_fields_str = "", 
            unsigned int offset = 0,
            unsigned int limit = 0,
            unsigned int batch_size = 100);
    template <class T_Document>
    bool FindOneDocument(
            T_Document &result, 
            const std::string &query_str, 
            const std::string &ret_fields_str = "");

    bool FindAndModify(
            const bson_t *query, 
            const bson_t *sort, 
            const bson_t *update, 
            const bson_t *fields, 
            bool _remove, 
            bool upsert, 
            bool _new, 
            bson_t *reply);
    bool FindAndModify(
            const char *query_str, 
            const char *sort_str, 
            const char *update_str, 
            const char *fields_str, 
            bool _remove, 
            bool upsert, 
            bool _new, 
            rapidjson::Value *retval,
            rapidjson::Value *last_error);

    bool Insert(
            const bson_t *document,
            mongoc_insert_flags_t flags = MONGOC_INSERT_NONE,
            const mongoc_write_concern_t *write_concern = NULL);
    bool InsertDocument(
            const Document *document,
            mongoc_insert_flags_t flags = MONGOC_INSERT_NONE,
            const mongoc_write_concern_t *write_concern = NULL);
    bool InsertBulk(
            const std::vector<const bson_t*> &documents,
            mongoc_insert_flags_t flags = MONGOC_INSERT_NONE,
            const mongoc_write_concern_t *write_concern = NULL,
            bson_t *reply = NULL);
    bool InsertBulk(
            const std::vector<std::string> &documents,
            mongoc_insert_flags_t flags = MONGOC_INSERT_NONE,
            const mongoc_write_concern_t *write_concern = NULL,
            rapidjson::Value *retval = NULL,
            rapidjson::Value *last_error = NULL);
    bool InsertBulkDocuments(
            const std::vector<const Document*> &documents,
            mongoc_insert_flags_t flags = MONGOC_INSERT_NONE,
            const mongoc_write_concern_t *write_concern = NULL,
            rapidjson::Value *retval = NULL,
            rapidjson::Value *last_error = NULL);

    bool Update(
            const bson_t *selector,
            const bson_t *update, 
            mongoc_update_flags_t flags,
            const mongoc_write_concern_t *write_concern = NULL);
    bool Update(
            const std::string &selector_str,
            const std::string &update_str, 
            mongoc_update_flags_t flags,
            const mongoc_write_concern_t *write_concern = NULL); 
    bool UpdateDocument(
            const Document *document, 
            mongoc_update_flags_t flags,
            const mongoc_write_concern_t *write_concern = NULL);

    bool Save(
            const bson_t *document,
            const mongoc_write_concern_t *write_concern = NULL);
    bool SaveDocument(
            const Document *document,
            const mongoc_write_concern_t *write_concern = NULL);

    bool Remove(
            const bson_t *selector,
            mongoc_remove_flags_t flags,
            const mongoc_write_concern_t *write_concern = NULL);
    bool Remove(
            const std::string &selector_str,
            mongoc_remove_flags_t flags,
            const mongoc_write_concern_t *write_concern = NULL);

public:
    static bool ParseCRUDReply(const bson_t *reply, rapidjson::Value *retval, rapidjson::Value *last_error);

private:
	// Forbid copy and assignment
	Collection(const Collection &other) {}
	Collection& operator=(const Collection &other) { return *this; }

private:
	mongoc_collection_t *raw_collection_;
};  // class Collection

}  // namespace mongoodm

#include "detail/collection_impl.h"

#endif  // MONGOODM_COLLECTION_H_

