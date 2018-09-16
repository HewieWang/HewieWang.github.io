#!/usr/bin/python
# coding: utf-8

"""
@ author: Hannibal
@ function: 密码字典生成器
@ date: 2018/9/15
"""

import os
import sys
import itertools as itr

words = "1234567890"
dict_file = "dict.txt"

class DictorGenerate():

    def __init__(self, minLength, maxLength):
        if maxLength > minLength:
            self.minLength = minLength
            self.maxLength = maxLength
        else:
            self.minLength = maxLength
            self.maxLength = minLength

    def generate(self):
        # 如果文件存在则删除
        if os.path.isfile(dict_file):
            os.remove(dict_file)

        dic = open(dict_file, "a", encoding='utf-8')
        for i in range(self.minLength, self.maxLength + 1):
            r = itr.product(words, repeat=i)
            for i in r:
                dic.write("%s\n" % "".join(i))

        dic.close()

if __name__ == "__main__":
    print("开始生成密码字典......")
    param_len = len(sys.argv[1:])
    param = str(sys.argv[1:])
    param = param.replace("'", "").replace("[", "").replace("]", "")

    params = param.split(",")

    try:
        if param_len == 2:
            min = int(params[0])
            max = int(params[1])
        elif param_len == 0:
            min = 3
            max = 4
        else:
            print("\nError:\n\n    仅能输入两个数字,例如:\n\n    python DictGenerate.py 3 6")
            sys.exit(0)
    except ValueError as e:
        print("\nError:\n\n    参数只能输入数字,例如:\n\n    python DictGenerate.py 3 6")
        sys.exit(0)

    dg = DictorGenerate(min, max)
    dg.generate()
    print("生成密码字典成功......")