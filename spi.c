/*Bài tập phần SPI:

BT1: Viết code hiển thị MSSV trên module Max7219

BT2: Viết code tạo số ngẫu nhiên trong khoảng [0;10000] mỗi 1s, sau đó hiển thị số này lên module Max7219. Lưu ý xóa các chữ số 0 vô nghĩa.

BT3: Viết code tạo 2 số thực ngẫu nhiên T và H:
        - T có giá trị trong khoảng [-20;40], 
        - H có giá trị trong khoảng [40; 80]
     Hiển thị T trên 4 led 7-đoạn bên trái, H trên 4 led 7-đoạn bên phải của module max7219
         vd:  -16.3_50.2

BT4: Viết code hiển thị hình trái tim trên module led ma trận

BT5: Cho phần cứng bao gồm raspberry, module nút nhấn, module led ma trận. Viết code cho trò chơi rắn săn mồi.
*/
#include "wiringPiSPI.h"
#include "stdint.h"
#include "wiringPi.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#define spi0 0
uint8_t mssv[8]= {2,1,1,4,6,2,9,7};
uint8_t num[8];
uint8_t count;
void sendData(uint8_t address, uint8_t value){
    uint8_t data[2];
    data[0]= address;
    data[1]= value;
    wiringPiSPIDataRW(spi0,data,2);
}
//cau1
void MSSV(){
    for(int i = 0; i < 8 ; i++){
        sendData(i+1,mssv[7-i]);
    }
}
//cau 2
int len_num(uint16_t r_n){ // do dai cua so

    while(r_n !=0){
        
        count++;
        r_n /=10;
    }
    return count;
}
void random_num(){
    uint16_t rand_num;
    rand_num = rand() % 10000;
    len_num(rand_num);
    sendData(0x0B,count-1);
    printf("random %d\n", rand_num);
    for(int i = 0; i <= count;i++){
        uint8_t b = rand_num % 10;
        sendData(i+1,b);
        rand_num /=10;
        if (i == count)
            {
                count = 0;
            }
        
    }
    delay(3000);
}
//cau 3
void random_float(){
    sendData(0x0B,7);
    srand(time(NULL));
    float randomValue_T = ((float)rand() / RAND_MAX) * 60.0 - 20.0;
    printf("Số thực ngẫu nhiên từ -20 đến 40 là: %.2f\n", randomValue_T);
    if(randomValue_T < 0) randomValue_T = randomValue_T * -1;
    float phan_thap_phan_T = randomValue_T - (int)randomValue_T;

    uint8_t temp1 = (int)(randomValue_T);
    uint8_t b_1 = (temp1%10)|0x80;
    sendData(6,b_1);
    temp1 = randomValue_T/10; 
    sendData(7,temp1%10);
    uint8_t a_1 = (int)(phan_thap_phan_T*10);
    sendData(5,a_1);


    float randomValue_H = ((float)rand() / RAND_MAX) * 40.0 + 40.0;
    printf("Số thực ngẫu nhiên từ 40 đến 80 là: %.2f\n", randomValue_H);
    float phan_thap_phan_H = randomValue_H - (int)randomValue_H;

    uint8_t temp2 = (int)(randomValue_H);
    uint8_t b_2 = (temp2%10)|0x80;
    sendData(2,b_2);
    temp2 = randomValue_H/10; 
    sendData(3,temp2%10);
    uint8_t a_2 = (int)(phan_thap_phan_H*10);
    sendData(1,a_2);

    delay(5000);
}

int main(void){
    wiringPiSPISetup(spi0,8*10^6);

    sendData(0x09,0xFF); //decode mode
    sendData(0x0A,0x08);
    
    sendData(0x0C,1);
    sendData(0x0F,0);

    
    while(1){
        random_float();
    }
    return 0;
}