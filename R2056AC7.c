#include <time.h>
#include <stdio.h>
int m = 0;
int zero = 0;
int inp = 0;
int fc = 0;
int carry = 0;
int A = 0;
int X = 0;
int Y = 0;
int SP = 0;
int P = 0;
int len = 0;
int op;
int cycles = 0;
double mult;
double cps;
int low;
int high;
int cachBool[2000] = {0};
struct argStore{
    int type;
    int a,b,c,fk,cyc;
};
struct argStore cache[2000];
int icarus_pro[] = {
    // A = 'A'
    0xA9, 65,
    0x8D, 111,

    // A = 'B'
    0xA9, 66,
    0x8D, 111,

    // A = 'C'
    0xA9, 67,
    0x8D, 111,

    313
};
int icarus_mem[65536] = {0};
static inline void oA9(){
    inp = icarus_pro[fc+1];
    inp&=0xFF;
    A = inp;
    zero = (A==0);
    fc+=2;
    cycles+=2;
}
static inline void oA5(){
    inp = icarus_pro[fc+1];
    inp&=0xFF;
    A = icarus_mem[inp];
    zero = (A==0);
    fc+=2;
    cycles+=3;
}
static inline void oAD(){
    low = icarus_pro[fc+1];
    high = icarus_pro[fc+2];
    inp = (high<<8)|low;
    A = icarus_mem[inp];
    zero = (A==0);
    fc+=3;
    cycles+=4;
}
static inline void o29(){
    inp = icarus_pro[fc+1];
    A&=inp;
    zero = (A==0);
    fc+=2;
}
static inline void o09(){
    inp = icarus_pro[fc+1];
    A|=inp;
    zero = (A==0);
    fc+=2;
}
static inline void o49(){
    inp = icarus_pro[fc+1];
    A^=inp;
    zero = (A==0);
    fc+=2;
}
static inline void oA2(){
    inp = icarus_pro[fc+1];
    inp&=0xFF;
    X = inp;
    zero = (X==0);
    fc+=2;
    cycles+=2;
}
static inline void oA0(){
    inp = icarus_pro[fc+1];
    inp&=0xFF;
    Y = inp;
    zero = (Y==0);
    fc+=2;
    cycles+=2;
}
static inline void o8D(){
    low = icarus_pro[fc+1];
    high = icarus_pro[fc+2];
    inp = (high << 8) | low;
    inp&=0xFFFF;
    icarus_mem[inp] = A;
    fc+=3;
    cycles+=4;
}
static inline void o86(){
    inp = icarus_pro[fc+1];
    inp&=0xFF;
    icarus_mem[inp] = X;
    fc+=2;
    cycles+=3;
}
static inline void o84(){
    inp = icarus_pro[fc+1];
    inp&=0xFF;
    icarus_mem[inp] = Y;
    fc+=2;
    cycles+=3;
}
static inline void o96(){
    inp = icarus_pro[fc+1];
    inp+=Y;
    inp&=0xFF;
    icarus_mem[inp] = X;
    fc+=2;
    cycles+=3;
}
static inline void o24(){
    low = icarus_pro[fc+1];
    high = icarus_pro[fc+2];
    inp = (high << 8) | low;
    zero = (A&inp);
    fc+=3;
}
static inline void o94(){
    inp = icarus_pro[fc+1];
    inp+=X;
    inp&=0xFF;
    icarus_mem[inp] = Y;
    fc+=2;
    cycles+=3;
}
static inline void o8E(){
    low = icarus_pro[fc+1];
    high = icarus_pro[fc+2];
    inp = (high << 8) | low;
    inp &=0xFFFF;
    icarus_mem[inp] = X;
    fc+=3;
    cycles+=3;
}
static inline void o8C(){
    low = icarus_pro[fc+1];
    high = icarus_pro[fc+2];
    inp = (high << 8) | low;
    inp &=0xFFFF;
    icarus_mem[inp] = Y;
    fc+=3;
    cycles+=3;
}
static inline void o69(){
    inp = icarus_pro[fc+1];
    A+=inp+carry;
    carry = (A > 255);
    A&=0xFF;
    zero = (A==0);
    fc+=2;
    cycles+=2;
}
static inline void o65(){
    inp = icarus_pro[fc+1];
    A+=icarus_mem[inp]+carry;
    carry = (A > 255);
    A&=0xFF;
    zero = (A==0);
    fc+=2;
    cycles+=2;
}
static inline void o6D(){
    low = icarus_pro[fc+1];
    high = icarus_pro[fc+2];
    inp = (high << 8) | low;
    A+=icarus_mem[inp]+carry;
    carry = (A > 255);
    A&=0xFF;
    zero = (A==0);
    fc+=3;
    cycles+=4;
}
static inline void o4C(){
    low = icarus_pro[fc+1];
    high = icarus_pro[fc+2];
    inp = (high << 8) | low;
    fc = inp;
}
static inline void oE9(){
    inp = icarus_pro[fc+1];
    A-=inp-(1-carry);
    if (A>255){
        A&=0xFF;
        carry = 1;
    }
    else{
        carry = 0;
    }
    zero = (A==0);
    fc+=2;
    cycles+=2;
}
static inline void oE8(){
    X++;
    if (X>255){
        X&=0xFF;
        carry = 1;
    }
    else{
         carry = 0;
    }
    zero = (X==0);
    fc++;
    cycles+=2;
}
static inline void oCA(){
    X--;
    if (X>255){
        X&=0xFF;
        carry = 1;
    }
    else{
         carry = 0;
    }
    zero = (X==0);
    fc++;
    cycles+=2;
}
static inline void oE6(){
    inp = icarus_pro[fc+1];
    icarus_mem[inp]++;
    icarus_mem[inp]&=0xFF;
    fc++;
    cycles+=2;
}  
static inline void oC6(){
    inp = icarus_pro[fc+1];
    icarus_mem[inp]--;
    icarus_mem[inp]&=0xFF;
    fc++;
    cycles+=2;
} 
static inline void oC8(){
    Y++;
    if (Y>255){
        Y&=0xFF;
        carry = 1;
    }
    else{
        carry = 0;
    }
    zero = (Y==0);
    fc++;
    cycles+=2;
}
static inline void o88(){
    Y--;
    if (Y>255){
        Y&=0xFF;
        carry = 1;
    }
    else{
        carry = 0;
    }
    zero = (Y==0);
    fc++;
    cycles+=2;
}
static inline void oC9(){
    inp = icarus_pro[fc+1] & 0xFF;
    carry = (A>=inp);
    zero = (A==inp);
    fc+=2;
    cycles+=2;
}
static inline void oE0(){
    inp = icarus_pro[fc+1] & 0xFF;
    carry = (X>=inp);
    zero = (X==inp);
    fc+=2;
    cycles+=2;
}
static inline void oC0(){
    inp = icarus_pro[fc+1] & 0xFF;
    carry = (Y>=inp);
    zero = (Y==inp);
    fc+=2;
    cycles+=2;
}
static inline void oAA(){
    X = A;
    fc++;
    cycles+=2;
}
static inline void o8A(){
    A = X;
    fc++;
    cycles+=2;
}
static inline void oA8(){
    Y = A;
    fc++;
    cycles+=2;
}
static inline void o9A(){
    SP = X;
    fc++;
    cycles+=2;
}
static inline void oEA(){
    fc++;
    cycles+=2;
}
static inline void oF0(){
    inp = icarus_pro[fc+1];
    if (zero){
        fc+=2+inp;
    }
    else{
        fc +=2;
    }
    cycles+=2;
}
static inline void oB0(){
    inp = icarus_pro[fc+1];
    if (carry){
        fc+=2+inp;
    }
    else{
        fc +=2;
    }
    cycles+=2;
}
static inline void o90(){
    inp = icarus_pro[fc+1];
    if (!carry){
        fc+=2+inp;
    }
    else{
        fc +=2;
    }
    cycles+=2;
}
static inline void oEE(){
    inp = icarus_pro[fc+1];
    if (carry){
        fc+=2+inp;
    }
    else{
        fc +=2;
    }
    icarus_mem[inp] = (icarus_mem[inp]+1) & 0xFF;
    fc+=2;
    cycles+=6;
}
int main(){
    struct timespec start, end;
    for (;icarus_pro[len]!=313;len++){} // Loop through the program memory until the stop code 313 is found where the increment, len, will store the length of the program
    //printf("%d\n",len);
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int l = 0; l<5000000; l++){
    fc = 0;
    while (fc < len){
	int op = icarus_pro[fc];
        if (cachBool[fc]){
        switch(op) { //The decoding switch for the cache
            case 0xA9:
                inp = icarus_pro[fc+1];
                inp&=0xFF;
                cache[fc].a = inp;
                A = inp;
                zero = (A==0);
                fc+=2;
                cycles+=2;
                cachBool[fc] = 1;
                break;
            case 0xA2:
                oA2();
                break;
            case 0xA0:
                oA0();
                break;
            case 0x8D:
                o8D();
                break;
            case 0x86:
                o86();
                break;
            case 0x84:
                o84();
                break;
            case 0x69:
                o69();
                break;
            case 0xE9:
                oE9();
                break;
            case 0xE8:
                oE8();
                break;
            case 0xC8:
                oC8();
                break;
            case 0xC9:
                oC9();
                break;
            case 0xAA:
                oAA();
                break;
            case 0x8A:
                o8A();
                break;
            case 0xA8:
                oA8();
                break;
            case 0x9A:
                o9A();
                break;
            case 0xEA:
                oEA();
                break;
            case 0xF0:
                oF0();
                break;
            case 0xB0:
                oB0();
                break;
            case 0x90:
                o90();
                break;
            }
        }
        else{
        switch(op) { //The execution switch for the cache
            case 0xA9:
                A = cache[fc].a;
                zero = (A==0);
                fc+=2;
                cycles+=2;
                break;
            case 0xA2:
                oA2();
                break;
            case 0xA0:
                oA0();
                break;
            case 0x8D:
                o8D();
                break;
            case 0x86:
                o86();
                break;
            case 0x84:
                o84();
                break;
            case 0x69:
                o69();
                break;
            case 0xE9:
                oE9();
                break;
            case 0xE8:
                oE8();
                break;
            case 0xC8:
                oC8();
                break;
            case 0xC9:
                oC9();
                break;
            case 0xAA:
                oAA();
                break;
            case 0x8A:
                o8A();
                break;
            case 0xA8:
                oA8();
                break;
            case 0x9A:
                o9A();
                break;
            case 0xEA:
                oEA();
                break;
            case 0xF0:
                oF0();
                break;
            case 0xB0:
                oB0();
                break;
            case 0x90:
                o90();
                break;
            }
        }
            if (inp == 111) {
                printf("%c", (char)icarus_mem[111]);
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec-start.tv_sec) + (end.tv_nsec-start.tv_nsec)/1e9;
    double cps = cycles/elapsed;
    printf("\n%f Cycles per Second\n",cps);
    printf("\n");
    printf("A: %d\n",A);
    printf("X: %d\n",X);
    printf("Y: %d\n",Y);
    printf("SP: %d\n",SP);
    printf("P: %d\n",P);
    for (int i = 0; i<65536;i++){
        if (icarus_mem[i] != 0){
            printf("Memory Slot %d\n",i);
            printf("Value: %d\n",icarus_mem[i]);
           // printf("\n");
        }   
    }
    return 0;
    }