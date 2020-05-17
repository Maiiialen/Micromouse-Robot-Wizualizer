/*
 * robot.c
 *
 *  Created on: 1 kwi 2020
 *      Author: Maialen
 */
#include "robot.h"
#include "tim.h"
#include "funkcje_rysujace.h"
#include "adc.h"

#define MAX_PREDKOSC 1000

void jedzProsto(Robot* robot) {
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_2, 0);

	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0.5*MAX_PREDKOSC);
	__HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_1, 0.5*MAX_PREDKOSC);

	HAL_Delay(1500);

	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_1, 0);

	rysujPolaczeniePrzedWejsciem(robot);
	HAL_Delay(500);
	rysujKwadratPusty(robot->posX, robot->posY);
	switch(robot->orientacja) {
	case 0:
		robot->posY--;
		break;
	case 1:
		robot->posX++;
		break;
	case 2:
		robot->posY++;
		break;
	case 3:
		robot->posX--;
		break;
	}

	rysujKwadratPelny(robot->posX, robot->posY);
	rysujPolaczeniePoWejsciu(robot);

	rysujPozycje(robot->posX, robot->posY);
}

// strona = 0 - obrot w lewo;
// strona = 1 - obrot w prawo
void obroc(Robot* robot, int strona) {
	// TODO //
	// Funkcja obslugujaca obrot w zadanym kierunku

	switch(strona) {
	case 0:
		robot->orientacja--;
		robot->orientacja %=4;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0.5*MAX_PREDKOSC);
		__HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_2, 0);
		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_1, 0.5*MAX_PREDKOSC);
		break;
	case 1:
		robot->orientacja++;
		robot->orientacja %=4;
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_2, 0.5*MAX_PREDKOSC);
		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0.5*MAX_PREDKOSC);
		__HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_1, 0);
		break;
	}

	HAL_Delay(300);

	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_2, 0);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0);
	__HAL_TIM_SET_COMPARE(&htim9, TIM_CHANNEL_1, 0);

	HAL_Delay(500);
}

void jedzPrawo(Robot* robot) {
	obroc(robot, 1);
	jedzProsto(robot);
}

void jedzLewo(Robot* robot) {
	obroc(robot, 0);
	jedzProsto(robot);
}

void jedzTyl(Robot* robot) {
	obroc(robot, 1);
	obroc(robot, 1);
	jedzProsto(robot);
}

void skanujObszar(Robot* robot) {
	void ADC_SetActiveChannel(ADC_HandleTypeDef *hadc, uint32_t AdcChannel);

	if(HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
		robot->odczytCzujnikow[0] = HAL_ADC_GetValue(&hadc1);
		ADC_SetActiveChannel(&hadc1, ADC_CHANNEL_11);
		HAL_ADC_Start(&hadc1);
	}
	if(HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
		robot->odczytCzujnikow[1] = HAL_ADC_GetValue(&hadc1);
		ADC_SetActiveChannel(&hadc1, ADC_CHANNEL_13);
		HAL_ADC_Start(&hadc1);
	}
	if(HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
		robot->odczytCzujnikow[2] = HAL_ADC_GetValue(&hadc1);
		ADC_SetActiveChannel(&hadc1, ADC_CHANNEL_7);
		HAL_ADC_Start(&hadc1);
	}

	if(HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
		robot->odczytCzujnikow[3] = HAL_ADC_GetValue(&hadc1);
		ADC_SetActiveChannel(&hadc1, ADC_CHANNEL_5);
		HAL_ADC_Start(&hadc1);
	}

	rysujCzujniki(robot->odczytCzujnikow[0], robot->odczytCzujnikow[1], robot->odczytCzujnikow[2], robot->odczytCzujnikow[3]);
}
