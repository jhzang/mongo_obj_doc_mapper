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
#ifndef MONGOODM_DEBUG_H_
#define MONGOODM_DEBUG_H_

#include <iostream>

namespace mongoodm {

#define MONGOODM_LOG_LEVEL_ERROR 0
#define MONGOODM_LOG_LEVEL_WARN  1
#define MONGOODM_LOG_LEVEL_INFO  2
#define MONGOODM_LOG_LEVEL_DEBUG 3
#define MONGOODM_LOG_LEVEL_TRACE 4

//#define MONGOODM_LOG_LEVEL MONGOODM_LOG_LEVEL_INFO
#define MONGOODM_LOG_LEVEL MONGOODM_LOG_LEVEL_TRACE

#if MONGOODM_LOG_LEVEL >= MONGOODM_LOG_LEVEL_TRACE
#define MONGOODM_LOG_TRACE(...) std::cout << "TRACE " << __VA_ARGS__
#else
#define MONGOODM_LOG_TRACE(...)
#endif

#if MONGOODM_LOG_LEVEL >= MONGOODM_LOG_LEVEL_DEBUG
#define MONGOODM_LOG_DEBUG(...) std::cout << "DEBUG " << __VA_ARGS__
#else
#define MONGOODM_LOG_DEBUG(...)
#endif

#if MONGOODM_LOG_LEVEL >= MONGOODM_LOG_LEVEL_INFO
#define MONGOODM_LOG_INFO(...) std::cout << " INFO " << __VA_ARGS__
#else
#define MONGOODM_LOG_INFO(...)
#endif

#if MONGOODM_LOG_LEVEL >= MONGOODM_LOG_LEVEL_WARN
#define MONGOODM_LOG_WARN(...) std::cerr << " WARN " << __VA_ARGS__
#else
#define MONGOODM_LOG_WARN(...)
#endif

#if MONGOODM_LOG_LEVEL >= MONGOODM_LOG_LEVEL_ERROR
#define MONGOODM_LOG_ERROR(...) std::cerr << "ERROR " << __VA_ARGS__
#else
#define MONGOODM_LOG_ERROR(...)
#endif

#define MONGOODM_LOG_BSON_ERROR(tag, error) \
	MONGOODM_LOG_ERROR("[" << tag << "] domain=" << error.domain << ",code=" << error.code << ",message=\"" << error.message << "\"" << std::endl)

}  // namespace mongoodm

#endif  // MONGOODM_DEBUG_H_

