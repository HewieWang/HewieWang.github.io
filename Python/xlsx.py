#!/usr/bin/env python3.6
# -*- coding: utf-8 -*-
__author__ = "eason"
from win32com.client import Dispatch
import os


def excel_decrypt(src_file: str, password: str, del_src: bool = False)->bool:
    """
    Excel自动解密
    :param src_file:待解密Excel文件路径
    :param password:密码,多个密码用英文逗号隔开
    :param del_src:是否删除原始加密文件
    :return:
    """
    flag = False
    if "," in password:
        passwords = password.split(",")
        for pwd in passwords:
            try:
                xlapp = Dispatch("Excel.Application")
                wb = xlapp.Workbooks.Open(src_file, False, True, None, pwd)
                file_name = src_file.split("\\")[-1]
                file_location = src_file[0:(len(src_file) - len(file_name))]
                save_path = os.path.join(file_location, ("(decrypted)" + file_name))
                xlapp.DisplayAlerts = False
                xlapp.ActiveWorkbook.SaveAs(save_path, None, "", "")
                wb.Close()
                xlapp.Quit()
                flag = True
                print("decrypt success![%s]" % pwd)
                if del_src:
                    try:
                        os.remove(src_file)
                        print("origin file delete success![%s]" % src_file)
                    except Exception as e:
                        print("origin file delete failed![%s]" % src_file, repr(e))
                break
            except Exception as e:
                print("wrong password![%s]" % pwd, repr(e))
    else:
        try:
            xlapp = Dispatch("Excel.Application")
            wb = xlapp.Workbooks.Open(src_file, False, True, None, password)
            file_name = src_file.split("\\")[-1]
            file_location = src_file[0:(len(src_file) - len(file_name))]
            save_path = os.path.join(file_location, ("(decrypted)" + file_name))
            xlapp.DisplayAlerts = False
            xlapp.ActiveWorkbook.SaveAs(save_path, None, "", "")
            wb.Close()
            xlapp.Quit()
            flag = True
            print("decrypt success![%s]" % password)
            if del_src:
                try:
                    os.remove(src_file)
                    print("origin file delete success![%s]" % src_file)
                except Exception as e:
                    print("origin file delete failed![%s]" % src_file, repr(e))
        except Exception as e:
            print("wrong password![%s]" % password, repr(e))
    return flag


if __name__ == "__main__":
    print(excel_decrypt(r"C:\Users\Administrator\Downloads\123.xlsx", password="1993,123456,121212,12345678", del_src=True))