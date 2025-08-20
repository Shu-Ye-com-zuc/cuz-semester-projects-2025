/*
@define 头部define是为了忽略VS环境下的安全性检查，因为该检查会导致程序不能执行；
@admin ShuYe.©
@version v1.1.9
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>


//干员
#define MAX_NAME 15
#define MAX_CAREER 15
#define MAX_BRANCH 15
#define MAX_BACKGROUND 20
#define MAX_ORGANIZATION 30
#define MAX_TAG 50
#define MAX_TALENT 201
//用户
#define MAX_USER_NAME 11
#define MAX_PASSWORD 11
#define MAX_STATUS 6
/*常量值*/

/*干员数据存储结构体*/
typedef struct Staff {
	int id;
	char name[MAX_NAME];
	char career[MAX_CAREER];
	char branch[MAX_BRANCH];
	char background[MAX_BACKGROUND];
	char organization[MAX_ORGANIZATION];
	int cost;
	char tag[MAX_TAG];
	char talent[MAX_TALENT];
}Staff;

/*干员链表节点结构体*/
typedef struct Node {
	Staff value;
	struct Node* next;
}Node;

/*用户数据存储结构体*/
typedef struct User {
	char user_name[MAX_USER_NAME];
	char password[MAX_PASSWORD];
	char status[MAX_STATUS];
}User;

/*用户链表节点结构体*/
typedef struct UserNode {
	User value;
	struct UserNode* next;
}UNode;

/*函数声明*/
//用户数据处理系统主体
char* get_name_password(int flag);	//获取输入的用户名或密码
UNode* create_user_node(User data);	//创建用户节点 注册用
int identity(UNode* head, const char* user_name, const char* password, char* user_name_out);	//身份确认 1是管理员，2是查询用户，0是登录失败
void add_user(UNode** head, User data, int check);	//新增用户节点 注册及读取数据用
UNode* find_user(UNode* head, const char* user_name);	//查找用户节点
int log_in(UNode* head, char* user_name);	//登录
void sign_in(UNode** head);	//注册
void modify_password(const char* user_name);	//修改密码
int delete_user(const char* user_name);	//注销账户
void read_user(UNode** head, const char* userFile);	//读取用户数据
void save_user(UNode* head, const char* userFile);	//保存用户数据
char log_sign_welcome();	//登录主页面
void free_user(UNode* head); //释放用户链表
//管理查询系统主菜单
char admin_welcome();	//管理员主菜单页面
char user_welcome();	//用户主菜单页面
//管理查询系统主体
void see_you();	//退出效果
int last_node_id(Node** head);	//查询最后一个干员编号
int add_officer(Node** head);	//新增干员 主调函数
Node* create_node(Staff data);	//创建新节点 被调
void add_node(Node** head, Staff data);		//新增干员信息节点 被调
int delete_officer(Node** head);	//删除干员 主调函数
void delete_node_by_id(Node** head, int id);	//按编号删除干员节点 被调
void delete_node_by_name(Node** head, const char* name);	//按姓名删除干员节点 被调
int modify_officer(Node* head);	//修改干员 主调函数
void modify_node(Node* head, int id);	//修改干员 被调
/*配合搜索查询功能*/
void display_all(Node* head);	//显示所有干员
void display_node(Node* node);	//显示单独干员详细信息
void display_node_brief(Node* node);	//显示泛搜索结果列表
int search_and_display(Node* head);	//综合搜索功能 主调函数
Node* find_node_by_id(Node* head, int id);		//按编号查找干员节点
int find_node_id_by_name(Node* head, const char* name);		//按姓名查找干员编号
void find_nodes_by_career(Node* head, const char* career, Node** results, int* count);	//按职业查找所有符合干员节点
void find_nodes_by_branch(Node* head, const char* branch, Node** results, int* count);	//按分支查找所有符合干员节点
void find_nodes_by_background(Node* head, const char* background, Node** results, int* count);	//按出身查找所有符合干员节点
void find_nodes_by_organization(Node* head, const char* organization, Node** results, int* count);	//按组织查找所有符合干员节点
void find_nodes_by_tag(Node* head, const char* tag, Node** results, int* count);	//按标签查找所有符合干员节点
/*数据文件处理*/
void read_csv(Node** head, const char* filename);	//读取CSV文件数据
void save_csv(Node* head, const char* filename);	//保存数据到CSV文件
void free_listnode(Node* head);		//释放链表节点


/*进入下一个页面前清屏，具备跨平台执行*/
void clear_screen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

/*实现> 按任意键继续...，具备跨平台执行*/
void system_pause() {
#ifdef _WIN32
	system("pause");  // Windows 系统使用 pause
#else
	// Linux/macOS 使用 read 命令
	system("read -p \"按任意键继续...\" -n1");
#endif
}

/*询问是否继续当前操作的页面*/
void continue_check() {
	printf("\n\t\t\t\t\t====*=**$**$$**$**=*====\n");
	printf("\t\t\t\t\t|======继续操作？======|\n");
	printf("\t\t\t\t\t||  _______  _______  ||\n");
	printf("\t\t\t\t\t|| | 是(1) || 否(2) | ||\n");
	printf("\t\t\t\t\t||  ```````  ```````  ||\n");
	printf("\t\t\t\t\t|=======*==**==*=======|\n");
	printf("\t\t\t\t\t#======================#\n");
	printf("\n\t\t\t\t$*> 请选择...");
}

/*无效操作提示*/
void none_show() {
	printf("\n\t\t\t\t\t    *===*$$*===*\n");
	printf("\t\t\t\t\t===$* 无效操作!*$===\n");
	printf("\t\t\t\t\t    *===*$$*===*\n");
}


/*全局固定常量*/
char filename[] = "Officers.csv";
char title_line[] = "编号,姓名,职业,分支,出身,组织,部署花费,标签,天赋";
char title[] = "编号\t姓名\t\t职业\t\t标签\n";
char userFile[] = "user.csv";
char user_title[] = "用户名,密码,身份";


int main() {
	int status_check;
	char user_name[MAX_USER_NAME] = "";
	UNode* user_head = NULL;
	read_user(&user_head, userFile);
	while (1) {
		clear_screen();
		char command = log_sign_welcome();

		if (command == '1') {
			status_check = log_in(user_head, user_name);
			if (status_check) {
				free_user(user_head);
				break;
			}
		}
		else if (command == '2') {
			sign_in(&user_head);
			save_user(user_head, userFile);
		}
		else if (command == '3') {
			clear_screen();
			save_user(user_head, userFile);
			free_user(user_head);
			see_you();
			return 0;
		}
		else {
			clear_screen();
			none_show();
			system_pause();
		}
	}

	Node* head = NULL;
	read_csv(&head, filename);
	printf("\n$*> 初始数据加载成功...\n");
	system_pause();

	char mgrCommand;
	if (status_check == 1) {
		while (mgrCommand = admin_welcome()){
			switch (mgrCommand)
			{
			case '1': {//新增干员
					clear_screen();
					while (1) {
						clear_screen();
						int check = add_officer(&head);
						if (check == 1) continue;
						else break;
					}
					break;
				} 
			case '2': {//删除干员
					clear_screen();
					while (1) {
						clear_screen();
						int check = delete_officer(&head);
						if (check == 1) continue;
						else break;
					}
					break;
				}
			case '3': {//修改干员信息
				clear_screen();
				while (1) {
					clear_screen();
					int check = modify_officer(head);
					if (check == 1) continue;
					else break;
				}
				break;
			}
			case '4': { // 显示所有干员
					clear_screen();
					display_all(head);
					system_pause();
					break;
				}
			case '5': { // 查询干员信息
					clear_screen();
					while (1) {
						clear_screen();
						int check = search_and_display(head);
						if (check == 1) continue;
						else break;
					}
					break;
				}
			case '6': { // 保存并退出
				clear_screen();
				save_csv(head, filename);
				free_listnode(head);
				see_you();
				return 0;
			}
			default: {
				clear_screen();
				none_show();
				system_pause();
				break;
			}
			}
		}
	}
	else if (status_check == 2) {
		while (mgrCommand = user_welcome()) {
			switch (mgrCommand) {
			case '1': {//显示所有干员信息
				clear_screen();
				display_all(head);
				system_pause();
				break;
			}
			case '2': {//按需求查找干员信息
				clear_screen();
				while (1) {
					clear_screen();
					int check = search_and_display(head);
					if (check == 1) continue;
					else break;
				}
				break;
			}
			case '3': {//修改密码
				clear_screen();
				modify_password(user_name);
				break;
			}
			case '4': {//注销账户
				clear_screen();
				int check = delete_user(user_name);
				if (check == 1) {
					save_csv(head, filename);
					free_listnode(head);
					see_you();
					return 0;
				}
				else {
					break;
				}
			}
			case '5': {
				clear_screen();
				save_csv(head, filename);
				free_listnode(head);
				see_you();
				return 0;
			}
			default: {
				clear_screen();
				none_show();
				system_pause();
				break;
			}
			}
		}
	}

	return 0;
}


/*读取键入的用户名或密码*/
char* get_name_password(int flag) {
	//@flag 1代表用“*”显示密码 2代表明文显示密码 3代表显示用户名

	if (flag == 1 || flag == 2) {
		char password[MAX_PASSWORD];
		int i = 0;
		char ch;

		while (1) {
			ch = _getch();

			if (ch == 13) {
				break;
			}
			else if (ch == 8) {
				if (i > 0) {
					printf("\b \b");
					i--;
				}
			}
			else if (ch != ' ' && i < MAX_PASSWORD - 1) {
				if (flag == 1) {
					printf("*");
					password[i] = ch;
					i++;
				}
				else if (flag == 2) {
					printf("%c", ch);
					password[i] = ch;
					i++;
				}
			}
		}
		password[i] = '\0';
		return password;
	}
	else if (flag == 3) {
		char name[MAX_USER_NAME];
		int i = 0;
		char ch;

		while (1) {
			ch = _getch();

			if (ch == 13) {
				break;
			}
			else if (ch == 8) {
				if (i > 0) {
					printf("\b \b");
					i--;
				}
			}
			else if (ch != ' ' && i < MAX_USER_NAME - 1) {
				printf("%c", ch);
				name[i] = ch;
				i++;
			}
		}
		name[i] = '\0';
		return name;
	}
}

/*创建用户节点*/
UNode* create_user_node(User data) {
	UNode* new_user = (UNode*)malloc(sizeof(UNode));
	if (new_user) {
		new_user->value = data;
		new_user->next = NULL;
	}
	return new_user;
}

/*新增用户节点*/
void add_user(UNode** head, User data, int check) {
	UNode* new_node = create_user_node(data);
	if (*head == NULL) {
		*head = new_node;
		return;
	}
	if (check == 1) {//读取用
		UNode* current = *head;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = new_node;
	}
	else if (check == 2) {//注册用
		new_node->next = *head;
		*head = new_node;
	}
}

/*查找用户节点*/
UNode* find_user(UNode* head, const char* user_name) {
	UNode* current = head;
	while (current != NULL) {
		if (strcmp(current->value.user_name, user_name) == 0) {
			return current;
		}
		current = current->next;
	}
	return current;
}

/*登录身份确认*/
int identity(UNode* head, const char* user_name, const char* password, char* user_name_out) {
	UNode* current = head;
	int check = 0;
	while (current != NULL) {
		if (strcmp(current->value.user_name, user_name) == 0 && strcmp(current->value.password, password) == 0) {
			strncpy(user_name_out, user_name, MAX_USER_NAME);
			if (strcmp(current->value.status, "admin") == 0) {
				printf("\n\n\t\t\t\t\t   *      *\n");
				printf("\t\t\t\t\t<$*登录成功*$>\n");
				printf("\t\t\t\t\t   *      *\n\n");
				printf("\t\t\t\t  <$*>尊敬的管理员，你好<*$>\n\n");
				system_pause();
				check = 1;
				return 1;
			}
			else if (strcmp(current->value.status, "user") == 0) {
				printf("\n\n\t\t\t\t\t   *      *\n");
				printf("\t\t\t\t\t<$*登录成功*$>\n");
				printf("\t\t\t\t\t   *      *\n\n");
				printf("\t\t\t\t  <$*>尊敬的用户，你好<*$>\n\n");
				system_pause();
				check = 1;
				return 2;
			}
		}
		current = current->next;
	}
	if (!check) {
		printf("\n\n\t\t\t\t\t   *      *\n");
		printf("\t\t\t\t\t<#*登录失败*#>\n");
		printf("\t\t\t\t\t   *      *\n\n");
		printf("\t\t\t\t   ]#*>用户名或密码错误<*#[\n\n");
		system_pause();
		return 0;
	}
}

/*登录主页面*/
char log_sign_welcome() {
	clear_screen();
	printf("\n\t\t\t\t====*==*$*欢 迎*$*==*====\n");
	printf("\t\t\t\t|===*==**=*$*$*=**==*===|\n");
	printf("\t\t\t\t||      _________      ||\n");
	printf("\t\t\t\t|$*    | 登录(1) |    *$|\n");
	printf("\t\t\t\t||      `````````      ||\n");
	printf("\t\t\t\t||      _________      ||\n");
	printf("\t\t\t\t|$*    | 注册(2) |    *$|\n");
	printf("\t\t\t\t||      `````````      ||\n");
	printf("\t\t\t\t||      _________      ||\n");
	printf("\t\t\t\t|$*    | 退出(3) |    *$|\n");
	printf("\t\t\t\t||      `````````      ||\n");
	printf("\t\t\t\t#===*==**=*$*$*=**==*===#\n");
	printf("\n$*> 选择你要执行的操作...");
	char command = getchar();
	char c;
	while ((c = getchar()) != '\n' && c != EOF);

	return command;
}

/*登录*/
int log_in(UNode* head, char* user_name) {
	clear_screen();
	char name_log[MAX_USER_NAME];
	char password_log[MAX_PASSWORD];

	printf("\n\t\t\t\t====*===**=*$>登录<$*=**===*====\n");

	//char c;
	//while ((c = getchar()) != '\n' && c != EOF);

	printf("\n\n\t\t\t\t$* 用户名: ");
	strncpy(name_log, get_name_password(3), MAX_USER_NAME - 1);
	printf("\n\n\t\t\t\t$* 密码: ");
	strncpy(password_log, get_name_password(1), MAX_PASSWORD - 1);

	return identity(head, name_log, password_log, user_name);
}

/*注册*/
void sign_in(UNode** head) {
	clear_screen();
	User new_user;
	memset(&new_user, 0, sizeof(User));  // 防乱码

	printf("\n\t\t\t\t====*===**=*$>注册<$*=**===*====\n");
	printf("\t\t\t$*$ 注意: 用户名应为 4 至 10 位纯英文字符; \n\t\t\t      密码应为 6 至 10 位数字或英文或标点字符。\n");
	//getchar(); // 清除缓冲

	char name[MAX_USER_NAME], password[MAX_PASSWORD], password_confirm[MAX_PASSWORD];

	printf("\n\n\t\t\t\t$* 请设置用户名: ");
	strncpy(name, get_name_password(3), MAX_USER_NAME - 1);
	printf("\n\n\t\t\t\t$* 请设置密码: ");
	strncpy(password, get_name_password(1), MAX_PASSWORD - 1);
	printf("\n\n\t\t\t\t$* 请确认密码: ");
	strncpy(password_confirm, get_name_password(1), MAX_PASSWORD - 1);

	if (strlen(name) >= 4 && strlen(password) >= 6) {
		if (find_user(*head, name)) {
			printf("\n\n\t\t\t\t#*用户名已存在，请更换*#\n");
			system_pause();
			return;
		}
		if (strcmp(password, password_confirm) == 0) {
			strncpy(new_user.user_name, name, MAX_USER_NAME - 1);
			new_user.user_name[MAX_USER_NAME - 1] = '\0';
			strncpy(new_user.password, password, MAX_PASSWORD - 1);
			new_user.password[MAX_PASSWORD - 1] = '\0';
			strncpy(new_user.status, "user", MAX_STATUS - 1);
			new_user.status[MAX_STATUS - 1] = '\0';

			add_user(head, new_user, 2);
			save_user(*head, userFile);
			printf("\n\n\t\t\t\t\t<$*注册成功*$>\n");
			system_pause();
		}
		else {
			clear_screen();
			printf("\n\n\t\t\t\t\t<$*注册失败*$>\n");
			printf("\n\t\t\t\t   |#*>密码不相同<*#|\n");
			system_pause();
		}
	}
	else {
		clear_screen();
		printf("\n\n\t\t\t\t\t<$*注册失败*$>\n");
		printf("\n\t\t\t\t|#*>用户名或密码过短<*#|\n");
		system_pause();
	}
}

/*修改密码*/
void modify_password(const char* user_name) {
	clear_screen();
	UNode* head = NULL;
	read_user(&head, userFile);

	char password_initial[MAX_USER_NAME];
	char password_modified[MAX_PASSWORD];
	char password_modified_confirm[MAX_PASSWORD];

	UNode* ud = find_user(head, user_name);
	if (ud) {
		printf("\n\t\t\t\t====*===**=*$修改密码$*=**===*====\n");
		printf("\t\t\t$*$ 注意: 密码应为 6 至 10 位数字或英文或标点字符。\n");
		//getchar();

		printf("\n\t\t\t\t$* 请输入原密码: ");
		strncpy(password_initial, get_name_password(2), MAX_PASSWORD - 1);
		printf("\n\n\t\t\t\t$* 请输入新密码: ");
		strncpy(password_modified, get_name_password(1), MAX_PASSWORD - 1);
		printf("\n\n\t\t\t\t$* 请确认新密码: ");
		strncpy(password_modified_confirm, get_name_password(1), MAX_PASSWORD - 1);

		if (strcmp(ud->value.password, password_initial) == 0) {
			if (strlen(password_modified) >= 6 && strcmp(password_modified, password_modified_confirm) == 0) {
				strncpy(ud->value.password, password_modified, MAX_PASSWORD - 1);
				save_user(head, userFile);
				printf("\n\n\t\t\t\t\t   *      *\n");
				printf("\t\t\t\t\t<$*修改成功*$>\n");
				printf("\t\t\t\t\t   *      *\n\n");
				system_pause();
				return;
			}
			else {
				printf("\n\n\t\t\t\t\t   *      *\n");
				printf("\t\t\t\t\t<$*修改失败*$>\n");
				printf("\t\t\t\t\t   *      *\n");
				printf("\n\t\t\t\t|#*>新密码不符合要求或密码不相同<*#|\n");
				system_pause();
				return;
			}
		}
		else {
			clear_screen();
			printf("\n\n\t\t\t\t\t   *      *\n");
			printf("\t\t\t\t\t<$*修改失败*$>\n");
			printf("\t\t\t\t\t   *      *\n");
			printf("\n\t\t\t\t      |#*>原密码错误<*#|\n");
			system_pause();
			return;
		}
	}
	else {
		clear_screen();
		printf("\n\n\t\t\t\t\t  *          *\n");
		printf("\t\t\t\t\t#*>用户不存在<*#");
		printf("\t\t\t\t\t  *          *\n");
		system_pause();
		return;
	}
}

/*删除节点*/
int delete_user(const char* user_name) {
	clear_screen();

	printf("\n\t\t\t\t#*===*==**=*$*注销*$*=**==*===*#\n");
	printf("\t\t\t\t  |=#$=*>确定要这么做吗<*=$#=|\n");
	printf("\t\t\t\t  ||       __________       ||\n");
	printf("\t\t\t\t  |#*     |  确定(1) |     *#|\n");
	printf("\t\t\t\t  ||       ``````````       ||\n");
	printf("\t\t\t\t  ||       __________       ||\n");
	printf("\t\t\t\t  |#*     |  取消(2) |     *#|\n");
	printf("\t\t\t\t  ||       ``````````       ||\n");
	printf("\t\t\t\t  ===*==**=*$*#**#*$*=**==*===\n");
	printf("\n$*> 请选择操作...");
	char checkCommand = getchar();
	char c;
	while ((c = getchar()) != '\n' && c != EOF);

	if (checkCommand == '1') {
		UNode* head = NULL;
		read_user(&head, userFile);

		UNode* ud = find_user(head, user_name);

		char password[MAX_PASSWORD];
		printf("\n\t\t\t\t$*> 请输入账户密码: ");
		strncpy(password, get_name_password(2), MAX_PASSWORD - 1);

		if (ud) {
			if (strcmp(password, ud->value.password) == 0) {
				if (ud == head) {
					head = head->next;
				}
				else {
					ud->value = ud->next->value;
					ud->next = ud->next->next;
				}

				save_user(head, userFile);
				free_user(head);

				clear_screen();
				printf("\n\n\t\t\t\t\t   *      *\n");
				printf("\t\t\t\t\t<$*注销成功*$>\n");
				printf("\t\t\t\t\t   *      *\n");
				printf("\n$*> 将自动退出程序...\n");
				system_pause();
				return 1;
			}
			else {
				clear_screen();
				printf("\n\n\t\t\t\t\t  *        *\n");
				printf("\t\t\t\t\t#*>密码错误<*#");
				printf("\t\t\t\t\t  *        *\n");
				printf("\n#*> 将自动取消操作并返回主页...\n");
				system_pause();
				return 0;
			}
		}
		else {
			clear_screen();
			printf("\n\n\t\t\t\t\t  *          *\n");
			printf("\t\t\t\t\t#*>用户不存在<*#");
			printf("\t\t\t\t\t  *          *\n");
			system_pause();
			return 0;
		}
	}
	else if (checkCommand == '2') {
		printf("\n$*> 操作已取消...\n");
		system_pause();
		return 0;
	}
	else {
		clear_screen();
		none_show();
		printf("\n#*> 将自动取消操作并返回主页...\n");
		system_pause();
		return 0;
	}

}

/*读取用户数据*/
void read_user(UNode** head, const char* userFile) {
	FILE* file = fopen(userFile, "r");
	if (!file) {
		perror("!#*> 打开文件失败...");
		return;
	}

	char line[1024];
	fgets(line, sizeof(line), file);	//跳过标题行

	while (fgets(line, sizeof(line), file)) {
		User u = { 0 };
		char* tokens[3];
		int token_count = 0;

		// 分割行
		char* token = strtok(line, ",");
		while (token != NULL && token_count < 3) {
			tokens[token_count++] = token;
			token = strtok(NULL, ",\n");
		}

		if (token_count < 2) continue;  // 跳过无效行

		// 解析字段
		strncpy(u.user_name, tokens[0], MAX_USER_NAME - 1);
		u.user_name[MAX_USER_NAME - 1] = '\0';
		strncpy(u.password, tokens[1], MAX_PASSWORD - 1);
		u.password[MAX_PASSWORD - 1] = '\0';

		// 处理天赋字段（可能包含逗号）
		if (token_count > 2) {
			strncpy(u.status, tokens[2], MAX_STATUS - 1);
			u.status[MAX_STATUS - 1] = '\0';
		}
		else {
			u.status[0] = '\0';
		}

		add_user(head, u, 1);
	}
	fclose(file);
}

/*保存用户数据*/
void save_user(UNode* head, const char* userFile) {
	FILE* file = fopen(userFile, "w");
	if (!file) {
		perror("\n!#*> 保存文件失败...\n");
		return;
	}

	fprintf(file, "%s\n", user_title);
	UNode* current = head;
	while (current != NULL) {
		User u = current->value;
		fprintf(file, "%s,%s,%s\n",
			u.user_name, u.password, u.status);
		current = current->next;
	}
	fclose(file);
	printf("\n$*> 用户数据已保存...\n");
}

/*释放用户链表*/
void free_user(UNode* head) {
	UNode* current = head;
	while (current != NULL) {
		UNode* next = current->next;
		free(current);
		current = next;
	}
}


/*管理员主菜单*/
char admin_welcome() {
	clear_screen();
	printf("\n\n\t\t\t=========****=======欢迎使用=======****=========\n");
	printf("\t\t\t|=========《明日方舟》六星干员管理系统=========|\n");
	printf("\t\t\t||*              1.增加干员信息              *||\n");
	printf("\t\t\t||*              2.删除干员信息              *||\n");
	printf("\t\t\t||*              3.修改干员信息              *||\n");
	printf("\t\t\t||*              4.显示所有干员              *||\n");
	printf("\t\t\t||*              5.查询干员信息              *||\n");
	printf("\t\t\t||*              6.保存并退出                *||\n");
	printf("\t\t\t|===**=====****=====********=====****=====**===|\n");
	printf("\t\t\t========$=======###====$$====###=======$========\n");

	printf("\n$*> 选择你要执行的操作...");
	char command = getchar();
	char c;
	while ((c = getchar()) != '\n' && c != EOF);

	return command;
}

/*用户主菜单*/
char user_welcome() {
	clear_screen();
	printf("\n\n\t\t\t=========****=======欢迎使用=======****=========\n");
	printf("\t\t\t|=========《明日方舟》六星干员查询系统=========|\n");
	printf("\t\t\t||*              1.显示所有干员              *||\n");
	printf("\t\t\t||*              2.查询干员信息              *||\n");
	printf("\t\t\t||*              3.修改账户密码              *||\n");
	printf("\t\t\t||*              4.注销账户                  *||\n");
	printf("\t\t\t||*              5.保存并退出                *||\n");
	printf("\t\t\t|===**=====****=====********=====****=====**===|\n");
	printf("\t\t\t========$=======###====$$====###=======$========\n");

	printf("\n$*> 选择你要执行的操作...");
	char command = getchar();
	char c;
	while ((c = getchar()) != '\n' && c != EOF);

	return command;
}

/*创建新节点*/
Node* create_node(Staff data) {
	Node* new_node = (Node*)malloc(sizeof(Node));
	if (new_node) {
		new_node->value = data;
		new_node->next = NULL;
	}
	return new_node;
}

/*查询尾节点的下一个节点编号*/
int last_node_id(Node** head) {
	if (*head == NULL || head == NULL) {
		return 1;
	}
	Node* current = *head;
	while (current->next != NULL) {
		current = current->next;
	}
	return current->value.id + 1;
}

/*根据姓名查找编号*/
int find_node_id_by_name(Node* head, const char* name) {
	Node* current = head;
	while (current != NULL) {
		if (strcmp(current->value.name, name) == 0) {
			return current->value.id;
		}
		current = current->next;
	}
	return -1;
}

/*根据编号查找干员节点，根查询方法*/
Node* find_node_by_id(Node* head, int id) {
	Node* current = head;
	while (current != NULL) {
		if (current->value.id == id) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}

/*新增干员 主调函数*/
int add_officer(Node** head) {
	Staff new_staff;
	//getchar();

	printf("\n\t\t\t=========*$$*=======新增干员=======*$$*=========\n");

	new_staff.id = last_node_id(head);

	printf("\t\t\t\t$* 姓名: ");
	fgets(new_staff.name, MAX_NAME, stdin);
	new_staff.name[strcspn(new_staff.name, "\n")] = '\0';

	if (find_node_id_by_name(*head, new_staff.name) != -1) {
		clear_screen();
		printf("\n\t\t\t\t\t==*=**$**$$**$**=*==\n");
		printf("\t\t\t\t\t#   该干员已存在   #\n");
		printf("\t\t\t\t\t==*=**$**$$**$**=*==\n");
		system_pause();
	}
	else {
		printf("\t\t\t\t$* 职业: ");
		fgets(new_staff.career, MAX_CAREER, stdin);
		new_staff.career[strcspn(new_staff.career, "\n")] = '\0';

		printf("\t\t\t\t$* 分支: ");
		fgets(new_staff.branch, MAX_BRANCH, stdin);
		new_staff.branch[strcspn(new_staff.branch, "\n")] = '\0';

		printf("\t\t\t\t$* 出身: ");
		fgets(new_staff.background, MAX_BACKGROUND, stdin);
		new_staff.background[strcspn(new_staff.background, "\n")] = '\0';

		printf("\t\t\t\t$* 组织: ");
		fgets(new_staff.organization, MAX_ORGANIZATION, stdin);
		new_staff.organization[strcspn(new_staff.organization, "\n")] = '\0';

		printf("\t\t\t\t$* 费用: ");
		scanf(" %d", &new_staff.cost);
		getchar();

		printf("\t\t\t\t$* 标签: ");
		fgets(new_staff.tag, MAX_TAG, stdin);
		new_staff.tag[strcspn(new_staff.tag, "\n")] = '\0';

		printf("\t\t\t\t$* 天赋: ");
		fgets(new_staff.talent, MAX_TALENT, stdin);
		new_staff.talent[strcspn(new_staff.talent, "\n")] = '\0';

		add_node(head, new_staff);
		save_csv(*head, filename);
		clear_screen();
		printf("\n\t\t\t\t\t ==*=**$**$$**$**=*==\n");
		printf("\t\t\t\t\t$>   新增干员成功！ <$\n");
		printf("\t\t\t\t\t ==*=**$**$$**$**=*==\n");
		system_pause();
	}

	clear_screen();
	continue_check();
	char check = getchar();
	char c;
	while ((c = getchar()) != '\n' && c != EOF);

	if (check == '1') {
		return 1;
	}
	else if (check == '2') {
		return 0;
	}
	else {
		clear_screen();
		none_show();
		printf("\n$*> 将自动返回主菜单...\n");
		system_pause();
		return 0;
	}
}

/*新增节点*/
void add_node(Node** head, Staff data) {
	Node* new_node = create_node(data);
	if (*head == NULL) {
		*head = new_node;
		return;
	}

	Node* current = *head;
	while (current->next != NULL) {
		current = current->next;
	}
	current->next = new_node;
}

/*删除干员 主调函数*/
int delete_officer(Node** head) {
	printf("\n\t\t\t=========*$$*=======删除干员=======*$$*=========\n");
	printf("\t\t\t#*              1.按编号删除                  *#\n");
	printf("\t\t\t#*              2.按姓名删除                  *#\n");
	printf("\t\t\t#*              3.取消并返回主菜单            *#\n");
	printf("\t\t\t==========*$$*=====*$*=**=*$*=====*$$*==========\n");
	printf("$*> 请选择删除方式...");
	char check = getchar();
	char c;
	while ((c = getchar()) != '\n' && c != EOF);

	if (check == '1') {
		clear_screen();
		printf("\n\t\t\t=========*$$*=======删除干员=======*$$*=========\n");
		int id_to_delete;
		printf("\t\t\t\t$*> 输入要删除的干员的编号...");
		scanf("%d", &id_to_delete);
		delete_node_by_id(head, id_to_delete);
	}
	else if (check == '2') {
		clear_screen();
		printf("\n\t\t\t=========*$$*=======删除干员=======*$$*=========\n");
		char name_to_delete[MAX_NAME];
		printf("\t\t\t\t$*> 输入要删除的干员的姓名...");
		//getchar();
		fgets(name_to_delete, MAX_NAME, stdin);
		name_to_delete[strcspn(name_to_delete, "\n")] = '\0';
		if (strlen(name_to_delete) > 0) {
			delete_node_by_name(head, name_to_delete);
		}
		else {
			clear_screen();
			printf("\n\t\t\t\t\t  *===*$#$*===*\n");
			printf("\t\t\t\t\t$*姓名不能为空!*$\n");
			printf("\t\t\t\t\t  *===*$#$*===*\n");
			system_pause();
		}
	}
	else if (check == '3') {
		printf("\n$*> 已取消...\n");
		system_pause();
		return 0;
	}
	else {
		clear_screen();
		none_show();
		system_pause();
		return 1;
	}

	clear_screen();
	continue_check();
	check = getchar();
	while ((c = getchar()) != '\n' && c != EOF);

	if (check == '1') {
		return 1;
	}
	else if (check == '2') {
		return 0;
	}
	else {
		clear_screen();
		none_show();
		printf("\n$*> 将自动返回主菜单...\n");
		system_pause();
		return 0;
	}
}

/*按编号删除干员，根删除方法，含二次确认*/
void delete_node_by_id(Node** head, int id) {
	if (*head == NULL) {
		clear_screen();
		printf("\n\t\t\t\t\t =*=**$**$$**$**=*=\n");
		printf("\t\t\t\t\t#*  干员列表为空  *#\n");
		printf("\t\t\t\t\t =*=**$**$$**$**=*=\n");
		system_pause();
		return;
	}

	Node* temp = *head;
	Node* previous = NULL;

	while (temp != NULL && temp->value.id != id) {
		previous = temp;
		temp = temp->next;
	}

	if (temp == NULL) {
		clear_screen();
		printf("\n\t\t\t\t\t =*=**$**$$**$**=*=\n");
		printf("\t\t\t\t\t#*  未找到该干员  *#\n");
		printf("\t\t\t\t\t =*=**$**$$**$**=*=\n");
		system_pause();
		return;
	}

	printf("\n\t\t\t================================================\n");
	printf("\t\t\t%s\n", title);
	printf("\t\t\t````````````````````````````````````````````````\n");
	display_node_brief(temp);
	printf("\n\t\t\t\t$*> 确认删除该干员吗（y/n）...");
	char confirm = getchar();
	char c;
	while ((c = getchar()) != '\n' && c != EOF);

	if (confirm == 'y' || confirm == 'Y') {
		if (previous == NULL) {
			*head = temp->next;

			Node* cur = *head;
			while (cur != NULL) {
				cur->value.id--;
				cur = cur->next;
			}
		}
		else {
			previous->next = temp->next;

			Node* cur = previous->next;
			while (cur != NULL) {
				cur->value.id--;
				cur = cur->next;
			}
		}

		free(temp);
		temp = NULL;
		save_csv(*head, filename);
		clear_screen();
		printf("\n\t\t\t\t\t =*=**$**$$**$**=*=\n");
		printf("\t\t\t\t\t$>  干员删除成功！<$\n");
		printf("\t\t\t\t\t =*=**$**$$**$**=*=\n");
		system_pause();
	}
	else if (confirm == 'n' || confirm == 'N') {
		printf("\n$*> 操作已取消...\n");
		system_pause();
	}
	else {
		clear_screen();
		none_show();
		system_pause();
	}
}

/*按姓名删除干员，使用根删除方法，本方法将传入编号给根方法*/
void delete_node_by_name(Node** head, const char* name) {
	if (name == NULL || strlen(name) == 0) {
		clear_screen();
		printf("\n\t\t\t\t\t =*=**$**$$**$**=*=\n");
		printf("\t\t\t\t\t#* 无效的干员姓名 *#\n");
		printf("\t\t\t\t\t =*=**$**$$**$**=*=\n");
		system_pause();
		return;
	}

	int id = find_node_id_by_name(*head, name);
	if (id == -1) {
		clear_screen();
		printf("\n\t\t\t\t\t|#*>未找到名为 \"%s\" 的干员<*#|\n", name);
		system_pause();
		return;
	}
	delete_node_by_id(head, id);
}

/*修改干员 主调函数*/
int modify_officer(Node* head) {
	char name_to_modify[MAX_NAME];
	printf("\n\t\t\t=========*$$*=======修改干员=======*$$*=========\n");
	printf("\n\t\t\t\t$*> 输入姓名以修改...");
	//getchar();
	fgets(name_to_modify, MAX_NAME, stdin);
	name_to_modify[strcspn(name_to_modify, "\n")] = '\0';
	int id = find_node_id_by_name(head, name_to_modify);
	if (id != -1) {
		modify_node(head, id);
	}
	else {
		clear_screen();
		printf("\n\t\t\t\t\t==*=**$**$$**$**=*==\n");
		printf("\t\t\t\t\t#*  未找到该干员  *#\n");
		printf("\t\t\t\t\t==*=**$**$$**$**=*==\n");
		system_pause();
		return 1;
	}

	clear_screen();
	continue_check();

	char check = getchar();
	char c;
	while ((c = getchar()) != '\n' && c != EOF);

	if (check == '1') {
		return 1;
	}
	else if (check == '2') {
		return 0;
	}
	else {
		clear_screen();
		none_show();
		printf("\n$*> 将自动返回主菜单...\n");
		system_pause();
		return 0;
	}
}

/*修改干员信息*/
void modify_node(Node* head, int id) {
	Node* node = find_node_by_id(head, id);

	if (node == NULL) {
		clear_screen();
		printf("\n\t\t\t\t\t|#*>未找到编号为 %3d 的干员...<*#|\n", id);
		system_pause();
		return;
	}

	Staff* s = &node->value;
	clear_screen();
	printf("\n\t\t\t=========*$$*=======修改干员=======*$$*=========\n");
	printf("\t\t\t =*$$*===修改干员信息(当前值按回车保留)===*$$*= \n");

	printf("\t\t\t\t$* 姓名[%s]: ", s->name);
	char input[201];
	if (fgets(input, MAX_NAME, stdin) && input[0] != '\n') {
		input[strcspn(input, "\n")] = '\0';
		strncpy(s->name, input, MAX_NAME - 1);
		s->name[MAX_NAME - 1] = '\0';
	}
	printf("\t\t\t\t$* 职业[%s]: ", s->career);
	if (fgets(input, MAX_CAREER, stdin) && input[0] != '\n') {
		input[strcspn(input, "\n")] = '\0';
		strncpy(s->career, input, MAX_CAREER - 1);
		s->career[MAX_CAREER - 1] = '\0';
	}
	printf("\t\t\t\t$* 分支[%s]: ", s->branch);
	if (fgets(input, MAX_BRANCH, stdin) && input[0] != '\n') {
		input[strcspn(input, "\n")] = '\0';
		strncpy(s->branch, input, MAX_BRANCH - 1);
		s->branch[MAX_BRANCH - 1] = '\0';
	}
	printf("\t\t\t\t$* 出身[%s]: ", s->background);
	if (fgets(input, MAX_BACKGROUND, stdin) && input[0] != '\n') {
		input[strcspn(input, "\n")] = '\0';
		strncpy(s->background, input, MAX_BACKGROUND - 1);
		s->background[MAX_BACKGROUND - 1] = '\0';
	}
	printf("\t\t\t\t$* 组织[%s]: ", s->organization);
	if (fgets(input, MAX_ORGANIZATION, stdin) && input[0] != '\n') {
		input[strcspn(input, "\n")] = '\0';
		strncpy(s->organization, input, MAX_ORGANIZATION - 1);
		s->organization[MAX_ORGANIZATION - 1] = '\0';
	}
	printf("\t\t\t\t$* 部署花费[%d]: ", s->cost);
	if (fgets(input, 10, stdin) && input[0] != '\n') {
		s->cost = atoi(input);
	}
	printf("\t\t\t\t$* 原标签: %s\n", s->tag);
	printf("\t\t\t\t  #* 修改: ");
	if (fgets(input, MAX_TAG, stdin) && input[0] != '\n') {
		input[strcspn(input, "\n")] = '\0';
		strncpy(s->tag, input, MAX_TAG - 1);
		s->tag[MAX_TAG - 1] = '\0';
	}
	printf("\t\t\t\t$* 原天赋: %s\n", s->talent);
	printf("\t\t\t\t  #* 修改: ");
	if (fgets(input, MAX_TALENT, stdin) && input[0] != '\n') {
		input[strcspn(input, "\n")] = '\0';
		strncpy(s->talent, input, MAX_TALENT - 1);
		s->talent[MAX_TALENT - 1] = '\0';
	}
	save_csv(head, filename);
	clear_screen();
	printf("\n\t\t\t\t\t ===*=**$**$$**$**=*===\n");
	printf("\t\t\t\t\t$>  干员信息修改成功！<$\n");
	printf("\t\t\t\t\t ===*=**$**$$**$**=*===\n");
	system_pause();
}

/* 显示所有干员 */
void display_all(Node* head) {
	printf("\n\t\t\t=========*$$*=======所有干员=======*$$*=========\n");
	printf("\t\t\t%s", title);
	printf("\t\t\t````````````````````````````````````````````````\n");

	Node* current = head;
	while (current != NULL) {
		Staff s = current->value;
		printf("\t\t\t[%3d]\t%-15s\t%-15s\t%-50s\n",
			s.id, s.name, s.career, s.tag);
		printf("\t\t\t------------------------------------------------\n");
		current = current->next;
	}
	printf("\t\t\t===**=====****====共 %3d 位干员====****=====**===\n", last_node_id(&head) - 1);
	printf("\t\t\t=========$=======###====$$====###=======$=========\n");

}

/* 显示单独干员详细信息 */
void display_node(Node* node) {
	if (node == NULL) {
		clear_screen();
		printf("\n\t\t\t\t\t =*=**$**$$**$**=*=\n");
		printf("\t\t\t\t\t#*   干员不存在   *#\n");
		printf("\t\t\t\t\t =*=**$**$$**$**=*=\n");
		system_pause();
		return;
	}

	clear_screen();
	Staff s = node->value;
	printf("\n\t\t\t=========*$$*=======干员详情=======*$$*=========\n");
	printf("\t\t\t\t$*> 编号: %d\n", s.id);
	printf("\t\t\t\t$*> 姓名: %s\n", s.name);
	printf("\t\t\t\t$*> 职业: %s\n", s.career);
	printf("\t\t\t\t$*> 分支: %s\n", s.branch);
	printf("\t\t\t\t$*> 出身: %s\n", s.background);
	printf("\t\t\t\t$*> 组织: %s\n", s.organization);
	printf("\t\t\t\t$*> 部署花费: %d\n", s.cost);
	printf("\t\t\t\t$*> 标签: %s\n", s.tag);
	printf("\t\t\t\t$*> 天赋: %s\n", s.talent);
	printf("\t\t\t=========$=======###====$$====###=======$=========\n");
	system_pause();
}

/*展示部分泛查询干员*/
void display_node_brief(Node* node) {
	Staff s = node->value;
	printf("\t\t\t[%3d]\t%-15s\t%-15s\t%-50s\n",
		s.id, s.name, s.career, s.tag);
	printf("\t\t\t------------------------------------------------\n");
}

/* 综合搜索 主调函数 */
int search_and_display(Node* head) {
	printf("\n\t\t\t=========*$$*=======搜索方式=======*$$*=========\n");
	printf("\t\t\t#*               1.按*编号*查询               *#\n");
	printf("\t\t\t#*               2.按*姓名*查询               *#\n");
	printf("\t\t\t#*               3.按*职业*查询               *#\n");
	printf("\t\t\t#*               4.按*分支*查询               *#\n");
	printf("\t\t\t#*               5.按*出身*查询               *#\n");
	printf("\t\t\t#*               6.按*组织*查询               *#\n");
	printf("\t\t\t#*               7.按*标签*查询               *#\n");
	printf("\t\t\t#*               8.返回主菜单                 *#\n");
	printf("\t\t\t==========*$$*=====*$*=**=*$*=====*$$*==========\n");
	printf("\n\t\t\t\t$*> 请选择搜索方式: ");

	char choice = getchar(); // 消耗换行符
	char c;
	while ((c = getchar()) != '\n' && c != EOF);

	if (choice == '8') return 0;

	Node* results[100]; // 结果数组
	int count = 0;
	char query[100];

	switch (choice) {
	case '1': {
		//clear_screen();
		// printf("\n\t\t\t=========*$$*=======$*搜索*$=======*$$*=========\n");
		printf("\n\t\t\t\t$*> 输入干员编号: ");
		int id;
		scanf(" %d", &id);
		getchar();
		Node* result = find_node_by_id(head, id);
		if (result) {
			display_node(result);
		}
		else {
			clear_screen();
			printf("\n\n\t\t\t\t\t =*=**$**$$**$**=*=\n");
			printf("\t\t\t\t\t#*  未找到该干员  *#\n");
			printf("\t\t\t\t\t =*=**$**$$**$**=*=\n");
			system_pause();
		}
		return 1;
	}
	case '2': {
		//clear_screen();
		//printf("\n\t\t\t=========*$$*=======$*搜索*$=======*$$*=========\n");
		printf("\n\t\t\t\t$*> 输入干员姓名: ");
		fgets(query, MAX_NAME, stdin);
		query[strcspn(query, "\n")] = '\0';
		int id = find_node_id_by_name(head, query);
		if (id != -1) {
			display_node(find_node_by_id(head, id));
		}
		else {
			clear_screen();
			printf("\n\n\t\t\t\t\t =*=**$**$$**$**=*=\n");
			printf("\t\t\t\t\t#*  未找到该干员  *#\n");
			printf("\t\t\t\t\t =*=**$**$$**$**=*=\n");
			system_pause();
		}
		return 1;
	}
	case '3': {
		printf("\n\t\t\t\t$*> 输入职业: ");
		fgets(query, MAX_CAREER, stdin);
		query[strcspn(query, "\n")] = '\0';
		find_nodes_by_career(head, query, results, &count);
		break;
	}
	case '4': {
		printf("\n\t\t\t\t$*> 输入分支: ");
		fgets(query, MAX_BRANCH, stdin);
		query[strcspn(query, "\n")] = '\0';
		find_nodes_by_branch(head, query, results, &count);
		break;
	}
	case '5': {
		printf("\n\t\t\t\t$*> 输入出身: ");
		fgets(query, MAX_BACKGROUND, stdin);
		query[strcspn(query, "\n")] = '\0';
		find_nodes_by_background(head, query, results, &count);
		break;
	}
	case '6': {
		printf("\n\t\t\t\t$*> 输入组织: ");
		fgets(query, MAX_ORGANIZATION, stdin);
		query[strcspn(query, "\n")] = '\0';
		find_nodes_by_organization(head, query, results, &count);
		break;
	}
	case '7': {
		printf("\n\t\t\t\t$*> 输入标签: ");
		fgets(query, MAX_TAG, stdin);
		query[strcspn(query, "\n")] = '\0';
		find_nodes_by_tag(head, query, results, &count);
		break;
	}
	default:
		clear_screen();
		none_show();
		system_pause();
		return 0;
	}

	// 显示多结果查询
	if (choice >= '3' && choice <= '7') {
		if (count == 0) {
			clear_screen();
			printf("\n\t\t\t\t\t ===*=**$**$$**$**=*===\n");
			printf("\t\t\t\t\t#*  未找到匹配的干员  *#\n");
			printf("\t\t\t\t\t ===*=**$**$$**$**=*===\n");
			system_pause();
			return 1;
		}
		clear_screen();
		printf("\n\t\t\t\t|$*>找到 %3d 位干员<*$|\n", count);
		printf("\t\t\t================================================\n");
		printf("\t\t\t%s", title);
		printf("\t\t\t````````````````````````````````````````````````\n");
		for (int i = 0; i < count; i++) {
			display_node_brief(results[i]); // 遍历显示所有结果
		}

		// 查看详情
		printf("\n\t\t\t\t$*> 输入编号查看详情(0返回主菜单): ");
		int id;
		scanf("%d", &id);
		getchar();
		if (id != 0) {
			Node* detail = find_node_by_id(head, id);
			if (detail) {
				display_node(detail);
				return 1;
			}
			else {
				clear_screen();
				printf("\n\t\t\t\t\t    *===*$$*===*\n");
				printf("\t\t\t\t\t===$* 编号无效!*$===\n");
				printf("\t\t\t\t\t    *===*$$*===*\n");
				system_pause();
				return 1;
			}
		}
		return 0;
	}
}

/* 按职业查找 */
void find_nodes_by_career(Node* head, const char* career, Node** results, int* count) {
	*count = 0;
	Node* current = head;
	while (current != NULL) {
		if (strcmp(current->value.career, career) == 0) {
			results[(*count)++] = current;
		}
		current = current->next;
	}
}

/* 按分支查找 */
void find_nodes_by_branch(Node* head, const char* branch, Node** results, int* count) {
	*count = 0;
	Node* current = head;
	while (current != NULL) {
		if (strcmp(current->value.branch, branch) == 0) {
			results[(*count)++] = current;
		}
		current = current->next;
	}
}

/* 按出身查找 */
void find_nodes_by_background(Node* head, const char* background, Node** results, int* count) {
	*count = 0;
	Node* current = head;
	while (current != NULL) {
		if (strcmp(current->value.background, background) == 0) {
			results[(*count)++] = current;
		}
		current = current->next;
	}
}

/* 按组织查找 */
void find_nodes_by_organization(Node* head, const char* organization, Node** results, int* count) {
	*count = 0;
	Node* current = head;
	while (current != NULL) {
		if (strstr(current->value.organization, organization) != NULL) {
			results[(*count)++] = current;
		}
		current = current->next;
	}
}

/* 按标签查找 */
void find_nodes_by_tag(Node* head, const char* tag, Node** results, int* count) {
	*count = 0;
	Node* current = head;
	while (current != NULL) {
		if (strstr(current->value.tag, tag) != NULL) {
			results[(*count)++] = current;
		}
		current = current->next;
	}
}

/*读取源数据*/
void read_csv(Node** head, const char* filename) {
	FILE* file = fopen(filename, "r");
	if (!file) {
		perror("!#*> 打开文件失败...");
		return;
	}

	char line[1024];
	fgets(line, sizeof(line), file);	//跳过标题行

	while (fgets(line, sizeof(line), file)) {
		Staff s = { 0 };
		char* tokens[9];
		int token_count = 0;

		// 分割行
		char* token = strtok(line, ",");
		while (token != NULL && token_count < 9) {
			tokens[token_count++] = token;
			token = strtok(NULL, ",\n");
		}

		if (token_count < 8) continue;  // 跳过无效行

		// 解析字段
		s.id = atoi(tokens[0]);
		strncpy(s.name, tokens[1], MAX_NAME - 1);
		s.name[MAX_NAME - 1] = '\0';
		strncpy(s.career, tokens[2], MAX_CAREER - 1);
		s.career[MAX_CAREER - 1] = '\0';
		strncpy(s.branch, tokens[3], MAX_BRANCH - 1);
		s.branch[MAX_BRANCH - 1] = '\0';
		strncpy(s.background, tokens[4], MAX_BACKGROUND - 1);
		s.background[MAX_BACKGROUND - 1] = '\0';
		strncpy(s.organization, tokens[5], MAX_ORGANIZATION - 1);
		s.organization[MAX_ORGANIZATION - 1] = '\0';
		s.cost = atoi(tokens[6]);
		strncpy(s.tag, tokens[7], MAX_TAG - 1);
		s.tag[MAX_TAG - 1] = '\0';

		// 处理天赋字段（可能包含逗号）
		if (token_count > 8) {
			strncpy(s.talent, tokens[8], MAX_TALENT - 1);
			s.talent[MAX_TALENT - 1] = '\0';
		}
		else {
			s.talent[0] = '\0';
		}

		add_node(head, s);
	}
	fclose(file);
}

/*保存数据*/
void save_csv(Node* head, const char* filename) {
	FILE* file = fopen(filename, "w");
	if (!file) {
		perror("!#*> 保存文件失败...\n");
		return;
	}

	fprintf(file, "%s\n", title_line);
	Node* current = head;
	while (current != NULL) {
		Staff s = current->value;
		fprintf(file, "%d,%s,%s,%s,%s,%s,%d,%s,%s\n",
			s.id, s.name, s.career, s.branch, s.background,
			s.organization, s.cost, s.tag, s.talent);
		current = current->next;
	}
	fclose(file);
	printf("$*> 数据已保存...\n");
}

/*退出页面*/
void see_you() {
	clear_screen();
	printf("\n\t\t\t\t+========<****>=======$**$=======<****>========+\n");
	printf("\t\t\t\t|$*>        感谢使用本系统，再见！          <*$|\n");
	printf("\t\t\t\t+========<****>=======$**$=======<****>========+\n");
	system_pause();
}

/* 释放链表节点 */
void free_listnode(Node* head) {
	Node* current = head;
	while (current != NULL) {
		Node* next = current->next;
		free(current);
		current = next;
	}
}