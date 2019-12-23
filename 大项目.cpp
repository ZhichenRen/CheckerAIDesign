// 大项目.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
//头文件
#include "pch.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "time.h"
#include "stdlib.h"

//宏定义
#define BOARD_SIZE 8
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define B_KING 3
#define W_KING 4
#define InBoard(i,j) (i>=0 && j>=0 && i<BOARD_SIZE && j<BOARD_SIZE)
#define IFINITY 10000000
#define TRUE 1
#define FALSE 0
#define START "START"
#define PLACE "PLACE"
#define TURN "TURN"
#define END "END"
#define DEPTHMAX 10
#define DEPTHLAST 8
#define HASHMAX 1000000

//调试用
//#define DEBUG

//定义的结构体
typedef struct
{
	int x[13];
	int y[13];
	int numStep;
}Command;
typedef struct
{
	int x;
	int y;
}Position;
typedef struct
{
	int isused;
	int value;
	int depth;
	int flag;
	long long int posValue;
}HashValue;
typedef struct
{
	int num;
	int x[13];
	int y[13];
	char piece[13];
}PieceEat;

//使用的全局变量
char board[BOARD_SIZE][BOARD_SIZE] = { 0,2,0,2,0,2,0,2,2,0,2,0,2,0,2,0,0,2,0,2,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,1,0,1,0,1,0,1,0 };
int dir[4][2] = { -1,-1,-1,1,1,-1,1,1 };
int me_flag;
int other_flag;
int timeout;
Command crtbestcommand;
int step = 0;//当前步数
//HashValue HashTable[HASHMAX];
char message[50];
//unsigned long long Zobrist[5][64];

//使用的函数

int CreateMove(char board[BOARD_SIZE][BOARD_SIZE], int me_flag, Command Move[]);
//接收一个当前局面board，当前棋手me，存储着法的数组Move，将生成的着法保存于Move数组中,并返回着法数
void PrintBoard(char board[BOARD_SIZE][BOARD_SIZE]);
//接收一个当前局面board，并将其打印
int search(char board[BOARD_SIZE][BOARD_SIZE], int i, int j, Command last, Command tempMove[], int *MoveIndex, int eatNum);
//接收一个当前局面board，一个位置i，j，一个上一个位置的指令，一个用于存储指令的数组，一个数组索引的地址，一个当前吃子数，返回该子最大吃子数
int JumpSearch(char board[BOARD_SIZE][BOARD_SIZE], int me_flag, Command Move[]);
//接收一个当前局面board，当前棋手me，返回生成的着法数
int NegaMax(char board[8][8], int depth, int me_flag, clock_t start, int alpha, int beta, int step, int depthmax);
//负极大值搜索
int evaluate(char board[8][8], int me_flag, int step, int depthmax);
//估值函数
void MoveSort(char board[BOARD_SIZE][BOARD_SIZE], Command Move[], int MoveNum, int step, int me_flag);
//为产生的走法进行一个浅层排序
void CancelCommand(char board[BOARD_SIZE][BOARD_SIZE], char temp, Command command, PieceEat eat);
//接收一个当前局面，一个指令command，一个用于保存实施指令前的棋子的变量temp，一个用于保存吃掉的棋子的变量eat，将当前局面返回至实施指令前的局面
void loop();
void turn();
void debug(const char *str);
Command aiTurn(char board[BOARD_SIZE][BOARD_SIZE], int me_flag);
/**
 * 轮到你落子。
 * 棋盘上0表示空白，1表示黑棋，2表示白棋
 * me表示你所代表的棋子(1或2)
 * 你需要返回一个结构体Command，其中numStep是你要移动的棋子经过的格子数（含起点、终点），
 * x、y分别是该棋子依次经过的每个格子的横、纵坐标
 */
void TakeCommand(char board[8][8], Command command, PieceEat *eat);
//接收一个当前局面与一个指令，对当前局面执行该指令
void initAI(int me_flag);
//初始化AI
int CalKey(char board[BOARD_SIZE][BOARD_SIZE]);
//用于计算某个局面Hash值的函数
unsigned long long CalPos(char board[BOARD_SIZE][BOARD_SIZE]);
//用于计算某个局面对应位置值的函数
unsigned long long rand64(void)
{
	srand((unsigned)time(NULL));
	return (unsigned long long)rand() ^ ((unsigned long long)rand() << 15) ^ ((unsigned long long)rand() << 30) ^ ((unsigned long long)rand() << 45) ^ ((unsigned long long)rand() << 60);
}
//产生一个64位的随机数
int shallowValue(char board[8][8], int me_flag, int step);

int main()
{
	loop();

	/*char test[8][8] = 
	{
		{0,0,0,0,0,0,0,2},
		{0,0,2,0,0,0,0,0},
		{0,0,0,1,0,0,0,0},
		{1,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{1,0,1,0,1,0,0,0},
		{0,1,0,1,0,1,0,1},
		{1,0,1,0,1,0,0,0}
	};
	clock_t start = clock();
	int depthmax = 12;
	int score = 0;
	int SearchDepth = 12;
	Command bestcommand;
	score = NegaMax(test, depthmax, BLACK, start, -IFINITY, IFINITY, 25, depthmax);
	clock_t end = clock();
	double time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("%d\n", crtbestcommand.numStep);
	for (int i = 0; i < crtbestcommand.numStep; i++)
	{
		printf("%d %d\n", crtbestcommand.x[i], crtbestcommand.y[i]);
	}
	printf("Depth: %d Score: %d Time: %lf\n", SearchDepth, score, time);
#ifdef DEBUG
	printf("Depth: %d\n", depthmax - 1);
#endif*/
	return 0;
}

void debug(const char *str)
{
	printf("DEBUG %s\n", str);
	fflush(stdout);
}

int search(char board[BOARD_SIZE][BOARD_SIZE], int i, int j, Command last, Command tempMove[], int *MoveIndex, int eatNum)//判断i，j是否有子可吃，并返回最大吃子数，将所有吃子路线保存于数组中
{
	int maxnum = 0;
	Command tempLast = { 0 };
	for (int k = 0; k < 4; k++)//判断一个子的四个方向
	{
		if (InBoard(i + dir[k][0], j + dir[k][1]) && board[i + dir[k][0]][j + dir[k][1]])//判断是否越界以及是否有子
		{
			if ((board[i + dir[k][0]][j + dir[k][1]] % 2) != (board[i][j] % 2))//是否是对手的棋子？
			{
				if (InBoard(i + 2 * dir[k][0], j + 2 * dir[k][1]) && !board[i + 2 * dir[k][0]][j + 2 * dir[k][1]])//判断是否可以落地
				{
					char tempEat;//用来保存被吃掉的子，回溯时还原
					char temp;//用来保存跳跃的棋子
					tempEat = board[i + dir[k][0]][j + dir[k][1]];
					temp = board[i][j];
					board[i + dir[k][0]][j + dir[k][1]] = 0;//拿掉被吃的棋子
					board[i + 2 * dir[k][0]][j + 2 * dir[k][1]] = board[i][j];
					board[i][j] = 0;
					int num = 0;//记录当前位置的后续过程中的最大吃子数
					if (eatNum == 0)//起点
					{
						tempMove[*MoveIndex].numStep = 2;
						tempMove[*MoveIndex].x[0] = i;
						tempMove[*MoveIndex].y[0] = j;
						tempMove[*MoveIndex].x[1] = i + 2 * dir[k][0];
						tempMove[*MoveIndex].y[1] = j + 2 * dir[k][1];
						(*MoveIndex)++;
						num = search(board, i + 2 * dir[k][0], j + 2 * dir[k][1], tempMove[*MoveIndex - 1], tempMove, MoveIndex, eatNum + 1);//搜索能否连吃
					}
					else
					{
						tempMove[*MoveIndex].numStep = last.numStep + 1;
						int count;
						for (count = 0; count < last.numStep; count++)
						{
							tempMove[*MoveIndex].x[count] = last.x[count];
							tempMove[*MoveIndex].y[count] = last.y[count];
						}
						tempMove[*MoveIndex].x[count] = i + 2 * dir[k][0];
						tempMove[*MoveIndex].y[count] = j + 2 * dir[k][1];
						(*MoveIndex)++;
						num = search(board, i + 2 * dir[k][0], j + 2 * dir[k][1], tempMove[*MoveIndex - 1], tempMove, MoveIndex, eatNum + 1);//搜索能否连吃
					}
					if (num + 1 > maxnum)
					{
						maxnum = num + 1;
					}
					board[i + dir[k][0]][j + dir[k][1]] = tempEat;//回溯
					board[i][j] = temp;
					board[i + 2 * dir[k][0]][j + 2 * dir[k][1]] = 0;
				}
			}
		}
	}
	return maxnum;
}

int JumpSearch(char board[BOARD_SIZE][BOARD_SIZE], int me_flag, Command Move[])//判断是否有子可吃
{
	int i, j;//遍历棋盘的哨兵
	int MoveIndex = 0;//记录着法数
	int max = -IFINITY;
	Command tempMove[100];
	int tempMoveIndex = 0;
	Command tempLast = { 0 };
	memset(tempMove, 0, sizeof(tempMove));
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)//遍历棋盘
		{
			if (board[i][j] != me_flag && board[i][j] != me_flag + 2)//若不是当前棋手的棋子就跳过
			{
				continue;
			}
			int num = search(board, i, j, tempLast/*一个用来充当last的指令*/, tempMove/*一个存放可能走法的指令数组*/, &tempMoveIndex/*走法数组的索引*/, 0);
			if(max < num)
			{
				max = num;
			}
		}
	}
	if (max == 0)
	{
		return 0;
	}
	int k;
	for (k = 0; k < tempMoveIndex; k++)
	{
		if (tempMove[k].numStep == max + 1)
		{
			Move[MoveIndex].numStep = tempMove[k].numStep;
			for (int count = 0; count < Move[MoveIndex].numStep; count++)
			{
				Move[MoveIndex].x[count] = tempMove[k].x[count];
				Move[MoveIndex].y[count] = tempMove[k].y[count];//将numStep个坐标记录下来
			}
			MoveIndex++;
		}
	}
	return MoveIndex;
}

void PrintBoard(char board[BOARD_SIZE][BOARD_SIZE])
{
	int i, j;
	printf("\n");
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			switch (board[i][j])
			{
			case EMPTY:
				printf(". ");
				break;
			case BLACK:
				printf("x ");
				break;
			case WHITE:
				printf("o ");
				break;
			case B_KING:
				printf("X ");
				break;
			case W_KING:
				printf("O ");
				break;
			}
		}
		printf("\n");
	}
}
int CreateMove(char board[BOARD_SIZE][BOARD_SIZE], int me_flag, Command Move[])
{
	int MoveIndex = 0;
	int i, j;
	//若不能吃子，这里生成闲逛的走法
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)//遍历棋盘
		{
			if (board[i][j] != me_flag && board[i][j] != (me_flag + 2))//若不为当前棋手的棋子就跳过
			{
				continue;
			}
			switch (board[i][j])
			{
			case BLACK:
				for (int k = 0; k < 2; k++)
				{
					if (InBoard(i + dir[k][0], j + dir[k][1]) && !board[i + dir[k][0]][j + dir[k][1]])
					{
						Move[MoveIndex].numStep = 2;
						Move[MoveIndex].x[0] = i;
						Move[MoveIndex].y[0] = j;
						Move[MoveIndex].x[1] = i + dir[k][0];
						Move[MoveIndex].y[1] = j + dir[k][1];//记录着法
						MoveIndex++;
					}
				}
				break;
			case WHITE:
				for (int k = 2; k < 4; k++)
				{
					if (InBoard(i + dir[k][0], j + dir[k][1]) && !board[i + dir[k][0]][j + dir[k][1]])
					{
						Move[MoveIndex].numStep = 2;
						Move[MoveIndex].x[0] = i;
						Move[MoveIndex].y[0] = j;
						Move[MoveIndex].x[1] = i + dir[k][0];
						Move[MoveIndex].y[1] = j + dir[k][1];//记录着法
						MoveIndex++;
					}
				}
				break;
			case B_KING:
				for (int k = 0; k < 4; k++)
				{
					if (InBoard(i + dir[k][0], j + dir[k][1]) && !board[i + dir[k][0]][j + dir[k][1]])
					{
						Move[MoveIndex].numStep = 2;
						Move[MoveIndex].x[0] = i;
						Move[MoveIndex].y[0] = j;
						Move[MoveIndex].x[1] = i + dir[k][0];
						Move[MoveIndex].y[1] = j + dir[k][1];//记录着法
						MoveIndex++;
					}
				}
				break;
			case W_KING:
				for (int k = 0; k < 4; k++)
				{
					if (InBoard(i + dir[k][0], j + dir[k][1]) && !board[i + dir[k][0]][j + dir[k][1]])
					{
						Move[MoveIndex].numStep = 2;
						Move[MoveIndex].x[0] = i;
						Move[MoveIndex].y[0] = j;
						Move[MoveIndex].x[1] = i + dir[k][0];
						Move[MoveIndex].y[1] = j + dir[k][1];//记录着法
						MoveIndex++;
					}
				}
				break;
			}
		}
	}
	return MoveIndex;
}


int NegaMax(char board[8][8], int depth, int me_flag, clock_t start, int alpha, int beta, int step, int depthmax)
{
	if (depth == 0)
	{
		int value = evaluate(board, me_flag, step, depthmax);
		//printf("%d\n", value);
		return value;//搜索到指定深度后返回估值
	}

	//int key = CalKey(board);
	//unsigned long long posValue = CalPos(board);
	//while (HashTable[key].isused && HashTable[key].posValue != posValue)
	//{
	//	key++;
	//	key = key % HASHMAX;
	//	//clock_t current = clock();
	//	//if ((double)(current - start) / CLOCKS_PER_SEC > 1.5)
	//	//{
	//	//	return alpha;
	//	//}
	//}
	////退出循环时key为一个空位置或者为储存当前棋盘估值的位置
	//if (depth != depthmax)//防止传入的局面在表中，直接返回值跳过搜索的情况
	//{
	//	if (HashTable[key].depth >= depth - 1 && depth > 1)//储存的估值的搜索深度可以接受
	//	{
	//		if (HashTable[key].isused)
	//		{
	//			if (me_flag == HashTable[key].flag)
	//			{
	//				return HashTable[key].value;
	//			}
	//			else
	//			{
	//				return -HashTable[key].value;
	//			}
	//		}
	//	}
	//}

	Command Move[50];
	int MoveIndex = 0;
	MoveIndex = JumpSearch(board, me_flag, Move);//生成着法
	if (MoveIndex == 0)
	{
		//这里可以使用空着裁剪

		//int NullMoveIndex = 0;
		//Command NullMove[50];
		//NullMoveIndex = JumpSearch(board, 3 - me_flag, NullMove);
		//if (!NullMoveIndex && depth > 2)//双方均无吃子的情况下，尝试进行空着裁剪，令对手先下一步棋，自己不动
		//{
		//	NullMoveIndex = CreateMove(board, 3 - me_flag, NullMove);
		//	if (NullMoveIndex != 0)
		//	{
		//		MoveSort(board, NullMove, NullMoveIndex, step + 1, 3 - me_flag);//为对手的走法排序，挑选最好的一个走法实施
		//		PieceEat eat = { 0 };
		//		char temp = board[NullMove[NullMoveIndex - 1].x[0]][NullMove[NullMoveIndex - 1].y[0]];
		//		TakeCommand(board, NullMove[NullMoveIndex - 1], &eat);
		//		int value = -NegaMax(board, depth - 3, 3 - me_flag, start, -beta, -beta + 1, step + 3, depthmax);
		//		CancelCommand(board, temp, NullMove[NullMoveIndex - 1], eat);
		//		//使用了极小窗口，下界为beta-1，上界为beta，若返回结果为beta（大于等于beta）则触发剪枝
		//		if (value >= beta)
		//		{
		//			return beta;
		//		}

		//		//for (int i = 0; i < NullMoveIndex; i++)
		//		//{
		//		//	PieceEat eat = { 0 };
		//		//	char temp = board[NullMove[i].x[0]][NullMove[i].y[0]];
		//		//	TakeCommand(board, NullMove[i], &eat);
		//		//	int value = -NegaMax(board, depth - 2, 3 - me_flag, start, -beta, -beta + 1, step + 2, depthmax);
		//		//	CancelCommand(board, temp, NullMove[i], eat);
		//		//	//使用了极小窗口，下界为beta-1，上界为beta，若返回结果为beta（大于等于beta）则触发剪枝
		//		//	if (value >= beta)
		//		//	{
		//		//		return beta;
		//		//	}
		//		//}

		//		//如果均小于beta，那么回归正常的PVS
		//	}
		//}
		MoveIndex = CreateMove(board, me_flag, Move);
		if (depth > 2 && MoveIndex != 0)//剩余搜索层数为2及以上则先排序
		{
			MoveSort(board, Move, MoveIndex, step, me_flag);
		}
	}
	if (MoveIndex == 0)
	{
		return -IFINITY + 1;
	}

	//下面是主要变例搜索，对于ab搜索，alpha值为下界，beta值为上界，返回值会处于alpha与beta之间
	//在着法的排序较好的情况下，当我们找到第一个介于alpha与beta之间的值的时候，我们可以认为它很有可能就是我们想要找的最优解
	//因此我们可以检验这个着法后面的兄弟着法们之间有没有更好的着法，方法是使用参数为alpha与alpha+1的ab搜索
	//这个搜索过程非常容易触发剪枝，因此耗时极少，同时返回值若为alpha则说明后续的兄弟节点的值并不会比当前节点更好
	//若返回值为alpha+1，则说明后继的兄弟节点中有一个比当前节点更好的着法，因此需要进行完整的ab搜索
	int findPV = 0;
	int i;
	for (i = 0; i < MoveIndex; i++)
	{
		PieceEat eat = { 0 };
		char temp = board[Move[i].x[0]][Move[i].y[0]];
		TakeCommand(board, Move[i], &eat);//实施生成的着法
		int value = (alpha + beta) / 2;
		if (findPV)
		{
			value = -NegaMax(board, depth - 1, 3 - me_flag, start, -alpha - 1, -alpha, step + 1, depthmax);
			if (value > alpha && value < beta)
			{
				value = -NegaMax(board, depth - 1, 3 - me_flag, start, -beta, -alpha, step + 1, depthmax);
			}
		}
		else
		{
			value = -NegaMax(board, depth - 1, 3 - me_flag, start, -beta, -alpha, step + 1, depthmax);
		}

		//PrintBoard(board);
		//printf("\n");

		CancelCommand(board, temp, Move[i], eat);//回溯
		//PrintBoard(board);
		//printf("\n");
		if (value >= beta)
			//这里的beta实际上是前面所有的兄弟节点中的最大值，如果这个节点的子节点返回的值大于beta，说明该节点最终的返回值也将大于beta，
			//而对于该节点的父节点来说，需要从这些节点返回值的负值中挑一个最大的，也就是说该节点返回值的负值一定小于-beta，
			//这就意味着父节点不会挑选当前节点，beta值可以看作是一个上界，当前节点的子结点中有一个返回了超过这个上界的值，便触发剪枝
		{
			return beta;
		}
		if (value > alpha)
			//alpha表示当前节点的已经遍历过的子节点们的返回值的最大值
		{
			alpha = value;
			if (depth == depthmax)
			{
				crtbestcommand = Move[i];
				//printf("Best move for now: %d\n", alpha);
			}
			findPV = 1;
		}
		clock_t end = clock();

		if (((double)(end - start) / CLOCKS_PER_SEC) > 1.49)
		{
			timeout = 1;
			return alpha;
		}
	}
	//if (depth == DEPTHMAX)
	//{
	//	printf("MAX: %d\n", alpha);
	//}

	//记录一个主要变例中局面的值
	//if (findPV)
	//{
	//	HashTable[key].isused = 1;
	//	HashTable[key].depth = depth;
	//	HashTable[key].value = alpha;
	//	HashTable[key].posValue = posValue;
	//	HashTable[key].flag = me_flag;
	//}
	return alpha;
}

int evaluate(char board[8][8], int me_flag, int step, int depthmax)
{
	int i, j;//哨兵
	int score = 0;
	int queueScore = 0;
	int kingScore = 0;
	int numScore = 0;
	int posScore = 0;
	int defenceScore = 0;
	int specialQueueScore = 0;//一些出现情况较少，但是一旦出现就可以产生较大收益的特殊阵型，例如一个棋子同时威胁两个棋子
	int dangerQueueScore = 0;//危险阵型，出现时极有可能被对手所利用
	int pieceNum[3] = { 0 };
	int eatScore = 0;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			if (board[i][j] == EMPTY)
			{
				continue;
			}
			switch (board[i][j] % 2)
			{
			case 1://黑棋或者黑王
				if (board[i][j] == BLACK)
				{
					numScore++;//黑子数量+1
					pieceNum[1]++;
				}
				else
				{
					numScore++;
					kingScore++;//王的数量+1
					pieceNum[1]++;
				}
				if (i <= 2)
				{
					posScore = posScore + 3 - i;//若靠近敌方底线，位置分+1
				}
				if (j >= 1 && j <= 6)
				{
					posScore++;//若靠近棋盘中央，位置分+1
				}
				//if (InBoard(i - 1, j - 1) && InBoard(i + 1, j + 1))//三子队列（主方向）
				//{
				//	if (board[i - 1][j - 1] == board[i][j] && board[i + 1][j + 1] == board[i][j])
				//	{
				//		queueScore++;
				//	}
				//}
				//if (InBoard(i - 1, j + 1) && InBoard(i + 1, j - 1))//三子队列（副方向）
				//{
				//	if (board[i - 1][j + 1] == board[i][j] && board[i][j] == board[i + 1][j - 1])
				//	{
				//		queueScore++;
				//	}
				//}
				if (me_flag == BLACK)
				{
					if (InBoard(i + 1, j - 1) && InBoard(i + 1, j + 1))//黑棋三角阵型
					{
						if ((board[i + 1][j - 1] == BLACK || board[i + 1][j - 1] == B_KING) && (board[i + 1][j + 1] == BLACK || board[i + 1][j + 1] == B_KING))
						{
							queueScore++;
						}
					}
				}
				//for (int k = 0; k < 4; k++)//是否存在被吃子的可能
				//{
				//	if (InBoard(i + dir[k][0], j + dir[k][1]) && InBoard(i - dir[k][0], j - dir[k][1]))
				//	{
				//		if (board[i + dir[k][0]][j + dir[k][1]] == WHITE || board[i + dir[k][0]][j + dir[k][1]] == W_KING)
				//		{
				//			if (board[i - dir[k][0]][j - dir[k][1]] == EMPTY)
				//			{
				//				if (me_flag == WHITE)
				//				{
				//					eatScore--;
				//				}
				//			}
				//		}
				//	}
				//}
				break;
			case 0:
				if (board[i][j] == WHITE)
				{
					numScore--;//黑子数量+1
					pieceNum[2]++;
				}
				else
				{
					numScore--;
					kingScore--;//王的数量+1
					pieceNum[2]++;
				}
				if (i >= 5)
				{
					posScore = posScore - (i - 4);//若靠近敌方底线，位置分+1
				}
				if (j >= 1 && j <= 6)
				{
					posScore--;//若靠近棋盘中央，位置分+1
				}
				//if (InBoard(i - 1, j - 1) && InBoard(i + 1, j + 1))//三子队列（主方向）
				//{
				//	if (board[i - 1][j - 1] == board[i][j] && board[i + 1][j + 1] == board[i][j])
				//	{
				//		queueScore--;
				//	}
				//}
				//if (InBoard(i - 1, j + 1) && InBoard(i + 1, j - 1))//三子队列（副方向）
				//{
				//	if (board[i - 1][j + 1] == board[i][j] && board[i][j] == board[i + 1][j - 1])
				//	{
				//		queueScore--;
				//	}
				//}
				if (me_flag == WHITE)
				{
					if (InBoard(i - 1, j - 1) && InBoard(i - 1, j + 1))//白棋三角阵型
					{
						if ((board[i - 1][j - 1] == WHITE || board[i - 1][j - 1] == W_KING) && (board[i - 1][j + 1] == WHITE || board[i - 1][j + 1] == W_KING))
						{
							queueScore--;
						}
					}
				}
				//for (int k = 0; k < 4; k++)//是否存在被吃子的可能
				//{
				//	if (InBoard(i + dir[k][0], j + dir[k][1]) && InBoard(i - dir[k][0], j - dir[k][1]))
				//	{
				//		if (board[i + dir[k][0]][j + dir[k][1]] == BLACK || board[i + dir[k][0]][j + dir[k][1]] == B_KING)
				//		{
				//			if (board[i - dir[k][0]][j - dir[k][1]] == EMPTY)
				//			{
				//				if (me_flag == BLACK)
				//				{
				//					eatScore++;
				//				}
				//			}
				//		}
				//	}
				//}
			}
		}
	}

	if (pieceNum[me_flag] == 0)
	{
		return -IFINITY + 1;
	}
	else if (pieceNum[3 - me_flag] == 0)
	{
		return IFINITY - 1;
	}
	if (step > 90 || pieceNum[1] < 5 || pieceNum[2] < 5)
	{
		score = 100000 * numScore + 5000 * queueScore + 20000 * kingScore + 500 * posScore;
		//后期时优先保证自己不被吃，在此前提下尽可能去升王，或利用特殊阵型打击对手
	}
	else
	{
		score = 10000 * numScore + 300 * queueScore + 2000 * kingScore + 100 * posScore;
	}
	if (me_flag == BLACK)
	{
		return score;
	}
	else if (me_flag == WHITE)
	{
		return -score;
	}
}

int shallowValue(char board[8][8], int me_flag, int step)
{
	int i, j;//哨兵
	int score = 0;
	int queueScore = 0;
	int kingScore = 0;
	int posScore = 0;
	int specialQueueScore = 0;//一些出现情况较少，但是一旦出现就可以产生较大收益的特殊阵型，例如一个棋子同时威胁两个棋子
	int dangerQueueScore = 0;//危险阵型，出现时极有可能被对手所利用
	int pieceNum[3] = { 0 };
	int eatScore = 0;
	for (i = 0; i < BOARD_SIZE; i++)
	{
		for (j = 0; j < BOARD_SIZE; j++)
		{
			if (board[i][j] == EMPTY)
			{
				continue;
			}
			switch (board[i][j] % 2)
			{
			case 1://黑棋或者黑王
				if (board[i][j] == B_KING)
				{
					pieceNum[1]++;
					kingScore++;
				}
				if (i <= 2)
				{
					posScore = posScore + (3 - i);//若靠近敌方底线，位置分+1
				}
				if (j >= 1 && j <= 6)
				{
					posScore++;//若靠近棋盘中央，位置分+1
				}
				if (me_flag == BLACK)
				{
					//if (InBoard(i - 1, j - 1) && InBoard(i + 1, j + 1))//三子队列（主方向）
					//{
					//	if (board[i - 1][j - 1] == board[i][j] && board[i + 1][j + 1] == board[i][j])
					//	{
					//		queueScore++;
					//	}
					//}
					//if (InBoard(i - 1, j + 1) && InBoard(i + 1, j - 1))//三子队列（副方向）
					//{
					//	if (board[i - 1][j + 1] == board[i][j] && board[i][j] == board[i + 1][j - 1])
					//	{
					//		queueScore++;
					//	}
					//}
					if (InBoard(i + 1, j - 1) && InBoard(i + 1, j + 1))//黑棋三角阵型
					{
						if ((board[i + 1][j - 1] == BLACK || board[i + 1][j - 1] == B_KING) && (board[i + 1][j + 1] == BLACK || board[i + 1][j + 1] == B_KING))
						{
							queueScore++;
						}
					}
				}
				if (InBoard(i - 2, j - 2) && InBoard(i - 1, j - 1) && InBoard(i + 1, j + 1) && InBoard(i + 2, j + 2))
				{
					if (board[i - 2][j - 2] == EMPTY && board[i + 2][j + 2] == EMPTY && (board[i - 1][j - 1] == WHITE || board[i - 1][j - 1] == W_KING) && (board[i + 1][j + 1] == WHITE || board[i + 1][j + 1] == W_KING))
					{
						specialQueueScore++;
					}
				}
				if (InBoard(i - 2, j + 2) && InBoard(i - 1, j + 1) && InBoard(i + 1, j - 1) && InBoard(i + 2, j - 2))
				{
					if (board[i - 2][j + 2] == EMPTY && board[i + 2][j - 2] == EMPTY && (board[i - 1][j + 1] == WHITE || board[i - 1][j + 1] == W_KING) && (board[i + 1][j - 1] == WHITE || board[i + 1][j - 1] == W_KING))
					{
						specialQueueScore++;
					}
				}
				for (int k = 0; k < 4; k++)//是否存在被吃子的可能
				{
					if (InBoard(i + dir[k][0], j + dir[k][1]) && InBoard(i - dir[k][0], j - dir[k][1]))
					{
						if (board[i + dir[k][0]][j + dir[k][1]] == WHITE || board[i + dir[k][0]][j + dir[k][1]] == W_KING)
						{
							if (board[i - dir[k][0]][j - dir[k][1]] == EMPTY)
							{
								if (me_flag == BLACK)
								{
									eatScore--;
								}
							}
						}
					}
				}
				break;
			case 0:
				if (board[i][j] == W_KING)
				{
					kingScore--;//黑子数量+1
					pieceNum[2]++;
				}
				if (i >= 5)
				{
					posScore = posScore - (i - 4);//若靠近敌方底线，位置分+1
				}
				if (j >= 1 && j <= 6)
				{
					posScore--;//若靠近棋盘中央，位置分+1
				}
				if (me_flag == WHITE)
				{
					//if (InBoard(i - 1, j - 1) && InBoard(i + 1, j + 1))//三子队列（主方向）
					//{
					//	if (board[i - 1][j - 1] == board[i][j] && board[i + 1][j + 1] == board[i][j])
					//	{
					//		queueScore--;
					//	}
					//}
					//if (InBoard(i - 1, j + 1) && InBoard(i + 1, j - 1))//三子队列（副方向）
					//{
					//	if (board[i - 1][j + 1] == board[i][j] && board[i][j] == board[i + 1][j - 1])
					//	{
					//		queueScore--;
					//	}
					//}
					if (InBoard(i - 1, j - 1) && InBoard(i - 1, j + 1))//白棋三角阵型
					{
						if ((board[i - 1][j - 1] == WHITE || board[i - 1][j - 1] == W_KING) && (board[i - 1][j + 1] == WHITE || board[i - 1][j + 1] == W_KING))
						{
							queueScore--;
						}
					}
				}
				if (InBoard(i - 2, j - 2) && InBoard(i - 1, j - 1) && InBoard(i + 1, j + 1) && InBoard(i + 2, j + 2))
				{
					if (board[i - 2][j - 2] == EMPTY && board[i + 2][j + 2] == EMPTY && (board[i - 1][j - 1] == BLACK || board[i - 1][j - 1] == B_KING) && (board[i + 1][j + 1] == BLACK || board[i + 1][j + 1] == B_KING))
					{
						specialQueueScore--;
					}
				}
				if (InBoard(i - 2, j + 2) && InBoard(i - 1, j + 1) && InBoard(i + 1, j - 1) && InBoard(i + 2, j - 2))
				{
					if (board[i - 2][j + 2] == EMPTY && board[i + 2][j - 2] == EMPTY && (board[i - 1][j + 1] == BLACK || board[i - 1][j + 1] == B_KING) && (board[i + 1][j - 1] == BLACK || board[i + 1][j - 1] == B_KING))
					{
						specialQueueScore--;
					}
				}
				for (int k = 0; k < 4; k++)//是否存在被吃子的可能
				{
					if (InBoard(i + dir[k][0], j + dir[k][1]) && InBoard(i - dir[k][0], j - dir[k][1]))
					{
						if (board[i + dir[k][0]][j + dir[k][1]] == BLACK || board[i + dir[k][0]][j + dir[k][1]] == B_KING)
						{
							if (board[i - dir[k][0]][j - dir[k][1]] == EMPTY)
							{
								if (me_flag == WHITE)
								{
									eatScore++;
								}
							}
						}
					}
				}
			}
		}
	}
	if (step > 90 || pieceNum[1] < 5 || pieceNum[2] < 5)
	{
		score = 5000 * queueScore + 30000 * kingScore + 500 * posScore + 100000 * eatScore + 10000 * specialQueueScore;
		//后期时优先保证自己不被吃，在此前提下尽可能去升王，或利用特殊阵型打击对手
	}
	else
	{
		score = 500 * queueScore + 3000 * kingScore + 100 * posScore + 20000 * eatScore + 10000 * specialQueueScore;
	}
	if (me_flag == BLACK)
	{
		return score;
	}
	else if (me_flag == WHITE)
	{
		return -score;
	}
}
void TakeCommand(char board[8][8], Command command, PieceEat *eat)
{
	int count;
	int eatindex = 0;
	eat->num = 0;
	char temp = board[command.x[0]][command.y[0]];
	for (count = 0; count < command.numStep - 1; count++)
	{
		if (abs(command.x[count + 1] - command.x[count]) == 2 && abs(command.y[count + 1] - command.y[count]) == 2)
		{
			eat->num++;
			eat->x[eatindex] = (command.x[count + 1] + command.x[count]) / 2;
			eat->y[eatindex] = (command.y[count + 1] + command.y[count]) / 2;
			eat->piece[eatindex] = board[(command.x[count + 1] + command.x[count]) / 2][(command.y[count + 1] + command.y[count]) / 2];
			board[(command.x[count + 1] + command.x[count]) / 2][(command.y[count + 1] + command.y[count]) / 2] = EMPTY;
			eatindex++;
		}
		board[command.x[count]][command.y[count]] = EMPTY;
	}
	board[command.x[count]][command.y[count]] = temp;
	if (temp == BLACK && command.x[count] == 0)
	{
		board[command.x[count]][command.y[count]] = B_KING;
	}
	else if (temp == WHITE && command.x[count] == 7)
	{
		board[command.x[count]][command.y[count]] = W_KING;
	}
}

void CancelCommand(char board[BOARD_SIZE][BOARD_SIZE], char temp, Command command, PieceEat eat)
{
	board[command.x[command.numStep - 1]][command.y[command.numStep - 1]] = EMPTY;
	board[command.x[0]][command.y[0]] = temp;
	if (eat.num)
	{
		for (int count = 0; count < eat.num; count++)
		{
			board[eat.x[count]][eat.y[count]] = eat.piece[count];
		}
	}
}
void turn()
{
	Command command = aiTurn(board, me_flag);
	PieceEat eat = { 0 };
	TakeCommand(board, command, &eat);
	printf("%d", command.numStep);
	for (int i = 0; i < command.numStep; i++)
	{
		printf(" %d,%d", command.x[i], command.y[i]);
	}
	printf("\n");
	fflush(stdout);
	step += 2;//每次行动过后步数+2，己方1，对方1
}

void loop()
{
	char tag[10] = { 0 };
	Command command = { 0 };
	int status;
	while (TRUE)
	{
		memset(tag, 0, sizeof(tag));
		scanf("%s", tag);
		if (strcmp(tag, START) == 0)
		{
			scanf("%d", &me_flag);
			other_flag = 3 - me_flag;
			step = me_flag;
			//memset(HashTable, 0, sizeof(HashTable));

			//产生随机值，用于计算散列值
			//for (int i = 0; i < 5; i++)
			//{
			//	for (int j = 0; j < 64; j++)
			//	{
			//		Zobrist[i][j] = rand64();
			//	}
			//}
			#ifdef DEBUG
			printf("\n");
			PrintBoard(board);//本地调试用
			printf("\n");
			#endif

			printf("OK\n");
			fflush(stdout);
		}
		else if (strcmp(tag, PLACE) == 0)
		{
			scanf("%d", &command.numStep);
			for (int i = 0; i < command.numStep; i++)
			{
				scanf("%d,%d", &command.x[i], &command.y[i]);
			}
			PieceEat eat = { 0 };
			TakeCommand(board, command, &eat);

			#ifdef DEBUG
			printf("\n");
			PrintBoard(board);//本地调试用
			printf("\n");
			#endif
		}
		else if (strcmp(tag, TURN) == 0)
		{
			turn();
			#ifdef DEBUG
			PrintBoard(board);//本地调试用
			printf("\n");//本地调试用
			#endif		
		}
		else if (strcmp(tag, END) == 0)
		{
			scanf("%d", &status);
		}
		//    printBoard();
	}
}
Command aiTurn(char board[BOARD_SIZE][BOARD_SIZE], int me_flag)
{
	Command bestcommand = { 0 };
	clock_t start = clock();
	clock_t end;
	crtbestcommand.numStep = 0;
	memset(crtbestcommand.x, 0, sizeof(crtbestcommand.x));
	memset(crtbestcommand.y, 0, sizeof(crtbestcommand.y));
	int SearchDepth = 0;
	//记录目前双方棋子数量
	int num[2] = { 0 };
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			if (board[i][j] == BLACK || board[i][j] == B_KING)
			{
				num[0]++;
			}
			if (board[i][j] == WHITE || board[i][j] == W_KING)
			{
				num[1]++;
			}
		}
	}

	//迭代加深的ab搜索
	int score = 0;

	if (step < 110)//普通走法
	{
		timeout = 0;
		int depthmax = 10;
		int temp = 0;
		while (timeout == 0)//还有足够的时间搜索
		{
			temp = NegaMax(board, depthmax, me_flag, start, -IFINITY, IFINITY, step, depthmax);//搜索指定深度
			if (timeout == 0)//如果搜索没有超时
			{
				score = temp;
				bestcommand = crtbestcommand;//记录结果
				SearchDepth = depthmax;
				depthmax += 2;//深度加深
			}
			end = clock();
		}
		if (timeout == 1 && depthmax == 10)//甚至没搜完第一次迭代
		{
			score = temp;
			bestcommand = crtbestcommand;
			SearchDepth = 10;
		}
		#ifdef DEBUG
		printf("Depth: %d\n", depthmax);
		#endif
	}
	else//残局搜索
	{
		int depthmax = 120 - step + 1;
		score = NegaMax(board, depthmax, me_flag, start, -IFINITY, IFINITY, step, depthmax);
		bestcommand = crtbestcommand;
		SearchDepth = depthmax;
	}
	

	//普通的ab搜索
	//int depthmax;
	//if (120 - step + 1 < 10)
	//{
	//	depthmax = 120 - step + 1;
	//}
	//else
	//{
	//	depthmax = 10;
	//}
	//score = NegaMax(board, depthmax, me_flag, start, -IFINITY, IFINITY, step, depthmax);
	//SearchDepth = 1;
	//bestcommand = crtbestcommand;

	end = clock();
	double runtime = (double)(end - start) / CLOCKS_PER_SEC;
	sprintf(message, "Runtime: %lf  SearchDepth: %d Score: %d", runtime, SearchDepth, score);
	debug(message);
	#ifdef DEBUG
	printf("Search Time: %lf\n", runtime);//本地调试用
	#endif
	if (bestcommand.numStep == 0)
	{
		Command Move[50];
		int MoveIndex = 0;
		MoveIndex = CreateMove(board, me_flag, Move);//生成着法
		MoveSort(board, Move, MoveIndex, step, me_flag);
		bestcommand = Move[0];
	}
	return bestcommand;
}

void MoveSort(char board[BOARD_SIZE][BOARD_SIZE], Command Move[], int MoveNum, int step, int me_flag)
{
	int value[50];
	for (int i = 0; i < MoveNum; i++)
	{
		PieceEat eat = { 0 };
		char temp = board[Move[i].x[0]][Move[i].y[0]];
		TakeCommand(board, Move[i], &eat);//实施生成的着法
		value[i] = shallowValue(board, me_flag, step);
		CancelCommand(board, temp, Move[i], eat);
	}
	int flag = 1;
	while (flag)
	{
		flag = 0;
		for (int i = 0; i < MoveNum - 1; i++)
		{
			if (value[i] < value[i + 1])
			{
				int temp = value[i];
				value[i] = value[i + 1];
				value[i + 1] = temp;
				Command tempCommand = Move[i];
				Move[i] = Move[i + 1];
				Move[i + 1] = tempCommand;
				flag = 1;
			}
		}
	}
}

//int CalKey(char board[BOARD_SIZE][BOARD_SIZE])
//{
//	unsigned long long key = 0;
//	for (int i = 0; i < BOARD_SIZE; i++)
//	{
//		for (int j = 0; j < BOARD_SIZE; j++)
//		{
//			key = key ^ Zobrist[board[i][j]][i * 8 + j];
//		}
//	}
//	int boardkey = key % HASHMAX;
//	return boardkey;
//}

//unsigned long long CalPos(char board[BOARD_SIZE][BOARD_SIZE])
//{
//	unsigned long long posNum = 0;
//	for (int i = 0; i < BOARD_SIZE; i++)
//	{
//		for (int j = 0; j < BOARD_SIZE; j++)
//		{
//			posNum = (posNum << 1) + (board[i][j] == EMPTY);
//		}
//	}
//	return posNum;
//}
/**	
 * 你的代码结束
 */ 
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
