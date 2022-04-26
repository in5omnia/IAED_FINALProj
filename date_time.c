/*
* File: date_time.c
* Author: Beatriz Gavilan - 102463
* Description: Contains all the functions related to the date and time.
*/

#include "BG_102463.h"


/*	Creates a date	*/
Date createDate(int day, int month, int year) {

	Date date;
	date.day = day;
	date.month = month;
	date.year = year;

	return date;
}

/*	Creates a dateTime object	*/
DateTime createDateTime(Date date, Time time) {

	DateTime dateTime;
	dateTime.date = date;
	dateTime.time = time;

	return dateTime;
}


/*	Creates a new date	*/
Date newDate(Date possibleDate, Date today) {

	today.day = possibleDate.day;
	today.month = possibleDate.month;
	today.year = possibleDate.year;

	return today;
}


/*	Checks if a date (taking as arguments a day, month and year) is in the past,
 * when compared with the system's "today". If so, it returns 1, if not, it
 * returns 0.	*/
int pastDate(int day, int month, int year, Date today) {

	return ((year < today.year) ||
			(year == today.year && month < today.month) ||
			(year == today.year && month == today.month && day < today.day));

}


/*	Checks if date (taking as arguments a day, month and year) is more than a
 * year in the future. Returns 1 if it is and 0 if not. */
int afterOneYear(int day, int month, int year, Date today) {

	Date later_than_one_year = createDate(today.day+1, today.month,
										today.year+1);

	return !pastDate(day, month, year, later_than_one_year);
}


/*	Presents a date in the standard output	*/
void outputDate(Date date) {

	printf(OUT_DATE, date.day, date.month, date.year);
}


/*	Checks a date's validity: checks if it's not in the past, no more than one
 * year later and within each month's day limit. Returns 1 if it's valid and 0
 * if not. */
int check_date(Date date, Date today) {

	int daysPerMonth[12] = {31,28,31,30,31, 30,
							31, 31, 30, 31, 30, 31};

	if (pastDate(date.day, date.month, date.year, today) ||
		afterOneYear(date.day, date.month, date.year, today) ||
		date.day > daysPerMonth[date.month-1]) {

		printf(INVALID_DATE);
		return 0;
	}
	return 1;
}


/*	Checks the duration's validity: if it's within the correct interval (no more
 * than 12 hours). Returns 1 if it's valid and 0 if not.	*/
int validDuration(Time duration) {

	if (duration.hour > 12 || (duration.hour == 12 && duration.min > 0)) {

		printf(INVALID_DURATION);
		return 0;
	}
	return 1;
}


/*	Compares 2 dates and returns 1 if they're the same. Returns 0 if not.	*/
int sameDate(Date date1, Date date2) {

	return (date1.day == date2.day && date1.month == date2.month
			&& date1.year == date2.year);
}


/*	Checks if a time object is in the past when compared to another. Returns 1
 * if it is and 0 if not.	*/
int beforeTime(Time time1, Time time2) {

	if (time1.hour < time2.hour ||
		(time1.hour == time2.hour && time1.min < time2.min))
		return 1;

	return 0;
}


/*	Checks if a DateTime object is in the past when compared to another.
 * Returns 1 if it's valid and 0 if not.	*/
int beforeDateTime(DateTime dateTime1, DateTime dateTime2) {

	int day1 = dateTime1.date.day, month1 = dateTime1.date.month,
		year1 = dateTime1.date.year;

	if (pastDate(day1, month1, year1, dateTime2.date))
		return 1;

	if (sameDate(dateTime1.date, dateTime2.date) &&
		beforeTime(dateTime1.time, dateTime2.time))
		return 1;

	return 0;
}


/* Returns the sum of a duration with a DateTime object (arrival DateTime
 * object).	*/
DateTime sumDuration(DateTime departure, Time duration) {

	int daysPerMonth[12] = {31,28,31,30,31, 30,
							31, 31, 30, 31, 30, 31};

	departure.time.hour += duration.hour;
	departure.time.min += duration.min;

	if (departure.time.min > 59) {
		departure.time.min -= 60;
		departure.time.hour += 1;
	}
	if (departure.time.hour > 23) {
		departure.time.hour -= 24;
		departure.date.day += 1;
	}
	if (departure.date.day > daysPerMonth[departure.date.month-1]) {
		departure.date.month++;
		departure.date.day = 1;
	}
	if (departure.date.month > 12) {
		departure.date.year++;
		departure.date.month = 1;
	}

	return departure;
}

