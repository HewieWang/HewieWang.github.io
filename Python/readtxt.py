import sys  
result=[]  
with open('accounts.txt','r') as f:  
    for line in f:  
        result.append(list(line.strip('\n').split(',')))  
print(result)
