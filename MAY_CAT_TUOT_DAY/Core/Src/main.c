/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "EOS.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "delay.h"
#include "i2c-lcd.h"
#include "port.h"
#include "encoder.h"
#include "dongco.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SELECT_PORT		GPIOB
#define BACK_PORT 		GPIOA
#define UP_PORT 			GPIOB
#define DOWN_PORT			GPIOB
#define BUTTON_PORT		GPIOB

#define SELECT_PIN 		GPIO_PIN_15
#define BACK_PIN 			GPIO_PIN_8
#define UP_PIN 				GPIO_PIN_14
#define DOWN_PIN 			GPIO_PIN_13
#define BUTTON_PIN		GPIO_PIN_12

//#define xungdonvi	28,571428
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* Khai bao bien the hien cac qua trinh chay cua may
	* process = 0; Chay qua trinh tuot day dau 1
	* process = 1; Qua trinh tuot day dau 2
	* process = 2; Qua trinh chay day du do dai
	* process = 3; Qua trinh cat roi nhich dau 2 ra mot doan
	* process = 4; Qua trinh tuot day dau 2
	* process = 5; Qua trinh chay day du do dai
	
*/
uint8_t process = 0;

/*Khai bao cac bien khac*/
extern uint16_t xungSetDao;
unsigned int a = 0;
char strNum[10];
uint8_t rx_data; //luu byte nhan duoc
uint8_t rx_buff[20]; //buffer luu chuoi nhan duoc
uint8_t _rxIndex; //con tro cua rxbuff

uint8_t tx_data[20]="DA STM32 Hello !!\r ";
uint16_t tx_flag = 0;
//uint8_t tx_data1[2] = (char)a;

uint16_t xung_dem1 = 0 ;
uint16_t xung_dem2 = 0 ;
uint16_t chieuquayEnc = 1; //1 la quay thuan, 0 la quay nghich
uint8_t setdao = 0; // 0 la chua set dao, 1 la da set dao
uint8_t sethienthisoluong = 0; // 0 la 

uint32_t trangthaimay = 0; // 0 la may tat, 1 la may bat
int loiday = 1;
int dodai = 200;
int dodaidautuot = 10;
int soluong = 1;
int soluonghientai = 0;

//Khai bao cac bien la so xung cua cac thong so

/*Khi vao while tinh cac gia tri xung*/

/*	  xung loi day = 
			xungDoDao = dodai*xungdonvi
			xungDodaituot = dodaituot*xungdonvi
*/
float xungdonvi	= 28.571428;
uint16_t xungLoiday = 1500; // xung loi day cho biet; 1600 tuong duong vs dao di dc 1mm
uint16_t xungDodai = 6286;
uint16_t xungDodaidautuot = 400;//143



char control = 0;
char buffer1[5];
char buffer2[5];
char buffer3[5];
char buffer4[5];
char buffer5[5];
uint8_t select = 0; // demtong
uint8_t back = 0; // demback
uint8_t up = 0;
uint8_t down = 0;
uint16_t count_menu1 = 0;
uint16_t count_menu2 = 0;
uint16_t count_menu3 = 0;
uint32_t up_down_main = 0;
uint32_t up_down_menuBatTat = 0;
uint32_t up_down_menuXacNhan = 0;
uint32_t up_down_menuThongSo = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void numUart(unsigned int num); //Ham hien thi len UART mot so nguyen duong

void mayCatTuot_Start(void);
void mayCatTuot_Stop(void);

//Khai bao cac chuong trinh cho menu
void home_screen(void);
void main_menu(void);
void select_main_menu(void);
void menuBatTat(void);
void select_menuCaiDat(void);
void select_menuBatTat(void);

void select_menuThongSo(void);
void thongSo(void);
void menuXacNhan(void);
void select_menuXacNhan(void);


void ok_ThongSo(void);
void change_ThongSo(void);
void select_button(void);
void back_button(void);
void up_button(void);
void down_button(void);


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
//int xungCat = 3199;
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint8_t MSG[50] = {'\0'};
  /* USER CODE END 1 */
	
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
	
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  MX_I2C1_Init();
	
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start(&htim1); // Khoi dong timer1 tao delay
	ENC1_START; //Khoi dong doc Encoder 1
	ENC2_START;	//Khoi dong doc Encoder 2
	lcd_init();
	lcd_clear();
	lcd_put_cur(0,2);
	lcd_send_string("DAI HOC CAN THO");
	lcd_put_cur(1,2);
	lcd_send_string("TRUONG BACH KHOA");
	lcd_put_cur(2,0);
	lcd_send_string("  ================ ");
	lcd_put_cur(3,2);
	lcd_send_string("MAY CAT TUOT DAY");
	delay_ms(500);
	home_screen();
	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
	EOS_init(1);
	setValueEnc1(32000);
	setValueEnc2(32000);
	
	setdao = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		
    /* USER CODE BEGIN 3 */
		
		xungDodai = round(dodai*xungdonvi);
		xungDodaidautuot = round(dodaidautuot*xungdonvi);
		xungLoiday = loiday*1500;
		
		if(trangthaimay == 1 && setdao == 1){
			if(process == 0){
				MOTOR1_QUAYTHUAN;
				MOTOR1_START;
				MOTOR2_QUAYTHUAN;
				MOTOR2_START;
				if(xung_dem1 >= (32000 + xungDodaidautuot)){
					MOTOR1_STOP;
					MOTOR2_STOP;
					setValueEnc1(32000);
					setValueEnc2(32000);
					tuotday_xuong(xungSetDao - xungLoiday); 
					process = 1;
				}
			}
			
			// keo day tuot dau 1, nhac dao tuot len
			if(process == 1){
				MOTOR1_QUAYNGHICH;
				MOTOR1_START;
				MOTOR2_QUAYNGHICH;
				MOTOR2_START;
				if(xung_dem1 <= (32000 - 100)){ 
					MOTOR1_STOP;
					setValueEnc1(32000);
					MOTOR2_STOP;
					setValueEnc2(32000);
					tuotday_len(xungSetDao - xungLoiday);
					process = 2;
				}
			}
		
			//Do do dai day 5
			if(process == 2){
				MOTOR1_QUAYTHUAN;
				MOTOR1_START;
				MOTOR2_QUAYTHUAN;
				MOTOR2_START;
				if(xung_dem1 >= (32000 + 100 + (xungDodai - 2*xungDodaidautuot))){
					setValueEnc1(32000);
					setValueEnc2(32000);
					MOTOR1_STOP;
					MOTOR2_STOP;
					tuotday_xuong(xungSetDao - xungLoiday);
					process = 3;
				}
			}
			
			if(process == 3){
				MOTOR1_QUAYNGHICH;
				MOTOR1_START;
				MOTOR2_QUAYNGHICH;
				MOTOR2_START;
				if(xung_dem1 <= (32000 - 100)){
					setValueEnc1(32000);
					setValueEnc2(32000);
					MOTOR1_STOP;
					MOTOR2_STOP;
					tuotday_len(xungSetDao - xungLoiday);
					process = 4;
				}
			}
			
			if(process == 4){
				MOTOR1_QUAYTHUAN;
				MOTOR1_START;
				MOTOR2_QUAYTHUAN;
				MOTOR2_START;
				if(xung_dem1 >= (32000 + (xungDodaidautuot - 100))){
					setValueEnc1(32000);
					setValueEnc2(32000);
					MOTOR1_STOP;
					MOTOR2_STOP;
					catDay();
					soluonghientai++; // Dem so luong day da hoan thanh
					if(soluonghientai == soluong) {
						trangthaimay = 0;
						sethienthisoluong = 0;
						process = 5;
					} else {
						process = 0;
					}
				}
			}
		}
		
		if(process == 5){
				MOTOR2_QUAYTHUAN;
				MOTOR2_START;
				if(xung_dem2 >= (32000 + xungDodai + 600 )){
					setValueEnc2(32000);
					MOTOR2_STOP;
					process = 0;
				}
			}
		
		
		if(trangthaimay == 0){
					MOTOR1_STOP;
					MOTOR2_STOP;
				}
		
		//	 Doan code menu lcd
		if(control == 0){
			if(select == 0) home_screen();
			select_button();
			back_button();
			up_button();
			down_button();
			ok_ThongSo();
		}
		
		if(control == 1){
			change_ThongSo();
			ok_ThongSo();
		}
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void SysTick_Handler(void)
{
	xung_dem1 = ENC1_XUNG; //Doc xung Encoder 1
	xung_dem2 = ENC2_XUNG; //Doc xung Encoder 2
}

void numUart(unsigned int num){
	sprintf(strNum,"%d",num); // chuyen so thanh chuoi
	HAL_UART_Transmit(&huart1, ((unsigned char*)strNum), sizeof(strNum), 100);
}

void home_screen(void){
	lcd_put_cur(0,0);
	lcd_send_string("MAY CAT VA TUOT DAY");
	if(trangthaimay == 1){
		lcd_put_cur(1,0);
		lcd_send_string("TRANG THAI MAY: BAT");
	}
	if(trangthaimay == 0){
		lcd_put_cur(1,0);
		lcd_send_string("TRANG THAI MAY: TAT");
	}
	lcd_put_cur(2,0);
	lcd_send_string("SL HIEN TAI: ");
	sprintf(buffer5,"%.3d",soluonghientai);
	lcd_send_string(buffer5);
	lcd_send_cmd(0x80|0x24);
	lcd_send_string(" day");
	lcd_put_cur(3,0);
	lcd_send_string(">THONG SO");
}

void thongSo(void){
	lcd_put_cur(0,0);
		lcd_send_string("DK LOI DAY DIEN ");
		sprintf(buffer1,"%.3d",loiday);
		lcd_send_string(buffer1);
		lcd_put_cur(1,0);
		lcd_send_string("DO DAI DAY      ");
		sprintf(buffer2,"%.3d",dodai);
		lcd_send_string(buffer2);
		lcd_put_cur(2,0);
		lcd_send_string("DO DAI DAU TUOT ");
		sprintf(buffer3,"%.3d",dodaidautuot);
		lcd_send_string(buffer3);
		lcd_put_cur(3,0);
		lcd_send_string("SO LUONG        ");
		sprintf(buffer4,"%.3d",soluong);
		lcd_send_string(buffer4);
}


void main_menu(void){
	if(up_down_main == 0){
		lcd_put_cur(0,0);
		lcd_send_string(" MAY CAT VA TUOT DAY");
		lcd_put_cur(1,0);
		lcd_send_string(">BAT/TAT MAY");
		lcd_put_cur(2,0);
		lcd_send_string(" CAI DAT THONG SO");
		lcd_put_cur(3,0);
		lcd_send_string(" XEM THONG SO DA CAI");
	}
	if(up_down_main == 1){
		lcd_put_cur(0,0);
		lcd_send_string(" MAY CAT VA TUOT DAY");
		lcd_put_cur(1,0);
		lcd_send_string(" BAT/TAT MAY");
		lcd_put_cur(2,0);
		lcd_send_string(">CAI DAT THONG SO");
		lcd_put_cur(3,0);
		lcd_send_string(" XEM THONG SO DA CAI");
	}
	if(up_down_main == 2){
		lcd_put_cur(0,0);
		lcd_send_string(" MAY CAT VA TUOT DAY");
		lcd_put_cur(1,0);
		lcd_send_string(" BAT/TAT MAY");
		lcd_put_cur(2,0);
		lcd_send_string(" CAI DAT THONG SO");
		lcd_put_cur(3,0);
		lcd_send_string(">XEM THONG SO DA CAI");
	}
}

void select_main_menu(void){
	switch(up_down_main){
		case 0:{
			lcd_put_cur(0,0);
			lcd_send_string("MAY CAT VA TUOT DAY");
			lcd_put_cur(1,0);
			lcd_send_string("  ================ ");
			lcd_put_cur(2,0);
			lcd_send_string("DA BAT MAY");
			GPIOC->BSRR = GPIO_PIN_13<<16u;
			break;
		}
		case 1:{
			lcd_put_cur(0,0);
			lcd_send_string("MAY CAT VA TUOT DAY");
			lcd_put_cur(1,0);
			lcd_send_string("  ================ ");
			lcd_put_cur(2,0);
			lcd_send_string("DA TAT MAY");
			GPIOC->BSRR = GPIO_PIN_13;
			break;
		}
	}
}

void menuBatTat(void){
	if(up_down_menuBatTat == 0){
		lcd_put_cur(0,0);
		lcd_send_string("MAY CAT VA TUOT DAY");
		lcd_put_cur(1,0);
		lcd_send_string("  ================ ");
		lcd_put_cur(2,0);
		lcd_send_string(">BAT MAY");
		lcd_put_cur(3,0);
		lcd_send_string(" TAT MAY");
	}
	if(up_down_menuBatTat == 1){
		lcd_put_cur(0,0);
		lcd_send_string("MAY CAT VA TUOT DAY");
		lcd_put_cur(1,0);
		lcd_send_string("  ================ ");
		lcd_put_cur(2,0);
		lcd_send_string(" BAT MAY");
		lcd_put_cur(3,0);
		lcd_send_string(">TAT MAY");
	}
}

void select_menuBatTat(void){
	switch(up_down_menuBatTat){
		case 0:{
			if(setdao == 1){
				lcd_put_cur(0,0);
				lcd_send_string("MAY CAT VA TUOT DAY");
				lcd_put_cur(1,0);
				lcd_send_string("  ================ ");
				lcd_put_cur(2,0);
				lcd_send_string("DA BAT MAY");		
				trangthaimay = 1;
				
				if(sethienthisoluong == 0) {
					soluonghientai = 0;
					sethienthisoluong = 1;
				}
				
				break;
			} else {
				lcd_put_cur(0,0);
				lcd_send_string("MAY CAT VA TUOT DAY");
				lcd_put_cur(1,0);
				lcd_send_string("  ================ ");
				lcd_put_cur(2,0);
				lcd_send_string("    CHUA SET DAO   ");
				lcd_put_cur(3,0);
				lcd_send_string("  VUI LONG SET DAO ");	
				break;
			}
		}
		case 1:{
			lcd_put_cur(0,0);
			lcd_send_string("MAY CAT VA TUOT DAY");
			lcd_put_cur(1,0);
			lcd_send_string("  ================ ");
			lcd_put_cur(2,0);
			lcd_send_string("DA TAT MAY");
			trangthaimay = 0;
			GPIOC->BSRR = GPIO_PIN_13;
			break;
		}
	}
}

void select_menuCaiDat(void){
	switch(trangthaimay){
		case 1:{ //May dang chay
			menuXacNhan();
			break;
		}
		case 0:{ //May da tat
			control = 1;
			change_ThongSo();
			break;
		}
	}
}

void menuXacNhan(void){
	if(up_down_menuXacNhan == 0){
		lcd_put_cur(0,0);
		lcd_send_string("BAN CO MUON TAT MAY");
		lcd_put_cur(1,0);
		lcd_send_string("DE THAY DOI THONG SO");
		lcd_put_cur(2,0);
		lcd_send_string(">CO                 ");
		lcd_put_cur(3,0);
		lcd_send_string(" KHONG              ");
	}
	if(up_down_menuXacNhan == 1){
		lcd_put_cur(0,0);
		lcd_send_string("BAN CO MUON TAT MAY");
		lcd_put_cur(1,0);
		lcd_send_string("DE THAY DOI THONG SO");
		lcd_put_cur(2,0);
		lcd_send_string(" CO                 ");
		lcd_put_cur(3,0);
		lcd_send_string(">KHONG              ");
	}
}

void select_menuXacNhan(void){
	switch(up_down_menuXacNhan){
		case 0:{
			trangthaimay = 0;
			control = 1;
			select_menuThongSo();
			break;
		}
		case 1:{ // Thoat ra menu truoc do
			select = 1;
			up_down_main = 0;
			main_menu();
			count_menu2 = 0;
			break;
		}
	}
}

void select_menuThongSo(void){
	if(up_down_menuThongSo == 0){
		lcd_put_cur(0,0);
		lcd_send_string(">DK LOI DAY DIEN ");
		sprintf(buffer1,"%.3d",loiday);
		lcd_send_string(buffer1);
		lcd_put_cur(1,0);
		lcd_send_string(" DO DAI DAY      ");
		sprintf(buffer2,"%.3d",dodai);
		lcd_send_string(buffer2);
		lcd_put_cur(2,0);
		lcd_send_string(" DO DAI DAU TUOT ");
		sprintf(buffer3,"%.3d",dodaidautuot);
		lcd_send_string(buffer3);
		lcd_put_cur(3,0);
		lcd_send_string(" SO LUONG        ");
		sprintf(buffer4,"%.3d",soluong);
		lcd_send_string(buffer4);
	}
	
	if(up_down_menuThongSo == 1){
		lcd_put_cur(0,0);
		lcd_send_string(" DK LOI DAY DIEN ");
		sprintf(buffer1,"%.3d",loiday);
		lcd_send_string(buffer1);
		lcd_put_cur(1,0);
		lcd_send_string(">DO DAI DAY      ");
		sprintf(buffer2,"%.3d",dodai);
		lcd_send_string(buffer2);
		lcd_put_cur(2,0);
		lcd_send_string(" DO DAI DAU TUOT ");
		sprintf(buffer3,"%.3d",dodaidautuot);
		lcd_send_string(buffer3);
		lcd_put_cur(3,0);
		lcd_send_string(" SO LUONG        ");
		sprintf(buffer4,"%.3d",soluong);
		lcd_send_string(buffer4);
	}
	if(up_down_menuThongSo == 2){
		lcd_put_cur(0,0);
		lcd_send_string(" DK LOI DAY DIEN ");
		sprintf(buffer1,"%.3d",loiday);
		lcd_send_string(buffer1);
		lcd_put_cur(1,0);
		lcd_send_string(" DO DAI DAY      ");
		sprintf(buffer2,"%.3d",dodai);
		lcd_send_string(buffer2);
		lcd_put_cur(2,0);
		lcd_send_string(">DO DAI DAU TUOT ");
		sprintf(buffer3,"%.3d",dodaidautuot);
		lcd_send_string(buffer3);
		lcd_put_cur(3,0);
		lcd_send_string(" SO LUONG        ");
		sprintf(buffer4,"%.3d",soluong);
		lcd_send_string(buffer4);
	}
	if(up_down_menuThongSo == 3){
		lcd_put_cur(0,0);
		lcd_send_string(" DK LOI DAY DIEN ");
		sprintf(buffer1,"%.3d",loiday);
		lcd_send_string(buffer1);
		lcd_put_cur(1,0);
		lcd_send_string(" DO DAI DAY      ");
		sprintf(buffer2,"%.3d",dodai);
		lcd_send_string(buffer2);
		lcd_put_cur(2,0);
		lcd_send_string(" DO DAI DAU TUOT ");
		sprintf(buffer3,"%.3d",dodaidautuot);
		lcd_send_string(buffer3);
		lcd_put_cur(3,0);
		lcd_send_string(">SO LUONG        ");
		sprintf(buffer4,"%.3d",soluong);
		lcd_send_string(buffer4);
	}
}

void up_button(void){
if((UP_PORT->IDR&UP_PIN)==0){
	delay_ms(10);
	if((UP_PORT->IDR&UP_PIN)==0){
		while((UP_PORT->IDR&UP_PIN)==0){}
	/*-------------scroll up in main_menu---------*/
			if(select == 1){ //
				if(up_down_main >= 2) up_down_main = 0;
					else up_down_main++;
				main_menu();
			}
	/*-------------scroll up in menuBatTat------------*/
			if(select == 2 && up_down_main == 0){ 
				if(up_down_menuBatTat >= 1) up_down_menuBatTat = 0;
					else up_down_menuBatTat++;
				menuBatTat();
			}
	/*----------scroll up in menuXacNhan----------*/
			if(select == 2 && up_down_main == 1){
				if(up_down_menuXacNhan >= 1) up_down_menuXacNhan = 0;
					else up_down_menuXacNhan++;
				menuXacNhan();
			}
		}
	}
}

void down_button(void){
	if((DOWN_PORT->IDR&DOWN_PIN)==0){
		delay_ms(10);
		if((DOWN_PORT->IDR&DOWN_PIN)==0){
			while((DOWN_PORT->IDR&DOWN_PIN)==0){}
		/*----------scroll down in main_menu---------*/
			if(select == 1){
				if(up_down_main <= 0) up_down_main = 2;
					else up_down_main--;
				main_menu();
			}
		/*-----------scroll down in menuBatTat-----------*/
			if((select == 2) && (up_down_main == 0)){
				if(up_down_menuBatTat <= 0) up_down_menuBatTat = 1;
					else up_down_menuBatTat--;
				menuBatTat();
			}
		/*----------scroll up in menuXacNhan----------*/
			if(select == 2 && up_down_main == 1){
				if(up_down_menuXacNhan <= 0) up_down_menuXacNhan = 1;
					else up_down_menuXacNhan--;
				menuXacNhan();
			}
		}
	}
}


void back_button(void){
	if((BACK_PORT->IDR&BACK_PIN)==0){
		delay_ms(10);
		if((BACK_PORT->IDR&BACK_PIN)==0){
			while((BACK_PORT->IDR&BACK_PIN)==0){
			}
			if(select > 0) lcd_clear();
			//-----do some thing-----------
			if((select == 1) && (up_down_main == 0)) {home_screen();select--;} 					// back to home_screen
			if((select == 2) && (up_down_main == 0)) {main_menu();select--; count_menu1 = 0;} 	// back to main_menu
			if((select == 3) && (count_menu1 == 1)) {menuBatTat();select--;} 										// back to menuBatTat
			if((select == 3) && (count_menu1 == 2)) {menuBatTat();select--; count_menu1--;} 			//back to menuBatTat
			if((select == 4) && (count_menu1 == 2)) {select--;}						//
			
			if((select == 1) && (up_down_main == 1)) {home_screen();select--;} 					// back to home_screen
			if((select == 2) && (up_down_main == 1)) {main_menu();select--; count_menu2 = 0;}	//
			if((select == 3) && (count_menu2 == 1)) {select--;}						//
			
			if((select == 1) && (up_down_main == 2)) {home_screen();select--;}			// back to home_screen
			if((select == 2) && (up_down_main == 2)) {main_menu();select--;count_menu3 = 0;}
			
		}
	}
}
 
void select_button(void){
	if((SELECT_PORT->IDR&SELECT_PIN)==0){
		delay_ms(10);
		if((SELECT_PORT->IDR&SELECT_PIN)==0){
			while((SELECT_PORT->IDR&SELECT_PIN)==0){}
			lcd_clear();
			//-----do some thing-----------
			select++;
			//Cac tang cua menu1
			if(select == 1) {main_menu();} 						// Open main_menu
			if(select == 2 && up_down_main == 0) {up_down_menuBatTat = 0; menuBatTat();count_menu1++;}	// Open menuBatTat
			if(select == 3 && count_menu1 == 1) {select_menuBatTat();}
			if(select == 4 && count_menu1 == 1) {select = 3;select_menuBatTat();}
			
			//Cac tang cua menu2
			if(select == 2 && up_down_main == 1) {count_menu2++;select_menuCaiDat();}
			if(select == 3 && count_menu2 == 1) {select_menuXacNhan();} // Open menu thay doi thong so
			if(select == 4 && count_menu2 == 1) select = 3;
			
			//Cac tang cua menu3
			if(select == 2 && up_down_main == 2) {thongSo();count_menu3++;}				// Open menu xem thong so da cai dat
			if(select == 3 && count_menu3 == 1) {select = 2;thongSo();}
			
			if(select >= 4){select = 3;}
		}
	}
}

void ok_ThongSo(void){
	
	if((BUTTON_PORT->IDR&BUTTON_PIN)==0){
		delay_ms(10);
		if((BUTTON_PORT->IDR&BUTTON_PIN)==0){
			while((BUTTON_PORT->IDR&BUTTON_PIN)==0){
				if((SELECT_PORT->IDR&SELECT_PIN)==0 || ((BACK_PORT->IDR&BACK_PIN)==0) || ((UP_PORT->IDR&UP_PIN)==0)|| ((DOWN_PORT->IDR&DOWN_PIN)==0)){
					control = 0;
				//Thoat ra menu truoc do
					select = 1;
					up_down_main = 0;
					lcd_clear();
					main_menu();
					count_menu2 = 0;
				}
			}
			
			up_down_menuThongSo++;
			if(up_down_menuThongSo >= 4) up_down_menuThongSo = 0;
			
			if(trangthaimay == 0 && setdao == 0){ //neu may tat va dao chua set
				setDao();
				HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
				setdao = 1;
			}
			
		}
	}
}

void change_ThongSo(void){
	select_menuThongSo();
	if((UP_PORT->IDR&UP_PIN)==0) //Count up 10
	{
		delay_ms(10);
		if((UP_PORT->IDR&UP_PIN)==0){
			while((UP_PORT->IDR&UP_PIN)==0){}
				switch(up_down_menuThongSo){
								case 0:{loiday-=10;if(loiday <= 0) loiday = 1;break;}
								case 1: {dodai-=10; if(dodai <= 150) dodai = 150; break;}
								case 2: {dodaidautuot-=10; if(dodaidautuot <= 10) dodaidautuot = 10; break;}
								case 3: {soluong-=10;if(soluong <= 0) soluong = 0; break;}
							}
		}
	}
	
	if((SELECT_PORT->IDR&SELECT_PIN)==0) //Count up 1
	{
		delay_ms(10);
		if((SELECT_PORT->IDR&SELECT_PIN)==0){
			while((SELECT_PORT->IDR&SELECT_PIN)==0){}
				switch(up_down_menuThongSo){
					case 0: {loiday++;if(loiday >= 2) loiday = 2; break;}
					case 1: {dodai++; break;}
					case 2: {dodaidautuot++;break;}
					case 3: {soluong++;break;}		
				}
			}
	}
	
	if((DOWN_PORT->IDR&DOWN_PIN)==0) //Count down 10
	{
		delay_ms(10);
		if((DOWN_PORT->IDR&DOWN_PIN)==0){
			while((DOWN_PORT->IDR&DOWN_PIN)==0){}
				switch(up_down_menuThongSo){
							case 0: {loiday+=10; if(loiday >= 2) loiday = 2; break;}
							case 1: {dodai+=10; break;}
							case 2: {dodaidautuot+=10;break;}
							case 3: {soluong+=10;break;}
						}
			}
	}
	
	if((BACK_PORT->IDR&BACK_PIN)==0) //Count down 1
	{
		delay_ms(10);
		if((BACK_PORT->IDR&BACK_PIN)==0){
			while((BACK_PORT->IDR&BACK_PIN)==0){}
				switch(up_down_menuThongSo){
						case 0: {loiday--;if(loiday <= 0) loiday = 1;break;}
						case 1: {dodai--; if(dodai <= 150) dodai = 150; break;}
						case 2: {dodaidautuot--; if(dodaidautuot <= 10) dodaidautuot = 10; break;}
						case 3: {soluong--;if(soluong <= 0) soluong = 0; break;}
					}
		}
	}
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
