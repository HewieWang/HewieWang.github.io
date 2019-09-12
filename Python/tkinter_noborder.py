import tkinter
from tkinter.messagebox import *
import sys
win = tkinter.Tk()
w = win.winfo_screenwidth()
h = win.winfo_screenheight()
win.geometry("%dx%d" %(w,h))
win.resizable(width=False, height=True)
win.overrideredirect(True)
win.attributes("-topmost",True)
label=tkinter.Label(win,text="Hello Python",font=('宋体',50))
label2=tkinter.Label(win,text="What u want?",font=('宋体',50))
label.pack()
label2.pack()
T=tkinter.Entry(width=50,font=('宋体',50))
T.pack()
def getMe():
	ps=T.get()
	if ps=="123456":
		showinfo(title='Hello', message="SUCCESS")
		sys.exit()
		pass
	else:
		showinfo(title='Hello', message="失败")
	pass
b = tkinter.Button(win, height=2, width=9,font=('宋体',40),text='确定',command=getMe)
b.pack()
win.mainloop()