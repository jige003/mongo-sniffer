#!/usr/bin/env python
# -*- coding:utf-8 -*-
from pymongo import MongoClient

def main():
    conn = MongoClient('127.0.0.1', 27017)
    db = conn.mydb
    
    db.jige003.insert({"name": "jige003"})

    db.jige003.find_one({"name": "jige003"})
    
    db.jige003.update({"name": "jige003"}, {"$set": {"name": "jige00000000x"}})
    
    db.jige003.remove({"name": "jige00000000x"})

if __name__ == '__main__':
    main()
