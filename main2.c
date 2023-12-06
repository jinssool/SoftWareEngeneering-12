#include <stdio.h>
#include <string.h>
#include "Project6.h"


char id[10];
char password[10];


int load_user_info(struct user_info user[])
{
	FILE* fp;
	if ((fp = fopen("user_data.txt", "r")) == NULL)
		return -1;
	int i;
    int j;
	struct user_info temp;
	for (i = 0; i < 10; i++) 
		fscanf(fp, "%d %10s %10s", &user[i].id_num, user[i].id, user[i].password);
	for (i = 0; i < 10; i++)
	{
		for (j = i + 1; j < 10; j++)
		{
			if (strcmp(user[i].id, user[j].id) > 0)
			{
				temp.id_num = user[i].id_num;
				user[i].id_num = user[j].id_num;
				user[j].id_num = temp.id_num;
				wcsncpy(temp.id, user[i].id);
				wcsncpy(user[i].id, user[j].id);
				wcsncpy(user[j].id, temp.id);
				wcsncpy(temp.password, user[i].password);
				wcsncpy(user[i].password, user[j].password);
				wcsncpy(user[j].password, temp.password);
			}
		}
	}
	fclose(fp);
	return i;
}

int check_login(struct user_info user[], char* id, char* password)
{
	int low;
    int high;
    int middle;
	low = 0;
	high = 9;
	while (low <= high)
	{
		middle = (low + high) / 2;
		if (strcmp(id, user[middle].id) > 0)
			low = middle + 1;
		else if (strcmp(id, user[middle].id) == 0)
		{
			if (strcmp(password, user[middle].password) == 0)
				return user[middle].id_num;
			else
				return -2;
		}
		else
			high = middle - 1;
	}
	return -1;
}
int print_seats(int s_array[][XSIZE])
{
	printf(" |0 1 2 3 4\n");
	printf("-----------\n");
	for (int i = 0; i < YSIZE; i++)
	{
		printf("%c|", (char)(i + 65));
		for (int j = 0; j < XSIZE; j++)
		{
			printf("%d ", s_array[i][j]);
		}
		printf("\n");
	}
	return 1;
}
int reserve(int s_array[][XSIZE], char r, int c, int num)
{
	if (r == 'A')
	{
		if (s_array[0][c] == 0)
		{
			s_array[0][c] = num;
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else if (r == 'B')
	{
		if (s_array[1][c] == 0)
		{
			s_array[1][c] = num;
			return 1;
		}
		else
		{
			return -1;
		}
	}
	else if (r == 'C')
	{
		if (s_array[2][c] == 0)
		{
			s_array[2][c] = num;
			return 1;
		}
		else
		{
			return -1;
		}
	}
	return 0;
}
int cancel(int s_array[][XSIZE], char r, int c, int num)
{
	if (r == 'A')
	{
		if (s_array[0][c] == 0)
		{
			return -1;
		}
		else if (s_array[0][c] == num)
		{
			s_array[0][c] = 0;
			return 1;
		}
		else
		{
			return -2;
		}
	}
	else if (r == 'B')
	{
		if (s_array[1][c] == 0)
		{
			return -1;
		}
		else if (s_array[1][c] == num)
		{
			s_array[1][c] = 0;
			return 1;
		}
		else
		{
			return -2;
		}
	}
	else if (r == 'C')
	{
		if (s_array[2][c] == 0)
		{
			return -1;
		}
		else if (s_array[2][c] == num)
		{
			s_array[2][c] = 0;
			return 1;
		}
		else
		{
			return -2;
		}
	}
	return 0;
}
int load_reservation_info(int s[][XSIZE])
{
	FILE* fp;
	if ((fp = fopen("reservation_data.txt", "r")) == NULL)
		return -1;
	int i;
	for (i = 0; i < YSIZE; i++)
		fscanf(fp, "%d %d %d %d %d ", &s[i][0], &s[i][1], &s[i][2], &s[i][3], &s[i][4]);
	fclose(fp);
	return i;
}
int save_reservation_info(int s[][XSIZE])
{
	FILE* fp;
	if ((fp = fopen("reservation_data.txt", "w")) == NULL)
		return -1;
	int i;
	for (i = 0; i < YSIZE; i++)
		fprintf(fp, "%d %d %d %d %d\n", s[i][0], s[i][1], s[i][2], s[i][3], s[i][4]);
	fclose(fp);
	return i;
}

