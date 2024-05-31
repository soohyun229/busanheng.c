#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define LEN_MIN 15
#define LEN_MAX 50
#define STM_MIN 0
#define STM_MAX 5
#define PROB_MIN 10
#define PROB_MAX 90
#define AGGRO_MIN 0
#define AGGRO_MAX 5

#define MOVE_LEFT 1
#define MOVE_STAY 0

#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

int game_turn = 0, rand_percent, train_len, M_STM, percent_prob, C_AGR = 1, M_AGR = 1, move, action, pull = 0, game_over = 0, C_stay = 0, Z_stay = 0, ZtoM = 0, C = 5, Z = 3, M = 2;

int scanf_info(void);
int draw_train(void);
int set_game(void);
int rand_per_C(void);
int rand_per_Z(void);
int C_info(void);
int Z_info(void);
int M_move(void);
int M_move_switch(void);
int print_move_switch(void);
int citizen_success(void);
int gameover_info(void);
int print_gameover(void);
int M_action(void);
int M_action_0(void);
int M_action_1(void);
int M_action_2(void);

int scanf_info(void) {
	while (1) {
		printf("train length(%d~%d)>> ", LEN_MIN, LEN_MAX);
		scanf_s("%d", &train_len);
		if (LEN_MIN > train_len || LEN_MAX < train_len) continue;
		else break;
	}
	while (1) {
		printf("madongseok stamina(%d~%d)>> ", STM_MIN, STM_MAX);
		scanf_s("%d", &M_STM);
		if (STM_MIN > M_STM || STM_MAX < M_STM) continue;
		else break;
	}
	while (1) {
		printf("percentile probability 'p'(%d~%d)>> ", PROB_MIN, PROB_MAX);
		scanf_s("%d", &percent_prob);
		if (PROB_MIN > percent_prob || PROB_MAX < percent_prob) continue;
		else break;
	}
}
int draw_train(void) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < train_len; j++) {
			if (i == 1 && j > 0 && j < train_len - 1) {
				if (train_len - C == j) printf("C");
				else if (train_len - Z == j) printf("Z");
				else if (train_len - M == j) printf("M");
				else printf(" ");
			}
			else printf("#");
		}
		printf("\n");
	}
}
int set_game(void) {
	game_turn++;
	C_stay = 0;
	Z_stay = 0;
	rand_per_C();
	rand_per_Z();
	draw_train();
	printf("\n\n");
	C_info();
	Z_info();
}
int rand_per_C(void) {
	if (game_turn % 2 != 0) { // 시민 턴 (홀수 턴)
		Z_stay = 1; // 시민 턴 이기 때문에 좀비가 멈춰야함 / 여기서 C_stay 혹은 Z_stay는 시민 혹은 좀비가 멈춰있는지 움직이는 상태인지를 확인하기 위해 만든 함수이다.
		rand_percent = rand() % 100 + 1;
		if (rand_percent <= 100 - percent_prob) C++;
		else C_stay = 1;
	}
}
int rand_per_Z(void) {
	if (game_turn % 2 == 0) { // 좀비 턴 (짝수 턴)
		C_stay = 1;
		if (game_turn % 4 == 0) Z_stay = 1;
		else {
			rand_percent = rand() % 100 + 1;
			if (rand_percent >= 100 - percent_prob && (C - Z != 1 || Z - M != 1)) {
				if (M_AGR > C_AGR && Z - M != 1) {
					Z--;
					ZtoM = 1;
				}
				else if (M_AGR < C_AGR || M_AGR == C_AGR) Z++;
			}
			else Z_stay = 1;
		}
	}
}
int C_info(void) {
	if (C_stay == 1) {
		if (game_turn % 2 == 0 || C_AGR == AGGRO_MIN) printf("citizen: stay %d (aggro: %d)\n", train_len - C, C_AGR);
		else {
			if (C_AGR == AGGRO_MIN) C_AGR = C_AGR;
			else C_AGR--;
			printf("citizen: stay %d (aggro: %d -> %d)\n", train_len - C, C_AGR + 1, C_AGR);
		}
	}
	else {
		if (C_AGR == AGGRO_MAX) C_AGR = C_AGR;
		else {
			C_AGR++;
			printf("citizen: %d -> %d (aggro: %d -> %d)\n", train_len - C + 1, train_len - C, C_AGR - 1, C_AGR);
		}
	}
}
int Z_info(void) {
	if (Z_stay == 1) {
		if (game_turn % 4 == 0 || pull == 1) { //여기에 붙들기 성공시 못움직인다 라는 조건도 붙일 예정
			printf("zombie: stay %d (cannot move)\n", train_len - Z);
			pull = 0;
		}
		else printf("zombie: stay %d\n", train_len - Z);
	}
	else if (ZtoM = 1 && Z - M != 1) printf("zombie: %d -> %d\n", train_len - Z - 1, train_len - Z);
	else if (ZtoM = 1 && Z - M == 1) printf("zombie: stay %d\n", train_len - Z);
	else printf("zombie: %d -> %d\n", train_len - Z + 1, train_len - Z);
	printf("\n\n");
}
int M_move(void) {
	while (1) {
		if (Z - M == 1) {
			printf("madongseok move(0:stay)>> ");
			scanf_s("%d", &move);
			if (move != 0) continue;
			else break;
		}
		else {
			printf("madongseok move(0:stay, 1:left)>> ");
			scanf_s("%d", &move);
			if (move < 0 || move > 1) continue;
			else break;
		}
	}
}
int M_move_switch(void) {
	switch (move) {
	case 0:
		if (M_AGR == AGGRO_MIN) M_AGR == M_AGR;
		else M_AGR--;
		break;
	case 1:
		if (M_AGR == AGGRO_MAX) M++;
		else {
			M++;
			M_AGR++;
		} break;
	}
}
int print_move_switch(void) {
	switch (move) {
	case 0:
		if (M_AGR == AGGRO_MIN) printf("madongseok: stay %d (aggro: %d, stamina: %d)\n", train_len - M, M_AGR, M_STM);
		else printf("madongseok: stay %d (aggro: %d -> %d, stamina: %d)\n", train_len - M, M_AGR + 1, M_AGR, M_STM);
		break;
	case 1:
		if (M_AGR == AGGRO_MAX) printf("madongseok: %d -> %d (aggro: %d, stamina: %d)\n", train_len - M + 1, train_len - M, M_AGR, M_STM);
		else printf("madongseok: %d -> %d (aggro: %d -> %d, stamina: %d)\n", train_len - M + 1, train_len - M, M_AGR - 1, M_AGR, M_STM);
		break;
	}
}
int citizen_success(void) {
	if (train_len - C == 1) {
		printf("SUCCESS!\n");
		printf("citizen(s) escaped to the next train\n");
		exit(1);
	}
	else printf("citizen does nothing.\n");
}
int gameover_info(void) {
	if ((C_AGR < M_AGR || C_AGR == M_AGR) && C - M == 2) { //시민 좀비 동석 순으로 양쪽다 인접하고 어그로 동일한 경우
		M_STM--;
		printf("Zomibe attacked madongseok (aggro: %d vs. %d, madongseok stamina: %d -> %d)\n", C_AGR, M_AGR, M_STM + 1, M_STM);
	}
	else if (C_AGR > M_AGR && C - M == 2) { //시민 좀비 동석 순으로 양쪽다 인접하고 시민이 더 큰 경우
		game_over = 1;
		printf("Zomibe attacked citizen (aggro: %d vs. %d)\n", C_AGR, M_AGR);
	}
	else if (Z - M == 1) { //동석 좀비 인접
		M_STM--;
		printf("Zomibe attacked madongseok (madongseok stamina: %d -> %d)\n", M_STM + 1, M_STM);
	}
	else printf("zombie attacked nobody.\n");
}
int print_gameover(void) {
	if (game_over == 1 || (C - Z == 1 && C - M != 2)) {
		printf("GAME OVER!\n");
		printf("Citizen(s) has(have) been attacked by a zombie\n");
		exit(1);
	}
	else if (M_STM == STM_MIN) {
		printf("GAME OVER!\n");
		printf("Madongseok has(have) been attacked by a zombie\n");
		exit(1);
	}
}
int M_action(void) {
	while (1) {
		if (Z - M == 1) {
			printf("madongseok action(0.rest, 1.provoke, 2. pull)>> ");
			scanf_s("%d", &action);
			if (action < 0 || action > 2) continue;
			else break;
		}
		else {
			printf("madongseok action(0.rest, 1.provoke)>> ");
			scanf_s("%d", &action);
			if (action < 0 || action > 1) continue;
			else break;
		}
	}
	if (action == 0) M_action_0();
	else if (action == 1) M_action_1();
	else if (action == 2) M_action_2();
}
int M_action_0(void) {
	printf("madongseok rests...\n");
	if (M_AGR == AGGRO_MIN) M_AGR == M_AGR;
	else M_AGR--;
	if (M_STM == STM_MAX) M_STM == M_STM;
	else M_STM++;
	if (M_AGR == AGGRO_MIN) printf("madongseok: %d (aggro: %d, stamina: %d -> %d)\n", train_len - M, M_AGR, M_STM - 1, M_STM);
	else if (M_STM == STM_MAX) printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", train_len - M, M_AGR + 1, M_AGR, M_STM);
	else printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", train_len - M, M_AGR + 1, M_AGR, M_STM - 1, M_STM);
}
int M_action_1(void) {
	printf("madongseok provoked zombie...\n");
	printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", train_len - M, M_AGR, AGGRO_MAX, M_STM);
	M_AGR = AGGRO_MAX;
}
int M_action_2(void) {
	rand_percent = rand() % 100 + 1;
	if (rand_percent <= 100 - percent_prob) {
		printf("madongseok pulled zombie... Next turn, it can't move\n");
		pull = 1;
	}
	else {
		printf("madongseok failed to pull zombie\n");
		pull = 0;
	}
}


int main(void) {
	srand((unsigned int)time(NULL));
	scanf_info();
	draw_train();
	printf("\n");
	while (1) {
		set_game();
		M_move();
		M_move_switch();
		draw_train();
		printf("\n");
		print_move_switch();
		printf("\n");
		citizen_success();
		gameover_info();
		print_gameover();
		M_action();
	}
}