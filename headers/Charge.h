#ifndef CHARGE_H
#define CHARGE_H
    
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Screen.h"
#include "Console.h"
#include "Time.h"
#include "Guest.h"
#include "Room.h"

int ChargeQuantity = 0;

typedef struct Charge{
    int     CI;
    Time    TimeCharge;
    char    Type[30];
    int     Cost;
    struct  Charge* Next;
    struct  Charge* Previous;
}Charge;

Charge* Charge_FirstCharge;
Charge* Charge_LastCharge;

void Charge_New(int CI, Time TimeCharge, char* Type, int Cost){
    Charge* NewItem = malloc(sizeof(Charge));
    if(ChargeQuantity == 0){
        Charge_FirstCharge = NewItem;
        Charge_LastCharge = NewItem;
        NewItem->Next = NULL;
        NewItem->Previous = NULL;
    }
    else{
        Charge_LastCharge->Next = NewItem;
        NewItem->Previous = Charge_LastCharge;
        Charge_LastCharge = NewItem;
        NewItem->Next = NULL;
    }

    ChargeQuantity++;
    NewItem->CI = CI;
    NewItem->TimeCharge = TimeCharge;
    strcpy(NewItem->Type, Type);
    NewItem->Cost = Cost;
}

void Charge_New_Null(){
    Charge_New(0, Time_Null(), "none", 0);
}

void Charge_Show_All(){
    Charge* p = Charge_FirstCharge;
    for(int i = 1; p; i++){
        printf("\t%-5d ", i);
        printf("%-10d ", p->CI);
        printf("%-12s ", Time_ToString(p->TimeCharge));
        printf("%-15s ", p->Type);
        printf("%-10d\n", p->Cost);
        p = p->Next;
    }
}

void Charge_Show_All_Reversed(){
    Charge* p = Charge_LastCharge;
    for(int i = ChargeQuantity; p; i--){
        printf("%d) ", i);
        printf("%d\t", p->CI);
        printf("%s\t", Time_ToString(p->TimeCharge));
        printf("%s\t", p->Type);
        printf("%d\n", p->Cost);
        p = p->Previous;
    }
}

void Charge_Show_Charge(Charge* charge){
    if(!charge)
        return;
    printf("%d\t", charge->CI);
    printf("%s\t", Time_ToString(charge->TimeCharge));
    printf("%s\t", charge->Type);
    printf("%d\n", charge->Cost);
}

void Charge_Memory_Clear(){
    Charge* p = Charge_FirstCharge;
    while(p){
        free(p);
        p = p->Next;
    }
    Charge_FirstCharge = NULL;
    Charge_LastCharge = NULL;
    ChargeQuantity = 0;
}

void Charge_File_Save(){
    Charge* p = Charge_FirstCharge;
    FILE* file = fopen("cargos.txt", "w");
    while(p){
        fprintf(file, "%d ", p->CI);
        fprintf(file, "%s\t",Time_ToString(p->TimeCharge));
        fprintf(file, "%s\t", p->Type);
        fprintf(file, "%d\n", p->Cost);
        p = p->Next;
    }
    fclose(file);
}

void Charge_File_Load(){
    //Check existence of file
    FILE* file = fopen("cargos.txt", "r");
    if(file == NULL){
        fclose(file);
        file = fopen("cargos.txt", "w");
        fclose(file);
        return;
    }
    //Check content file
    int count = 0;
    while(!feof(file)){
        fgetc(file);
        count++;
        if(count > 1)
            break;
    } 
    
    if(count == 1)
        return;
    else{
        rewind(file);
        Charge_Memory_Clear();
    }

    //Start to read file
    int     CI;
    Time    TimeCharge;
    char    Type[50];
    int     Cost;

    while(!feof(file)){
        //read values
        fscanf(file, "%d\t", &CI);
        fscanf(file, "%d/%d/%d ", &TimeCharge.Day, &TimeCharge.Month, &TimeCharge.Year);
        fscanf(file,"%s\t", Type);
        fscanf(file,"%d\n", &Cost);
        //save in memory
        Charge_New(CI, TimeCharge, Type, Cost);
        //reset values of variables in function    
        CI = 0;
        TimeCharge = Time_Null();
        strcpy(Type, "none");
        Cost = 0;
    }
    fclose(file);
}

void Charge_File_Clear(){
    FILE* file = fopen("cargos.txt", "w");
    fclose(file);
}

int Charge_Delete_ByNumber(int Selection){
    Charge *Pointer = NULL;

    if(Selection < 1 || Selection > ChargeQuantity){
        return 0;
    }
    else if(Selection - 1 == 0){
        Pointer = Charge_FirstCharge;
        if(ChargeQuantity == 1){
            Charge_FirstCharge = NULL;
            Charge_LastCharge = NULL;
        }
        else{
            Pointer->Next->Previous = NULL;
            Charge_FirstCharge = Charge_FirstCharge->Next;
        }
        free(Pointer);
        ChargeQuantity--;
        return 1;
    }
    else if(Selection == ChargeQuantity){
        Pointer = Charge_LastCharge;
        Pointer->Previous->Next = NULL;
        Charge_LastCharge = Charge_LastCharge->Previous;
        free(Pointer);
        ChargeQuantity--;
        return 1;
    }
    else{
        Pointer = Charge_FirstCharge;
        for(int i = 0; i < Selection - 1; i++){
            Pointer = Pointer->Next;
        }
        Pointer->Next->Previous = Pointer->Previous;
        Pointer->Previous->Next = Pointer->Next;
        free(Pointer);
        ChargeQuantity--;
        return 1;
    }
}

void Charge_Delete_ByPointer(Charge * Selection){
    if(Selection == NULL){
        return;
    }
    else if(Selection == Charge_FirstCharge){
        if(ChargeQuantity == 1){
            Charge_FirstCharge = NULL;
            Charge_LastCharge = NULL;
        }
        else{
            Selection->Next->Previous = NULL;
            Charge_FirstCharge = Charge_FirstCharge->Next;
        }
        free(Selection);
        ChargeQuantity--;
    }
    else if(Selection == Charge_LastCharge){
        Selection->Previous->Next = NULL;
        Charge_LastCharge = Charge_LastCharge->Previous;
        free(Selection);
        ChargeQuantity--;
    }
    else{
        Selection->Next->Previous = Selection->Previous;
        Selection->Previous->Next = Selection->Next;
        free(Selection);
        ChargeQuantity--;
    }
}

void Charge_Delete_ByAllPointers(Charge ** Selection){
    if(!Selection)
        return;
    for(int i = 0; Selection[i]; i++){
        Charge_Delete_ByPointer(Selection[i]);
    }
}

//-------------------Setters and Getters---------------------

Charge* Charge_Get_ByNumber(int Selection){
    Charge *Pointer = Charge_FirstCharge;
    if(Selection < 1 || Selection > ChargeQuantity)
        return NULL;
    else{
        for(int i = 2; i <= Selection; i++)
            Pointer = Pointer->Next;
        return Pointer;
    }
}

int Charge_Get_CI_ByNumber(int Selection){
    Charge *Pointer = Charge_FirstCharge;
    if(Selection < 1 || Selection > ChargeQuantity)
        return 0;
    else{
        for(int i = 2; i <= Selection; i++)
            Pointer = Pointer->Next;
        return Pointer->CI;
    }
}

Time Charge_Get_TimeCharge_ByNumber(int Selection){
    Charge *Pointer = Charge_FirstCharge;
    if(Selection < 1 || Selection > ChargeQuantity)
        return Time_Null();
    else{
        for(int i = 2; i <= Selection; i++)
            Pointer = Pointer->Next;
        return Pointer->TimeCharge;
    }
}

char* Charge_Get_Type_ByNumber(int Selection){
    Charge *Pointer = Charge_FirstCharge;
    if(Selection < 1 || Selection > ChargeQuantity)
        return NULL;
    else{
        for(int i = 2; i <= Selection; i++)
            Pointer = Pointer->Next;
        return Pointer->Type;
    }
}

int Charge_Get_Cost_ByNumber(int Selection){
    Charge *Pointer = Charge_FirstCharge;
    if(Selection < 1 || Selection > ChargeQuantity)
        return 0;
    else{
        for(int i = 2; i <= Selection; i++)
            Pointer = Pointer->Next;
        return Pointer->Cost;
    }
}

void Charge_Set_CI(int Selection, int CI){
    Charge * Charge = Charge_Get_ByNumber(Selection);
    if(Charge == NULL)
        return;
    Charge->CI = CI;
}

void Charge_Set_TimeCharge(int Selection, Time TimeCharge){
    Charge * Charge = Charge_Get_ByNumber(Selection);
    if(Charge == NULL)
        return;
    Time_Copy(&Charge->TimeCharge, TimeCharge);
}

void Charge_Set_Type(int Selection, char* Type){
    Charge * Charge = Charge_Get_ByNumber(Selection);
    if(Charge == NULL)
        return;
    strcpy(Charge->Type, Type);
}

void Charge_Set_Cost(int Selection, int Cost){
    Charge * Charge = Charge_Get_ByNumber(Selection);
    if(Charge == NULL)
        return;
    Charge->Cost = Cost;
}

//-------------------Search and Find---------------------

int Charge_Search_ByCI(int CI){
    Charge * Pointer = Charge_FirstCharge;
    while(Pointer){
        if(Pointer->CI == CI)
            return 1;
        Pointer = Pointer->Next;
    }
    return 0;
}

Charge ** Charge_Search_ByCI_AllPointers(int CI){
    Charge * Pointer = Charge_FirstCharge;
    Charge ** Found = (Charge**) malloc(sizeof(Charge*));
    int FoundQuantity = 0;

    while(Pointer){
        if(Pointer->CI == CI){
            Found[FoundQuantity] = Pointer;
            FoundQuantity++;
            Found = realloc(Found, sizeof(Charge*) * (FoundQuantity + 1));
		}
        Pointer = Pointer->Next;
	}
	if(!FoundQuantity){
		free(Pointer);
		free(Found);
		return NULL;
	}
	Found[FoundQuantity] = NULL;
    return Found;
}

 void Charge_Show_BillByPointer(Guest * guest){
	if(guest == NULL)
		return;

    Charge** charges = Charge_Search_ByCI_AllPointers(guest->CI);
    Room *room = Room_Search_ByNumber(guest->Number);
    long GuestDays = Time_DaysBetween(guest->Input, guest->Output);
    int CostTotal = GuestDays * room->Cost;
    
	if(!charges){
		printf("\tCliente: %s %s\n\n", guest->Name, guest->LastName);
		printf("\t%-4s %-20s %-13s\n","N", "CARGOS", "MONTO (BsF)");
        ScreenResource_DivitionBar(42,1,1);
        printf("\t%-4d %-20s %-13ld\n", 1, "Alojamiento",GuestDays * room->Cost);
		printf("\t<Sin cargos adicionales>\n");
        ScreenResource_DivitionBar(42,1,1);
        printf("\t%25s %d\n", "TOTAL: ", CostTotal);
		return;
	}
		
	printf("\tCliente: %s %s\n\n", guest->Name, guest->LastName);
	printf("\t%-4s %-20s %-13s\n","N", "CARGOS", "MONTO (BsF)");
    ScreenResource_DivitionBar(42,1,1);
    printf("\t%-4d %-20s %-13ld\n", 1, "Alojamiento",GuestDays * room->Cost);
	for(int i = 0; charges[i]; i++){
		printf("\t%-4d %-20s %-13d\n", i+2, charges[i]->Type, charges[i]->Cost);
		CostTotal += charges[i]->Cost;
	}
	ScreenResource_DivitionBar(42,1,1);
	printf("\t%25s %d\n", "TOTAL: ", CostTotal);
}

#endif /* CHARGE_H */