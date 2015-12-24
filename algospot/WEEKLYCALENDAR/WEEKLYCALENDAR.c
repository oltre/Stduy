#include <stdio.h>
#include <string.h>

int getMaxDayMonth(int month) {
	int maxday = 0;
	switch (month) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			maxday = 31;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			maxday = 30;
			break;
		default:
			maxday = 28;
	}

	return maxday;
}

int makeWeekly(int startday, int maxday, int * weekly) {
	int i;
	for (i = 0; i < 7; i++) {
		if (startday > maxday) {
			startday = 1;
		}
		weekly[i] = startday++;
	}
}

int main() {
	int i, startday, beforeM;
	int T;
	int M, D;
	char day[10];

	int maxday = 0;
	int weekly[7];

	scanf("%d", &T);
	while (T--) {
		scanf("%d %d %s", &M, &D, day);
		
		maxday = getMaxDayMonth(M);
		beforeM = M - 1;
		if (beforeM < 1) {
			beforeM = 12;
		}

		startday = 0;
		if (!strcmp(day, "Sunday")) {
			startday = D;
		} else if (!strcmp(day, "Monday")) {
			startday = D - 1;
		} else if (!strcmp(day, "Tuesday")) {
			startday = D - 2;
		} else if (!strcmp(day, "Wednesday")) {
			startday = D - 3;
		} else if (!strcmp(day, "Thursday")) {
			startday = D - 4;
		} else if (!strcmp(day, "Friday")) {
			startday = D - 5;
		} else if (!strcmp(day, "Saturday")) {
			startday = D - 6;
		}
		
		if (startday < 1) {
			maxday = getMaxDayMonth(beforeM);
			startday = maxday - (0 - startday);
		}
		makeWeekly(startday, maxday, weekly);

		for (i = 0; i < 7; i++) {
			printf("%d ", weekly[i]);
		}
		printf("\n");
	}
	return 0;
}
