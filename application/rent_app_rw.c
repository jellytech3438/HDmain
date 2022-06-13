#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/datastruct.h"
#include "../include/function.h"

#define NAME_LEN 30

enum room_type {
  EMPTY_ROOM,
  FOR_ACCOMMODATE,
  FOR_OFFICE,
  FOR_WAREHOUSE
};

struct room{
    int floor;
    int room_num;
    enum room_type room_type;

    char exroomer_name[NAME_LEN];
    long long int exroomer_phone;

    char roomer_name[NAME_LEN];
    long long int roomer_phone;
    int rent;
    int violate;
    int report[5];
    int report_rule[5];

    struct room *next;
};

struct ll{
    int floor;
    struct ll *next;
    struct room *HEAD;
};

struct ll *create_building(int floor_num, int room_num);
struct room* search_room(int tar_number, int floor_num, int room_num, struct ll *HEAD_floor);
struct room* search_name(char *target, struct ll *cur_floor);
void print_type(struct room *target);
void print_info(struct room *target);
void print_violate(struct room *target);
void delete_info(struct room *target);
void modify_info(struct room *target, int floor_num, int room_num);
void change_type(struct room *target);
void change_name(struct room *target);
void change_phone(struct room *target);
void change_rent(struct room *target);
void violation_report(struct room *target, int floor_num, int room_num);
void punish(struct room *target);
void traverse_phone(struct ll *cur_floor);
void sort(struct ll *cur_floor, int floor_num,int room_num);
void print_floor(struct ll *cur_floor,int floor_num);
void convert_to_table(struct ll *cur_floor);

int main(){
    /************************************************************************************/
    FILE *fp_in=fopen("input.txt", "w");
    FILE *fp_out=fopen("output.txt", "r");
    char ch;
    if((fp_in!=NULL)&&(fp_out!=NULL)){          //the structure last time copy to the current data
        while((ch = getc(fp_out)) != EOF){
            putc(ch,fp_in);
        }
        fclose(fp_in);
        fclose(fp_out);
    }
    /************************************************************************************/
    FILE *fp1=fopen("input.txt", "r");//for read
    FILE *fp2=fopen("output.txt", "w");//for write

    int floor_num, room_num;
    struct ll *building;

    //ask the user whether the first time to use this program
    /******************************************************/
    while(1){

        int i = 0;
        printf("First time to use this program?\n");
        printf("[1] Yes. [otherwise] No.  : ");
        scanf("%d", &i);
        if (i == 1){//first time
            /****************************************************/
            /*let the user input the info of his or her building*/
            while(1){
                fprintf(stderr, "Please input how many floor do you have :");
                scanf("%d", &floor_num);
                fprintf(fp2, "%d\n", floor_num);
                fprintf(stderr, "Please input how many room do you have in a floor :");
                scanf("%d", &room_num);
                fprintf(fp2, "%d\n", room_num);
                if(room_num<100&&room_num>0&&floor_num>0){
                    break;
                }
                printf("Please input right number!(room_num<100&&room_num>0)/(floor_num>0)\n");
            }
            building=(struct ll*)malloc(sizeof(struct ll));
            building = create_building(floor_num, room_num);

            break;
        }else{//not first time

            fscanf(fp1, "%d", &floor_num);
            fprintf(fp2,"%d\n", floor_num);
            fscanf(fp1, "%d", &room_num);
            fprintf(fp2,"%d\n", room_num);
            building=(struct ll*)malloc(sizeof(struct ll));
            building = create_building(floor_num, room_num);
            struct ll *temp_floor=building;

            /*regain the structure which have built last time*/
            /*************************************************/
            while(temp_floor != NULL){
                struct room *temp_room=temp_floor->HEAD;
                while(temp_room != NULL){
                    fscanf(fp1, "%d", &temp_room->room_type);
                    fscanf(fp1, "%s", temp_room->exroomer_name);
                    fscanf(fp1, "%lld", &temp_room->exroomer_phone);
                    fscanf(fp1, "%s", temp_room->roomer_name);
                    fscanf(fp1, "%lld", &temp_room->roomer_phone);
                    fscanf(fp1, "%d", &temp_room->rent);
                    fscanf(fp1, "%d", &temp_room->violate);
                    for(int i=0; i<5; i++){
                        fscanf(fp1, "%d", &temp_room->report[i]);
                    }
                    for(int i=0; i<5; i++){
                        fscanf(fp1, "%d", &temp_room->report_rule[i]);
                    }
                    temp_room=temp_room->next;
                }
                temp_floor=temp_floor->next;
            }
            break;
        }
    }

    /***************************************/
    /*deal with the command that user makes*/
    while(1){
        int command;
        printf("Command : [1] search. [2] delete. [3] modify. [4] show phone number. [5] sort. [6] watch info. [7] finish.\n");
        printf("What do you want to do ? :");
        scanf("%d", &command);

        if(command == 1){
            int search_type;
            printf("[1] room number. [2] roomer name.\n");
            printf("Please choose the type of target you want to search. : ");
            scanf("%d", &search_type);

            if(search_type == 1){
                int tar_number;
                printf("Please input the room number of the target. : ");
                scanf("%d", &tar_number);

                struct room *target;
                target=search_room(tar_number, floor_num, room_num,building);

                if(target == NULL){             //not found
                    continue;
                }
                if(target->room_type == EMPTY_ROOM&&strcmp(target->roomer_name,"No_roomer")==0){    //found the room but empty room
                    printf("The room is empty.\n");
                    printf("The room used to belong to \"%s\".\n", target->exroomer_name);
                    printf("Phone number : 09-%lld\n", target->exroomer_phone);
                }else{                          //found
                    print_type(target);
                    print_info(target);
                    print_violate(target);
                }
            }else if(search_type == 2){
                char target_name[NAME_LEN];
                printf("Please input the name of the target. : ");
                scanf("%s", target_name);

                struct room *target;
                target=search_name(target_name, building);

                if(target == NULL){
                    continue;
                }else{
                    int target_room=100*(target->floor)+(target->room_num);
                    printf("============================================\n");
                    printf("\"%s\" has the room \"%d\"\n", target_name, target_room);
                    printf("============================================\n");
                }
            }else{
                printf("Invalid Operation.\n");
            }
        }else if(command == 2){
            int tar_number;
            struct room *target;
            printf("Please input the room number of the target. : ");
            scanf("%d", &tar_number);

            target=search_room(tar_number, floor_num, room_num, building);

            if(target == NULL){
                continue;
            }else{
                delete_info(target);
            }
        }else if(command == 3){
            int tar_number;
            struct room *target;
            printf("Please input the room number of the target. : ");
            scanf("%d", &tar_number);

            target=search_room(tar_number, floor_num, room_num, building);

            if(target == NULL){
                continue;
            }else{
                modify_info(target,floor_num,room_num);
            }
        }else if(command == 4){
            traverse_phone(building);
        }else if(command == 5){
            sort(building,floor_num,room_num);
        }else if(command == 6){
            print_floor(building,floor_num);
        }else if(command == 7){
            //update the roomer info
            struct ll *temp_floor=building;
            while(temp_floor != NULL){     //store the sturcture for the next time to use
                struct room *temp_room=temp_floor->HEAD;
                while(temp_room != NULL){
                    fprintf(fp2, "%d\n", temp_room->room_type);
                    fprintf(fp2, "%s\n", temp_room->exroomer_name);
                    fprintf(fp2, "%lld\n", temp_room->exroomer_phone);
                    fprintf(fp2, "%s\n", temp_room->roomer_name);
                    fprintf(fp2, "%lld\n", temp_room->roomer_phone);
                    fprintf(fp2, "%d\n", temp_room->rent);
                    fprintf(fp2, "%d\n", temp_room->violate);
                    for(int i=0; i<5; i++){
                        fprintf(fp2, "%d\n", temp_room->report[i]);
                    }
                    for(int i=0; i<5; i++){
                        fprintf(fp2, "%d\n", temp_room->report_rule[i]);
                    }
                    temp_room=temp_room->next;
                }
                temp_floor=temp_floor->next;
            }
            break;
        }else{
            printf("===============================\n");
            printf("    The command is invalid!\n");
            printf("===============================\n");
        }
    }
    fclose(fp1);
    fclose(fp2);
    return 0;
}


struct ll* create_building(int floor_num, int room_num){

    struct ll *first_floor=NULL, *cur_floor=NULL;

    for(int i=1; i<=floor_num; i++){        //create linked list of floors
        struct ll *new_floor;
        new_floor=(struct ll*)malloc(sizeof(struct ll));

        new_floor->floor=i;

        if(i==1){
            new_floor->next=first_floor;
            first_floor=new_floor;
            cur_floor=first_floor;
        }else{
            new_floor->next=cur_floor->next;
            cur_floor->next=new_floor;
            cur_floor=new_floor;
        }
    }

    cur_floor=first_floor;

    for(int i=0; i<floor_num; i++){         //create linked list of rooms for each floor

        struct room *cur_room=NULL;

        for(int j=0; j<room_num; j++){

            struct room *new_room;
            new_room=(struct room*)malloc(sizeof(struct room));

            //every room should be initialized at first
            new_room->room_num=j+1;
            new_room->floor=i+1;
            new_room->room_type=EMPTY_ROOM;
            strcpy(new_room->exroomer_name, "No_ex-roomer");
            new_room->exroomer_phone=0;
            strcpy(new_room->roomer_name, "No_roomer");
            new_room->rent = 0;
            new_room->roomer_phone = 0;
            new_room->violate=0;
            for(int k=0; k<5; k++){
                new_room->report[k]=0;
            }
            for (int l = 0; l < 5;l++){
                new_room->report_rule[l] = 0;
            }


            if(j==0){
                cur_floor->HEAD=new_room;
                new_room->next=cur_room;
                cur_room=new_room;
            }else{
                new_room->next=cur_room->next;
                cur_room->next=new_room;
                cur_room=new_room;
            }
        }
        cur_floor=cur_floor->next;
    }

    return first_floor;
}

struct room* search_room(int tar_number, int floor_num, int room_num, struct ll *cur_floor){
    if(tar_number<=100){
        printf("==============================\n");
        printf("       Wrong number!\n");
        printf("==============================\n");
        return NULL;
    }

    int floor=tar_number/100;
    int room=tar_number%100;

    if(floor > floor_num || room > room_num){
        printf("==============================\n");
        printf("    The room doesn't exist!\n");
        printf("==============================\n");
        return NULL;
    }

    struct ll *temp_floor=cur_floor;        //go to the target floor
    while(temp_floor->floor != floor){
        temp_floor=temp_floor->next;
    }

    struct room *temp_room=temp_floor->HEAD;    //find the target room
    while(temp_room->room_num != room){
        temp_room=temp_room->next;
    }

    return temp_room;
}

struct room* search_name(char *target, struct ll *cur_floor){
    struct ll *temp_floor=cur_floor;

    while(temp_floor != NULL){          // go to specific floor
        struct room *temp_room=temp_floor->HEAD;

        while(temp_room != NULL){
            if(strcmp(temp_room->roomer_name, target) == 0){        //find the target name
                return temp_room;
            }
            temp_room=temp_room->next;
        }
        temp_floor=temp_floor->next;
    }

    if(temp_floor == NULL){
        printf("====================================================\n");
        printf("No one called \"%s\" has rent a room here.\n", target);
        printf("====================================================\n");
    }
    return NULL;
}

void print_type(struct room *target){
    printf("====================================================\n");
    if(target->room_type == FOR_ACCOMMODATE ){
        printf("%d is for accommodation.\n",(target->room_num+target->floor*100));
    }else if(target->room_type == FOR_OFFICE){
        printf("%d is for office.\n",(target->room_num+target->floor*100));
    }else{
        printf("%d is for warehouse.\n",(target->room_num+target->floor*100));
    }
    return;
}

void print_info(struct room *target){
    printf("====================================================\n");
    printf("Ex-roomer:                  \"%s\".\n", target->exroomer_name);
    printf("Phone number :              #09-%lld\n", target->exroomer_phone);
    printf("----------------------------------------------------\n");
    printf("ROOMER :                    \"%s\"\n", target->roomer_name);
    printf("Phone number :              #09-%lld\n", target->roomer_phone);
    printf("The rent for this month :   %d\n", target->rent);
    printf("====================================================\n");
    return;
}

void print_violate(struct room *target){
    if(target->violate == 0){
        printf("This room hasn't been reported yet.\n");
        printf("Very beautiful,Very powerful!\n");
        printf("====================================================\n");
        return;
    }
    else{
        printf("This room has been reported for %d times.\n", target->violate);
        for(int i=0; i<target->violate; i++){
            printf("NO.%d : %d", (i+1), target->report[i]);
            printf("/rule:%d\n",target->report_rule[i]);
        }
        printf("=====================\n");
        printf("RULE:\n");
        printf("0.None\n");
        printf("1.Too noisy\n");
        printf("2.Too stinky\n");
        printf("3.Too selfish\n");
        printf("4.Play fire\n");
        printf("5.Illegal acting\n");
        printf("=====================\n");
    }
    return;
}

void delete_info(struct room *target){
    //initialize the target room
    strcpy(target->exroomer_name, target->roomer_name);
    strcpy(target->roomer_name, "No_roomer");
    target->exroomer_phone=target->roomer_phone;
    target->room_type=EMPTY_ROOM;
    target->rent=0;
    target->violate=0;
    for (int i = 0; i < 5; i++){
        target->report[i] = 0;
    }
    for (int i = 0; i < 5;i++){
        target->report_rule[i] = 0;
    }
}

void modify_info(struct room *target,int floor_num,int room_num){
    //modify some info of target room
    while(1){
        if(target->violate==5){
            punish(target);
            break;
        }else{
            int choose;
            printf("Please choose the information you want to modify.\n");
            printf("[1] room type. [2] roomer name. [3] roomer phone. [4] rent. [5] report. [6] nothing. :");
            scanf("%d", &choose);
            if(choose == 1){
                change_type(target);
            }else if(choose == 2){
                change_name(target);
            }else if(choose == 3){
                change_phone(target);
            }else if(choose == 4){
                change_rent(target);
            }else if(choose == 5){
                violation_report(target,floor_num,room_num);
            }else if(choose == 6){
                break;
            }else{
                printf("Invalid Operation.\n");
            }
        }
    }
    return;
}

void change_type(struct room *target){
    //modify room_type of target room
    printf("OLD ROOM TYPE : ");
    if(target->room_type == EMPTY_ROOM){
        printf("Empty\n");
    }else if(target->room_type == FOR_ACCOMMODATE){
        printf("For Accommodation\n");
    }else if(target->room_type == FOR_OFFICE){
        printf("For Office\n");
    }else{
        printf("For Warehouse\n");
    }

    while(1){
        int new_type=0;
        printf("[0] FOR EMPTY. [1] FOR ACCOMMODATION. [2] FOR OFFICE. [3] FOR WAREHOUSE.\n");
        printf("The new room type is: ");
        scanf("%d", &new_type);
        if(new_type>3||new_type<0){
            printf("Please input right type!\n");
        }else{
            target->room_type=(enum room_type)new_type;
            break;
        }
    }
    return;
}

void change_name(struct room *target){
    //modify the roomer name of target room
    char new_name[NAME_LEN];
    printf("The new name is:");
    scanf("%s", new_name);

    strcpy(target->roomer_name, new_name);
    return;
}

void change_phone(struct room *target){
    //modify the phone number of target room
    long long int new_number;
    long long int temp;
    while(1){
        printf("The new number is:");
        scanf("%lld", &new_number);
        temp = new_number;

        if(temp<999999999&&temp>900000000){
            new_number %= 100000000;
            target->roomer_phone = new_number;
            break;
        }
        else{
            printf("Be sure to enter right phone number!\n");
        }
    }
    return;
}

void change_rent(struct room *target){
    //modify the rent of target room
    int new_rent;
    while(1){
        printf("Rent:");
        scanf("%d", &new_rent);
        if(new_rent>=0)
        {
            target->rent=new_rent;
            break;
        }
        printf("Please input right number!\n");
    }
    return;
}

void violation_report(struct room *target,int floor_num,int room_num){
    //record report
    int report_room;
    while(1){
        printf("The room is reported by: ");
        scanf("%d", &report_room);
        if(report_room<=100||(report_room/100>floor_num)||(report_room%100>room_num)||(report_room==(target->room_num+target->floor*100))){
            printf("Be sure to enter right room number!\n");
        }else{
            int rule;
            printf("Which rule:");
            scanf("%d",&rule);
            if(rule>0&&rule<=5){
                target->report_rule[target->violate]=rule;
                target->report[target->violate]=report_room;
                (target->violate)++;
                break;
            }else{
                printf("There is no such rule!\n");
            }
        }
    }
}

void punish(struct room *target){
    //to see whether any room should be banished
    int room_num=100*(target->floor)+target->room_num;
    if(target->violate == 5){
        printf("===========================================\n");
        printf("=%d has been reported for 5 times.       =\n", room_num);
        printf("=Therefore, the roomer of %d is banished.=\n", room_num);
        printf("===========================================\n");
        delete_info(target);
    }
}

void traverse_phone(struct ll *cur_floor){
    //phone number directory
    struct ll *temp_floor=cur_floor;
    printf("================================================\n");
    while(temp_floor != NULL){
        struct room *temp_room=temp_floor->HEAD;

        while(temp_room != NULL){
            int room_num=(100*temp_room->floor)+temp_room->room_num;
            printf("%d : #09-%lld\n", room_num, temp_room->roomer_phone);
            temp_room=temp_room->next;
        }
        printf("----------FLOOR:%d----------\n", temp_floor->floor);
        temp_floor=temp_floor->next;
    }
    printf("================================================\n");
}

void sort(struct ll *cur_floor,int floor_num,int room_num){

    int count = floor_num * room_num;
    int i=0;
    struct room store[count];

    while(cur_floor!=NULL){
        struct room *cur_room=cur_floor->HEAD;
        while(cur_room!=NULL){
            store[i]=*cur_room;
            i++;
            cur_room=cur_room->next;
        }
        cur_floor=cur_floor->next;
    }

    printf("What do you want to sort?[1]:rent [2]:violate\n");
    int choose;
    scanf("%d",&choose);
    if(choose==1){
        struct room temp_store;

        for (int i = 0; i < count-1;i++){   //bubble sort
            for (int j = 0; j < count-1;j++){
                if(store[j].rent>store[j+1].rent){
                    temp_store = store[j];
                    store[j] = store[j + 1];
                    store[j + 1] = temp_store;
                }
            }
        }
        printf("===========================================\n");
        for (int i = 0; i < count;i++){
            printf("Rent:%04d\tRoom:%d\tRoomer:%s\n", store[i].rent, (store[i].floor*100)+store[i].room_num, store[i].roomer_name);
        }
        printf("===========================================\n");
    }else if(choose==2){
        struct room temp_store;

        for (int i = 0; i < count-1;i++){       //bubble sort
            for (int j = 0; j < count-1;j++){

                if(store[j].violate<store[j+1].violate){
                    temp_store = store[j];
                    store[j] = store[j + 1];
                    store[j + 1] = temp_store;
                }
            }
        }
        printf("===========================================\n");
        for (int i = 0; i < count;i++){
            printf("Violate:%d\tRoom:%d\tRoomer:%s\n", store[i].violate,  (store[i].floor*100)+store[i].room_num, store[i].roomer_name);
        }
        printf("===========================================\n");
    }else{
        printf("==============================\n");
        printf("        Invalid sort\n");
        printf("==============================\n");
    }
    return ;
}

void print_floor(struct ll *cur_floor,int floor_num){   // watch info of specific floor
    printf("Which floor do you want to see:");
    int floor;
    scanf("%d",&floor);

    if(floor>floor_num||floor<=0){
        printf("==============================\n");
        printf("The floor doesn't exist!\n");
        printf("==============================\n");
    }else{

        struct ll *temp_floor=cur_floor;
        for(int i=1;i<floor;i++){
            temp_floor=temp_floor->next;
        }

        struct room *temp_room=temp_floor->HEAD;
        printf("This is floor %d.\n",floor);
        while(temp_room!=NULL){
            printf("=====================================\n");
            printf("Room:        %d\n",temp_room->room_num+temp_room->floor*100);
            printf("Name:        %s\n",temp_room->roomer_name);
            printf("Rent:        %d\n",temp_room->rent);
            printf("Roomer phone:#09-%lld\n",temp_room->roomer_phone);

            temp_room=temp_room->next;
        }
        printf("=====================================\n");
    }
    return ;
}

void convert_to_table(struct ll *cur_floor){
  table* newtable = (table*)malloc(sizeof(table));
  char t_name[MAXCHARSIZE];
  sprintf(t_name, "floor_%d", cur_floor->floor);
  newtable->name = t_name;
  newtable->data_len = 0;
  newtable->column_len = 9;
  column cur_column = {.data_type=INT};
  strcpy(cur_column.name,"floor");
  add_column(newtable,&cur_column);
  cur_column.data_type=INT;
  strcpy(cur_column.name,"room_num");
  add_column(newtable,&cur_column);
  cur_column.data_type=INT;
  strcpy(cur_column.name,"room_type");
  add_column(newtable,&cur_column);
  cur_column.data_type=STRING;
  strcpy(cur_column.name,"exroomer_name");
  add_column(newtable,&cur_column);
  cur_column.data_type=INT;
  strcpy(cur_column.name,"exroomer_phone");
  add_column(newtable,&cur_column);
  cur_column.data_type=STRING;
  strcpy(cur_column.name,"roomer_name");
  add_column(newtable,&cur_column);
  cur_column.data_type=INT;
  strcpy(cur_column.name,"roomer_phone");
  add_column(newtable,&cur_column);
  strcpy(cur_column.name,"rent");
  add_column(newtable,&cur_column);
  strcpy(cur_column.name,"violate");
  add_column(newtable,&cur_column);
}
