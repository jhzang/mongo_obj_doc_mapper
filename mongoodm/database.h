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
#ifndef MONGOODM_DATABASE_H_
#define MONGOODM_DATABASE_H_

#include <mongoc.h>

namespace mongoodm {

class Collection;

class Database
{
public:
	Database(mongoc_database_t *raw_db)
		: raw_db_(raw_db)
	{
	}
	virtual ~Database()
	{
		if (raw_db_ != NULL) {
			mongoc_database_destroy(raw_db_);
		}
	}

	mongoc_database_t* GetRaw() { return raw_db_; }
    const char* GetName() const;
	bool Drop();

    bool ExecuteSimpleCommand(
            const bson_t *command,
            const mongoc_read_prefs_t *read_prefs,
            bson_t *reply);
    bool ExecuteSimpleCommand(
            const std::string &command_str,
            const mongoc_read_prefs_t *read_prefs,
            std::string &reply_str);

	bool GetCollectionNames(std::vector<std::string> &names) const;
    int64_t HasCollection(const char *name) const;
    Collection* GetCollection(const char *name);

private:
	// Forbid copy and assignment
	Database(const Database &other) {}
	Database& operator=(const Database &other) { return *this; }

private:
	mongoc_database_t *raw_db_;
};  // class Database

}  // namespace mongoodm

#endif  // MONGOODM_DATABASE_H_

