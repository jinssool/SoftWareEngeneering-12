#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#define XSIZE	5
#define YSIZE	3

struct user_info {
	int id_num;
	char id[10];
	char password[10];
};
char id[10], password[10];

int load_user_info(struct user_info user[]);
int check_login(struct user_info user[]);
int print_seats(int s_array[][XSIZE]);
int reserve(int s_array[][XSIZE], char r, int c, int id);
int cancel(int s_array[][XSIZE], char r, int c, int id);
int load_reservation_info(int s[][XSIZE]);
int save_reservation_info(int s[][XSIZE]);

int main(void)
{
	struct user_info user[10];
	int c;
	if ((c = load_user_info(user)) < 0)
		fprintf(stderr, "error\n");
	char row;
	int C, id_num, col;
	int select = 0, seat[YSIZE][XSIZE] = { 0 };
	printf("�α����� �ϼ���.\n");
	while (1)
	{
		printf("id: ");
		scanf("%s", id);
		printf("password: ");
		scanf("%s", password);
		C = check_login(user);
		if (C == -1)
			printf("�ش� ���̵� �����ϴ�. �ٽ� �α����� �ϼ���.\n");
		else if (C == -2)
			printf("�ش� ���̵�� �����ϳ� �н����尡 �ٸ��ϴ�. �ٽ� �α����� �ϼ���.\n");
		else
		{
			id_num = C;
			printf("%s�� �ݰ����ϴ�.\n", id);
			break;
		}
	}
	load_reservation_info(seat);

	do
	{
		printf("1---�¼�Ȯ���ϱ�\n");
		printf("2---�����ϱ�\n");
		printf("3---��������ϱ�\n");
		printf("4---�ٸ� ����ڷ� �α����ϱ�\n");
		printf("5---�����ϱ�\n");
		printf("�޴��� �����Ͻÿ�: ");
		scanf("%d", &select);

		if (select == 1)
		{
			printf("���õ� �޴� = �¼�Ȯ���ϱ�\n\n");
			print_seats(seat);
		}
		else if (select == 2)
		{
			printf("���õ� �޴� = �����ϱ�\n\n");
			printf("������ ���ϴ� �ڸ���(��-A2)? ");
			scanf(" %c%d", &row, &col);
			reserve(seat, row, col, id_num);
			print_seats(seat);
			save_reservation_info(seat);
		}
		else if (select == 3)
		{
			printf("���õ� �޴� = ��������ϱ�\n\n");
			printf("������Ҹ� ���ϴ� �ڸ���(��-A2)? ");
			scanf(" %c%d", &row, &col);
			cancel(seat, row, col, id_num);
			print_seats(seat);
			save_reservation_info(seat);
		}
		else if (select == 4)
		{
			printf("���õ� �޴� = �ٸ� ����ڷ� �α����ϱ�\n\n");
			printf("�α����� �ϼ���.\n");
			while (1)
			{
				printf("id: ");
				scanf("%s", id);
				printf("password: ");
				scanf("%s", password);
				C = check_login(user);
				if (C == -1)
					printf("�ش� ���̵� �����ϴ�. �ٽ� �α����� �ϼ���.\n");
				else if (C == -2)
					printf("�ش� ���̵�� �����ϳ� �н����尡 �ٸ��ϴ�. �ٽ� �α����� �ϼ���.\n");
				else
				{
					id_num = C;
					printf("%s�� �ݰ����ϴ�.\n", id);
					break;
				}
			}
		}
	} while (select != 5);
	printf("���õ� �޴� = �����ϱ�\n\n");
	printf("�̿��� �ּż� �����մϴ�.\n");
	return 0;
}

int load_user_info(struct user_info user[])
{
	FILE* fp;
	if ((fp = fopen("user_data.txt", "r")) == NULL)
		return -1;
	int i, j;
	struct user_info temp;
	for (i = 0; i < 10; i++) 
		fscanf(fp, "%d %s %s", &user[i].id_num, user[i].id, user[i].password);
	for (i = 0; i < 10; i++)
	{
		for (j = i + 1; j < 10; j++)
		{
			if (strcmp(user[i].id, user[j].id) > 0)
			{
				temp.id_num = user[i].id_num;
				user[i].id_num = user[j].id_num;
				user[j].id_num = temp.id_num;
				strcpy(temp.id, user[i].id);
				strcpy(user[i].id, user[j].id);
				strcpy(user[j].id, temp.id);
				strcpy(temp.password, user[i].password);
				strcpy(user[i].password, user[j].password);
				strcpy(user[j].password, temp.password);
			}
		}
	}
	fclose(fp);
	return i;
}
int check_login(struct user_info user[])
{
	int low, high, middle;
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
			printf("������ �Ϸ�Ǿ����ϴ�.\n");
			return 1;
		}
		else
		{
			printf("�̹� ����� �ڸ��Դϴ�.\n");
			return -1;
		}
	}
	else if (r == 'B')
	{
		if (s_array[1][c] == 0)
		{
			s_array[1][c] = num;
			printf("������ �Ϸ�Ǿ����ϴ�.\n");
			return 1;
		}
		else
		{
			printf("�̹� ����� �ڸ��Դϴ�.\n");
			return -1;
		}
	}
	else if (r == 'C')
	{
		if (s_array[2][c] == 0)
		{
			s_array[2][c] = num;
			printf("������ �Ϸ�Ǿ����ϴ�.\n");
			return 1;
		}
		else
		{
			printf("�̹� ����� �ڸ��Դϴ�.\n");
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
			printf("������� ���� �ڸ��Դϴ�.\n");
			return -1;
		}
		else if (s_array[0][c] == num)
		{
			s_array[0][c] = 0;
			printf("������Ұ� �Ϸ�Ǿ����ϴ�.\n");
			return 1;
		}
		else
		{
			printf("������Ҹ� �� �� �����ϴ�.\n");
			return -2;
		}
	}
	else if (r == 'B')
	{
		if (s_array[1][c] == 0)
		{
			printf("������� ���� �ڸ��Դϴ�.\n");
			return -1;
		}
		else if (s_array[1][c] == num)
		{
			s_array[1][c] = 0;
			printf("������Ұ� �Ϸ�Ǿ����ϴ�.\n");
			return 1;
		}
		else
		{
			printf("������Ҹ� �� �� �����ϴ�.\n");
			return -2;
		}
	}
	else if (r == 'C')
	{
		if (s_array[2][c] == 0)
		{
			printf("������� ���� �ڸ��Դϴ�.\n");
			return -1;
		}
		else if (s_array[2][c] == num)
		{
			s_array[2][c] = 0;
			printf("������Ұ� �Ϸ�Ǿ����ϴ�.\n");
			return 1;
		}
		else
		{
			printf("������Ҹ� �� �� �����ϴ�.\n");
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