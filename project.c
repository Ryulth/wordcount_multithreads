#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


void countFrequency(void *arg){
    pthread_t id; 
    id=pthread_self();
    char *fileName = (char*)malloc(sizeof(char));
    fileName = (char*)arg;
    char *result = (char*)malloc(sizeof(result)*100);
    int count=0;
    int spaceCnt=0;
    int enterCnt=0;
    int i = 0;
    FILE *file;
    file = fopen(fileName, "r");//파일열기
    if (file == NULL){
        printf("Couldn't open file: ");
    }

    else {
        while ( !feof(file))
        {   
            char buff[1000];
            char *pStr;
            enterCnt++;
            if(fgets(buff, sizeof(buff), file)==NULL)break;
            for ( i =0;i<sizeof(buff);)
            {
                if(buff[i] & 0x80)
                {
                    //printf("%.3s 한글 \n",buff+i);
                    i=i+3;//utf-8 일땐 한글 3바이트 EUC-KR 일떈 2 
                    count++;
                    if(i>strlen(buff))
                    {
                        i=i-3; 
                        break;
                    }
                }
                else
                {
                    //printf("%c 한글 아님\n",buff[i]);
                    if (buff[i]==32)//공백 구분
                    {
                        spaceCnt++;
                    }

                    i++;
                    count++;
                    if(i>strlen(buff))
                    {
                        i--;
                        break;
                    }

                }

            }
            count=count-2;// 쓰래기값 삭제
        }
    }
    
    enterCnt=enterCnt-2;//엔터수는 row 수보다 2 작다
    snprintf(result, 10000, "Thread ID: %ud 파일이름:%s \n공백포함:%d자 공백제외: %d자  줄수:%d줄", (int)id,fileName,count+enterCnt,count-spaceCnt,enterCnt+1);
    fclose(file);
    printf("%s\n", result);

}

int main(int argc, char *argv[]){

    pthread_t threads[argc-1];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    int i;
    for (i = 1; i < argc; i++){//인자값 별로 할당하기 위한 loop
        char* arguments = (char*)malloc(sizeof(argv[i])+1);
        arguments = argv[i];
        //새로운 쓰레드 생성
        pthread_create(&threads[i], &attr, (void*) countFrequency, (void*) arguments);
    }
    for (i = 1; i < argc; i++){

        pthread_join(threads[i], NULL);
    }

    return 0;
}
