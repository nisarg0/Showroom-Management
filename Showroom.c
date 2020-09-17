#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>


typedef enum{FAILURE,SUCCESS} status_code;
typedef enum{UNSOLD,SOLD} status;

int carVIN1=100;
int customerID1=1000;
int salesmanID1=10000;
int kindCAR=0;

int carVIN2=200;
int customerID2=2000;
int salesmanID2=20000;

int carVIN3=300;
int customerID3=3000;
int salesmanID3=30000;



float totalsold=0;             //Sum of all cars sold this month
typedef struct popular_tag
{
    int num;//NUmber of times the car appperars
    char name[40];
}popular;

popular popular_car[50];



typedef struct car_tag
{
    char name[40];
    char color[10];
    float price;
    char fuel_type[10];  //p:-Petrol  d:-diesel
    char type[10];
    int kind;           //number all cars with same name will have same kind number
}car;

typedef struct customer_details1_tag
{
    int ID;
    char name[20];
    char address[40];
    char mobile_number[15];
    int modeOfPayment;   //mode of payment   1.Cash Payment
                                        //   2:- 9.00% rate of interest for 84 months EMI.
                                        //   3:- 8.75% rate of interest for 60 months EMI.
                                        //   4:- 8.50% rate of interest for 36 months EMI.
}customer_details1;

typedef struct customer_tag
{
	int ID;
	struct customer_tag *left;
	struct customer_tag *right;
	customer_details1 customer_details;
	int height;
}customer;

customer* root_customer=NULL;

typedef struct salesman_details1_tag
{

    int ID;
    char name[20];
    float target;      //How much more to sell to achieve the target 50,00,000rs
    float achieved;    //How much sell he made
    float commission;  //The money he gets extra due to his sell(2% of total sells he made in that month)
    int customer_ID[10];

}salesman_details1;

typedef struct temp_salesman_tag
{
    int ID;
    char name[20];
}temp_salesman;

typedef struct salesman_tag
{
    int ID;
    struct salesman_tag *left;
	struct salesman_tag *right;
	int height;
    salesman_details1 salesman_details;
}salesman;

salesman *root_salesman=NULL;

typedef struct sold_cars_tag
{
    int VIN;                    //Unique for each car
    struct sold_cars_tag *left;
	struct sold_cars_tag *right;
	int height;
    car car_details;            //details of car sold
    salesman_details1 salesman_details;
    customer_details1 customer_details;  //To which customer the car is sold
}sold_cars;

sold_cars *root_sold_cars=NULL;        //Global address
//*********************************************FOR ANY NEW CAR***********************************************************

typedef struct all_cars_tag
{
    int VIN;
    struct all_cars_tag *left;
    struct all_cars_tag *right;
    int height;
    int car_status;        //0-CAR NOT SOLD
    car car_details;
    customer_details1 customer_details;
    salesman_details1 salesman_details;
}all_cars;

all_cars *root_all_cars = NULL;//global address


int max_all_cars(int a, int b);
int height_all_cars(all_cars *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
int max_all_cars(int a, int b)
{
    return (a > b)? a : b;
}

all_cars* newNode_all_cars(int VIN,car temp_car)
{
    all_cars* node = (all_cars*)malloc(sizeof(all_cars));
    node->VIN   = VIN;
    node->car_details=temp_car;
    node->car_status=0;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;
    return(node);
}

all_cars *rightRotate_all_cars(all_cars *y)
{
    all_cars *x = y->left;
    all_cars *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max_all_cars(height_all_cars(y->left), height_all_cars(y->right))+1;
    x->height = max_all_cars(height_all_cars(x->left), height_all_cars(x->right))+1;

    return x;
}


all_cars *leftRotate_all_cars(all_cars *x)
{
    all_cars *y = x->right;
    all_cars *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max_all_cars(height_all_cars(x->left), height_all_cars(x->right))+1;
    y->height = max_all_cars(height_all_cars(y->left), height_all_cars(y->right))+1;


    return y;
}

int getBalance_all_cars(all_cars *N)
{
    if (N == NULL)
        return 0;
    return height_all_cars(N->left) - height_all_cars(N->right);
}

all_cars* insert_all_cars(all_cars* node, int VIN,car temp_car)
{

    if (node == NULL)
        return(newNode_all_cars(VIN,temp_car));

    if (VIN < node->VIN)
        node->left  = insert_all_cars(node->left, VIN,temp_car);
    else if (VIN > node->VIN)
        node->right = insert_all_cars(node->right, VIN,temp_car);
    else
        return node;

    node->height = 1 + max_all_cars(height_all_cars(node->left),height_all_cars(node->right));

    int balance = getBalance_all_cars(node);

    if (balance > 1 && VIN < node->left->VIN)
        return rightRotate_all_cars(node);

    if (balance < -1 && VIN > node->right->VIN)
        return leftRotate_all_cars(node);

    if (balance > 1 && VIN > node->left->VIN)
    {
        node->left =  leftRotate_all_cars(node->left);
        return rightRotate_all_cars(node);
    }

    if (balance < -1 && VIN < node->right->VIN)
    {
        node->right = rightRotate_all_cars(node->right);
        return leftRotate_all_cars(node);
    }

    return node;
}

void preOrder_all_cars(all_cars *root)
{
    int temp;
    if(root != NULL)
    {
        preOrder_all_cars(root->left);
        temp=root->VIN/100;
        printf("------------Showroom%d-----------\n",temp);
        printf("VIN=>%d\tName=>%s\tcolor=>%s\tPrice=>%f\tType=>%sFuel Type=>%s\n", root->VIN,root->car_details.name,root->car_details.color,root->car_details.price,root->car_details.type,root->car_details.fuel_type);
        if(root->car_status)
        {
            printf("Customer Details:\n");
            printf("ID=>%d\tName=>%s\tAddress=>%s\tMobile=>%s\t",root->customer_details.ID,root->customer_details.name,root->customer_details.address,root->customer_details.mobile_number);
            printf("\nSalesman Details:\n");
            printf("ID=>%d\tName=>%s\n",root->salesman_details.ID,root->salesman_details.name);

        }
        else
        {
            printf("Car is not sold\n");
        }

        preOrder_all_cars(root->right);
    }
}
//******************************************************FOR SOLD CARS *******************************************************************


int height_sold_cars(sold_cars *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
int max_sold_cars(int a, int b)
{
    return (a > b)? a : b;
}

sold_cars* newNode_sold_cars(int VIN,car temp_car,salesman* temp_salesman,customer_details1 temp_customer)
{
    sold_cars* node = (sold_cars*)malloc(sizeof(sold_cars));
    node->VIN   = VIN;
    node->car_details=temp_car;
    node->salesman_details=temp_salesman->salesman_details;
    node->customer_details=temp_customer;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;
    return(node);
}

sold_cars *rightRotate_sold_cars(sold_cars *y)
{
    sold_cars *x = y->left;
    sold_cars *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max_sold_cars(height_sold_cars(y->left), height_sold_cars(y->right))+1;
    x->height = max_sold_cars(height_sold_cars(x->left), height_sold_cars(x->right))+1;

    return x;
}


sold_cars *leftRotate_sold_cars(sold_cars *x)
{
    sold_cars *y = x->right;
    sold_cars *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max_sold_cars(height_sold_cars(x->left), height_sold_cars(x->right))+1;
    y->height = max_sold_cars(height_sold_cars(y->left), height_sold_cars(y->right))+1;


    return y;
}

int getBalance_sold_cars(sold_cars *N)
{
    if (N == NULL)
        return 0;
    return height_sold_cars(N->left) - height_sold_cars(N->right);
}

sold_cars* insert_sold_cars(sold_cars* node, int VIN,car temp_car,salesman* temp_salesman,customer_details1 temp_customer)
{

    if (node == NULL)
        return(newNode_sold_cars(VIN,temp_car,temp_salesman,temp_customer));

    if (VIN < node->VIN)
        node->left  = insert_sold_cars(node->left,VIN,temp_car,temp_salesman,temp_customer);
    else if (VIN > node->VIN)
        node->right = insert_sold_cars(node->right, VIN,temp_car,temp_salesman,temp_customer);
    else
        return node;

    node->height = 1 + max_sold_cars(height_sold_cars(node->left),height_sold_cars(node->right));

    int balance = getBalance_sold_cars(node);

    if (balance > 1 && VIN < node->left->VIN)
        return rightRotate_sold_cars(node);

    if (balance < -1 && VIN > node->right->VIN)
        return leftRotate_sold_cars(node);

    if (balance > 1 && VIN > node->left->VIN)
    {
        node->left =  leftRotate_sold_cars(node->left);
        return rightRotate_sold_cars(node);
    }

    if (balance < -1 && VIN < node->right->VIN)
    {
        node->right = rightRotate_sold_cars(node->right);
        return leftRotate_sold_cars(node);
    }

    return node;
}

void preOrder_sold_cars(sold_cars *root)
{int temp;
    if(root != NULL)
    {
        preOrder_sold_cars(root->left);
        temp=root->VIN/100;
        printf("-----------Showroom%d----------\n",temp);
        printf("VIN=>%d\tName=>%s\tcolor=>%s\tPrice=>%f\tType=>%sFuel Type=>%s\n", root->VIN,root->car_details.name,root->car_details.color,root->car_details.price,root->car_details.type,root->car_details.fuel_type);
        printf("Customer Details:\n");
        printf("ID=>%d\tName=>%s\tAddress=>%s\tMobile=>%s\t",root->customer_details.ID,root->customer_details.name,root->customer_details.address,root->customer_details.mobile_number);
        printf("\nSalesman Details:\n");
        printf("ID=>%d\tName=>%s\n",root->salesman_details.ID,root->salesman_details.name);

        preOrder_sold_cars(root->right);
    }

}

//********************************************************SALESMAN****************************************************************



int height_salesman(salesman *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
int max_salesman(int a, int b)
{
    return (a > b)? a : b;
}

salesman* newNode_salesman(temp_salesman temp)
{
    int i;
    salesman* node = (salesman*)malloc(sizeof(salesman));
    salesman_details1 tempdata;
    strcpy(tempdata.name,temp.name);
    tempdata.ID=temp.ID;
    tempdata.target=5000000.0;
    tempdata.achieved=0.0;
    tempdata.commission=0.0;
    for(i=0;i<10;i++)
    {
        tempdata.customer_ID[i]=0;
    }

    node->ID   = temp.ID;
    node->salesman_details=tempdata;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;
    return(node);
}

salesman *rightRotate_salesman(salesman *y)
{
    salesman *x = y->left;
    salesman *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max_salesman(height_salesman(y->left), height_salesman(y->right))+1;
    x->height = max_salesman(height_salesman(x->left), height_salesman(x->right))+1;

    return x;
}


salesman *leftRotate_salesman(salesman *x)
{
    salesman *y = x->right;
    salesman *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max_salesman(height_salesman(x->left), height_salesman(x->right))+1;
    y->height = max_salesman(height_salesman(y->left), height_salesman(y->right))+1;


    return y;
}

int getBalance_salesman(salesman *N)
{
    if (N == NULL)
        return 0;
    return height_salesman(N->left) - height_salesman(N->right);
}

salesman* insert_salesman(salesman* node,temp_salesman temp)
{ int ID=temp.ID;

    if (node == NULL)
        return(newNode_salesman(temp));

    if (ID < node->ID)
        node->left  = insert_salesman(node->left, temp);
    else if (ID > node->ID)
        node->right = insert_salesman(node->right, temp);
    else
        return node;

    node->height = 1 + max_salesman(height_salesman(node->left),height_salesman(node->right));

    int balance = getBalance_salesman(node);

    if (balance > 1 && ID < node->left->ID)
        return rightRotate_salesman(node);

    if (balance < -1 && ID > node->right->ID)
        return leftRotate_salesman(node);

    if (balance > 1 && ID > node->left->ID)
    {
        node->left =  leftRotate_salesman(node->left);
        return rightRotate_salesman(node);
    }

    if (balance < -1 && ID < node->right->ID)
    {
        node->right = rightRotate_salesman(node->right);
        return leftRotate_salesman(node);
    }

    return node;
}

void preOrder_salesman(salesman *root)
{
    int i,temp;
    if(root != NULL)
    {
        preOrder_salesman(root->left);
        temp=root->ID/10000;
        printf("Showroom%d\t",temp);
        printf("ID=>%d\tName=>%s\tTarget=>%f\tAchieved=>%f\tCommission=>%f\n", root->ID,root->salesman_details.name,root->salesman_details.target,root->salesman_details.achieved,root->salesman_details.commission);
        printf("The Customer ID's of customers of the salesman are:");
        for(i=0;i<10;i++)
        {
            if(root->salesman_details.customer_ID[i]!=0)
            {
                printf("%d ",root->salesman_details.customer_ID[i]);
            }
        }
    printf("\n");
        preOrder_salesman(root->right);
    }
}
//*******************************************CUSTOMER*****************************************************************

int height_customer(customer *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
int max(int a, int b)
{
    return (a > b)? a : b;
}

customer* newNode_customer(customer_details1 customer_details)
{
    customer* node = (customer*)malloc(sizeof(customer));
    node->ID   = customer_details.ID;
    node->customer_details=customer_details;
    node->left   = NULL;
    node->right  = NULL;
    node->height = 1;
    return(node);
}

customer *rightRotate_customer(customer *y)
{
    customer *x = y->left;
    customer *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height_customer(y->left), height_customer(y->right))+1;
    x->height = max(height_customer(x->left), height_customer(x->right))+1;

    return x;
}


customer *leftRotate_customer(customer *x)
{
    customer *y = x->right;
    customer *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height_customer(x->left), height_customer(x->right))+1;
    y->height = max(height_customer(y->left), height_customer(y->right))+1;


    return y;
}

int getBalance_customer(customer *N)
{
    if (N == NULL)
        return 0;
    return height_customer(N->left) - height_customer(N->right);
}

customer* insert_customer(customer* node, customer_details1 customer_details)
{
    int ID=customer_details.ID;
    if (node == NULL)
        return(newNode_customer(customer_details));

    if (ID < node->ID)
        node->left  = insert_customer(node->left, customer_details);
    else if (ID > node->ID)
        node->right = insert_customer(node->right,customer_details);
    else
        return node;

    node->height = 1 + max(height_customer(node->left),height_customer(node->right));

    int balance = getBalance_customer(node);

    if (balance > 1 && ID < node->left->ID)
        return rightRotate_customer(node);

    if (balance < -1 && ID > node->right->ID)
        return leftRotate_customer(node);

    if (balance > 1 && ID > node->left->ID)
    {
        node->left =  leftRotate_customer(node->left);
        return rightRotate_customer(node);
    }

    if (balance < -1 && ID < node->right->ID)
    {
        node->right = rightRotate_customer(node->right);
        return leftRotate_customer(node);
    }

    return node;
}

void preOrder_customer(customer *root)
{
    int temp;
    if(root != NULL)
    {
        preOrder_customer(root->left);
        temp=root->ID/1000;
        printf("Showroom%d\t",temp);
        printf("ID=>%d\tName=>%s\tAddress=>%s\tMobile=>%s\n",root->ID,root->customer_details.name,root->customer_details.address,root->customer_details.mobile_number);
        preOrder_customer(root->right);
    }
}
//*******************************************************SELL FUNn************************************************************************
all_cars* search_all_cars(all_cars* node, int VIN)
{
    if (node == NULL || node->VIN == VIN)
       return node;

    if (node->VIN < VIN)
       return search_all_cars(node->right, VIN);

    return search_all_cars(node->left, VIN);
}

salesman* search_salesman(salesman* root,int ID)
{

    if (root == NULL || root->ID == ID)
      return root;


    if (root->ID < ID)
       return search_salesman(root->right, ID);

    return search_salesman(root->left, ID);

}

car UpdateInAllCars(int VIN,salesman* temp_salesman,customer_details1 temp_customer)
{

    all_cars *temp_node=search_all_cars(root_all_cars,VIN);
    car temp_car=temp_node->car_details;
    temp_node->car_status=1;
   // salesman* temp_salesman=search_salesman(root_salesman,seller_ID);
    temp_node->salesman_details=temp_salesman->salesman_details;
    temp_node->customer_details=temp_customer;
    return temp_car;

}
float emi_calculator(float p, float r, float t)
{
    float emi;

    r = r / (12 * 100); // one month interest
    emi = (p * r * pow(1 + r, t)) / (pow(1 + r, t) - 1);
    emi=emi*t;


    return (emi);
}
void UpdateSalesmanDetails(salesman* temp_salesman,float price,customer_details1 temp_customer)
{
    int i=0,flag=1;
    for(i=0;i<10&&flag;i++)
    {
        if(temp_salesman->salesman_details.customer_ID[i]==0&&flag)
        {
            temp_salesman->salesman_details.customer_ID[i]=temp_customer.ID;
            flag=0;

        }
    }
    temp_salesman->salesman_details.achieved+=price;
    temp_salesman->salesman_details.target-=price;
    float val=price;
    float emi;
    float rate;
    float time;
    switch(temp_customer.modeOfPayment)
    {
        case 1:
        temp_salesman->salesman_details.commission+=(val/200.0);
        //printf("%s-%f ",temp_salesman->salesman_details.name,val/200);
        break;
        case 2:
        rate = 9.0;
        time = 84;
        emi = emi_calculator(price, rate,time);
        temp_salesman->salesman_details.commission+=(val/200.0);
        //printf("%s-%f ",temp_salesman->salesman_details.name,val/200);
        break;
        case 3:
        rate = 8.75;
        time = 60;
        emi = emi_calculator(price, rate, time);
       temp_salesman->salesman_details.commission+=(val/200.0);
        //printf("%s-%f ",temp_salesman->salesman_details.name,val/200);
        break;
        case 4:
        rate = 8.50;
        time = 36;
        emi = emi_calculator(price, rate, time);
        temp_salesman->salesman_details.commission+=(val/200.0);
      //  printf("%s-%f ",temp_salesman->salesman_details.name,val/200);

    }

}

void sell(int VIN,int seller_ID,customer_details1 temp_customer)
{

    salesman* temp_salesman=search_salesman(root_salesman,seller_ID);
    car temp_car=UpdateInAllCars(VIN,temp_salesman,temp_customer);

    root_sold_cars=insert_sold_cars(root_sold_cars,VIN,temp_car,temp_salesman,temp_customer);
    root_customer=insert_customer(root_customer,temp_customer);
    UpdateSalesmanDetails(temp_salesman,temp_car.price,temp_customer);

    popular_car[temp_car.kind].num++;   //To find most popular car
    totalsold+=temp_car.price;
    //printf("\ntotalsold-%llu carprice-%f\n",totalsold,temp_car.price);
}

void take_input_from_file()
{

    FILE *fp = fopen("dspd.txt","r") ;

    int no_cars,i,no_salesman,no_soldcars;
    fscanf(fp,"%d",&no_cars);

    printf("At present cars in inventory are:\n");
    char car_name[20],car_color[10],type[10],fuel_type[10];
    float car_price;
    int car_kind;
    car temp_car;
    printf("\n\n******************************Showroom 1********************************\n");
    for(i=0;i<no_cars/3;i++)
    {
        fscanf(fp," %[^\n]%*c",&car_name);
        fscanf(fp," %[^\n]%*c",&car_color);
        fscanf(fp,"%f",&car_price);
        fscanf(fp," %[^\n]%*c",&fuel_type);
        fscanf(fp," %[^\n]%*c",&type);
        fscanf(fp,"%d",&car_kind);

        popular_car[car_kind].num=0;
        strcpy(popular_car[car_kind].name,car_name);

        strcpy(temp_car.name,car_name);
        strcpy(temp_car.color,car_color);
        temp_car.price=car_price;
        strcpy(temp_car.fuel_type,fuel_type);
        strcpy(temp_car.type,type);
        temp_car.kind=car_kind;


        root_all_cars=insert_all_cars(root_all_cars,carVIN1,temp_car);
        printf("VIN:%d\tName:%s\tColor:%s\tPrice:%f\tFuel Type:%s\tType:%s\n",carVIN1,car_name,car_color,car_price,fuel_type,type);
        carVIN1++;
        kindCAR++;
    }
    printf("\n\n******************************Showroom 2********************************\n");
    for(;i<no_cars*2/3;i++)
    {
        fscanf(fp," %[^\n]%*c",&car_name);
        fscanf(fp," %[^\n]%*c",&car_color);
        fscanf(fp,"%f",&car_price);
        fscanf(fp," %[^\n]%*c",&fuel_type);
        fscanf(fp," %[^\n]%*c",&type);
        fscanf(fp,"%d",&car_kind);

        popular_car[car_kind].num=0;
        strcpy(popular_car[car_kind].name,car_name);

        strcpy(temp_car.name,car_name);
        strcpy(temp_car.color,car_color);
        temp_car.price=car_price;
        strcpy(temp_car.fuel_type,fuel_type);
        strcpy(temp_car.type,type);
        temp_car.kind=car_kind;


        root_all_cars=insert_all_cars(root_all_cars,carVIN2,temp_car);
        printf("VIN:%d\tName:%s\tColor:%s\tPrice:%f\tFuel Type:%s\tType:%s\n",carVIN2,car_name,car_color,car_price,fuel_type,type);
        carVIN2++;
    }
    printf("\n\n******************************Showroom 3********************************\n");
       for(;i<no_cars;i++)
    {


        fscanf(fp," %[^\n]%*c",&car_name);
        fscanf(fp," %[^\n]%*c",&car_color);
        fscanf(fp,"%f",&car_price);
        fscanf(fp," %[^\n]%*c",&fuel_type);
        fscanf(fp," %[^\n]%*c",&type);
        fscanf(fp,"%d",&car_kind);

        popular_car[car_kind].num=0;
        strcpy(popular_car[car_kind].name,car_name);

        strcpy(temp_car.name,car_name);
        strcpy(temp_car.color,car_color);
        temp_car.price=car_price;
        strcpy(temp_car.fuel_type,fuel_type);
        strcpy(temp_car.type,type);
        temp_car.kind=car_kind;


        root_all_cars=insert_all_cars(root_all_cars,carVIN3,temp_car);
        printf("VIN:%d\tName:%s\tColor:%s\tPrice:%f\tFuel Type:%s\tType:%s\n",carVIN3,car_name,car_color,car_price,fuel_type,type);
        carVIN3++;
    }


    printf("\n\n\nSaleman working are:\n");
    fscanf(fp,"%d",&no_salesman);
    char salesman_name[20];
    temp_salesman temp_seller;
    printf("\n\n******************************Showroom 1********************************\n");
    for(i=0;i<no_salesman/3;i++)
    {
        fscanf(fp," %[^\n]%*c",&salesman_name);
        strcpy(temp_seller.name,salesman_name);
        temp_seller.ID=salesmanID1;
        printf("ID=>%d\tName=>%s\n",temp_seller.ID,temp_seller.name);
        salesmanID1++;
        root_salesman=insert_salesman(root_salesman,temp_seller);
    }
    printf("\n\n******************************Showroom 2********************************\n");
     for(;i<no_salesman*2/3;i++)
    {
        fscanf(fp," %[^\n]%*c",&salesman_name);
        strcpy(temp_seller.name,salesman_name);
        temp_seller.ID=salesmanID2;
        printf("ID=>%d\tName=>%s\n",salesmanID2,salesman_name);
        salesmanID2++;
        root_salesman=insert_salesman(root_salesman,temp_seller);
    }
    printf("\n\n******************************Showroom 3********************************\n");
     for(;i<no_salesman;i++)
    {
        fscanf(fp," %[^\n]%*c",&salesman_name);
        strcpy(temp_seller.name,salesman_name);
        temp_seller.ID=salesmanID3;
        printf("ID=>%d\tName=>%s\n",salesmanID3,salesman_name);
        salesmanID3++;
        root_salesman=insert_salesman(root_salesman,temp_seller);
    }

    printf("\n\n\nFollowing cars are sold to the following customers\n");
    fscanf(fp,"%d",&no_soldcars);
    customer_details1 temp_customer;
    char cus_name[20];
    char cus_add[40];
    char cus_mob[15];
    int sellVIN;
    int sellerID;
    int payment;
    printf("*****************************ShowRoom1****************************************\n\n");
    for(i=0;i<no_soldcars/3;i++)
    {
        fscanf(fp," %[^\n]%*c",&cus_name);
        fscanf(fp," %[^\n]%*c",&cus_add);
        fscanf(fp," %[^\n]%*c",&cus_mob);
        fscanf(fp,"%d",&payment);
        fscanf(fp,"%d",&sellVIN);
        fscanf(fp,"%d",&sellerID);

        temp_customer.ID=customerID1;
        strcpy(temp_customer.name,cus_name);
        strcpy(temp_customer.address,cus_add);
        strcpy(temp_customer.mobile_number,cus_mob);
        temp_customer.modeOfPayment=payment;

        sell(sellVIN,sellerID,temp_customer);
        printf("VIN: %d is sold to ID:%d %s by salesman with ID:%d\n",sellVIN,customerID1,cus_name,sellerID);
        customerID1++;
    }
    printf("*****************************ShowRoom2****************************************\n\n");
    for(;i<no_soldcars*2/3;i++)
    {
        fscanf(fp," %[^\n]%*c",&cus_name);
        fscanf(fp," %[^\n]%*c",&cus_add);
        fscanf(fp," %[^\n]%*c",&cus_mob);
        fscanf(fp,"%d",&payment);
        fscanf(fp,"%d",&sellVIN);
        fscanf(fp,"%d",&sellerID);

        temp_customer.ID=customerID2;
        strcpy(temp_customer.name,cus_name);
        strcpy(temp_customer.address,cus_add);
        strcpy(temp_customer.mobile_number,cus_mob);
        temp_customer.modeOfPayment=payment;


        sell(sellVIN,sellerID,temp_customer);

        printf("VIN: %d is sold to ID:%d %s by salesman with ID:%d\n",sellVIN,customerID2,cus_name,sellerID);
        customerID2++;
    }
    printf("*****************************ShowRoom3****************************************\n");
    for(;i<no_soldcars;i++)
    {
        fscanf(fp," %[^\n]%*c",&cus_name);
        fscanf(fp," %[^\n]%*c",&cus_add);
        fscanf(fp," %[^\n]%*c",&cus_mob);
        fscanf(fp,"%d",&payment);
        fscanf(fp,"%d",&sellVIN);
        fscanf(fp,"%d",&sellerID);

        temp_customer.ID=customerID3;
        strcpy(temp_customer.name,cus_name);
        strcpy(temp_customer.address,cus_add);
        strcpy(temp_customer.mobile_number,cus_mob);
        temp_customer.modeOfPayment=payment;

        sell(sellVIN,sellerID,temp_customer);
        printf("VIN: %d is sold to ID:%d %s by salesman with ID:%d\n",sellVIN,customerID3,cus_name,sellerID);
        customerID3++;
    }


}

salesman* searchBestsalesman(salesman* root)
{
    if (root == NULL)
    return (NULL);

    salesman* res = root;
    salesman* lres = searchBestsalesman(root->left);
    salesman* rres = searchBestsalesman(root->right);
    if(lres!=NULL)
    {
        if (lres->salesman_details.achieved > res->salesman_details.achieved)
         res = lres;
    }
    if(rres!=NULL)
    {
        if (rres->salesman_details.achieved > res->salesman_details.achieved)
        res = rres;
    }
    return res;

}

void rangeSearch(all_cars* root,int lower_lim,int upper_lim)
{
    int showroom;
      if ( NULL == root )
      return;
       if ( lower_lim < root->VIN )
         rangeSearch(root->left, lower_lim, upper_lim);

       if ( lower_lim <= root->VIN && upper_lim >= root->VIN )
         {
                showroom=root->VIN/100;
                printf("------------Showroom%d----------",showroom);
               printf("VIN=>%d\tName=>%s\tcolor=>%s\tPrice=>%f\tType=>%sFuel Type=>%s\n",root->VIN,root->car_details.name,root->car_details.color,root->car_details.price,root->car_details.type,root->car_details.fuel_type);
               if(root->car_status)
                {

                    printf("Customer Details:\n");
                    printf("ID=>%d\tName=>%s\tAddress=>%s\tMobile=>%s\t",root->customer_details.ID,root->customer_details.name,root->customer_details.address,root->customer_details.mobile_number);
                    printf("\nSalesman Details:\n");
                    printf("ID=>%d\tName=>%s\n",root->salesman_details.ID,root->salesman_details.name);
                }
                else
                {
                     printf("Car is not sold yet\n");
                }

         }

       if ( upper_lim > root->VIN )
         rangeSearch(root->right, lower_lim, upper_lim);
}






//************************************************************MAIN*************************************************************************
int main()
{

    int flag=1,input,input2,input3,input4,input1;

    char salesman_name[20];
    temp_salesman temp_seller;

    int seller_id;
    salesman* salesexicutive;
    float temp;
    char car_name[20],car_color[10],type[10],fuel_type[10];
    float car_price;
    int car_kind;
    car temp_car;

    customer_details1 temp_customer;
    char cus_name[20];
    char cus_add[40];
    char cus_mob[15];
    int sellVIN;
    int sellerID;
    int payment;
    int showroom;

    int VIN;
    int ip;
    int lower_lim;
    int upper_lim;
    int most_popular;
    most_popular=popular_car[0].num;
    int i=1;
    long long prediction=15000000;


    all_cars *temp_node;

    for(i=0;i<50;i++)
    {
        popular_car[i].num=-1;
    }

    printf("\n\nWe have assumed that last to last month The company has made 15000000Rs\n");

    take_input_from_file();                 //Input from file


    while(flag)
    {
        printf("Welcome USER\n\n");
        printf("Who are you?\tEnter?\n1.Owner\n2.Salesman\n3.QuickSearch\n4.Popular car list\n5.exit\n");
        scanf("%d",&input);
       if(input==1)
       {

                printf("Enter number to perform the action:\n");
                printf("1.All salesman Details:\n");
                printf("2.All cars details:\n");
                printf("3.Sold car details:\n");
                printf("4.Customer details:\n");
                printf("5.Add New Salesman:\n");
                printf("6.Add car to showroom:\n");
                printf("7.Best salesman:\n");
                printf("8.Predict next month: \n");
                printf("9.Most popular car:\n");

                scanf("%d",&input1);
                switch(input1)
                {
                case 1:
                    preOrder_salesman(root_salesman);
                    break;
                case 2:
                    preOrder_all_cars(root_all_cars);
                    break;
                case 3:
                    preOrder_sold_cars(root_sold_cars);
                    break;
                case 4:
                    preOrder_customer(root_customer);
                    break;
                case 5:
                    printf("Enter the Showroom you want to add the salesman:");
                    scanf("%d",&showroom);
                    printf("Enter Salesman name?\n");
                    scanf(" %[^\n]%*c",&salesman_name);
                    if(showroom==1)
                    {
                        strcpy(temp_seller.name,salesman_name);
                        temp_seller.ID=salesmanID1;
                        printf("Salesman %s is added to data base with salesman ID:%d\n",salesman_name,salesmanID1);
                        root_salesman=insert_salesman(root_salesman,temp_seller);
                        salesmanID1++;
                    }
                    else if(showroom==2)
                    {
                        strcpy(temp_seller.name,salesman_name);
                        temp_seller.ID=salesmanID2;
                        printf("Salesman %s is added to data base with salesman ID:%d\n",salesman_name,salesmanID2);
                        root_salesman=insert_salesman(root_salesman,temp_seller);
                        salesmanID2++;
                    }
                    else if(showroom==3)
                    {
                        strcpy(temp_seller.name,salesman_name);
                        temp_seller.ID=salesmanID3;
                        printf("Salesman %s is added to data base with salesman ID:%d\n",salesman_name,salesmanID3);
                        root_salesman=insert_salesman(root_salesman,temp_seller);
                        salesmanID3++;
                    }
                    else
                    printf("INVALD");
                    break;
                case 6:
                    printf("Enter Showroom\n");
                    scanf("%d",&showroom);
                    printf("Enter Car name:");
                    scanf(" %[^\n]%*c",&car_name);
                    printf("Enter car color:");
                    scanf(" %[^\n]%*c",&car_color);
                    printf("Enter car price:");
                    scanf("%f",&car_price);
                    printf("Enter fuel type of car:");
                    scanf(" %[^\n]%*c",&fuel_type);
                    printf("Enter type of car:");
                    scanf(" %[^\n]%*c",&type);

                    printf("Is the car of new kind:\t1:YES\t2:NO\n");
                    scanf("%d",&ip);
                    if(ip==1){
                    printf("This kind of cars is added to kind %d\n",kindCAR);
                    popular_car[kindCAR].num=0;
                    strcpy(popular_car[kindCAR].name,car_name);
                    kindCAR++;
                    }
                    else{
                    printf("Enter the car kind:");
                    scanf("%d",&car_kind);
                    }
                    strcpy(temp_car.name,car_name);
                    strcpy(temp_car.color,car_color);
                    temp_car.price=car_price;
                    strcpy(temp_car.fuel_type,fuel_type);
                    strcpy(temp_car.type,type);
                    temp_car.kind=car_kind;

                    if(showroom==1)
                    {
                        root_all_cars=insert_all_cars(root_all_cars,carVIN1,temp_car);
                        printf("VIN:%d\tName:%s\tColor:%s\tPrice:%f\tFuel Type:%s\tType:%s\n",carVIN1,car_name,car_color,car_price,fuel_type,type);
                        carVIN1++;
                    }
                    else if(showroom==2)
                    {
                        root_all_cars=insert_all_cars(root_all_cars,carVIN2,temp_car);
                        printf("VIN:%d\tName:%s\tColor:%s\tPrice:%f\tFuel Type:%s\tType:%s\n",carVIN2,car_name,car_color,car_price,fuel_type,type);
                        carVIN2++;
                    }
                    else if(showroom==3)
                    {
                        root_all_cars=insert_all_cars(root_all_cars,carVIN3,temp_car);
                        printf("VIN:%d\tName:%s\tColor:%s\tPrice:%f\tFuel Type:%s\tType:%s\n",carVIN3,car_name,car_color,car_price,fuel_type,type);
                        carVIN3++;
                    }
                    else
                    printf("INVALID");
                    break;
                case 7:
                    salesexicutive=searchBestsalesman(root_salesman);
                    printf("%s is the salesman with highest sell for the month of %f\n",salesexicutive->salesman_details.name,salesexicutive->salesman_details.achieved);
                    printf("Do you want to give a extra 1%% for such good sales?\n1.YES\t2.No\n");
                    scanf("%d",&ip);
                    if(ip==1)
                    {
                        temp=salesexicutive->salesman_details.achieved/100;
                        printf("The salesman is awarded with %f more commision for Awesome work\n",temp);
                        salesexicutive->salesman_details.commission+=temp;
                    }
                    break;
                case 8:
                    //Next month sales will be average of last two months
                    prediction+=totalsold;
                    prediction/=2;
                    printf("Expected sales for next month is %ull Rs\n",prediction);

                    break;
                case 9:
                    while(i<50&&popular_car[i].num>=0)
                    {
                        if(popular_car[i].num>most_popular)
                        {
                            most_popular=i;
                            i++;
                        }
                    }
                    printf("The most popular car is %s of kind %d and we have sold %d cars\n",popular_car[most_popular].name,most_popular,popular_car[most_popular].num);
                    break;
                default:
                    break;
                }
       }
            else if(input==2)
            {


                printf("1.To know your current status\n");
                printf("2.Sell a car\n");
                printf("3.Exit\n");
                scanf("%d",&input3);
                switch(input3)
                {
                case 1:
                    printf("Enter your id:");
                    scanf("%d",&seller_id);
                    salesexicutive=search_salesman(root_salesman,seller_id);
                    showroom=seller_id/10000;
                    printf("Showroom:%d\t",showroom);
                    printf("ID=>%d\tName=>%s\tTarget=>%f\tAchieved=>%f\tCommission=>%f\n", salesexicutive->ID,salesexicutive->salesman_details.name,salesexicutive->salesman_details.target,salesexicutive->salesman_details.achieved,salesexicutive->salesman_details.commission);
                    printf("The Customer ID's of customers of the salesman are:");
                    for(i=0;i<10;i++)
                    {
                        if(salesexicutive->salesman_details.customer_ID[i]!=0)
                        {
                            printf("%d ",salesexicutive->salesman_details.customer_ID[i]);
                        }
                    }
                    break;
                case 2:

                        printf("Enter Customer name:");
                        scanf(" %[^\n]%*c",&cus_name);
                        printf("Enter Customer Address");
                        scanf(" %[^\n]%*c",&cus_add);
                        printf("Enter Customer mobile number");
                        scanf(" %[^\n]%*c",&cus_mob);
                        printf("Enter mode of payment \n1.Cash\n2:- 9.00% rate of interest for 84 months\n3:- 8.75% rate of interest for 60 months EMI\n4:- 8.50% rate of interest for 36 months EMI");
                        scanf("%d",&payment);
                        printf("Enter the VIN of car sold:");
                        scanf("%d",&sellVIN);
                        printf("Enter the seller ID");
                        scanf("%d",&sellerID);
                        if(sellVIN/100==seller_id/10000){
                        temp_customer.ID=customerID1;
                        strcpy(temp_customer.name,cus_name);
                        strcpy(temp_customer.address,cus_add);
                        strcpy(temp_customer.mobile_number,cus_mob);
                        temp_customer.modeOfPayment=payment;

                        sell(sellVIN,sellerID,temp_customer);
                        printf("VIN: %d is sold to ID:%d %s by salesman with ID:%d\n",sellVIN,customerID1,cus_name,sellerID);}
                        else
                        printf("INVALID seller ID or VIN");
                        break;
                case 3:
                    break;
                }
            }
            else if(input==3)
            {

                    printf("Enter 1.Search for particular car:\t2.Range search of cars:\n");
                    scanf("%d",&input4);
                    if(input4==1)
                    {
                        printf("Enter VIN for the required car:\n");
                        scanf("%d",&VIN);
                        showroom=VIN/100;
                        temp_node=search_all_cars(root_all_cars,VIN);
                        printf("------------Showroom%d----------",showroom);
                        printf("VIN=>%d\tName=>%s\tcolor=>%s\tPrice=>%f\tType=>%sFuel Type=>%s\n",temp_node->VIN,temp_node->car_details.name,temp_node->car_details.color,temp_node->car_details.price,temp_node->car_details.type,temp_node->car_details.fuel_type);
                        if(temp_node->car_status)
                            {

                                printf("Customer Details:\n");
                                printf("ID=>%d\tName=>%s\tAddress=>%s\tMobile=>%s\t",temp_node->customer_details.ID,temp_node->customer_details.name,temp_node->customer_details.address,temp_node->customer_details.mobile_number);
                                printf("\nSalesman Details:\n");
                                printf("ID=>%d\tName=>%s\n",temp_node->salesman_details.ID,temp_node->salesman_details.name);

                            }
                        else
                            {
                                printf("Car is not sold yet\n");
                            }

                    }
                    else if(input4==2)
                    {
                        printf("Enter lower limit of VIN:");
                        scanf("%d",&lower_lim);
                        printf("Enter upper limit of VIN ");
                        scanf("%d",&upper_lim);
                        rangeSearch(root_all_cars,lower_lim,upper_lim);
                    }
                    else{
                        printf("invalid input");
                    }
            }
            else if(input==4)
            {
                    printf("The popular car list is as follows\n");
                    i=0;
                    while(i<50&&popular_car[i].num>=0)
                    {
                        printf("Car with kind %d name %s is sold %d times\n",i,popular_car[i].name,popular_car[i].num);
                        i++;
                    }

            }

            else if(input==5){
                flag=0;
                  printf("Exiting...");
            }
            else{
                flag=0;
                  printf("INALID INPUT");
            }


        }
  return 0;
}
