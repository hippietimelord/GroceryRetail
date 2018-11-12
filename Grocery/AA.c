#include <stdio.h>
#include <stdlib.h>				//include standard library header files
#include <string.h>

void menu();					//initialize functions
void itemInven();
void purchaseItem();
void editItems();
void dailyTrans();
void addItems();
void delItems();

int t_transaction;              //initialize global variables
float salesTgst;
float salesTngst;
float t_Gst;

struct items{					//initialize class used in purchase items
	char itemCode[10];
	char itemName[40];
	float itemPrice;
	int itemQty;
};
struct items gstItems;
struct items ngstItems;

int main (){												//main function
	int option;													//variables
	
	printf("\n------------------------------------\n");			//menu
	printf("Grocery Retail\n");
	printf("------------------------------------\n");
	printf("   1. Purchase items\n");
	printf("   2. Edit items\n");
	printf("   3. Add items\n");
	printf("   4. Delete items\n");
	printf("   5. Show inventory\n");
	printf("   6. Show daily transaction\n");
	printf("   7. Exit\n");
	
	printf("\nSelect an option from 1 to 7 only. Enter the corresponding number.\n");
	scanf("%d", &option);
	
	if(option>=1&&option<7){
		while(option>=1&&option<7){		//restriction for option selection in menu
			switch(option){					//activate menu
				case 1:							//execute purchase items
				purchaseItem();
				menu();								//return to menu
				scanf("%d", &option);
				break;								//break
		
				case 2:							//execute edit items 
				editItems();
				menu();								//return to menu
				scanf("%d", &option);
				break;								//break
		
				case 3:							//execute add items 
				addItems();
				menu();								//return to menu
				scanf("%d", &option);
				break;								//break
		
				case 4:							//execute delete items 
				delItems();
				menu();								//return to menu
				scanf("%d", &option);
				break;								//break
		
				case 5:							//execute item inventory 
				itemInven();
				menu();								//return to menu
				scanf("%d", &option);
				break;								//break
		
				case 6:							//execute daily transaction
				dailyTrans();
				menu();								//return to menu
				scanf("%d", &option);
				break;								//break
		
				case 7:							//exit option
				exit(0);							//exit program
				break;								//break
			}
		}
	}else{
		printf("Invalid option requested. Enter a number from 1 to 7 only.");
		scanf("%d", &option);
	}
}

void menu(){						//menu layout function
	printf("\n\n------------------------------------\n");
	printf("Grocery Retail\n");
	printf("------------------------------------\n");
	printf("   1. Purchase items\n");
	printf("   2. Edit items\n");
	printf("   3. Add items\n");
	printf("   4. Delete items\n");
	printf("   5. Show inventory\n");
	printf("   6. Show daily transaction\n");
	printf("   7. Exit\n");
	printf("\nSelect an option from 1 to 7 only. Enter the corresponding number.\n");
}

void purchaseItem(){					//purchase items function        	
	int userQty;							//initializes variables 
	char userItemCode[10]={0};				//array for item codes obtained from input
	char itemCode[10];
	char itemName[40];
	float price;
	float subtotal=0;
	float total=0;
	int numOfTransaction=0;
	int updQty;
	float salesgst=0;
	float salesngst =0;
	float gst=0;
	
	FILE *gstPtr;  							//initialize file pointers to files				
	FILE *ngstPtr; 								
	FILE *receiptPtr;
	FILE *purcPtr;
	
	receiptPtr = fopen ("receipt.txt", "w");  		//writes to file
	purcPtr = fopen ("purchase.txt", "w");
	gstPtr = fopen ("gst.txt", "r");				//reads file
	ngstPtr = fopen ("ngst.txt", "r");				
	receiptPtr = fopen ("receipt.txt", "a");		//adds to file
				   	
	printf ("Enter Item Code and Quantity (Enter -1 TWICE to end):  "); //Instructs users to input item code and quantity of item
	scanf ("%s %d", userItemCode, &userQty);
	
							
	while ((!strcmp(userItemCode, "-1")) && userQty != -1){ //loop repeats until -1 is entered twice						               
									               //set flag
		int eof=0; 					               //flag ensures another iteration added on the last line to compare against user input
    	int check=0; 				               //flag to check invalid input flag
		while ((!feof(gstPtr) && !feof(ngstPtr)) || eof==0){	
			if((feof(gstPtr)&&feof(ngstPtr))){
				eof=1;
			}
			while (!feof(gstPtr)){	
				fscanf (gstPtr, "%[^;];%[^;];%f;%d\n", gstItems.itemCode, gstItems.itemName, &gstItems.itemPrice, &gstItems.itemQty);	//fscanf reads the input from gstPtr
				if (!strcmp(gstItems.itemCode, userItemCode)){				//compares user input string against available string																								
					if(userQty > gstItems.itemQty){															//if entered quantity of item exceeds the available quantity...		
						printf("The maximum quantity of this item is %d. Please try again.\n", gstItems.itemQty);//notify user of invalid input
					}else{																		//else...
						check=1;
						subtotal = gstItems.itemPrice * (float)userQty; 											//calculates the subtotal of the item
	 					printf("\nUnit Price: RM %.2f \t  Subtotal: RM%.2f\n\n", gstItems.itemPrice, subtotal);		
						//gstItems.itemQty-=userQty;
						fprintf (receiptPtr, "%s;%s;%.2f;%d;%.2f\n",gstItems.itemCode, gstItems.itemName, gstItems.itemPrice, userQty, subtotal);		
						fprintf (purcPtr, "%s;%s;%.2f;%d\n",gstItems.itemCode, gstItems.itemName, gstItems.itemPrice, userQty);		
						total += subtotal;															//calculates the total amount		
						salesgst = total; 															//total amount assigned to amount made from selling taxable items
						numOfTransaction+=userQty;													//transactions incremented by 1 for every different item purchased
						gst+=(userQty*(gstItems.itemPrice*0.06));												//gst price calculations
					}
				} 
			}while (!feof(ngstPtr)){	        
	 			fscanf (ngstPtr, "%[^;];%[^;];%f;%d\n", ngstItems.itemCode, ngstItems.itemName, &ngstItems.itemPrice, &ngstItems.itemQty);	
				if (!strcmp(ngstItems.itemCode, userItemCode)){
					if(userQty > ngstItems.itemQty){
						printf("The maximum quantity of this item is %d. Please try again.\n", ngstItems.itemQty);
					}else{	
						check=1;
					    subtotal = ngstItems.itemPrice * (float)userQty; 
					    printf("\nUnit Price: RM %.2f \t  Subtotal: RM%.2f\n", ngstItems.itemPrice, subtotal);
						fprintf (receiptPtr, "%s;%s;%.2f;%d;%.2f\n", ngstItems.itemCode, ngstItems.itemName, ngstItems.itemPrice, userQty, subtotal);
						fprintf (purcPtr, "%s;%s;%.2f;%d\n",ngstItems.itemCode, ngstItems.itemName, ngstItems.itemPrice, userQty);		
						total += subtotal;
						salesngst = total - salesgst; 				//sales of non-taxable items calculations
						numOfTransaction+=userQty;  
					}
				} 
			}
		} 
		if(check==0){
			printf("Invalid item or quantity.\n");
		}
		gstPtr = fopen ("gst.txt", "r");
		ngstPtr = fopen ("ngst.txt", "r");
					
		printf ("Enter Item Code and Quantity (Enter -1 TWICE to end):  ");
		scanf ("%s %d", userItemCode, &userQty);			
	}
	fclose(purcPtr);					
	printf("---------------------------------------------------------------- \n \t\t\t\tReceipt \n---------------------------------------------------------------- \n");   //print receipt	
	printf ("Item Code\tItem Name\tUnit Price\tQty\tSubtotal\n");
	fclose(receiptPtr);
	receiptPtr = fopen ("receipt.txt", "r");					//reads purchased items from file
	while (!feof(receiptPtr)){
		fscanf (receiptPtr, "%[^;];%[^;];%f;%d;%f\n", itemCode, itemName, &price, &userQty, &subtotal);
		printf ("%s%20s\tRM%5.2f\t     %5d\tRM%5.2f\n", itemCode, itemName, price, userQty, subtotal);
	}
	
	printf("\nAmount of taxable items\t\t:RM %.2f", salesgst);
	printf("\nAmount of non-taxable items\t:RM %.2f", salesngst);
	printf ("\nTotal\t\t\t\t:RM %.2f", total);  
	printf("\nTotal Sales with GST\t\t:RM %.2f\n", ((total)+(gst)));
							
	int t_salesgst = salesgst;
	int t_salesngst = salesngst;
	int t_total = total;
	
	t_transaction=numOfTransaction;
	salesTgst=salesgst;
	salesTngst=salesngst;
	t_Gst=gst;
							
	fclose(gstPtr);
	fclose(ngstPtr);
    fclose(receiptPtr);           	//close receipt file
}

void editItems(){							//edit items function
	char itemCode[10];							//variables
	char userCode[10];
	char itemName[40];
	char nitemName[40];
	int select;
	int qty;
	int nitemQty;
	int checker=0;
	float price;
	float nitemPrice;
	
	FILE *gstPtr = fopen ("gst.txt", "r");			//initializes file pointers and reads files
	FILE *ngstPtr = fopen ("ngst.txt", "r");
	FILE *gtempPtr = fopen ("gtemp.txt", "w");		//initializes file pointers and writes to files
	FILE *ngtempPtr = fopen ("ngtemp.txt", "w");
	
	printf("Enter the item code for the respective item to be edited.");		//requests user to enter item code
	scanf("%s", userCode);
	
	if(gstPtr!=NULL){																			//if gst file is there...
		while(!feof(gstPtr)){																		//execute code while gst file is open
			fscanf (gstPtr, "%[^;];%[^;];%f;%d\n", itemCode, itemName, &price, &qty);					//reads the item info from gst file
			if(strcmp(itemCode, userCode)==0){															//if entered item code matches item code in gst file...
				printf("\nSelect an option by entering the corresponding number.");
				printf("\n   1. Edit item name\n   2. Edit item price\n   3. Edit item quantity\n");		//requests user to select what to edit
				scanf("%d", &select);
				checker=1;
				if(select==1){																				//if edit item name is selected...	
					printf("Enter new item name: ");															//requests user to enter new item name
					scanf("%s", nitemName);
					fprintf(gtempPtr, "%s;%s;%.2f;%d\n", itemCode, nitemName, price, qty);						//writes new item name along with item info into temp.gst file
				}else if(select==2){																		//else if edit price is selected...
					printf("Enter new item price: ");															//requests user to enter new item price
					scanf("%f", &nitemPrice);
					fprintf(gtempPtr, "%s;%s;%.2f;%d\n", itemCode, itemName, nitemPrice, qty);					//writes new item price along with item info into temp.gst file
				}else if(select==3){																		//else if edit quantity is selected...
					printf("Enter new item quantity: ");														//requests user to enter new item quantity
					scanf("%d", &nitemQty);
					fprintf(gtempPtr, "%s;%s;%.2f;%d\n", itemCode, itemName, price, nitemQty);					//writes new item quantity along with item info into temp.gst file
				}else{																						//else...
					printf("Invalid option selected.");															//notifies user that invalid option was selected
				}
			}else{																						//else...
				fprintf(gtempPtr, "%s;%s;%.2f;%d\n", itemCode, itemName, price, qty);						//write item info into temp.gst file
			}														
		}
	}else{																					//else...
		printf("File could not be opened.");													//display file cannot be opened
	}
	fclose(gstPtr);																			//close gst file
	fclose(gtempPtr);																		//close temporary gst file
	
	remove("gst.txt");																		//delete gst file
	rename("gtemp.txt", "gst.txt");															//rename temp.gst file to replace gst file
	
	if(ngstPtr!=NULL){																		//if non-gst file is there...
		while(!feof(ngstPtr)){																	//execute code while non-gst file is open
			fscanf (ngstPtr, "%[^;];%[^;];%f;%d\n", itemCode, itemName, &price, &qty);				//reads the item info from non-gst file
			if(strcmp(itemCode, userCode)==0){														//if entered item code matches item code in non-gst file...
				checker=1;
				printf("\nSelect an option by entering the corresponding number.");
				printf("\n   1. Edit item name\n   2. Edit item price\n   3. Edit item quantity\n");	//requests user to select what to edit
				scanf("%d", &select);
				if(select==1){																			//if edit item name is selected...	
					printf("Enter new item name: ");														//requests user to enter new item name
					scanf("%s", nitemName);	
					fprintf(ngtempPtr, "%s;%s;%.2f;%d\n", itemCode, nitemName, price, qty);					//writes new item name along with item info into temp.non-gst file
				}else if(select==2){																	//else if edit price is selected...
					printf("Enter new item price: ");														//requests user to enter new item price
					scanf("%f", &nitemPrice);
					fprintf(ngtempPtr, "%s;%s;%.2f;%d\n", itemCode, itemName, nitemPrice, qty);				//writes new item price along with item info into temp.non-gst file
				}else if(select==3){																	//else if edit quantity is selected...
					printf("Enter new item quantity: ");													//requests user to enter new item quantity
					scanf("%d", &nitemQty);														
					fprintf(ngtempPtr, "%s;%s;%.2f;%d\n", itemCode, itemName, price, nitemQty);				//writes new item quantity along with item info into temp.non-gst file
				}else{																					//else...
					printf("Invalid option selected.");														//notifies user that invalid option was selected
				}
			}else{																					//else...
				fprintf(ngtempPtr, "%s;%s;%.2f;%d\n", itemCode, itemName, price, qty);					//write item info into temp.non-gst file
			}
		}
	}else{																					//else...
		printf("File could not be opened.");													//display file cannot be opened
	}
	fclose(ngstPtr);																		//close non-gst file
	fclose(ngtempPtr);																		//close temporary non-gst file
	
	remove("ngst.txt");																		//delete non-gst file
	rename("ngtemp.txt", "ngst.txt");														//rename temp.non-gst file to replace non-gst file
	
	if(checker==0){																			//checks if item was found in files
		printf("\nItem not found.\n");
	}
}

void addItems(){									//add items function
	FILE *gstPtr=fopen ("gst.txt", "r");				//initializes file pointers and reads files
	FILE *ngstPtr=fopen ("ngst.txt", "r");	
	gstPtr=fopen ("gst.txt", "a");						//initializes file pointers and adds to files
	ngstPtr=fopen ("ngst.txt", "a");	
	
	int option;											//variables
	char itemCode[10];									
	char userCode[10];
	char itemName[40];
	char nitemName[40];
	float price;
	float nitemPrice;
	int qty;
	int nitemQty;
	
	printf("Select an option by entering the corresponding number.");
	printf("\n   1. Add new item to gst text file\n   2. Add item to non-gst text file\n");		//requests user to select text file to add onto
	scanf("%d", &option);
	
	if(option==1){																				//if gst file is selected...
		printf("Enter new gst item code: ");														//request user to enter new item code
		scanf("%s", userCode);
		if(gstPtr!=NULL){																			//if gst file is there...
			while(!feof(gstPtr)){																		//execute code while gst file is open
				fscanf (gstPtr, "%[^;];%[^;];%f;%d\n", itemCode, itemName, &price, &qty);				//reads the item info from gst file
				if(strcmp(itemCode, userCode)==0){														//if entered item code matches item code in gst file...
					printf("Item already exists.");															//informs user that the item already exists
				}else{																					//else...
					printf("Enter the new item name: ");													//requests user to enter new item name
					scanf("%s", nitemName);
					printf("Enter the new item price: ");													//requests user to enter new item price
					scanf("%f", &nitemPrice);							
					printf("Enter the new item quantity: ");												//requests user to enter new item quantity
					scanf("%d", &nitemQty);						
					break;																					//break
				}								
			}
			fprintf(gstPtr, "\n%s;%s;%.2f;%d", userCode, nitemName, nitemPrice, nitemQty);				//write new item info into gst file
			fclose(gstPtr);																				//close gst file
		}else{																						//else...
			printf("File could not be opened.");														//display file cannot be opened
		}
	}else if(option==2){																		//else if non-gst file is selected...														
		printf("Enter new non-gst item code: ");													//requests user to enter new item code
		scanf("%s", userCode);						
		if(ngstPtr!=NULL){																			//if non-gst file is there...
			while(!feof(ngstPtr)){																	//execute code while non-gst file is open
				fscanf (ngstPtr, "%[^;];%[^;];%f;%d\n", itemCode, itemName, &price, &qty);				//reads the item info from non-gst file
				if(strcmp(itemCode, userCode)==0){														//if entered item code matches item code in non-gst file...
					printf("Item already exists.");															//informs user that the item already exists
				}else{																					//else...
					printf("Enter the new item name: ");													//requests user to enter new item name
					scanf("%s", nitemName);
					printf("Enter the new item price: ");													//requests user to enter new item price
					scanf("%f", &nitemPrice);
					printf("Enter the new item quantity: ");												//requests user to enter new item quantity
					scanf("%d", &nitemQty);
					break;																					//break
				}					
			}
			fprintf(ngstPtr, "\n%s;%s;%.2f;%d", userCode, nitemName, nitemPrice, nitemQty);			//write new item info into non-gst file
			fclose(ngstPtr);																		//close non-gst file
		}else{																					//else...
			printf("File could not be opened.");													//display file cannot be opened
		}
	}else{																					//else...
		printf("Invalid option selected.");														//informs user invalid data was enter
	}
}

void delItems(){							//delete items function
	char itemCode[10];							//variables		
	char userCode[10];
	char itemName[40];
	int checker;
	float price;
	int qty;
	
	FILE *gstPtr=fopen("gst.txt", "r");			//initializes file pointers and reads files		
	FILE *ngstPtr=fopen("ngst.txt", "r");
	FILE *gtempPtr=fopen("gtemp.txt","w");		//initializes file pointers and writes to files
	FILE *ngtempPtr=fopen("ngtemp.txt","w");
	
	printf("Enter the item code for the respective item to be deleted.");					//requests user to enter item code
	scanf("%s", userCode);
	
	if(gstPtr!=NULL){																	//if gst file is there...
		while(!feof(gstPtr)){																//execute code while gst file is open
			fscanf (gstPtr, "%[^;];%[^;];%f;%d\n", itemCode, itemName, &price, &qty);			//reads the item info from gst file
			if(strcmp(itemCode, userCode)==0){													//if entered item code matches item code in gst file...
				if(qty==0){																			//if item quantity is zero...
					printf("Do you want to delete this item? Enter 1 or 0. (1=Yes and 0=No)");			//requests user to confirm to delete item or not
					scanf("%d", &checker);													
					if (checker==0){																	//if user does not want to delete item...
						fprintf(gtempPtr, "%s;%s;%.2f;%d\n", itemCode, itemName, price, qty);				//write item info into temp.gst file
					}else if (checker!=1 && checker!=0){												//else if user enters invalid data instead of confirming to delete item or not...
						printf("Invalid input entered.");													//notifies user that invalid data was entered
					}
				}else{																				//else...
					printf("Item quantity is not zero.");												//notifies user item quantity for selected item is not zero
					fprintf(gtempPtr, "%s;%s;%.2f;%d\n", itemCode, itemName, price, qty);				//write item info into temp.gst file
				}
			}else{																				//else...
				fprintf(gtempPtr, "%s;%s;%.2f;%d\n", itemCode, itemName, price, qty);				//write item info into temp.gst file
			}
		}
	}else{																					//else...
		printf("File could not be opened.");													//display file cannot be opened
	}
	fclose(gstPtr);																		//close gst file
	fclose(gtempPtr);																	//close temporary gst file
	
	remove("gst.txt");																	//delete gst file
	rename("gtemp.txt", "gst.txt");														//rename temp.gst file to replace gst file
	
	if(ngstPtr!=NULL){																	//if non-gst file is there...
		while(!feof(ngstPtr)){																//execute code while non-gst file is open
			fscanf (ngstPtr, "%[^;];%[^;];%f;%d\n", itemCode, itemName, &price, &qty);			//reads the item info from non-gst file
			if(strcmp(itemCode, userCode)==0){													//if entered quantity of item exceeds the available quantity...		
				if(qty==0){																			//if item quantity is zero...	
					printf("Do you want to delete this item? Enter 1 or 0. (1=Yes and 0=No)");			//requests user to confirm to delete item or not
					scanf("%d", &checker);											
					if (checker==0){																	//if user does not want to delete item...
						fprintf(ngtempPtr, "%s;%s;%.2f;%d\n", itemCode, itemName, price, qty);				//write item info into temp.non-gst file
					}else if (checker!=1 && checker!=0){												//else if user enters invalid data instead of confirming to delete item or not...
						printf("Invalid input entered.");													//notifies user that invalid data was entered
					}
				}else{																				//else...
					printf("Item quantity is not zero.");												//notifies user item quantity for selected item is not zero
					fprintf(ngtempPtr, "%s;%s;%.2f;%d\n", itemCode, itemName, price, qty);				//write item info into temp.non-gst file
				}
			}else{																				//else...
				fprintf(ngtempPtr, "%s;%s;%.2f;%d\n", itemCode, itemName, price, qty);				//write item info into temp.non-gst file
			}
		}
	}else{																				//else...
		printf("File could not be opened.");												//display file cannot be opened
	}	
	fclose(ngstPtr);																		//close non-gst file
	fclose(ngtempPtr);																		//close temporary non-gst file
	
	remove("ngst.txt");																		//delete non-gst file
	rename("ngtemp.txt", "ngst.txt");														//rename temp.non-gst file to replace non-gst file
}

void itemInven(){							//show inventory function
	FILE *gstPtr=fopen("gst.txt", "r");			//initializes file pointers and reads files		
	FILE *ngstPtr=fopen("ngst.txt", "r");
	
	char itemCode [10];							//variables		
	char itemName [40];
	float price;
	int qty;
	if(gstPtr!=NULL){																	//if file is there...
		printf("\nTaxable items\n");														//prints taxable items in inventory
		while(!feof(gstPtr)){																//execute code while gst file is open
			fscanf(gstPtr, "%[^;]; %[^;] ; %f ; %d\n", itemCode, itemName, &price, &qty);		//reads file and disregards ;
			printf("%s\t %s %.2f %d\n", itemCode, itemName, price, qty);						//prints data on times excluding ;
		}
	}else{																				//else...
		printf("File could not be opened.");												//display file cannot be opened
	}
	fclose(gstPtr);																			//closes gst file
	if(gstPtr!=NULL){																		//if file is there...
		printf("\nNon-taxable items\n");														//prints non-taxable items in inventory
		while(!feof(ngstPtr)){																	//execute code while non-gst file is open
			fscanf(ngstPtr, "%[^;]; %[^;] ; %f ; %d\n", itemCode, itemName, &price, &qty);			//reads file and disregards ;
			printf("%s\t %s %.2f %d\n", itemCode, itemName, price, qty);							//prints data on items excluding ;
		}
	}else{																					//else...
		printf("File could not be opened.");													//display file cannot be opened
	}
	fclose(ngstPtr);																		//closes non-gst file
}

void dailyTrans(){							//daily transaction function	
	char itemCode [10];							//variables		
	char itemName [40];
	float price;
	int qty;
	
	FILE *purcPtr = fopen ("purchase.txt", "r");										//initializes file pointer and reads file	
	
	if(purcPtr!=NULL){																	//if file is there...
		printf("\nDaily Transactions\n");													//prints daily transactions
		while(!feof(purcPtr)){																//execute code while purchase file is open
			fscanf(purcPtr, "%[^;]; %[^;] ; %f ; %d\n", itemCode, itemName, &price, &qty);		//reads file and disregards ;
			printf("%s\t %s %.2f %d\n", itemCode, itemName, price, qty);						//prints data of items purchased daily excluding ;
		}
	}else{																				//else...
		printf("File could not be opened.");												//display file cannot be opened
	}
	fclose(purcPtr);																	//close file
				
	printf ("\nTotal transaction\t\t:\t%d", t_transaction);			//prints total transactions	
	printf("\nSales with GST\t\t\t:\tRM %.2f", salesTgst);			//prints total sales with GST
	printf("\nSales without GST\t\t:\tRM %.2f", salesTngst);		//prints total sales without GST
	printf("\nGST collected\t\t\t:\t%.2f", t_Gst);					//prints total GST
}
