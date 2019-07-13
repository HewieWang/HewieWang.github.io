#站长工具关键词挖掘
# -*- coding=utf-8 -*-
 
import requests
from lxml import etree
import re
import xlwt
import time
 
 
headers={
    'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_3) AppleWebKit/534.55.3 (KHTML, like Gecko) Version/5.1.3 Safari/534.53.10'
}
 
 
#查询关键词是否能找到相关的关键字
def search_keyword(keyword):
    data={
        'kw': keyword,
        'page': '1',
        'by': '0',
    }
    url="http://stool.chinaz.com/baidu/words.aspx"
    html=requests.post(url,data=data,headers=headers).text
    time.sleep(3)
    #print(html)
    con=etree.HTML(html)
    key_result=con.xpath('//div[@class="col-red lh30 fz14 tc"]/text()')
    try:
        key_result=key_result[0] #没有找到相关的关键字
    except:
        key_result=[]
    #print(key_result)
    return key_result
 
 
 
#获取关键词页码数和记录条数
def get_page_number(keyword):
    data = {
        'kw': keyword,
        'page': '1',
        'by': '0',
    }
    url = "http://stool.chinaz.com/baidu/words.aspx"
    html = requests.post(url, data=data, headers=headers).text
    time.sleep(3)
    # print(html)
    con = etree.HTML(html)
    page_num = con.xpath('//span[@class="col-gray02"]/text()')
    page_numberze = r'共(.+?)页'
    page_number = re.findall(page_numberze, page_num[0], re.S)
    page_number = page_number[0]
    #print(page_number)
    total_data = con.xpath('//p[@class="col-gray lh24 fr pr5"]')  # 数据记录
    total_datas = total_data[0].xpath('string(.)')  # 获取节点所有文本
    #print(total_datas)
    print(f'挖掘关键词：{keyword}-{total_datas}')
    return page_number
 
 
#获取关键词数据
def get_keyword_datas(keyword,page_number):
    datas_list = []
    for i in range(1,page_number+1):
        print(f'正在采集第{i}页关键词挖掘数据...')
        data = {
            'kw': keyword,
            'page': i,
            'by': '0',
        }
        #print(data)
        url = "http://stool.chinaz.com/baidu/words.aspx"
        html = requests.post(url, data=data, headers=headers).text
        time.sleep(3)
        #print(html)
        con = etree.HTML(html)
        key_words = con.xpath('//p[@class="midImg"]/a/span/text()')  # 关键词
        #print(key_words)
        keyword_all_datas = []
        keyword_datas = con.xpath('//ul[@class="ResultListWrap "]/li/div[@class="w8-0"]/a')
        for keyword_data in keyword_datas:
            keyword_data = keyword_data.text
            if keyword_data != None:
                keyword_all_datas.append(keyword_data)
        #print(keyword_all_datas)
        overall_indexs = keyword_all_datas[0::5]  # 整体指数
        #print(overall_indexs )
        pc_indexs = keyword_all_datas[1::5]  # pc指数
        #print(pc_indexs)
        mobile_indexs = keyword_all_datas[2::5]  # 移动指数
        #print(mobile_indexs)
        s360_indexs = keyword_all_datas[3::5]  # 360指数
        #print(s360_indexs)
        collections = keyword_all_datas[4::5]  # 收录量
        #print(collections)
        ips = con.xpath('//ul[@class="ResultListWrap "]/li/div[@class="w15-0 kwtop"]/text()') # 预估流量
        if ips==[]:
            ips =['--']
        #print(ips)
        first_place_hrefs = con.xpath(
            '//ul[@class="ResultListWrap "]/li/div[@class="w18-0 lh24 tl"]/a/text()')  # 首页位置链接
        if first_place_hrefs==[]:
            first_place_hrefs=con.xpath('//ul[@class="ResultListWrap "]/li/div[@class="w18-0 lh24 tl"]/text()')
        #print(first_place_hrefs)
        first_place_titles = con.xpath(
            '//ul[@class="ResultListWrap "]/li/div[@class="w18-0 lh24 tl"]/p[@class="lh17 pb5"]/text()')  # 首页位置标题
        if first_place_titles == []:
            first_place_titles=['--']
        #print(first_place_titles)
        data_list = []
        for key_word, overall_index, pc_index, mobile_index, s360_index, collection, ip, first_place_href, first_place_title in zip(
                key_words, overall_indexs, pc_indexs, mobile_indexs, s360_indexs, collections, ips, first_place_hrefs,
                first_place_titles
        ):
            data = [
                key_word,
                overall_index,
                pc_index,
                mobile_index,
                s360_index,
                collection,
                ip,
                first_place_href,
                first_place_title,
            ]
            print(data)
            print('\n')
            data_list.append(data)
            time.sleep(3)
        datas_list.extend(data_list) #合并关键词数据
    return datas_list
 
 
#保存关键词数据为excel格式
def bcsj(keyword,data):
    workbook = xlwt.Workbook(encoding='utf-8')
    booksheet = workbook.add_sheet('Sheet 1', cell_overwrite_ok=True)
    title = [['关键词', '整体指数', 'PC指数', '移动指数', '360指数', '预估流量（ip）', '收录量', '网站首位链接', '网站首位标题']]
    title.extend(data)
    #print(title)
    for i, row in enumerate(title):
        for j, col in enumerate(row):
            booksheet.write(i, j, col)
    workbook.save(f'{keyword}.xls')
    print(f"保存关键词数据为 {keyword}.xls 成功！")
 
 
if __name__ == '__main__':
    keyword = input('请输入关键词>>')
    print('正在查询，请稍后...')
    result=search_keyword(keyword)
    if result=="没有找到相关的关键字":
        print('\n')
        print (result)
        print("该关键词没有挖掘到关键词数据")
    else:
        print('\n')
        page_number=get_page_number(keyword)
        print('\n')
        print('正在采集关键词挖掘数据，请稍后...')
        print('\n')
        page_number=int(page_number)
        datas_list=get_keyword_datas(keyword,page_number)
        print('\n')
        print('关键词挖掘数据采集结果：')
        print('========================采集结果========================\n\n')
        for datas in datas_list:
            print(datas)
        print('\n\n========================采集结束========================\n')
        bcsj(keyword, datas_list)