from selenium import webdriver
import time
from selenium.webdriver.common.keys import Keys
from tkinter import *
import tkinter.messagebox as messagebox
import random
import os, sys
try:
    from tkinter import *
except ImportError:  #Python 2.x
    PythonVersion = 2
    from Tkinter import *
    from tkFont import Font
    from ttk import *
    #Usage:showinfo/warning/error,askquestion/okcancel/yesno/retrycancel
    from tkMessageBox import *
    #Usage:f=tkFileDialog.askopenfilename(initialdir='E:/Python')
    #import tkFileDialog
    #import tkSimpleDialog
else:  #Python 3.x
    PythonVersion = 3
    from tkinter.font import Font
    from tkinter.ttk import *
    from tkinter.messagebox import *
    #import tkinter.filedialog as tkFileDialog
    #import tkinter.simpledialog as tkSimpleDialog    #askstring()
def shua(url,tel,wd):
	browser = webdriver.Chrome()
	browser.get(url)
	browser.find_element_by_class_name("loginLink").click()
	time.sleep(3)
	browser.find_element_by_link_text("账号密码登录").click()
	time.sleep(3)
	browser.find_element_by_id("login_name").send_keys(tel)
	browser.find_element_by_id("password_").send_keys("weixin123")
	time.sleep(4)
	browser.find_element_by_xpath("//div[@class='login-form ordinary']/form/div[4]/a").click()
	time.sleep(3)
	# 登陆结束
	browser.find_element_by_class_name("answer_btn").click()
	time.sleep(2)
	tels=['15699569904','13458743124','13550607794','18474391931','18474391931','15111538746','13068877544','18474392360','18602057020','18613017382','18620732396','18613016395','18602056783','18602050131','13550636164','15643003604','13011724176','13288651327','13610146983','18482396618','18200410325','18280530942','15678540193','15008344800','18245485294','18602067170','15643051474','13487545145','15884503440','15183834934']
	telran=random.randint(0,29)
	ntel=tels[telran]
	wds=['回复1','回复2','回复3','回复4','回复5','回复6','回复7','回复8','回复9','回复10','回复11','回复12','回复13','回复14','回复15','回复16','回复17','回复18','回复19','回复20','回复21','回复22','回复23','回复24']
	wdran=random.randint(0,23)
	nwd=wds[wdran]
	iframe = browser.find_element_by_class_name("ke-edit-iframe")
	browser.switch_to_frame(iframe)
	browser.find_element_by_xpath('/html/body').send_keys(wd)
	browser.switch_to_default_content()
	browser.find_element_by_id("submit_reply").click()
	time.sleep(3)
	browser.find_element_by_xpath("//div[@class='p_info']/div[@class='down_list']/a/span[text()='退出']")
	time.sleep(2)
	browser.quit()
	pass
class Application_ui(Frame):
    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.master.title('soyoung')
        self.master.geometry('757x385')
        self.createWidgets()
    def createWidgets(self):
        self.top = self.winfo_toplevel()
        self.style = Style()
        self.style.configure('Label1.TLabel',anchor='w', font=('宋体',24))
        self.Label1 = Label(self.top, text='URL地址', style='Label1.TLabel')
        self.Label1.place(relx=0.045, rely=0.135, relwidth=0.16, relheight=0.08)
        self.Text1Var = StringVar(value='')
        self.Text1 = Entry(self.top, textvariable=self.Text1Var, font=('宋体',9))
        self.Text1.place(relx=0.238, rely=0.116, relwidth=0.714, relheight=0.099)
        self.style.configure('Label3.TLabel',anchor='w', foreground='#FF0000', font=('宋体',26))
        self.Label3 = Label(self.top, text='密码默认为weixin123', style='Label3.TLabel')
        self.Label3.place(relx=0.045, rely=0.829, relwidth=0.499, relheight=0.118)

        self.style.configure('Command1.TButton',font=('宋体',9))
        self.Command1 = Button(self.top, text='开刷', command=self.Command1_Cmd, style='Command1.TButton')
        self.Command1.place(relx=0.645, rely=0.81, relwidth=0.262, relheight=0.137)

class Application(Application_ui):
    def __init__(self, master=None):
        Application_ui.__init__(self, master)
    def Command1_Cmd(self, event=None):
        #TODO, Please finish the function here!
        url=self.Text1Var.get()
# 随机获取账号
        tels=['15699569904','13458743124','13550607794','18474391931','18474391931','15111538746','13068877544','18474392360','18602057020','18613017382','18620732396','18613016395','18602056783','18602050131','13550636164','15643003604','13011724176','13288651327','13610146983','18482396618','18200410325','18280530942','15678540193','15008344800','18245485294','18602067170','15643051474','13487545145','15884503440','15183834934']
        telran=random.randint(0,29)
        tel=tels[telran]
# 随机回复
        wds=['回复1','回复2','回复3','回复4','回复5','回复6','回复7','回复8','回复9','回复10','回复11','回复12','回复13','回复14','回复15','回复16','回复17','回复18','回复19','回复20','回复21','回复22','回复23','回复24']
        wdran=random.randint(0,23)
        wd=wds[wdran]
        n=0
        while n<10000:
        	newtelnum=random.randint(0,29)
        	newwdnum=random.randint(0,23)
        	newtel=tels[newtelnum]
        	newwd=wds[newwdnum]
        	shua(url,newtel,newwd)
        	n=n+1
        pass
if __name__ == "__main__":
    top = Tk()
    Application(top).mainloop()
    try: top.destroy()
    except: pass
