#include <stdio.h>
#include <time.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define TOTAL_MONTHS 12
#define JANUARY 1
#define FEBRUARY 2
#define MARCH 3
#define APRIL 4
#define MAY 5
#define JUNE 6
#define JULY 7
#define AUGUST 8
#define SEPTEMBER 9
#define OCTOBER 10
#define NOVEMBER 11
#define DECEMBER 12

#define TOTAL_YEAR_DAYS 365
#define TWENTY_EIGHT 28
#define THIRTY 30
#define THIRTY_ONE 31

#define FULL_SEGMENT "█"
#define EMPTY_SEGMENT "░"
#define LEFT_BAR_LIMIT "⎢"
#define RIGHT_BAR_LIMIT "⎟"
#define MAX_BAR_LENGTH 100

/* ============= Colors ================ */
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_LIGHT_RED     "\x1b[1;31m"

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_LIGHT_GREEN   "\x1b[1;32m"

#define ANSI_COLOR_YELLOW  "\x1b[33m"

#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_CYAN    "\x1b[36m"

#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_LIGHT_MAGENTA "\x1b[1;35m"

#define ANSI_COLOR_RESET   "\x1b[0m"
/* ===================================== */

#define DEFAULT 0
#define RED 1
#define GREEN 2
#define MAGENTA 3
#define PARTY 4
#define ERROR -1

#define ERROR_MESSAGE "ERROR. Use => year_progress -<color_mode>\n  Possible colors: \n -<nothing> default (Blue) \n -RED (Red)\n -GREEN (Green)\n -MAGENTA (Magenta)\n -PARTY (????)"

#define ON 1
#define OFF 0

int get_days (int current_month){
	int total_days = 0, month;
	for (month = JANUARY; month <= current_month; ++month) {
		switch (month) {
			case APRIL:
			case JUNE:
			case SEPTEMBER:
			case NOVEMBER:
				total_days += THIRTY;
				break;
			case JANUARY:
			case MARCH:
			case MAY:
			case JULY:
			case AUGUST:
			case OCTOBER:
			case DECEMBER:
				total_days += THIRTY_ONE;
      	break;
			case FEBRUARY:
				total_days += TWENTY_EIGHT;
      	break;
		}
	}
	return total_days;
}

void draw_bar (int color) {

	unsigned int ms = 30000;
	time_t current_time = time(NULL);
	struct tm tm = *localtime(&current_time);
	int total_days = get_days(tm.tm_mon) + tm.tm_mday;
	struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);
	int n_columns = w.ws_col;
	int i, percentage = (int)(total_days * MAX_BAR_LENGTH)/TOTAL_YEAR_DAYS;
	int party_mode = OFF;
	srand(time(NULL));

	if (color == PARTY) {
		party_mode = ON;
	}

	printf("\n");

	printf("%s", LEFT_BAR_LIMIT);

	for (i = 0; i < (n_columns - 10); ++i) {

		if (party_mode == ON) {
			color = ( rand() % 4 );
		}

		if (i <= (percentage*n_columns)/MAX_BAR_LENGTH) {

			switch (color) {
				case 0:
					printf( ANSI_COLOR_BLUE "%s", FULL_SEGMENT);
				break;
				case 1:
					printf( ANSI_COLOR_RED "%s", FULL_SEGMENT);
				break;
				case 2:
					printf( ANSI_COLOR_GREEN "%s", FULL_SEGMENT);
				break;
				case 3:
					printf( ANSI_COLOR_MAGENTA "%s", FULL_SEGMENT);
				break;
			}

		} else {

			switch (color) {
				case 0:
					printf( ANSI_COLOR_CYAN "%s", EMPTY_SEGMENT);
				break;
				case 1:
					printf( ANSI_COLOR_LIGHT_RED "%s", EMPTY_SEGMENT);
				break;
				case 2:
					printf( ANSI_COLOR_LIGHT_GREEN "%s", EMPTY_SEGMENT);
				break;
				case 3:
					printf( ANSI_COLOR_LIGHT_MAGENTA "%s", EMPTY_SEGMENT);
				break;
			}
		}
		fflush(stdout);
    usleep(ms);
	}
	usleep(ms);
	printf( ANSI_COLOR_RESET "%s %d％", RIGHT_BAR_LIMIT, percentage);
	printf("\n\n");

}

int correct_arg (const char color_str[]) {

	int color = DEFAULT;

	if (strcmp(color_str, "-RED") == 0) {
		color = RED;
	} else if (strcmp(color_str, "-GREEN") == 0) {
		color = GREEN;
	} else if (strcmp(color_str, "-MAGENTA") == 0) {
		color = MAGENTA;
	} else if (strcmp(color_str, "-PARTY") == 0) {
		color = PARTY;
	} else {
		color = ERROR;
	}

	return color;
}

int main (int argc, char const *argv[]){

	int color;

	if (argc == 2) {
		color = correct_arg(argv[1]);
	}

	if ( argc == 1 || (argc == 2 && color != -1) ){
		draw_bar(color);
	} else {
		printf("%s\n", ERROR_MESSAGE);
	}

	return 0;
}
