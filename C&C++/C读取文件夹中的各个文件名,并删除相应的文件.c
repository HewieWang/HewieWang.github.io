#include<stdio.h>   
#include<string.h>   
#include<stdlib.h>   
#include<io.h> 
#include <process.h>
#include <memory.h>
 
void SearchFile(const char *);
 int main()   
 {    
     SearchFile("C:\\mm");
     printf("\n");
     system("pause");   
     return 0;   
 }   
 void SearchFile(const char *dir)
 {       
     
     struct _finddata_t   ffblk; 
     char path[256],path1[256],path2[256];
     char ch;   
     sprintf(path,"%s\\*.php*",dir); 
     sprintf(path1,"%s\\",dir);  
     long done = _findfirst(path,&ffblk);   
     int find=0;  
      printf("%s\n",path);
     while (find==0)     
     {     
         if(strcmp(ffblk.name,".php"))
         {
             strcpy(path2,path1);
             strcat(path2,ffblk.name);
            // system(path2);
            printf("确定要删除文件：%s\n按'Y'删除，'N'取消\n",path2);
             scanf("%c",&ch);
             if(ch=='Y' || ch=='y')
             {
                 if   (remove(path2)   ==   0) 
                   printf( "Have Removed:   %s.\n ",path2); 
                else 
                   perror( "remove "); 
            }
             getchar();
         }
        find=_findnext(done,&ffblk);
     }
     _findclose(done); 
     
}