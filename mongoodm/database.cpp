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
#include "database.h"
#include "debug.h"
#include "collection.h"

namespace mongoodm {

const char* Database::GetName() const
{
	assert(raw_db_ != NULL);
    return mongoc_database_get_name(raw_db_);
}

bool Database::Drop()
{
	assert(raw_db_ != NULL);

	bson_error_t error;
	bool retflag = mongoc_database_drop(raw_db_, &error);
	if (!retflag) {
		LOG_BSON_ERROR("Database.Drop", error);
	}
	return retflag;
}

bool Database::ExecuteSimpleCommand(
        const bson_t *command,
        const mongoc_read_prefs_t *read_prefs,
        bson_t *reply)
{
	assert(raw_db_ != NULL && command != NULL && reply != NULL);

	bson_error_t error;
    bool retflag = mongoc_database_command_simple(raw_db_, command, read_prefs, reply, &error);
    if (!retflag) {
		LOG_BSON_ERROR("Database.ExecuteSimpleCommand", error);
	}
    return retflag;
}

bool Database::ExecuteSimpleCommand(
        const std::string &command_str,
        const mongoc_read_prefs_t *read_prefs,
        std::string &reply_str)
{
	assert(raw_db_ != NULL && command_str.size() > 0);

    bson_error_t error;
    bson_t *command = bson_new_from_json((const uint8_t*)command_str.c_str(), command_str.size(), &error);
    if (NULL == command) {
        LOG_BSON_ERROR("Database.ExecuteSimpleCommand", error);
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

bool Database::GetCollectionNames(std::vector<std::string> &names) const
{
	assert(raw_db_ != NULL);

	bson_error_t error;
	char **strv = mongoc_database_get_collection_names(raw_db_, &error);
	if (NULL == strv) {
		LOG_BSON_ERROR("Database.GetCollectionNames", error);
		return false;
	}
	for (size_t i = 0; strv[i] != NULL; ++i) {
		names.push_back(std::string(strv[i]));
	}
	bson_strfreev(strv);
	return true;
}

int64_t Database::HasCollection(const char *name) const
{
    assert(raw_db_ != NULL && name != NULL);

    bson_error_t error;
    bool retflag = mongoc_database_has_collection(raw_db_, name, &error);
    if (!retflag) {
        LOG_BSON_ERROR("Database.HasCollection", error);
    }
    return retflag;
}

Collection* Database::GetCollection(const char *name)
{
    assert(raw_db_ != NULL && name != NULL);

    mongoc_collection_t *raw_collection = mongoc_database_get_collection(raw_db_, name);
    if (NULL == raw_collection) {
        return NULL;
    }
    return new Collection(raw_collection);
}

}  // namespace mongoodm

