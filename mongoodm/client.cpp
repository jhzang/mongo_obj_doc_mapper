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
#include "client.h"
#include "collection.h"
#include "debug.h"

namespace mongoodm {

void Client::Cleanup()
{
	if (raw_client_ != NULL && is_owner_) {
		mongoc_client_destroy(raw_client_);
		raw_client_ = NULL;
	}
}

bool Client::Initialize(const char *uri_string)
{
	Cleanup();

	raw_client_ = mongoc_client_new(uri_string);
	is_owner_ = true;
	return raw_client_ != NULL;
}

const char* Client::GetUriString() const
{
	assert(raw_client_ != NULL);

	const mongoc_uri_t *uri = mongoc_client_get_uri(raw_client_);
	return mongoc_uri_get_string(uri);
}

bool Client::GetReadPrefs(mongoc_read_mode_t &read_mode, std::string &tags_str)
{
	assert(raw_client_ != NULL);

	const mongoc_read_prefs_t *read_prefs = mongoc_client_get_read_prefs(raw_client_);
	if (NULL == read_prefs) {
		return false;
	}
	read_mode = mongoc_read_prefs_get_mode(read_prefs);
	const bson_t *tags = mongoc_read_prefs_get_tags(read_prefs);
	if (tags != NULL) {
		size_t length = 0;
		char *str = bson_as_json(tags, &length);
		tags_str.assign(str, length);
		bson_free(str);
	}
	return true;
}

bool Client::SetReadPrefs(mongoc_read_mode_t read_mode, const bson_t *tags)
{
	mongoc_read_prefs_t *read_prefs = mongoc_read_prefs_new(read_mode);
	if (read_prefs == NULL) {
		return false;
	}
	if (tags != NULL) {
		mongoc_read_prefs_set_tags(read_prefs, tags);
	}
	mongoc_client_set_read_prefs(raw_client_, read_prefs);
	mongoc_read_prefs_destroy(read_prefs);
	return true;
}

bool Client::SetReadPrefs(mongoc_read_mode_t read_mode, const std::string &tags_str)
{
	assert(raw_client_ != NULL);

	bson_t *tags = NULL;
	if (tags_str.size() > 0) {
		bson_error_t error;
		tags = bson_new_from_json((const uint8_t*)tags_str.c_str(), tags_str.size(), &error);
		if (tags == NULL) {
			LOG_BSON_ERROR("Client.SetReadPrefs", error);
			return false;
		}
	}
	bool retflag = SetReadPrefs(read_mode, tags);
	if (tags != NULL) {
		bson_destroy(tags);
	}
	return retflag;
}

bool Client::GetWriteConcern(int &w, std::string &wtag, int &wtimeout_msec, bool &fsync_, bool &journal)
{
	assert(raw_client_ != NULL);

	const mongoc_write_concern_t *write_concern = mongoc_client_get_write_concern(raw_client_);
	w = mongoc_write_concern_get_w(write_concern);
	const char *wtag_ = mongoc_write_concern_get_wtag(write_concern);
	if (wtag_ != NULL) {
		wtag.assign(wtag_);
	}
	wtimeout_msec = mongoc_write_concern_get_wtimeout(write_concern);
	fsync_ = mongoc_write_concern_get_fsync(write_concern);
	journal = mongoc_write_concern_get_journal(write_concern);
	return true;
}

bool Client::SetWriteConcern(int w, const char *wtag, int wtimeout_msec, bool fsync_, bool journal)
{
	assert(raw_client_ != NULL);

	mongoc_write_concern_t *write_concern = mongoc_write_concern_new();
	if (NULL == write_concern) {
		return false;
	}
	mongoc_write_concern_set_w(write_concern, w);
	if (wtag != NULL) {
		mongoc_write_concern_set_wtag(write_concern, wtag);
	}
	mongoc_write_concern_set_wtimeout(write_concern, wtimeout_msec);
	mongoc_write_concern_set_fsync(write_concern, fsync_);
	mongoc_write_concern_set_journal(write_concern, journal);
	mongoc_client_set_write_concern(raw_client_, write_concern);
	mongoc_write_concern_destroy(write_concern);
	return true;
}

bool Client::GetDatabaseNames(std::vector<std::string> &db_names) const
{
	assert(raw_client_ != NULL);

	bson_error_t error;
	char **strv = mongoc_client_get_database_names(raw_client_, &error);
	if (NULL == strv) {
		LOG_BSON_ERROR("Client.GetDatabaseNames", error);
		return false;
	}
	for (size_t i = 0; strv[i] != NULL; ++i) {
		db_names.push_back(std::string(strv[i]));
	}
	bson_strfreev(strv);
	return true;
}

bool Client::GetServerStatus(
        std::string &reply_str, 
        mongoc_read_prefs_t *read_prefs/* = NULL*/)
{
	assert(raw_client_ != NULL);

    bson_t reply = BSON_INITIALIZER;
	bson_error_t error;
    bool retflag = mongoc_client_get_server_status(raw_client_, read_prefs, &reply, &error);
    if (retflag) {
        size_t length = 0;
        char *str = bson_as_json(&reply, &length);
        reply_str.assign(str, length);
        bson_free(str);
    }
	else {
		LOG_BSON_ERROR("Client.GetServerStatus", error);
	}
    bson_destroy(&reply);
    return retflag;
}

bool Client::ExecuteSimpleCommand(
        const char *db_name,
        const bson_t *command,
        const mongoc_read_prefs_t *read_prefs,
        std::string &reply_str)
{
	assert(raw_client_ != NULL && db_name != NULL && command != NULL);

    bson_t reply = BSON_INITIALIZER;
	bson_error_t error;
    bool retflag = mongoc_client_command_simple(raw_client_, db_name, command, read_prefs, &reply, &error);
    if (retflag) {
        size_t length = 0;
        char *str = bson_as_json(&reply, &length);
        reply_str.assign(str, length);
        bson_free(str);
    }
	else {
		LOG_BSON_ERROR("Client.ExecuteSimpleCommand", error);
	}
    bson_destroy(&reply);
    return retflag;
}

Collection* Client::GetCollection(const char *db_name, const char *collection_name)
{
	assert(raw_client_ != NULL && db_name != NULL && collection_name != NULL);

	mongoc_collection_t *collection = mongoc_client_get_collection(raw_client_, db_name, collection_name);
	if (NULL == collection) {
		return NULL;
	}
	return new Collection(collection);
}

bool Client::RenameCollection(
        const char * db_name, const char *collection_name,
        const char * new_db_name, const char *new_collection_name,
        bool drop_target_before_rename)
{
    assert(raw_client_ != NULL);
    assert(db_name != NULL && collection_name != NULL);
    assert(new_db_name != NULL && new_collection_name != NULL);

	mongoc_collection_t *collection = mongoc_client_get_collection(raw_client_, db_name, collection_name);
	if (NULL == collection) {
		return false;
	}
    bson_error_t error;
    bool retflag = mongoc_collection_rename(collection, new_db_name, new_collection_name, drop_target_before_rename, &error);
    if (!retflag) {
        LOG_BSON_ERROR("Client.RenameCollection", error);
    }
    mongoc_collection_destroy(collection);
    return retflag;
}

bool Client::DropCollection(const char *db_name, const char *collection_name)
{
    assert(raw_client_ != NULL && db_name != NULL && collection_name != NULL);

	bson_error_t error;
    mongoc_collection_t *collection = mongoc_client_get_collection(raw_client_, db_name, collection_name);
    bool retflag = mongoc_collection_drop(collection, &error);
    mongoc_collection_destroy(collection);
	if (!retflag) {
		LOG_BSON_ERROR("Client.DropCollection", error);
	}
    return retflag;
}

}  // namespace mongoodm

