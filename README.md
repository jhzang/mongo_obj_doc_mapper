MongoDB object-document mapper(mongoodm for short) for C++ (based on mongo C driver) and code generator.
Developed by Junheng Zang(junheng.zang@gmail.com).

#Installation

You can include the source code in your project and get it compiled together with your application. Below is mongoodm's dependencies and how to compile it.

(1) mongoodm relies on mongo-c-driver, libbson and rapidjson. Make sure these dependencies have been installed in your system, and the include file search path and dependent library search path have been setup correctly.

  - mongo-c-driver: https://github.com/mongodb/mongo-c-driver.git

  - libbson: https://github.com/mongodb/libbson.git

  - rapidjson: https://github.com/miloyip/rapidjson.git

(2) Add the parent directory of mongoodm into your project's include file search path(e.g. -I*/your/path/to/mongo_obj_doc_mapper*) and add mongoodm/mongoodm_all.cpp into your project's source file list. 
