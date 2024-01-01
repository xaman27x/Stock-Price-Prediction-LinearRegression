#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

double regression_calc(double close[], double x[], int len);
void regression_coefficients(char *fname, double *global_beta_close_and_open, double *global_beta_close_and_high, double *global_beta_close_and_low, double *global_beta_close_and_volume);

typedef struct {
    int day;
    int month;
} Date;

double get_daily_high(char* fname) {
    double high = -INFINITY; 
    int i = 0, count = 0;
    char buffer[256], c, *p;
    int fd = open(fname, O_RDONLY);
    if (fd < 0) {
        perror("Error Opening Data File");
        return high; 
    }

    while (read(fd, &c, 1) > 0) {
        if (c == '\n') {
            buffer[i] = '\0';
            p = strtok(buffer, " ");
            p = strtok(NULL, ",");
            if (p != NULL) { 
                double value = atof(p);
                if (high < value) {
                    high = value;
                }
            }
            count++;
            i = 0; 
            break;
        } else {
            buffer[i++] = c;
        }
    }

    close(fd); 
    return high;
}

double get_weekly_high(char* fname) {
    double high = -INFINITY; 
    int i = 0, count = 0;
    char buffer[256], c, *p;
    int fd = open(fname, O_RDONLY);
    if (fd < 0) {
        perror("Error Opening Data File");
        return high; 
    }

    for (int z = 0; z <= 24; z++) {
        while (read(fd, &c, 1) > 0 && c != '\n') {
	}
    }

    while (read(fd, &c, 1) > 0 && count < 8) {
        if (c == '\n') {
            buffer[i] = '\0';
            p = strtok(buffer, ",");
            p = strtok(NULL, ",");
            if (p != NULL) { 
                double value = atof(p);
                if (high < value) {
                    high = value;
                }
            }
            count++;
            i = 0; 
        } else {
            buffer[i++] = c;
        }
    }

    close(fd); 
    return high;
}

double get_monthly_high(char* fname) {
    double high = -INFINITY; 
    int i = 0;
    char buffer[256], c, *p;
    int fd = open(fname, O_RDONLY);
    if (fd < 0) {
        perror("Error Opening Data File");
        return high; 
    }
    while (read(fd, &c, 1) > 0 && c != '\n') {
		}

    while (read(fd, &c, 1) > 0) {
        if (c == '\n') {
            buffer[i] = '\0';
            p = strtok(buffer, ",");
            p = strtok(NULL, ",");
            if (p != NULL) { 
                double value = atof(p);
                if (high < value) {
                    high = value;
                }
            }
            i = 0; 
        } else {
            buffer[i++] = c;
        }
    }

    close(fd); 
    return high;
}

double get_stock_mean(char* fname) {
    double mean = 0, sum = 0;
    int i = 0;
    char buffer[256], c, *p;
    int fd = open(fname, O_RDONLY);
    if (fd < 0) {
        perror("Error Opening Data File");
        return mean; 
    }
    while (read(fd, &c, 1) > 0 && c != '\n') {
		}

    while (read(fd, &c, 1) > 0) {
        if (c == '\n') {
            buffer[i] = '\0';
            p = strtok(buffer, ",");
            p = strtok(NULL, ",");
            if (p != NULL) { 
                double value = atof(p);
                    sum += value;
            }
            i = 0; 
        } else {
            buffer[i++] = c;
        }
    }

    close(fd);
    mean = sum/30;
    return mean;
}

double regression_calc(double close[], double x[], int len) {
    double sum_of_xy = 0, sum_of_squared_x = 0, sum_of_x = 0, sum_of_y = 0;

    for (int i = 0; i < len; i++) {
        sum_of_xy += close[i] * x[i];
        sum_of_squared_x += x[i] * x[i];
        sum_of_x += x[i];
        sum_of_y += close[i];
    }

    if (len * sum_of_squared_x - sum_of_x * sum_of_x == 0) {
        printf("Error: Division by zero in regression calculation\n");
        return 0.0;
    }

    double beta = (len * sum_of_xy - sum_of_x * sum_of_y) / (len * sum_of_squared_x - sum_of_x * sum_of_x);
    return beta;
}

	
	
void regression_coefficients(char *fname, double *global_beta_close_and_open, double *global_beta_close_and_high, double *global_beta_close_and_low, double *global_beta_close_and_volume) {
    int i = 0, a = 0, b = 0, c1 = 0, d = 0, e = 0;
    char buffer[256], c, *p;
    double opening[30], high[30], low[30], closing[130], volume[30];
    int fd = open(fname, O_RDONLY);

    if (fd < 0) {
        perror("Error Opening Data File");
        return;
    }

    while (read(fd, &c, 1) > 0 && c != '\n') {
    }

    while (read(fd, &c, 1) > 0) {
        if (c == '\n') {
            buffer[i] = '\0';
            p = strtok(buffer, ",");
            opening[a++] = atof(p);
            p = strtok(NULL, ",");
            high[b++] = atof(p);
            p = strtok(NULL, ",");
            low[c1++] = atof(p);
            p = strtok(NULL, ",");
            closing[d++] = atof(p);
            p = strtok(NULL, ",");
            volume[e++] = atof(p);
            i = 0;
        } else {
            buffer[i++] = c;
        }
    }

    double beta_close_and_open = regression_calc(closing, opening, 30);
    double beta_close_and_high = regression_calc(closing, high, 30);
    double beta_close_and_low = regression_calc(closing, low, 30);
    double beta_close_and_volume = regression_calc(closing, volume, 30);

    *global_beta_close_and_open = beta_close_and_open;
    *global_beta_close_and_high = beta_close_and_high;
    *global_beta_close_and_low = beta_close_and_low;
    *global_beta_close_and_volume = beta_close_and_volume;

    printf("\n| β_CLOSE<->OPEN | = %f\n| β_CLOSE<->HIGH | = %f\n| β_CLOSE<->LOW | = %f\n| β_CLOSE<->VOLUME | = %f\n\n",
           beta_close_and_open, beta_close_and_high, beta_close_and_low, beta_close_and_volume);
    close(fd);
}

double error_calc(char* fname, double *global_beta_close_and_open, double *global_beta_close_and_high, double *global_beta_close_and_low) {
    double error_sum = 0;
    double temp;
    int i = 0;
    char buffer[256], c, *p;
    int fd = open("data.txt", O_RDONLY);
    int opening, closing, high, low;

    if (fd < 0) {
        perror("Error Opening File");
        return 0.00;
    }

    lseek(fd, 0, SEEK_SET);

    while (read(fd, &c, 1) > 0 && c != '\n') {
    }

    while (read(fd, &c, 1) > 0) {
        if (c == '\n') {
            buffer[i] = '\0';
            p = strtok(buffer, ",");
            opening = atof(p);
            p = strtok(NULL, ",");
            high = atof(p);
            p = strtok(NULL, ",");
            low = atof(p);
            p = strtok(NULL, ",");
            closing = atof(p);
            i = 0;
            temp = ((*global_beta_close_and_open) * opening + (*global_beta_close_and_high) * high + (*global_beta_close_and_low) * low) / 3;
            error_sum += (closing - temp);
        } else {
            buffer[i++] = c;
        }
    }

    close(fd);
    double mean_error = error_sum / 30;
    return mean_error;
}


void plot_data(char* fname) {
    char buffer[256], c, *p;
    int fd = open(fname, O_RDONLY);
    double closing[128];
    int a = 0, i = 0;

    if (fd < 0) {
        perror("Error Opening File");
        return;
    }

    lseek(fd, 0, SEEK_SET);
    while(read(fd, &c, 1) > 0 && c != '\n') {
    }
    
    while (read(fd, &c, 1) > 0) {
        if (c == '\n') {
            buffer[i] = '\0'; 
            p = strtok(buffer, ",");
            p = strtok(NULL, ",");
            p = strtok(NULL, ",");
            p = strtok(NULL, ",");
            closing[a++] = atof(p);
            i = 0;
        } else {
            buffer[i++] = c;
        }
    }

    close(fd);
    int fd1 = open("gnuplot -persistent", O_CREAT | O_WRONLY);
    if (fd1 < 0) {
        perror("Error Opening Gnuplot Pipe");
        return;
    }

    write(fd1, "plot '-' with lines\n", sizeof("plot '-' with lines\n"));

    for (int k = 0; k < a; k++) {
        dprintf(fd1, "%d %g\n", k + 1, closing[k]);
    }

    write(fd1, "e\n", sizeof("e\n"));
    close(fd1);
}


int is_within_lunar_bounds_2024(int current_day, int current_month) {
    int max_day_in_bounds = current_day + 14;
    int lunar_days[12] = {25, 24, 25, 24, 23, 22, 21, 19, 18, 17, 16, 15}; // These are the days in 2024 when full moon is observed
    int lunar_months[12] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    for (int i = 0; i < 12; i++) {
        if (max_day_in_bounds > 31) {
            if ((current_day >= lunar_days[i] && current_month == lunar_months[i]) ||
                (current_day < max_day_in_bounds - 31 && current_month == (lunar_months[i] % 12) + 1)) {
                return 1; 
            }
        } else {
            if (current_day >= lunar_days[i] && current_day < max_day_in_bounds && current_month == lunar_months[i]) {
                return 1; 
            }
        }
    }
    return 0;
};

void count_determiner(int count) {
	if(count == 4) {
		printf("*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*\n");
		printf("| CONCLUSION: The Current Stock Has Cleared all 4 Parameters of Weekly High, Monthly High , Lunar Phase and Extrapolated Price Range. Therefore , YOU SHOULD DEFINITELY BUY THIS STOCK..|\n");
		printf("*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*\n");
	} else if(count == 3) {
		printf("*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*\n");
		printf("| CONCLUSION: The Current Stock Has Cleared 3 Parameters out of 4 among Weekly, MOnthly High ,Lunar Phase and Extrapolated Price Range. Therefore, THS STOCK IS GREAT AND CAN BE BOUGHT..|\n");
		printf("*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*\n");
	} else if(count == 2) {
		printf("*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*\n");
		printf("| CONCLUSION: The Current Stock Has Cleared 2 Parameters out of 4 among Weekly, Monthly High, Lunar Phase and Extrapolated Price Range. Therefore, THE STOCK IS GOOD BUT YOU CAN SEEK BETTER OPTIONS..|\n");
		printf("*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*\n");	
	} else if(count == 1) {
		printf("*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*\n");
		printf("| CONCLUSION: The Current Stock Has Cleared Only 1 Parameter out of 4 among Weekly, Montly High, Lunar Phase and Extrapolated Price Range. Therefore. YOU SHOULD SEEK BETTER OPTIONS..       |\n");
		printf("*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*\n");
	} else {
		printf("*-------------------------------------------------------------------------------------------------------------------------------------*\n");
		printf("| CONCLUSION: The Current Stock Has Cleared 0 Parameters out of 4 among Weekly, Montly High, Lunar Phase and Extrapolated Price Range.|\n");
		printf("| Therefore, YOU SHOULD NOT BUY THIS STOCK AND SEEK DIFFERENT OPTIONS..                                                               |\n");
		printf("*-------------------------------------------------------------------------------------------------------------------------------------*\n");
	};
};

void lunar_effect_full_moon() {
	printf("The Current Day lies within the Full Moon --> New Moon Lunar Span of this Current year\nHence the stock has the tendency to show a BEARISH trend , i.e Downtrend.\nThis phenomenon is directly related to the psychological and biological effects of lunar phases on living species and hence is known as the LUNAR EFFECT on STOCK MARKET.\n\n\n");
};

void lunar_effect_new_moon() {
	printf("The Current Day does not lie within the Full Moon --> New Moon Lunar Span of this Current year\nHence the stock has the tendency to show a slight BULLISH trend , i.e UpTrend.\nThis phenomenon is directly related to the PSYCHOLOGICAL and BIOLOGICAL effects of lunar phases on living species and hence is known as the LUNAR EFFECT on STOCK MARKET.\n\n\n");
};


void display_thank_you_ascii_art() {
    printf("      ✿╔══════════════════════╗✿\n");
    printf("      ✿║   ║║     ║           ║✿\n");
    printf("      ✿║  ═╬╠╗╔╗╔╗╠╦╗╔╔╦╗╔    ║✿\n");
    printf("      ✿║ ══╩╝╚╩╩╝╚╝╚╚╣╚╝╚╩══  ║✿\n");
    printf("      ✿║           ══╝        ║✿\n");
    printf("      ✿╚══════════════════════╝✿\n");
};


int main() {
    int count = 0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    double x1, x2, x3, x4;
    double global_beta_close_and_open, global_beta_close_and_high, global_beta_close_and_low, global_beta_close_and_volume;
    printf("\n-> STOCK's Daily High: %f INR\n", get_daily_high("data.txt"));
    printf("-> STOCK's Weekly High: %f INR\n", get_weekly_high("data.txt"));
    printf("-> STOCK's Monthly High: %f INR\n", get_monthly_high("data.txt"));
    printf("-> STOCK's Monthly Average: %f INR\n", get_stock_mean("data.txt"));
    regression_coefficients("data.txt", &global_beta_close_and_open, &global_beta_close_and_high, &global_beta_close_and_low, &global_beta_close_and_volume);
    printf("*----------WHAT is β->Beta?----------*\nβ or Beta in this case is termed as the REGRESSION COEFFICIENT between a dependent and an independent variable. It is a calculated factor which represents the rate at which dependent variable changes with respect to any singular independent variable, keeping all other variables zero , i.e can be understood as the partial derivative of y wrt x.\n\n");
    printf("The Linear Regression Equation For The Variation of Stock Price is empirically found out to be dependent upon OPENING PRICE, CURRENT HIGH, CURRENT LOW and The VOLUME Traded Till Now. Hence,the equation can be expressed as:\n\n| Close= β1*Open + β2*High + β3*Low + β4*Volume + Delta |\n\nWhere, Delta = Mean Error\nβ1, β2, β3 and β4 are REGRESSION COEFFICIENTS\n");
    printf("\nEnter The Current Open: ");
    scanf("%lf", &x1);
    printf("\nEnter The Current High: ");
    scanf("%lf", &x2);
    printf("\nEnter The Current Low: ");
    scanf("%lf", &x3);
    printf("\nEnter the Current Traded Volume: ");
    scanf("%lf", &x4);
    double new_close_and_open = (global_beta_close_and_open * x1), new_close_and_high = (global_beta_close_and_high * x2), new_close_and_low = (global_beta_close_and_low * x3);
    printf("LINEAR REGRESSION EXTRAPOLATED PRICE WRT OPEN: %f INR\n", new_close_and_open);
    printf("LINEAR REGRESSION EXTRAPOLATED PRICE WRT HIGH: %f INR\n", new_close_and_high);
    printf("LINEAR REGRESSION EXTRAPOLATED PRICE WRT LOW: %f INR\n", new_close_and_low);
    double new_mean = (new_close_and_open + new_close_and_high + new_close_and_low)/3, new_error = error_calc("data.txt", &global_beta_close_and_open, &global_beta_close_and_high, &global_beta_close_and_low);
    printf("STOCK MEAN = %f INR\n", new_mean);
    printf("Mean ERROR: %f INR\n", new_error);
    printf("Traded Volume Error: %fINR\n", global_beta_close_and_volume*x4);
    if(global_beta_close_and_volume < 0) {
    	printf("EXTRAPOLATED CLOSING PRICE EXTREMITIES: %f to %f INR\n", new_mean + new_error + (global_beta_close_and_volume)*x4, new_mean + new_error - (global_beta_close_and_volume)*x4);
    } else {
    	printf("EXTRAPOLATED CLOSING PRICE EXTREMITIES: %f to %f INR\n", new_mean + new_error - (global_beta_close_and_volume)*x4, new_mean + new_error + (global_beta_close_and_volume)*x4);
    };
   
    double approximated_closing_price = new_mean + new_error;

    printf("EXTRAPOLATED CLOSING PRICE APPROXIMATION: %f INR\n\n", approximated_closing_price);
    double percent_return = approximated_closing_price/x1;
   
   
    if (percent_return > 1.05) { // 5% Return Approximated
        printf("+------------------------------------------------------------------------+\n");
	printf("|OPINION W.R.T RETURNS --> BUY This STOCK | as the APPROXIMATED CLOSING  |\n");
	printf("|PRICE of the Stock is above the opening price of the stock              |\n");
	printf("+------------------------------------------------------------------------+\n\n");
	count++;
    } else if (percent_return > 1) { // 0 to 5% Return Approximated
        printf("+------------------------------------------------------------------------+\n");
	printf("|OPINION W.R.T RETURNS --> SEEK BETTER OPTIONS | as the APPROXIMATED     |\n");
	printf("|CLOSING PRICE of the stock is slightly above or equal to                |\n");
	printf("|the opening price of the stock                                          |\n");
	printf("+------------------------------------------------------------------------+\n\n");
    } else {
        printf("+------------------------------------------------------------------------+\n");
	printf("|OPINION W.R.T RETURNS --> DO NOT BUY This STOCK | as the APPROXIMATED   |\n");
	printf("|CLOSING PRICE of the stock is below the opening price of the stock      |\n");
	printf("+------------------------------------------------------------------------+\n\n");
    };
    
    plot_data("data.txt");
    int curr_day = tm.tm_mday, curr_month = tm.tm_mon + 1;
    
    if(is_within_lunar_bounds_2024(curr_day, curr_month)) {
    	lunar_effect_full_moon();
    } else if(!is_within_lunar_bounds_2024(curr_day, curr_month)) {
    	lunar_effect_new_moon();
    	count++;
    };
    
    if(x1 > get_weekly_high("data.txt")) {
    	printf("ADDITIONAL NOTE: | STRONG BUY OPINION For This CURRENT STOCK | as the current opening price of this stock is above the weekly high or previous 7 days data of this stock\n");
    	count++;
    };
    
    if(x1 > get_monthly_high("data.txt")) {
    	printf("ADDITIONAL NOTE: | VERY STRONG BUY OPINION For This CURRENT STOCK | as the current opening price is above the monthkly high or previous 30 days data of this stock\n");
    	count++;
    };
    count_determiner(count);
    
    int readCode = system("gnuplot -persistent < 'gnuplot -persistent'");
    system("'rm' ANALYSIS");
    system("'rm' data.txt");
    system("'rm' 'gnuplot -persistent'");
    printf("\n<-- Generating GNUPLOT for the Current Stock for the TIME SPAN of 30 Days -->\n");
    display_thank_you_ascii_art();
    return 0;
}

			
	
