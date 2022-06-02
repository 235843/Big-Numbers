#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int add(const char* number1, const char* number2, char** result);

int count_digits(const char* text);

int validate_expression(const char *expr);

char* multiply_me(const char* num, char n);

int multiply(const char* number1, const char* number2, char** result);

int calculate(const char* expr, char **result);

void setTerminator(char* t)
{
    for(int i=0;*(t+i)!='\0';i++)
    {
        if(!isdigit(*(t+i+1)))
            *(t+i+1)=0;
    }
}

void destroy_zeros(char* t)
{
    while(*t=='0')
    {
        for(int i=0;*(t+i)!='\0';i++)
            *(t+i)=*(t+i+1);
    }

}

void setZeros(char* t, int size)
{
    for(int i=0;i<size-1;i++)
    {
        *(t+i) = '0';
    }
}

void skipOne(char** t)
{
    for(int i=0;*((*t)+i)!='\0';i++)
        *((*t)+i) = *((*t)+i+1);
}

void minuseFirst(char* t)
{
    for(int i=(int)strlen(t);i>=0;i--)
        *(t+i+1) = *(t+i);
    *t='-';
}

int findNextNumber(const char *t, int index)
{
    index--;
    for(;index>=0;index--)
    {
        if(*(t+index)!='0')
            return index;
    }
    return 0;
}

int validate(const char *number){
    if(number == NULL) return -1;

    if(strlen(number) == 0) return 2;

    if(*number!='-' && !isdigit(*number))
        return 2;

    if(*number=='0' && strlen(number)>1)
        return 2;

    if(*number=='-' && strlen(number)==1)
        return 2;

    if(*number=='-' && *(number+1)=='0')
        return 2;

    for(int i=1;*(number+i)!=0;i++)
    {
        if(!isdigit(*(number+i)))
            return 2;
    }

    return 0;
}

int compare(const char* number1, const char* number2){
    if(number1 == NULL || number2 == NULL) return 2;

    if(validate(number1) != 0 || validate(number2) != 0) return 3;

    if(*number2=='-' && isdigit(*number1))
        return 1;

    if(*number1=='-' && isdigit(*number2))
        return -1;

    if(strlen(number1)>strlen(number2))
        return 1;

    if(strlen(number1)<strlen(number2))
        return -1;

    if(*number2=='-' && *number1=='-')
    {
        for(int i=0;*(number1+i)!=0;i++)
        {
            if(*(number1+i)>*(number2+i))
                return -1;
            if(*(number1+i)<*(number2+i))
                return 1;
        }
    }
    else
    {
        for(int i=0;*(number1+i)!=0;i++)
        {
            if(*(number1+i)>*(number2+i))
                return 1;
            if(*(number1+i)<*(number2+i))
                return -1;
        }
    }

    return 0;
}

int subtract(const char* number1, const char* number2, char** result)
{
    if(!number1 || !number2 || !result)
        return 1;

    if(validate(number1) || validate(number2))
        return 2;

    int bigger=0;
    if(compare(number1,number2)==1)
        bigger=1;
    else if(compare(number1,number2)==-1)
        bigger=2;

    if(*number2=='0')
    {
        *result = strdup(number1);
        if(!*result)
            return 3;
        return 0;
    }

    if(*number1=='0')
    {
        *result=strdup(number2);
        if(!*result)
            return 3;
        if(**result=='-')
            skipOne(result);
        else {
            char* temp = realloc(*result,strlen(*result)+2);
            if(!temp)
            {
                free(*result);
                return 3;
            }
            minuseFirst(temp);
            *result=temp;
        }
        return 0;
    }

    if(bigger==0)
    {
        *result = strdup("0");
        if(!*result)
            return 3;
        return 0;
    }

    if(*number2=='-' && *number1=='-')
    {
        if(subtract(number1+1,number2+1,result))
            return 3;

        if(compare(number1+1,number2+1)==1)
        {
            char* temp = realloc(*result,strlen(*result)+2);
            if(!temp)
            {
                free(*result);
                return 3;
            }
            minuseFirst(temp);
            *result=temp;
        }
        else
            skipOne(result);
        return 0;
    }

    if(*number2=='-')
    {
        int x = add(number1,number2+1,result);
        return x;
    }

    if(*number1=='-')
    {
        if(add(number1+1,number2,result))
            return 3;
        minuseFirst(*result);
        return 0;
    }


    int smallsize, bigsize;
    if(bigger==1) {
        smallsize = (int) strlen(number2) - 1;
        bigsize = (int )strlen(number1)-1;
        *result = calloc((int )strlen(number1)+3,1);
        if(!*result)
        {
            *result=NULL;
            return 3;
        }
        int index=(int )strlen(number1)+1;

        int ind=100000;

        setZeros(*result,bigsize+3);
        for(;bigsize>=0;smallsize--,bigsize--,index--)
        {
            int temp;
            if(smallsize>=0)
                temp = *(number2+smallsize)-'0';
            else
                temp=0;
            int temp2 = *(number1+bigsize)-'0';
            if(bigsize>ind)
                temp2+=9;
            if(bigsize==ind)
                temp2--;
            if(temp>temp2)
            {
                ind = findNextNumber(number1,bigsize);
                temp2+=10;
            }
            temp2-=temp;
            *(*result+index) = (char)(temp2+'0');
        }

        destroy_zeros(*result);
        setTerminator(*result);

    }
    else{
        smallsize = (int) strlen(number1) - 1;
        bigsize = (int )strlen(number2)-1;
        *result = calloc((int )strlen(number2)+3,1);
        if(!*result)
        {
            *result=NULL;
            return 3;
        }
        int index=(int )strlen(number2)+1;

        setZeros(*result,bigsize+3);
        int ind=100000;
        for(;bigsize>=0;smallsize--,bigsize--,index--)
        {
            int temp;
            if(smallsize>=0)
                temp = *(number1+smallsize)-'0';
            else
                temp=0;
            int temp2 = *(number2+bigsize)-'0';
            if (bigsize>ind)
                temp2+=9;
            if (bigsize==ind)
                temp2--;
            if(temp>temp2)
            {
                ind = findNextNumber(number2,bigsize);
                temp2+=10;
            }
            temp2-=temp;
            *(*result+index) = (char)(temp2+'0');
        }
        destroy_zeros(*result);

        minuseFirst(*result);
        setTerminator(*result);
    }
    return 0;
}

int add(const char* number1, const char* number2, char** result)
{
    if(!number1 || !number2 || !result)
        return 1;

    if(validate(number1) || validate(number2))
        return 2;

    int  indexBig;
    if(compare(number1,number2)==1)    {
        indexBig = (int )strlen(number1)-1;
    }
    else  {
        indexBig = (int )strlen(number2)-1;
    }

    if(*number1=='0')
    {
        *result = strdup(number2);
        if(!*result)
            return 3;
        return 0;
    }

    if(*number2=='0')
    {
        *result = strdup(number1);
        if(!*result)
            return 3;
        return 0;
    }

    if(*number1=='-' && *number2=='-')
    {
        if( add(number1+1,number2+1,result))
            return 3;
        char* temp = realloc(*result,strlen(*result)+2);
        if(!temp)
        {
            free(*result);
            return 3;
        }
        minuseFirst(temp);
        *result=temp;
        return 0;
    }
    if(*number2=='-')
    {

        return subtract(number1,number2+1,result);;
    }

    if(*number1=='-')
    {
        subtract(number1+1,number2,result);
        if(!*result)
            return 3;
        if(**result=='-')
            skipOne(result);
        else {
            char* temp = realloc(*result,strlen(*result)+2);
            if(!temp)
            {
                free(*result);
                return 3;
            }
            minuseFirst(temp);
            *result=temp;
        }
        return 0;
    }

    *result = calloc(indexBig+4,1);
    if(!*result)
        return 3;
    int index1 = (int)strlen(number1)-1;
    int index2 = (int)strlen(number2)-1;
    int indexRes = indexBig+1;
    setZeros(*result, indexRes+2);

    for(;;indexRes--, index1--, index2--)
    {
        if(index1<0 || index2<0)
            break;
        int temp = *(number1+index1) - '0';
        int temp2 = *(number2+index2) - '0';
        int res = *(*result+indexRes) - '0' + temp + temp2;
        if(res>9)
        {
            res%=10;
            *(*result+indexRes-1) +=1;
        }
        *(*result+indexRes) = (char)(res+'0');
    }
    if(index2<0)
    {
        for(;index1>=0;index1--,indexRes--)
        {
            int temp = *(number1+index1) - '0';
            int res = *(*result+indexRes) - '0' + temp;
            if(res>9)
            {
                res%=10;
                *(*result+indexRes-1) +=1;
            }
            *(*result+indexRes) = (char)(res+'0');
        }
    }
    if(index1<0)
    {
        for(;index2>=0;index2--,indexRes--)
        {
            int temp = *(number2+index2) - '0';
            int res = *(*result+indexRes) - '0' + temp;
            if(res>9)
            {
                res%=10;
                *(*result+indexRes-1) +=1;
            }
            *(*result+indexRes) = (char)(res+'0');
        }
    }
    destroy_zeros(*result);
    setTerminator(*result);

    return 0;
}

int count_digits(const char* text){
    int counter = 0;
    for(size_t i = 0; i < strlen(text); i++){
        if(isdigit(*(text+i))) counter++;
    }

    return counter;
}

int validate_expression(const char *expr)
{
    if(!expr)
        return 2;

    for(int i=0;*(expr+i)!='\0';i++)
    {
        if(!isdigit(*(expr+i)) && *(expr+i)!='-' && *(expr+i)!='+' && *(expr+i)!='*')
            return 1;
    }
    if(!isdigit(*expr) && *(expr)!='-')
        return 1;

    for(int i=1;*(expr+i)!='\0';i++)
    {
        if(!isdigit(*(expr+i)))
        {
            if((*(expr+i)=='*' && !isdigit(*(expr+i-1))) || (*(expr+i)=='*' && (!isdigit(*(expr+i+1)) && *(expr+i+1)!='-')))
                return 1;
            if((*(expr+i)=='+' && !isdigit(*(expr+i-1))) || (*(expr+i)=='+' && (!isdigit(*(expr+i+1)) && *(expr+i+1)!='-')))
                return 1;
            if(*(expr+i)=='-' && (*(expr+i-1)=='-' && !isdigit(*(expr+i+1))))
                return 1;
            if(*(expr+i)=='-' && (*(expr+i+1)=='+' || *(expr+i+1)=='*'))
                return 1;
            if((*(expr+i)=='-' || *(expr+i)=='*' || *(expr+i)=='+') && *(expr+i+1)==0)
                return 1;
        }
    }
    return 0;
}

char* multiply_me(const char* num, char n)
{
    if(!num)
        return 0;

    int len = count_digits(num);
    char* res;
    res = calloc(len+2,1);
    if(!res)
        return NULL;
    int r=0;
    for(int j=len-1;j>=0;j--)
    {
        int t = (int)(n-'0') * (int)(*(num+j)-'0') + r;
        r=0;
        if(t>=10)
        {
            r=t/10;
            t%=10;
        }
        *(res+j+1) = (char)(t+'0');
    }
    if(r>0)
    {
        *res =(char)(r+'0');
    }
    else
    {
        *res = *(res+1);
        for(int i=0;*(res+i)!='\0';i++)
        {
            *(res+i)=*(res+i+1);
        }
    }
    return res;
}

int multiply(const char* number1, const char* number2, char** result)
{
    if(!number1 || !number2 || !result)
        return 1;

    if(validate(number2) || validate(number1))
        return 2;

    if(*number1=='0' || *number2=='0')
    {
        *result=calloc(2,1);
        if(!*result)
            return 3;
        **result='0';
        return 0;
    }

    if(count_digits(number2)>count_digits(number1))
    {
        multiply(number2,number1,result);
        return 0;
    }


    int isneg1=0, isneg2=0;
    if(*number1=='-')
        isneg1=1;
    if(*number2=='-')
        isneg2=1;

    int len2 = (int)strlen(number2);

    if(isneg1&&isneg2)
    {
        int x=multiply(number1+1,number2+1,result);
        return x;
    }
    if(isneg1)
    {
        int x=multiply(number1+1,number2,result);
        int sizer=(int)strlen(*result);
        for(int i=(int)strlen(*result);i>0;i--)
        {
            *(*(result)+i) = *(*(result)+i-1);
        }
        **result='-';
        *(*(result)+sizer+1)=0;
        return x;
    }
    else if(isneg2)
    {
        int x=multiply(number1,number2+1,result);
        int sizer=(int)strlen(*result);
        for(int i=(int)strlen(*result);i>0;i--)
        {
            *(*(result)+i) = *(*(result)+i-1);
        }
        **result='-';
        *(*(result)+sizer+1)=0;
        return x;
    }


    int size=count_digits(number1)+count_digits(number2)+1;
    *result=calloc(size+1,1);
    if(!*result)
        return 3;


    for(int i=0;i<size;i++)
        *(*(result)+i)='0';

    size--;
    for(int i=len2-1,counter=0;i>=0;i--,size--,counter++)
    {
        char c = *(number2+i);
        char* temp = multiply_me(number1, c);
        if(!temp)
        {
            free(*result);
            return 3;
        }
        int r=0, s=size;
        for(int j=(int)strlen(temp)-1;j>=0;j--,s--)
        {
            int t = (int)*(*(result)+s)-'0' + (int)(*(temp+j)-'0') + r;
            r=0;
            if(t>=10)
            {
                r=t/10;
                t%=10;
            }
            *(*(result)+s) = (char)(t+'0');
        }

        *(*(result)+s)+=r;
        free(temp);
    }
    while(**result=='0')
    {
        for(int i=0;*(*(result)+i)!=0;i++)
            *(*(result)+i) = *(*(result)+i+1);
    }
    return 0;
}

int calculate(const char* expr, char **result)
{
    if(!expr || !result)
        return 1;

    if(validate_expression(expr))
        return 2;

    *result=calloc(3*strlen(expr)+1,1);
    if(!*result)
    {
        return 3;
    }

    int i=0;
    do {
        *(*(result)+i) = *(expr+i);
        i++;
    }while (isdigit(*(expr+i)));

    if(*(expr+i)==0)
        return 0;
    char operation;

    for(;;)
    {
        operation = *(expr+i);
        i++;
        int j=0;
        char *num2 = calloc(102,1);
        if(!num2)
        {
            free(*result);
            return 3;
        }
        do {
            *(num2+j) = *(expr+i);
            i++;j++;
        }while (isdigit(*(expr+i)));

        char* t = strdup(*result);
        free(*result);

        switch (operation) {
            case '+':
                if(add(t, num2, result))
                {
                    free(t);
                    free(num2);
                    return 3;
                }
                break;
            case '*':
                if(multiply(t, num2, result))
                {
                    free(t);
                    free(num2);
                    return 3;
                }
                break;
            case '-':
                if(subtract(t, num2, result))
                {
                    free(t);
                    free(num2);
                    return 3;
                }
                break;
        }
        free(t);
        free(num2);
        if(*(expr+i)==0)
            break;
    }
    return 0;
}

int main()
{
    char *t=calloc(501,1);
    if(!t)
    {
        printf("Failed to allocate memory");
        return 8;
    }
    printf("Podaj :");
    scanf("%500s", t);
    char *tt;
    int x = calculate(t, &tt);
    if(x==2)
    {
        printf("Incorrect input");
        free(t);
        return 1;
    }
    if(x==3)
    {
        printf("Failed to allocate memory");
        free(t);
        return 8;
    }
    printf("%s", tt);
    free(t);
    free(tt);

    return 0;
}