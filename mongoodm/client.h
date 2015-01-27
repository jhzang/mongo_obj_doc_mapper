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
#ifndef MONGOODM_CLIENT_H_
#define MONGOODM_CLIENT_H_

#include <vector>
#include <string>
#include <mongoc.h>

namespace mongoodm {

class Collection;

class Client
{
public:
	Client() : raw_client_(NULL), is_owner_(false) {}
	Client(const char *uri_string) { Initialize(uri_string); }
	Client(mongoc_client_t *raw_client) : raw_client_(raw_client) , is_owner_(false) {}
	virtual ~Client() { Cleanup(); }

	bool Initialize(const char *uri_string);
	mongoc_client_t* GetRaw() { return raw_client_; }
	void Cleanup();

	const char* GetUriString() const;

	bool GetReadPrefs(mongoc_read_mode_t &read_mode, std::string &tags_str);
	bool SetReadPrefs(mongoc_read_mode_t read_mode, const bson_t *tags);
	bool SetReadPrefs(mongoc_read_mode_t read_mode, const std::string &tags_str);

	bool GetWriteConcern(int &w, std::string &wtag, int &wtimeout_msec, bool &fsync_, bool &journal);
	bool SetWriteConcern(int w, const char *wtag, int wtimeout_msec, bool fsync_, bool journal);

	bool GetDatabaseNames(std::vector<std::string> &db_names) const;
    bool GetServerStatus(
            std::string &reply_str, 
            mongoc_read_prefs_t *read_prefs = NULL);

    bool ExecuteSimpleCommand(
            const char *db_name,
            const bson_t *command,
            const mongoc_read_prefs_t *read_prefs,
            std::string &reply_str);

	Collection* GetCollection(const char *db_name, const char *collection_name);
    bool RenameCollection(
            const char * db_name, const char *collection_name,
            const char * new_db_name, const char *new_collection_name,
            bool drop_target_before_rename);
    bool DropCollection(const char *db_name, const char *collection_name);

	// TODO: gridfs

private:
	// If true, this object is responsible for destroying the underlying mongoc_client_t*
	bool is_owner_;
	mongoc_client_t *raw_client_;
};  // class Client

}  // namespace mongoodm

#endif  // MONGOODM_CLIENT_H_
