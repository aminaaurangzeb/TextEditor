#include "graphics.h"
#include <iostream>
#include <fstream>
using namespace std;
const int lineSpace = 20;
const int pageLength = 650;
const int pageHieght = 450;
const int charSpace = 10;
struct cpyNode{//for cut paste and copy paste
	char ch;
	cpyNode* nxt;
};
struct node {
	char ch[2];
	node * next, *prev;
	int x, y;
};
void newPage(){//new rectangle
	cleardevice();
	rectangle(10, 5, 700, 485);
}
void print(node* curr, node * head, int  x, int y){//prints list
	newPage();
	x = head->x; y = head->y;
	node * temp = head;
	while (temp != nullptr)	{
		if (temp->ch[0] == 13){
			x = temp->x;
			y += lineSpace;
			moveto(x, y);
			outtext(temp->ch);
		}
		else if (x > 645){
			x = temp->x;
			y += lineSpace;
			moveto(x, y);
			outtext(temp->ch);
		}
		else{
			moveto(x, y);
			outtext(temp->ch);
			x += charSpace;
		}
		temp = temp->next;
	}
}
void removeCursor(int x, int y){
	setcolor(BLACK);
	line(x, y, x, y + 15);
	setcolor(WHITE);
}
void cursor(int x, int y){
	setcolor(WHITE);
	line(x, y, x, y + 15);
}
void removeChar(int x, int y){
	setfillstyle(1, BLACK);
	bar(x - charSpace - 2, y, x, y + 18);
	setcolor(WHITE);
}
void back(node * & curr, node*head, int &x, int &y){//Backspace 
	removeCursor(x, y);
	if (curr == head){
		if (head->next != nullptr){//only one member in list
			curr = head->next;
			curr->prev = nullptr;
			head = curr;
			delete head;
		}
		else{//normal
			delete head;
			delete curr;
			head = curr = nullptr;
		}
		removeCursor(x, y);
		removeChar(x, y);
		x -= charSpace;
		cursor(x, y);
	}
	else if (curr->next == nullptr){//at tail
		node* temp = curr;
		curr = curr->prev;
		curr->next = nullptr;
		delete temp;
		removeCursor; (x, y);
		removeChar(x, y);
		setcolor(WHITE);
		if (x == 25){
			x = (curr->x) + charSpace;
			y -= lineSpace;
			cursor(x, y);
		}
		else if (x == 15){
			x = 645;
			y -= lineSpace;
			cursor(x, y);
		}
		else{
			x -= charSpace; cursor(x, y);
		}
	}
	else{//in between
		node * text = curr->next;
		removeCursor(x, y);
		setcolor(WHITE);
		int x1 = x - 10, y1 = y;
		while (text != nullptr){
			if (x1 == 5){
				x1 = 645; y1 -= 20;
				text->x = x1;
				text->y = y1;
				x1 += charSpace;
			}
			else if (curr->ch[0] == 13){
				y1 = y1 - 20;
				x1 = (curr->prev->x) + 10;
				node* temp = text->next;
				while (temp != nullptr)	{
					if (temp->ch[0] == 13){
						y1 -= 20;
						temp->y = y;
					}
					else if (x1 > 650)	{
						x1 = 15;
						y1 += 20;
						temp->x = x1;
						temp->y = y1;
					}
					else {
						temp->x = x1;
						temp->y = y1;
						x1 += 10;
					}
					temp = temp->next;
				}
			}
			else if (x < 650){
				if (text->ch[0] == 13)	{
					x1 = 15; y1 += 20;
					text->x = 15;
					text->y = y1;
				}
				else{
					text->x = x1;
					text->y = y1;
					x1 -= 10;
				}
			}
			else {
				x1 = 15; y += 20;
				text->x = x1;
				text->y = y1;
				x1 += 10;
			}
			text = text->next;
		}
		node * text2 = curr;//now reestablishing link and removing node
		curr->prev->next = curr->next;
		curr->next->prev = curr->prev;
		curr = curr->prev;
		delete text2;
		if (x == 15){
			x = curr->x;
			y -= 20;
		}
		else
			x -= 10;
		print(curr, head, x, y);//print list
		cursor(x, y);
	}
}

void addNode(node * & head, node * & curr, char * ch, int x, int y){
	if (head == nullptr){ //empty list
		head = new node;
		head->ch[0] = ch[0];
		head->ch[1] = '\0';
		head->next = nullptr;
		head->prev = nullptr;
		curr = head;
		if (ch[0] == 13){
			curr->x = 15;
			curr->y = y + lineSpace;
		}
		else {
			curr->x = x;
			curr->y = y;
		}
	}
	else if (curr->next == nullptr){ //add at end
		node* temp = new node;
		temp->prev = curr;
		curr->next = temp;
		temp->next = nullptr;
		temp->ch[0] = ch[0];
		temp->ch[1] = '\0';
		curr = temp;
		if (ch[0] == 13){
			curr->x = 15;
			curr->y = y + lineSpace;
		}
		else {
			curr->x = x;
			curr->y = y;
		}
	}
	else if (curr->next != nullptr){ //middle
		node* temp3 = new node;
		temp3->next = curr->next;
		temp3->prev = curr;
		curr->next = temp3;
		temp3->next->prev = temp3;
		curr = temp3;
		curr->ch[0] = ch[0];
		curr->ch[1] = '\0';
		removeCursor(x, y);
		int x1 = x + charSpace;
		int y1 = y;
		node* text = curr;
		while (text != nullptr){
			if (text->ch[0] == 13){ //if enter is inseted
				node* temp = text;
				x1 = 15; y1 += lineSpace;
				while (temp != nullptr){//loop till tail
					if (temp->ch[0] == 13){ //another enter in between
						x1 = 15; y1 += lineSpace;
						temp->x = x1;
						temp->y = y1;
						x1 += charSpace;
					}
					else if (x1 > 650){//line end
						x1 = 15; y1 += lineSpace;
						temp->x = x1;
						temp->y = y1;
						x1 += charSpace;
					}
					else{//normal
						temp->x = x1;
						temp->y = y1;
						x1 += charSpace;
					}
					temp = temp->next;
				}
				text = temp;
			}
			else if (x1 > 650){
				x1 = 15; y1 += lineSpace;
				node * temp = text;
				while (temp != nullptr){//loop till tail
					if (temp->ch[0] == 13){ //another enter
						x1 = 15; y1 += lineSpace;
						temp->x = x1;
						temp->y = y1;
						x1 += charSpace;
					}
					else if (x1 > 650){//line end
						x1 = 15; y1 += lineSpace;
						temp->x = x1;
						temp->y = y1;
						x1 += charSpace;
					}
					else{//normal
						temp->x = x1;
						temp->y = y1;
						x1 += charSpace;
					}
					temp = temp->next;
				}
				text = temp;
			}
			else{
				text->x = x1;
				text->y = y1;
				x1 += charSpace;
				text = text->next;
			}
		}
		if (curr->ch[0] == 13){
			x = curr->x; y += lineSpace;
		}
		else if (x > 645) {
			x = 15; y += lineSpace;
		}
		else
			x += charSpace;
		setcolor(WHITE);
		print(curr, head, x, y);
		cursor(x, y);
	}
}
void endSelection(node* &sHead, node* &sTail, int right, int left){
	node* temp = new node;
	int x = sHead->x, y = sHead->y;
	if (right == 1){
		temp = sHead->prev;
		while (temp != sTail->next){
			removeCursor(x, y);
			if (temp->ch[0] == 13)	{//enter
				x = temp->next->x;
				y += lineSpace;
				setcolor(BLACK);
				line(temp->x, y + 20, temp->x + 15, y + 20);
				x += charSpace; cursor(x, y);
			}
			else if (x > pageLength){//line end
				x = 15; y += lineSpace;
				setcolor(BLACK);
				line(x, y + 20, temp->x + 15, y + 20);
				x += charSpace; cursor(x, y);
			}
			else{//normal
				setcolor(BLACK);
				line(temp->x, y + 20, temp->x + 15, y + 20);
				x += charSpace; cursor(x, y);
			}
			temp = temp->next;
			removeCursor(x, y);
		}
	}
	if (left == 1){
		temp = sHead;
		while (temp != sTail->prev){
			removeCursor(x, y);
			if (temp->ch[0] == 13){
				x = temp->x;
				y -= lineSpace;
				setcolor(BLACK);
				line(temp->x + 15, y + 20, x, y + 20);
				x -= charSpace; cursor(x, y);
			}
			else if (x == 15 && y>lineSpace){
				x = 645; y -= 20;
				setcolor(BLACK);
				line(x, y + 20, temp->x, y + 20);
				x -= charSpace; cursor(x, y);
			}
			else{
				setcolor(BLACK);
				line(temp->x, y + 20, temp->x + 15, y + 20);
				x -= charSpace; cursor(x, y);
			}
			temp = temp->prev;
			removeCursor(x, y);
		}
	}
	sTail = sHead = nullptr;
	setcolor(WHITE);
}
void shift(node* &curr, node * &sHead, node* &sTail, int &x, int &y, char sKey, int &right, int &left){
	sTail = curr;
	removeCursor(x, y);
	setcolor(WHITE);
	switch (sKey){
	case 77://right
		if (sTail != nullptr){
			if (sHead == nullptr)//sHead updated only once when shift pressed for the first time
				sHead = curr->next;
			right = 1;
			if (sTail->ch[0] == 13)	{//enter
				x = sTail->next->x;
				y += lineSpace;
				line(sTail->x, y + 20, sTail->x + 15, y + 20);
				x += charSpace; cursor(x, y);
			}
			else if (x > pageLength){//line end
				x = 15; y += lineSpace;
				line(x, y + 20, sTail->x + 15, y + 20);
				x += charSpace; cursor(x, y);
			}
			else{//normal
				line(sTail->x, y + 20, sTail->x + 15, y + 20);
				x += charSpace; cursor(x, y);
			}
			sTail = sTail->next;
		}
		curr = sTail;
		break;
	case 75: //left
		if (sTail != nullptr){
			if (sHead == nullptr)//sHead updated only once when shift pressed for the first time
				sHead = curr;
			left = 1;
			if (sTail->ch[0] == 13){
				x = sTail->x;
				y -= lineSpace;
				line(sTail->x + 15, y + 20, x, y + 20);
				x -= charSpace; cursor(x, y);
			}
			else if (x == 15 && y>lineSpace){
				x = 645; y -= 20;
				line(x, y + 20, sTail->x, y + 20);
				x -= charSpace; cursor(x, y);
			}
			else{
				line(sTail->x, y + 20, sTail->x + 15, y + 20);
				x -= charSpace; cursor(x, y);
			}
			sTail = sTail->prev;
		}
		curr = sTail;
		break;
	}
	cursor(x, y);
}

void Copy(node* sHead, node* sTail, cpyNode* &cHead, cpyNode* &cTail){
	cpyNode* head2 = nullptr;
	cpyNode* curr2 = head2;
	node* temp = sHead;
	while (temp != sTail->next){//created a copy of selection
		if (curr2 == head2 == NULL){
			head2 = new cpyNode;
			head2->ch = temp->ch[0];
			head2->nxt = nullptr;
			curr2 = head2;
		}
		else if (curr2 != NULL && curr2->nxt == nullptr){
			cpyNode* temp2 = new cpyNode;
			temp2->ch = temp->ch[0];
			curr2->nxt = temp2;
			temp2->nxt = nullptr;
			curr2 = curr2->nxt;
		}
		temp = temp->next;
	}
	cHead = head2;
	cTail = curr2;
}
void Cut(node* head, node* &curr, int& x, int &y, node* &sHead, node* &sTail, cpyNode* &cHead, cpyNode* &cTail){
	removeCursor(x, y);
	Copy(sHead, sTail, cHead, cTail);
	node* temp = sHead;
	while (temp != sTail->next){
		back(temp, head, x, y);
		temp = temp->next;
	}
	cursor(x, y);
}void Save(node* head){
	char fileName[20];
	setcolor(BLUE);
	rectangle(730, 200, 920, 400);
	setcolor(GREEN);
	moveto(770, 210);
	outtext(" |  SAVE AS  | ");
	moveto(760, 330);
	outtext("_________________");
	moveto(750, 260);
	outtext("Hit cursor key to save");
	int i = 0, x = 770, y = 320;
	char ch[2] = { '\0', '\0' };
	cursor(x, y);
	while (!kbhit()){
		ch[0] = getch();
		removeCursor(x, y);
		moveto(x, y);
		outtext(ch);
		x += charSpace;
		if (x >= 900){
			x = 750;
			y += lineSpace;
		}
		cursor(x, y);
		fileName[i] = ch[0];
		i++;
	}
	fileName[i] = '\0';
	ofstream fout(fileName);
	node* temp = head;
	while (temp->next != nullptr){
		fout << temp->ch[0] << endl;
		fout << temp->x << endl;
		fout << temp->y << endl;
		temp = temp->next;
	}
	setfillstyle(1, BLACK);
	bar(705, 100, 950, 410);
}
void Load(node* &head){
	char key[20];
	setcolor(BLUE);
	rectangle(730, 200, 920, 400);
	setcolor(GREEN);
	moveto(770, 210);
	outtext(" |  OPEN  | ");
	moveto(760, 330);
	outtext("_________________");
	moveto(750, 260);
	outtext("Hit cursor key to open");
	int i = 0, x = 770, y = 320;
	char ch[2] = { '\0', '\0' };
	cursor(x, y);
	while (!kbhit()){
		ch[0] = getch();
		removeCursor(x, y);
		moveto(x, y);
		outtext(ch);
		x += charSpace;
		if (x >= 900){
			x = 750;
			y += lineSpace;
		}
		cursor(x, y);
		key[i] = ch[0];
		i++;
	}
	key[i] = '\0';
	ifstream fin(key);
	node* tempHead = nullptr;
	node* curr = tempHead;
	while (fin.eof() != true){
		fin >> ch[0];
		fin >> x;
		fin >> y;
		addNode(tempHead, curr, ch, x, y);
	}
	head = tempHead;
	print(curr, tempHead, x, y);
	cursor(x, y);
	setfillstyle(1, BLACK);
	bar(705, 100, 950, 410);
}
void Find(node* head , node* & findloc){
	char find[20];
	//initwindow(300, 300, "FIND");
	setcolor(BLUE);
	rectangle(730, 200, 920, 400);
	setcolor(GREEN);
	moveto(770, 210);
	outtext(" |  LOOK FOR  | ");
	moveto(760, 330);
	outtext("_________________");
	moveto(750, 260);
	outtext("Hit cursor key to find");
	int i = 0, x = 770, y = 320;
	char ch[2] = { '\0', '\0' };
	cursor(x, y);
	while (!kbhit()){
		ch[0] = getch();
		removeCursor(x, y);
		moveto(x, y);
		outtext(ch);
		x += charSpace;
		cursor(x, y);
		find[i] = ch[0];
		i++;
	}
	//code
	node* temp = new node;
	temp = head;
	bool found=0;
	int k = 0;
	while (temp->next != nullptr && found == 0){
		k = 0;
		if (temp->ch[0] == find[k]){
			found = 1; node* temp2 = temp;
			for (int k = 0; find[k] != 0; k++){
				if (temp2->ch[0] == find[k]){
					k++;
					temp2 = temp2->next;
				}
				else{
					found = 0;
					break;
				}
			}
		}
		if (k == strlen(find) && found == 1){
			findloc = temp;
			break;
		}
		temp = temp->next;
	}
	if (found == 1){
		moveto(770, 120);
		outtext("FOUND");
	}
	else{
		moveto(770, 120);
		outtext("NO MATCH FOUND");
	}
	Sleep(2000);
	setfillstyle(1, BLACK);
	bar(705, 100, 950, 410);
	
}
void Replace(node* head,node* findloc,int &xMain,int &yMain){
	initwindow(300, 300, "Replace");
	char key[20];
	setcolor(BLUE);
	rectangle(10, 10, 270, 280);
	setcolor(GREEN);
	moveto(040, 20);
	outtext(" |  REPLACE WITH  | ");
	moveto(060, 160);
	outtext("_________________");
	moveto(040, 40);
	outtext("Hit cursor key to replace");
	int i = 0, x = 70, y = 120;
	char ch[2] = { '\0', '\0' };
	cursor(x, y);
	while (!kbhit()){
		ch[0] = getch();
		removeCursor(x, y);
		moveto(x, y);
		outtext(ch);
		x += charSpace;
		cursor(x, y);
		key[i] = ch[0];
		i++;
	}
	//code
	for (int j = 0; j < strlen(key); j++){
		ch[0] = key[j];
		addNode(head, findloc, ch, xMain, yMain); 
		outtext(ch);
		xMain += charSpace;
		cursor(xMain, yMain);
		findloc = findloc->next;
	}
	setfillstyle(1, BLACK);
	bar(5, 5, 280, 290);
}
void Paste(node* head, node* &curr, int &x, int&y, cpyNode* &cHead, cpyNode* &cTail){
	cpyNode* temp = cHead;
	char ch[2] = { '\0', '\0' };
	while (temp->nxt != nullptr){
		ch[0] = temp->ch;
		setcolor(WHITE);
		addNode(head, curr, ch, x, y);
		outtext(ch);
		x += charSpace;
		temp = temp->nxt;
	}
	cursor(x, y);
}
int main(){
	node* delInsertPoint;
	int delX;
	int delY;
	bool deletePressed = 0;
	node* head = nullptr;
	node* curr = head;
	node* findloc;
	initwindow(1000, 500, "FAST EDIT");
	newPage();
	int x = 15, y = 20, right = 0, left = 0; bool selected = 0;
	moveto(x, y);
	char ch[2] = { '\0', '\0' };
	char key; char sKey;
	node* sHead = nullptr; node* sTail = nullptr; cpyNode* cHead = nullptr; cpyNode* cTail = nullptr;
	while (true){
		right = 0; left = 0;//returns in which direction shift combinated
		while (GetAsyncKeyState(VK_LSHIFT) || GetAsyncKeyState(VK_RSHIFT))	{//Shift key down
			if ((kbhit() && GetAsyncKeyState(VK_LSHIFT)) || (kbhit() && GetAsyncKeyState(VK_RSHIFT))){//shift key and arrow key
				sKey = getch();
				removeCursor(x, y);
				shift(curr, sHead, sTail, x, y, sKey, right, left);
			}
			selected = 1;
		}
		ch[0] = getch();
		if (selected == 1){
			if (ch[0] == '\x03'){//copy
				Copy(sHead, sTail, cHead, cTail);
				ch[0] = getch();
			}
			if (ch[0] == '\x7F'){//delete
				delInsertPoint = curr;
				delX = x;
				delY = y;
				deletePressed = 1;
				Cut(head, curr, x, y, sHead, sTail, cHead, cTail);

				ch[0] = getch();
			}
			if (ch[0] == '\x18'){//cut

				Cut(head, curr, x, y, sHead, sTail, cHead, cTail);

				ch[0] = getch();
			}
		}

		if (ch[0] == '\x16'){//paste
			Paste(head, curr, x, y, cHead, cTail);
			selected = 0;

			ch[0] = getch();
		}
		if (ch[0] == '\x1A'){//ctrl+Z
			Paste(head, delInsertPoint, delX, delY, cHead, cTail);
			selected = 0;

			ch[0] = getch();
		}
		if (ch[0] == '\x13'){//save
			Save(head);

			ch[0] = getch();
		}
		if (ch[0] == '\x0C'){//load
			Load(head);

			ch[0] = getch();
		}
		if (ch[0] == '\x06'){//find
			Find(head,findloc);

			ch[0] = getch();
		}
		if (ch[0] == '\x12'){//replace
			Replace(head);

			ch[0] = getch();
		}
		if (kbhit()){
			if (sHead != NULL && sTail != NULL){
				endSelection(sHead, sTail, right, left);
			}
			key = getch();
			if (key == 75){ // move curr left
				if (curr->prev != nullptr){
					if (curr->ch[0] == 13){
						removeCursor(x, y);
						x = (curr->prev->x) + charSpace; y -= lineSpace;
						removeCursor(x, y);
						setcolor(WHITE);
						cursor(x, y);
					}
					else if (x == 15){
						removeCursor(x, y);
						x = 650; y -= lineSpace;
						removeCursor(x, y);
						setcolor(WHITE); cursor(x, y);
					}
					else{
						removeCursor(x, y);
						setcolor(WHITE);
						x -= charSpace;
						cursor(x, y);
					}
					curr = curr->prev;
				}
			}
			if (key == 72){//up
				int x1 = x, y1 = y;
				removeCursor(x, y);
				if (y >= 20 + lineSpace){
					for (int i = 0; i < pageLength; i += charSpace){
						if (curr->prev != nullptr){
							if (curr->ch[0] == 13){
								x = (curr->prev->x) + charSpace; y -= lineSpace;
								i = i + (pageLength - curr->x);
							}
							else if (x == 15){
								x = 650; y -= lineSpace;
								i = i + (pageLength - curr->x);
							}
							else{
								x -= charSpace;
							}
							curr = curr->prev;
						}
					}
				}
				setcolor(WHITE);
				cursor(x, y);
			}
			if (key == 80){//down
				int x1 = x, y1 = y;
				removeCursor(x, y);
				if (y <= pageHieght - lineSpace){
					for (int i = 0; i < pageLength; i += charSpace)
					if (curr->next != nullptr){
						curr = curr->next;
						if (curr->ch[0] == 13){ //border check
							x = curr->next->x; y += lineSpace;
							i = i + (pageLength - curr->x);
						}
						else if (x < 600 && y < 600){ //border check
							x += charSpace; cursor(x, y);
							i = i + (pageLength - curr->x);
						}
						else{
							x = 15; y += lineSpace;
						}
					}
				}
			}
			setcolor(WHITE);
			cursor(x, y);
		}
		if (key == 77){ //move right
			if (curr->next != nullptr){
				curr = curr->next;
				if (curr->ch[0] == 13){ //border check
					removeCursor(x, y);
					x = curr->next->x; y += lineSpace;
					removeCursor(x, y);
					setcolor(WHITE);
					cursor(x, y);
				}
				else if (x < 600 && y < 600){ //border check
					removeCursor(x, y);
					setcolor(WHITE);
					x += charSpace; cursor(x, y);
				}
				else{
					removeCursor(x, y);
					x = 15; y += lineSpace;
					setcolor(WHITE);
					x += charSpace;
					cursor(x, y);
				}
			}
		}
		else if (ch[0] != 77 && ch[0] != 75 && ch[0] != 0 /*&& ch[0] != '\x18'*/){
			if (x > 650)
			{
				removeCursor(x, y);
				setcolor(WHITE);
				x = 15; y += lineSpace;
			}
			moveto(x, y);
			if (ch[0] == 13){
				removeCursor(x, y);
				setcolor(WHITE);
				addNode(head, curr, ch, x, y);
				y += lineSpace; x = 5;
				moveto(x, y);
				//	 outtext(ch);
				line(x + charSpace, y, x + charSpace, y + 20);
				x += charSpace;
			}
			else if (ch[0] == 8){
				back(curr, head, x, y);
			}
			else{
				setcolor(WHITE);
				addNode(head, curr, ch, x, y);
				outtext(ch);
				x += charSpace;
				cursor(x, y);
			}
		}
	}
}