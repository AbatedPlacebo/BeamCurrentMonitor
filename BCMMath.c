#include "BCMMath.h"

double calcQ(int* arr, int size, int wnd1, int wnd2, double QK, int gain, double gainK){
	double integral = 0.0;
	int i;
	for (i = wnd1; i < wnd2; i++){
		integral += fabs(arr[i]); 
	}
	return QK * pow(10.0, -gain * gainK / 20.0) * integral;

}
