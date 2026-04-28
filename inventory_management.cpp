//PROJECT 
//Inventory Management System
// L1F23BSCS0516
//ZAIN KASHIF 
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
using namespace std;

const int maxLenitem = 1000;
const int maxLen = 100;

void prompt();
void MENU();
int length(char*);
void add_item(char*);
void display_inventory(char*);
void update_item(char*);
void copy(char*, char*);
bool compare(char*, char*);
void delete_item(char*);
void Enter_char_letters(char*);
void search_item(char*);
void Enter_double_value(double&);
void Enter_integer_value(int&);
void freememory();
void prompt1();
void prompt2();
void prompt3();
void prompt4();
void prompt5();

char** product_name = new char*[maxLenitem];
double* product_quantity = new double[maxLenitem];
char** product_id = new char*[maxLenitem];
char** product_weight = new char*[maxLenitem];
int* product_price = new int[maxLenitem];
int item_count = 0;


int main() {
	char path[] = "inventory.csv";
	prompt();
	char ch = '\0';
	int choice = 0;

	for (int i = 0; i < maxLenitem; ++i) {
		product_name[i] = new char[maxLen];
		product_id[i] = new char[maxLen];
		product_weight[i] = new char[maxLen];
	}

	do {
		MENU();
		cin >> choice;
		cout << endl;
		switch (choice) {
		case 1:
			add_item(path);
			break;
		case 2:
			delete_item(path);
			break;
		case 3:
			update_item(path);
			break;
		case 4:
			search_item(path);
			break;
		case 5:
			display_inventory(path);
			break;
		case 6:
			ch = 'n';
			prompt5();
			break;
		default:
			prompt1();
			break;
		}
		prompt2();
		cin >> ch;
	} while (ch == 'y' || ch == 'Y');

	prompt3();

	freememory();
	return 0;
}

void Enter_integer_value(int&arr) {
	cin >> arr;
}

void Enter_char_letters(char* arr) {
	cin >> arr;
}

void Enter_double_value(double& arr) {
	cin >> arr;
}

int length(char* str) {
	int len = 0;
	while (str[len] != '\0') {
		len++;
	}
	return len;
}

void add_item(char* path) {
	if (item_count >= maxLenitem) {
		cout << "Maximum inventory size reached!" << endl;
		return;
	}

	ofstream writer(path, ios::app);
	if (!writer.is_open()) {
		cout << "Error opening file for writing! " << path << endl;
		return;
	}
	cout << "Enter details of the new item:" << endl;
	cout << "Product Name: ";
	Enter_char_letters(product_name[item_count]);
	cout << "Product price: ";
	Enter_integer_value(product_price[item_count]);
	cout << "Product quantity: ";
	Enter_double_value(product_quantity[item_count]);
	cout << "Product weightage measurement(e.g. Ltr,kg,Pcs,Box): ";
	Enter_char_letters(product_weight[item_count]);
	cout << "Product ID(code): ";
	Enter_char_letters(product_id[item_count]);

	writer << product_name[item_count] << ","
           << product_price[item_count] << ","
           << product_quantity[item_count] << ","
           << product_weight[item_count] << ","
           << product_id[item_count] << endl;
	writer.close();

	cout << "Item added successfully in your inventory!" << endl;
	item_count++;
}

void delete_item(char* path) {
	ifstream reader(path);
	if (!reader.is_open()) {
		cout << "Error opening file for reading! " << path << endl;
		return;
	}
	
	char input[maxLen];
	cout << "Enter item name or code to delete: ";
	Enter_char_letters(input);

	// Read all items from CSV
	item_count = 0;
	char line[500];
	
	// Skip any empty lines
	while (reader.getline(line, 500)) {
		if (strlen(line) > 0) {
			char* token = strtok(line, ",");
			if (token) {
				copy(product_name[item_count], token);
				token = strtok(NULL, ",");
				if (token) product_price[item_count] = atoi(token);
				token = strtok(NULL, ",");
				if (token) product_quantity[item_count] = atof(token);
				token = strtok(NULL, ",");
				if (token) copy(product_weight[item_count], token);
				token = strtok(NULL, ",");
				if (token) copy(product_id[item_count], token);
				item_count++;
			}
		}
	}
	reader.close();

	bool found = false;
	int delete_index = -1;
	for (int i = 0; i < item_count; ++i) {
		if (compare(product_id[i], input) || compare(product_name[i], input)) {
			found = true;
			delete_index = i;
			break;
		}
	}

	if (found) {
		for (int i = delete_index; i < item_count - 1; ++i) {
			copy(product_name[i], product_name[i + 1]);
			product_price[i] = product_price[i + 1];
			product_quantity[i] = product_quantity[i + 1];
			copy(product_weight[i], product_weight[i + 1]);
			copy(product_id[i], product_id[i + 1]);
		}
		item_count--;
		
		ofstream writer(path);
		if (!writer.is_open()) {
			cout << "Error opening file for writing!" << endl;
			return;
		}
		
		for (int i = 0; i < item_count; ++i) {
			writer << product_name[i] << ","
                   << product_price[i] << ","
                   << product_quantity[i] << ","
                   << product_weight[i] << ","
                   << product_id[i] << endl;
		}
		writer.close();
		cout << "Item deleted successfully from Inventory." << endl;
	}
	else {
		cout << "No Item found in Inventory." << endl;
	}
}

void update_item(char* path) {
	int n = 0;
	ifstream reader(path);
	if (!reader.is_open()) {
		cout << "Error opening file for reading! " << path << endl;
		return;
	}

	char input[maxLen];
	cout << "Enter item name or code to update: ";
	Enter_char_letters(input);

	// Read all items from CSV
	item_count = 0;
	char line[500];
	
	while (reader.getline(line, 500)) {
		if (strlen(line) > 0) {
			char* token = strtok(line, ",");
			if (token) {
				copy(product_name[item_count], token);
				token = strtok(NULL, ",");
				if (token) product_price[item_count] = atoi(token);
				token = strtok(NULL, ",");
				if (token) product_quantity[item_count] = atof(token);
				token = strtok(NULL, ",");
				if (token) copy(product_weight[item_count], token);
				token = strtok(NULL, ",");
				if (token) copy(product_id[item_count], token);
				item_count++;
			}
		}
	}
	reader.close();

	bool found = false;
	int update_index = -1;
	for (int i = 0; i < item_count; ++i) {
		if (compare(product_id[i], input) || compare(product_name[i], input)) {
			found = true;
			update_index = i;
			break;
		}
	}
	
	if (found) {
		cout << "Current details of the item:" << endl;
		cout << "Product Name: " << product_name[update_index] << endl;
		cout << "Product Price: " << product_price[update_index] << endl;
		cout << "Product Quantity: " << product_quantity[update_index] << " " << product_weight[update_index] << endl;
		cout << "Product Code: " << product_id[update_index] << endl;
		cout << "Enter 1 to update quantity, 2 to update price, 3 to update name, 4 to update code: ";
		cin >> n;
		
		if (n == 1){
			cout << "Enter new quantity: ";
			Enter_double_value(product_quantity[update_index]);
		}
		if (n == 2){
			cout << "Enter new price: ";
			Enter_integer_value(product_price[update_index]);
		}
		if (n == 3){
			cout << "Enter new product name: ";
			Enter_char_letters(product_name[update_index]);
		}
		if (n == 4){
			cout << "Enter new code: ";
			Enter_char_letters(product_id[update_index]);
		}
		
		ofstream writer(path);
		if (!writer.is_open()) {
			cout << "Error opening file for writing! " << path << endl;
			return;
		}
		
		for (int i = 0; i < item_count; ++i) {
			writer << product_name[i] << ","
                   << product_price[i] << ","
                   << product_quantity[i] << ","
                   << product_weight[i] << ","
                   << product_id[i] << endl;
		}
		writer.close();
		cout << "Item updated successfully in your inventory." << endl;
	}
	else {
		cout << "No Item found in Inventory." << endl;
	}
}

void search_item(char* path) {
	ifstream reader(path);
	if (!reader.is_open()) {
		cout << "Error opening file for reading! " << path << endl;
		return;
	}

	char searchInput[maxLen];
	cout << "Enter item name, code, or price to search: ";
	Enter_char_letters(searchInput);

	char prod[maxLen], pri[maxLen], qua[maxLen], wei[maxLen], cod[maxLen];
	bool found = false;
	char line[500];

	cout << "\n==================================================" << endl;
	
	while (reader.getline(line, 500)) {
		if (strlen(line) > 0) {
			char* token = strtok(line, ",");
			if (token) {
				copy(prod, token);
				token = strtok(NULL, ",");
				if (token) copy(pri, token);
				token = strtok(NULL, ",");
				if (token) copy(qua, token);
				token = strtok(NULL, ",");
				if (token) copy(wei, token);
				token = strtok(NULL, ",");
				if (token) copy(cod, token);
				
				if (compare(prod, searchInput) || compare(cod, searchInput) || compare(pri, searchInput)) {
					cout << "Product Name: " << prod << endl;
					cout << "Product Price: " << pri << endl;
					cout << "Product Quantity: " << qua << " " << wei << endl;
					cout << "Product Code: " << cod << endl;
					cout << "--------------------------------------------------" << endl;
					found = true;
				}
			}
		}
	}
	
	if (!found) {
		cout << "Item not available in your inventory." << endl;
	}
	reader.close();
}

void display_inventory(char* path) {
	ifstream reader(path);
	if (!reader.is_open()) {
		cout << "Error opening file for reading! " << path << endl;
		return;
	}

	cout << "\n==================================================================================" << endl;
	cout << setw(25) << left << "Product Name" 
         << setw(10) << right << "Price" 
         << setw(12) << right << "Quantity" 
         << setw(12) << right << "Weight" 
         << setw(15) << right << "Code" << endl;
	cout << "==================================================================================" << endl;
	
	item_count = 0;
	char line[500];
	
	while (reader.getline(line, 500)) {
		if (strlen(line) > 0) {
			char* token = strtok(line, ",");
			if (token) {
				copy(product_name[item_count], token);
				token = strtok(NULL, ",");
				if (token) product_price[item_count] = atoi(token);
				token = strtok(NULL, ",");
				if (token) product_quantity[item_count] = atof(token);
				token = strtok(NULL, ",");
				if (token) copy(product_weight[item_count], token);
				token = strtok(NULL, ",");
				if (token) copy(product_id[item_count], token);
				
				cout << setw(25) << left << product_name[item_count]
				     << setw(10) << right << product_price[item_count]
				     << setw(12) << right << product_quantity[item_count]
				     << setw(12) << right << product_weight[item_count]
				     << setw(15) << right << product_id[item_count] << endl;
				item_count++;
			}
		}
	}
	
	cout << "==================================================================================" << endl;
	
	if (item_count == 0) {
		cout << "No items found in inventory!" << endl;
	}
	
	reader.close();
}

void copy(char* dest, char* src) {
	int i = 0;
	while (src[i] != '\0') {
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

bool compare(char* cstr1, char* cstr2) {
	int i = 0;
	while (cstr1[i] != '\0' && cstr2[i] != '\0' && cstr1[i] == cstr2[i]) {
		i++;
	}
	return cstr1[i] == cstr2[i];
}

void MENU() {
	cout << "\t\t\t+------------------------------------------------------------------+" << endl;
	cout << "\t\t\t|                             MAIN MENU                            |" << endl;
	cout << "\t\t\t+------------------------------------------------------------------+" << endl;
	cout << "\t\t\t| Press 1  -  To Add Item In Inventory                             |" << endl;
	cout << "\t\t\t| Press 2  -  To Delete Item From Inventory                        |" << endl;
	cout << "\t\t\t| Press 3  -  To Update Item In Inventory                          |" << endl;
	cout << "\t\t\t| Press 4  -  To Search Item In Inventory                          |" << endl;
	cout << "\t\t\t| Press 5  -  To Display All Items Information                     |" << endl;
	cout << "\t\t\t| Press 6  -  To EXIT Menu                                         |" << endl;
	cout << "\t\t\t+------------------------------------------------------------------+" << endl;
	cout << endl << endl;
	cout << "ENTER YOUR CHOICE" << endl;
	cout << "-----------------" << endl;
}

void prompt() {
	cout << "\t\t\t\t+-------------------------------------------------------+" << endl;
	cout << "\t\t\t\t|                <<INVENTORY MANAGEMENT>>               |" << endl;
	cout << "\t\t\t\t+-------------------------------------------------------+" << endl;
	cout << endl << endl << endl;
}

void prompt1(){
	cout << "Invalid choice! Please Enter Numbers between 1 to 6....." << endl;
}

void prompt2(){
	cout << "\n\nDo you want to see the menu again? (Enter y for (yes) to see menu, and Enter any n for (no) to exit ): ";
}

void prompt3(){
	cout << "Your data is saved successfully in CSV format!!." << endl;
}

void prompt4(){
	cout << "EXIT THE MENU SUCCESSFULLY!!!" << endl;
}

void prompt5(){
	cout << "EXIT! THE MENU SUCCESSFULLY" << endl;
}

void freememory(){
	for (int i = 0; i < maxLenitem; ++i) {
		delete[] product_name[i];
		delete[] product_id[i];
		delete[] product_weight[i];
	}

	delete[] product_name;
	delete[] product_id;
	delete[] product_weight;
	delete[] product_quantity;
	delete[] product_price;
}