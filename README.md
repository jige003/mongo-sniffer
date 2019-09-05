# mongo-sniffer
> mongo sniffer network traffic 

### 功能
* 支持 100% 协议解析
* 支持 OP_REPLY 协议解析
* 支持 OP_UPDATE 协议解析
* 支持 OP_INSERT 协议解析
* 支持 RESERVED 协议解析
* 支持 OP_QUERY 协议解析
* 支持 OP_GET_MORE 协议解析
* 支持 OP_DELETE 协议解析
* 支持 OP_KILL_CURSORSOP 协议解析
* 支持 OP_MSG 协议解析
* 兼容版本4.2下 废弃协议OP_COMMAND和OP_COMMANDREPLY解析

### Usage
> 二进制文件编译于centos7 
```
Copyright by jige003

Usage:
    mongo-sniffer [-h] -i interface -p port

```

### 测试
运行python test_mongo.py, 解包如下
```
./mongo-sniffer -i lo
[*] sniffe on interface: lo
2019-09-04 19:04:13  127.0.0.1:55662 -> 127.0.0.1:27017 [ req ]

  collection : admin.$cmd
  skip : 0
  n_return : -1
  query : { "ismaster" : 1, "client" : { "driver" : { "name" : "PyMongo", "version" : "3.9.0" }, "os" : { "type" : "Linux", "name" : "CentOS Linux 7.6.1810 Core", "architecture" : "x86_64", "version" : "3.10.0-514.26.2.el7.x86_64" }, "platform" : "CPython 2.7.5.final.0" } }

2019-09-04 19:04:13  127.0.0.1:27017 -> 127.0.0.1:55662 [ resp ]

  cursor_id : 0
  start_from : 0
  n_returned : 1
  documents : { "ismaster" : true, "maxBsonObjectSize" : 16777216, "maxMessageSizeBytes" : 48000000, "maxWriteBatchSize" : 1000, "localTime" : { "$date" : "2019-09-04T11:04:13.013Z" }, "maxWireVersion" : 4, "minWireVersion" : 0, "ok" : 1.0 }

2019-09-04 19:04:13  127.0.0.1:55664 -> 127.0.0.1:27017 [ req ]

  collection : admin.$cmd
  skip : 0
  n_return : -1
  query : { "ismaster" : 1, "client" : { "driver" : { "name" : "PyMongo", "version" : "3.9.0" }, "os" : { "type" : "Linux", "name" : "CentOS Linux 7.6.1810 Core", "architecture" : "x86_64", "version" : "3.10.0-514.26.2.el7.x86_64" }, "platform" : "CPython 2.7.5.final.0" }, "compression" : [  ] }

2019-09-04 19:04:13  127.0.0.1:27017 -> 127.0.0.1:55664 [ resp ]

  cursor_id : 0
  start_from : 0
  n_returned : 1
  documents : { "ismaster" : true, "maxBsonObjectSize" : 16777216, "maxMessageSizeBytes" : 48000000, "maxWriteBatchSize" : 1000, "localTime" : { "$date" : "2019-09-04T11:04:13.014Z" }, "maxWireVersion" : 4, "minWireVersion" : 0, "ok" : 1.0 }

2019-09-04 19:04:13  127.0.0.1:55664 -> 127.0.0.1:27017 [ req ]

  collection : mydb.$cmd
  skip : 0
  n_return : -1
  query : { "insert" : "jige003", "ordered" : true, "documents" : [ { "_id" : { "$oid" : "5d6f9a2d010dea6760510dc3" }, "name" : "jige003" } ] }

2019-09-04 19:04:13  127.0.0.1:27017 -> 127.0.0.1:55664 [ resp ]

  cursor_id : 0
  start_from : 0
  n_returned : 1
  documents : { "ok" : 1, "n" : 1 }

2019-09-04 19:04:13  127.0.0.1:55664 -> 127.0.0.1:27017 [ req ]

  collection : mydb.$cmd
  skip : 0
  n_return : -1
  query : { "find" : "jige003", "filter" : { "name" : "jige003" }, "limit" : 1, "singleBatch" : true }

2019-09-04 19:04:13  127.0.0.1:27017 -> 127.0.0.1:55664 [ resp ]

  cursor_id : 0
  start_from : 0
  n_returned : 1
  documents : { "waitedMS" : 0, "cursor" : { "firstBatch" : [ { "_id" : { "$oid" : "5d6f96c3d7f50116aa6e5ed0" }, "name" : "jige003" } ], "id" : 0, "ns" : "mydb.jige003" }, "ok" : 1.0 }

2019-09-04 19:04:13  127.0.0.1:55664 -> 127.0.0.1:27017 [ req ]

  collection : mydb.$cmd
  skip : 0
  n_return : -1
  query : { "update" : "jige003", "ordered" : true, "updates" : [ { "q" : { "name" : "jige003" }, "u" : { "$set" : { "name" : "jige00000000x" } }, "multi" : false, "upsert" : false } ] }

2019-09-04 19:04:13  127.0.0.1:27017 -> 127.0.0.1:55664 [ resp ]

  cursor_id : 0
  start_from : 0
  n_returned : 1
  documents : { "ok" : 1, "nModified" : 1, "n" : 1 }

2019-09-04 19:04:13  127.0.0.1:55664 -> 127.0.0.1:27017 [ req ]

  collection : mydb.$cmd
  skip : 0
  n_return : -1
  query : { "delete" : "jige003", "ordered" : true, "deletes" : [ { "q" : { "name" : "jige00000000x" }, "limit" : 0 } ] }

2019-09-04 19:04:13  127.0.0.1:27017 -> 127.0.0.1:55664 [ resp ]

  cursor_id : 0
  start_from : 0
  n_returned : 1
  documents : { "ok" : 1, "n" : 1 }
```

### 集群内部协议测试
```
MongoDB shell version: 3.2.22
connecting to: test
Server has startup warnings:
2019-07-17T17:59:48.350+0800 I CONTROL  [initandlisten]
2019-07-17T17:59:48.350+0800 I CONTROL  [initandlisten] ** WARNING: /sys/kernel/mm/transparent_hugepage/enabled is 'always'.
2019-07-17T17:59:48.350+0800 I CONTROL  [initandlisten] **        We suggest setting it to 'never'
2019-07-17T17:59:48.350+0800 I CONTROL  [initandlisten]
2019-07-17T17:59:48.350+0800 I CONTROL  [initandlisten] ** WARNING: /sys/kernel/mm/transparent_hugepage/defrag is 'always'.
2019-07-17T17:59:48.350+0800 I CONTROL  [initandlisten] **        We suggest setting it to 'never'
2019-07-17T17:59:48.350+0800 I CONTROL  [initandlisten]
> db.version()
3.2.22
> db.acl.insert({"name": "jige"})
WriteResult({ "nInserted" : 1 })
> db.acl.find()
```

输出如下
```
./mongo-sniffer -i lo
[*] sniffe on interface: lo
2019-09-04 19:00:13  127.0.0.1:55636 -> 127.0.0.1:27017 [ req ]

  database : admin
  commandName : buildinfo
  metadata : { "buildinfo" : 1.0 }
  commandArgs : { }

2019-09-04 19:00:13  127.0.0.1:27017 -> 127.0.0.1:55636 [ resp ]

  metadata : { "version" : "3.2.22", "gitVersion" : "105acca0d443f9a47c1a5bd608fd7133840a58dd", "modules" : [  ], "allocator" : "tcmalloc", "javascriptEngine" : "mozjs", "sysInfo" : "deprecated", "versionArray" : [ 3, 2, 22, 0 ], "openssl" : { "running" : "OpenSSL 1.0.1e-fips 11 Feb 2013", "compiled" : "OpenSSL 1.0.1e-fips 11 Feb 2013" }, "buildEnvironment" : { "distmod" : "rhel70", "distarch" : "x86_64", "cc" : "/opt/mongodbtoolchain/bin/gcc: gcc (GCC) 4.8.2", "ccflags" : "-fno-omit-frame-pointer -fPIC -fno-strict-aliasing -ggdb -pthread -Wall -Wsign-compare -Wno-unknown-pragmas -Winvalid-pch -Werror -O2 -Wno-unused-local-typedefs -Wno-unused-function -Wno-deprecated-declarations -Wno-unused-but-set-variable -Wno-missing-braces -fno-builtin-memcmp", "cxx" : "/opt/mongodbtoolchain/bin/g++: g++ (GCC) 4.8.2", "cxxflags" : "-Wnon-virtual-dtor -Woverloaded-virtual -Wno-maybe-uninitialized -std=c++11", "linkflags" : "-fPIC -pthread -Wl,-z,now -rdynamic -fuse-ld=gold -Wl,-z,noexecstack -Wl,--warn-execstack", "target_arch" : "x86_64", "target_os" : "linux" }, "bits" : 64, "debug" : false, "maxBsonObjectSize" : 16777216, "storageEngines" : [ "devnull", "ephemeralForTest", "mmapv1", "wiredTiger" ], "ok" : 1.0 }
  commandReply : { }

2019-09-04 19:00:13  127.0.0.1:55636 -> 127.0.0.1:27017 [ req ]

  database : test
  commandName : isMaster
  metadata : { "isMaster" : 1.0, "forShell" : 1.0 }
  commandArgs : { }

2019-09-04 19:00:13  127.0.0.1:27017 -> 127.0.0.1:55636 [ resp ]

  metadata : { "ismaster" : true, "maxBsonObjectSize" : 16777216, "maxMessageSizeBytes" : 48000000, "maxWriteBatchSize" : 1000, "localTime" : { "$date" : "2019-09-04T11:00:13.734Z" }, "maxWireVersion" : 4, "minWireVersion" : 0, "ok" : 1.0 }
  commandReply : { }

2019-09-04 19:00:17  127.0.0.1:55636 -> 127.0.0.1:27017 [ req ]

  database : test
  commandName : insert
  metadata : { "insert" : "acl", "documents" : [ { "_id" : { "$oid" : "5d6f994124446d46f7a596f5" }, "name" : "jige" } ], "ordered" : true }
  commandArgs : { }

2019-09-04 19:00:17  127.0.0.1:27017 -> 127.0.0.1:55636 [ resp ]

  metadata : { "ok" : 1, "n" : 1 }
  commandReply : { }

2019-09-04 19:00:17  127.0.0.1:55636 -> 127.0.0.1:27017 [ req ]

  database : test
  commandName : isMaster
  metadata : { "isMaster" : 1.0, "forShell" : 1.0 }
  commandArgs : { }

2019-09-04 19:00:17  127.0.0.1:27017 -> 127.0.0.1:55636 [ resp ]

  metadata : { "ismaster" : true, "maxBsonObjectSize" : 16777216, "maxMessageSizeBytes" : 48000000, "maxWriteBatchSize" : 1000, "localTime" : { "$date" : "2019-09-04T11:00:17.136Z" }, "maxWireVersion" : 4, "minWireVersion" : 0, "ok" : 1.0 }
  commandReply : { }

2019-09-04 19:00:30  127.0.0.1:55636 -> 127.0.0.1:27017 [ req ]

  database : test
  commandName : find
  metadata : { "find" : "acl", "filter" : {  } }
  commandArgs : { }

2019-09-04 19:00:30  127.0.0.1:27017 -> 127.0.0.1:55636 [ resp ]

  metadata : { "waitedMS" : 0, "cursor" : { "firstBatch" : [ { "_id" : { "$oid" : "5d6f57b6654df9ac6ef16fa2" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f57dd654df9ac6ef16fa3" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f57fb654df9ac6ef16fa4" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f5831654df9ac6ef16fa5" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f58ce654df9ac6ef16fa6" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f5928654df9ac6ef16fa7" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f5971654df9ac6ef16fa8" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f597e654df9ac6ef16fa9" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f5996654df9ac6ef16faa" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f5a61654df9ac6ef16fab" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f5cf5654df9ac6ef16fac" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f5d3d654df9ac6ef16fad" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f5d5d654df9ac6ef16fae" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f5d85654df9ac6ef16faf" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f5db8654df9ac6ef16fb0" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f5e18654df9ac6ef16fb1" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f5f98654df9ac6ef16fb2" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f5fe7654df9ac6ef16fb3" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f5ff5654df9ac6ef16fb4" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f600a654df9ac6ef16fb5" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f6027654df9ac6ef16fb6" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f6106654df9ac6ef16fb7" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f61c5654df9ac6ef16fb8" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f61ee654df9ac6ef16fb9" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f6237654df9ac6ef16fba" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f626d654df9ac6ef16fbb" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f8aee654df9ac6ef16fbc" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f8b6a654df9ac6ef16fbd" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f8b7c654df9ac6ef16fbe" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f8c56654df9ac6ef16fbf" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f8c85654df9ac6ef16fc0" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f8c96654df9ac6ef16fc1" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f8de9654df9ac6ef16fc2" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f8ea6654df9ac6ef16fc3" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f8ebf654df9ac6ef16fc4" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f8eff654df9ac6ef16fc5" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f90fd654df9ac6ef16fc6" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f911b654df9ac6ef16fc7" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f91b1654df9ac6ef16fc8" }, "name" : "jige" }, { "_id" : { "$oid" : "5d6f994124446d46f7a596f5" }, "name" : "jige" } ], "id" : 0, "ns" : "test.acl" }, "ok" : 1.0 }
  commandReply : { }

2019-09-04 19:00:30  127.0.0.1:55636 -> 127.0.0.1:27017 [ req ]

  database : test
  commandName : isMaster
  metadata : { "isMaster" : 1.0, "forShell" : 1.0 }
  commandArgs : { }

2019-09-04 19:00:30  127.0.0.1:27017 -> 127.0.0.1:55636 [ resp ]

  metadata : { "ismaster" : true, "maxBsonObjectSize" : 16777216, "maxMessageSizeBytes" : 48000000, "maxWriteBatchSize" : 1000, "localTime" : { "$date" : "2019-09-04T11:00:30.022Z" }, "maxWireVersion" : 4, "minWireVersion" : 0, "ok" : 1.0 }
  commandReply : { }
```

### 参考文档
 * https://docs.mongodb.com/manual/reference/mongodb-wire-protocol/
 * http://bsonspec.org/spec.html
 * https://github.com/jeroen/mongolite/tree/master/src/mongoc

