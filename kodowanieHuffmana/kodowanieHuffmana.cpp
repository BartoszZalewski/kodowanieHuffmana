

#include "stdafx.h"
#include "string.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#pragma warning(disable:4996)
using namespace std;


struct Node {
	int count;
	char znak;
	Node *left = NULL;
	Node *right = NULL;
	char * kod;
	int dl;

	Node(int c, char z)
	{
		count = c;
		znak = z;
		kod = "";
		dl = 0;
	}
};


struct ElemL {
	Node *n;
	ElemL *next = NULL;
	ElemL *prev = NULL;

	ElemL(Node * a)
	{
		n = a;
	}

	ElemL(int x, char a)
	{
		n = new Node(x, a);
	}
};


struct ListTree {
	int ktr = 0;
	ElemL *first;
	ElemL *last;

	void put(ElemL *e)
	{
		ElemL *p = first;
		if (p == NULL)
		{
			first = e;
			last = e;
		}
		else
		{
			if (e->n->count <= p->n->count) {
				first->prev = e;
				e->next = first;
				first = e;
				return;
			}
			else if (first->next == NULL)
			{
				first->next = e;
				e->prev = first;
				last = e;
				return;
			}
			else
				p = p->next;
			if (p == NULL)
			{
				first->next = e;
				e->prev = first;
				last = e;
				return;
			}
			while (p != NULL)
				if (e->n->count > p->n->count)
					p = p->next;
				else
				{
					p->prev->next = e;
					e->prev = p->prev;
					p->prev = e;
					e->next = p;
					return;
				}
			last->next = e;
			e->prev = last;
			last = e;
		}
	}


	void del(ElemL *e)
	{
		ElemL *p = first;
		if (e == first)
		{
			if (first->next != NULL)
				first->next->prev = NULL;
			first = first->next;
		}
		else
		{
			while (p != last)
				if (e == p)
				{
					p->next->prev = p->prev;
					p->prev->next = p->next;
					return;
				}
				else
					p = p->next;
			last->prev->next = NULL;
			last = last->prev;
		}
	}

	void print()
	{
		ElemL *p = first;
		while (p != NULL)
		{
			if (p->n->znak == 10)
				printf("%d(/)", p->n->count);
			else
				printf("%d(%c)", p->n->count, p->n->znak);
			p = p->next;
		}
		printf("\n");
	}

};


struct Point
{
	int count;
	char znak;
	char *kod;
	int dl;
	Point *next;
	Point *prev;

	Point(int x, char z)
	{
		count = x;
		znak = z;
		next = NULL;
		prev = NULL;
		dl = 0;
		kod = "";
	}

	Point(char z, char *k, int d)
	{
		znak = z;
		next = NULL;
		prev = NULL;
		kod = k;
		dl = d;
	}
};


struct ListWord
{
	Point *first = NULL;
	Point *last = NULL;

	ListWord()
	{
		first = NULL;
		last = NULL;
	}

	void add(Point *e)
	{
		Point *p = first;
		if (p == NULL)
		{
			first = e;
			last = e;
		}
		else
		{
			while (p != NULL)
				if (p->znak != e->znak)
					p = p->next;
				else
				{
					p->count++;
					return;
				}
			last->next = e;
			e->prev = last;
			last = e;
		}
	}

	Point * kod(char z)
	{
		Point *p = first;
		while (p != NULL)
		{
			if (p->znak == z)
				return p;
			else
				p = p->next;
		}
		return NULL;
	}

	void wsadz(ListTree *lista)
	{
		Point *p = first;

		while (p != NULL)
		{
			ElemL *a = new ElemL(p->count, p->znak);
			lista->put(a);
			p = p->next;
		}
	}


	void print()
	{
		Point *p = first;
		while (p != NULL)
		{
			if (p->znak == 10)
				printf("%d(/)", p->count);
			else
				printf("%d(%c)", p->count, p->znak);
			p = p->next;
		}
		printf("\n");
	}
};

int max(int a, int b)
{
	if (a>b)
		return a;
	else
		return b;
}


void fHuffmana(ListTree *lista)
{
	ElemL *p = lista->first;
	while (p->next != NULL)
	{
		lista->print();
		Node *n = new Node(p->n->count + p->next->n->count, '$');
		n->left = p->n;
		n->right = p->next->n;
		lista->del(lista->first);
		lista->del(lista->first);
		ElemL *fn = new ElemL(n);
		lista->put(fn);
		p = lista->first;
	}
}


void KLP(Node *r)
{
	cout << "-" << r->znak << "-";
	if (r->left != NULL)
		KLP(r->left);
	if (r->right != NULL)
		KLP(r->right);
}



void KLP(Node *r, FILE *plik, ListWord *q, int wys)
{
	wys++;
	if (r->left != NULL)
	{
		int h = wys;
		r->left->kod = (char*)malloc(h + 1);
		for (int i = 0; i < h - 1; i++)
			r->left->kod[i] = r->kod[i];
		r->left->kod[h - 1] = '0';
		r->left->kod[h] = '\0';

		r->left->dl = h;

		if (r->left->znak != '$')
		{
			fprintf(plik, "%c:", r->left->znak);
			for (int i = 0; i < h; i++)
			{
				fprintf(plik, "%c", r->left->kod[i]);
			}
			fprintf(plik, "\n");

			Point *p = new Point(r->left->znak, r->left->kod, h);
			q->add(p);
		}
		KLP(r->left, plik, q, wys);
	}
	if (r->right != NULL)
	{
		int h = wys;
		r->right->kod = (char*)malloc(h + 1);
		for (int i = 0; i < h - 1; i++)
			r->right->kod[i] = r->kod[i];
		r->right->kod[h - 1] = '1';
		r->right->kod[h] = '\0';

		r->right->dl = h;

		if (r->right->znak != '$')
		{

			fprintf(plik, "%c:", r->right->znak);
			for (int i = 0; i < h; i++)
			{
				fprintf(plik, "%c", r->right->kod[i]);
			}
			fprintf(plik, "\n");

			Point *p = new Point(r->right->znak, r->right->kod, h);
			q->add(p);
		}
		KLP(r->right, plik, q, wys);
	}
}


int main()
{

	while (true)
	{
		printf("Co chcemy zrobic?\n<1> Szyfrowanie pliku (plik.txt) \n<2> Deszyfrowanie z pliku (kodowanie_liter.txt i kodowanie_plik.txt) \n<0> Zakoncz\n");

		int x = 0;
		cin >> x;

		if (x == 1)
		{
			ListTree *lista = new ListTree();
			FILE * plik = fopen("plik.txt", "r");
			FILE *zapis = fopen("kodowanie_liter.txt", "w");
			int number_letter = NULL;
			ListWord *q = new ListWord;
			while (number_letter != EOF)
			{
				char letter = getc(plik);
				Point *p = new Point(1, letter);
				q->add(p);
				number_letter = letter;
			}
			fclose(plik);
			q->wsadz(lista);
			lista->ktr = 1;
			fHuffmana(lista);
			lista->print();
			ListWord *sz = new ListWord;
			KLP(lista->first->n, zapis, sz, 0);
			fclose(zapis);

			plik = fopen("plik.txt", "r");
			zapis = fopen("kodowanie_plik.txt", "w");
			number_letter = NULL;
			while (number_letter != EOF)
			{
				char letter = getc(plik);
				Point *p = sz->kod(letter);
				for (int i = 0; i < p->dl; i++)
					fprintf(zapis, "%c", p->kod[i]);
				number_letter = letter;
			}

			fclose(zapis);
			fclose(plik);
			printf("\n\nPlik zakodowano do pliku kodowanie_plik.txt, a w pliku kodowanie_liter.txt znajduja sie kodowania liter. \n\n\n");
		}
		else if (x == 2)
		{
			FILE *plik = fopen("kodowanie_liter.txt", "r");
			Node *drzewo = new Node(1, 36);
			int kon = 0;
			while (kon != 2)
			{
				Node *po = drzewo;
				char letter = getc(plik);
				if (letter == EOF)
					kon++;
				if (kon != 2)
				{
					char kropka = getc(plik);
					kropka = getc(plik);
					while (kropka != 10)
					{
						if (kropka == '1')
						{
							if (po->right == NULL)
							{
								Node *qq = new Node(1, 36);
								po->right = qq;
							}
							po = po->right;
						}
						if (kropka == '0')
						{
							if (po->left == NULL)
							{
								Node *qq = new Node(1, 36);
								po->left = qq;
							}
							po = po->left;
						}
						kropka = getc(plik);
					}
					po->znak = letter;
				}
			}
			fclose(plik);
			plik = fopen("kodowanie_plik.txt", "r");
			FILE * zapis = fopen("dekodowanie.txt", "w");
			int number_letter = NULL;
			Node *de = drzewo;
			while (number_letter != EOF)
			{
				char letter = getc(plik);
				if (letter == '1')
					de = de->right;
				else
					de = de->left;
				if (de->znak != 36)
				{
					fprintf(zapis, "%c", de->znak);
					de = drzewo;
				}
				number_letter = letter;
			}

			fclose(zapis);
			fclose(plik);
			printf("\n\nPlik dekodowanie.txt zawieta odkodowany tekst.\n\n\n");
		}
		else
			break;

	}

	return 0;
}