#!/usr/bin/env python
# -*- coding: utf-8 -*-
# python3操作Mysql

import pymysql

conn= pymysql.connect('localhost', 'root', 'root', '360',charset='utf8')
cursor = conn.cursor()

try:
    sql='SELECT * FROM user'
    cursor.execute(sql)
    results = cursor.fetchall()
    for row in results:
      id = row[2]
      yes_no = row[3]
      no_msg = row[4]
       # 打印结果
      print ("id=%s,yes_no=%s,no_msg=%s"%\
             (id, yes_no,no_msg))
        
except:
    import traceback
    traceback.print_exc()
    # 发生错误时会滚
    conn.rollback()
finally:
    # 关闭游标连接
    cursor.close()
    # 关闭数据库连接
    conn.close()