#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// functii numerice:

// varifica daca un sir de caractere contine un numar
int is_number(char s[])
{
    int i;
    for (i = 0; i < strlen(s); i++)
        if (s[i] < '0' || s[i] > '9')
            return 0;
    return 1;
}

// calculeaza puterea n a lui 10
int putere(int n)
{
    int x = 1, i;
    for (i = 1; i <= n; i++)
        x = x * 10;
    return x;
}

// transforma un sir de caractere in numar
int str_to_number(char s[])
{
    int n = 0, t, i;
    t = strlen(s);
    for (i = t - 1; i >= 0; i--)
        n = n + (s[i] - '0') * putere(t - i - 1);
    return n;
}

// lista dublu inlantuita (definitie):
// nodul pentru lista dublu inlantuita
typedef struct node
{
    char data;
    int line, poz;
    struct node *next, *prev;
} node;

// lista - voi folosi o lista dublu inlantuita cu santinela
typedef struct TDoubleLinkedList
{
    struct node *sentinel;
    int len;
} TDoubleLinkedList;

// stiva si functiile aferente ei:
// nodul pentru stiva
typedef struct S_Node
{
    // numele comenzii, informatii despre comanda
    char command[3], info[100], info1[100];
    // pozitia cursorului inainte si dupa aplicarea operatiei
    node *first, *last;
    struct S_Node *next;
} S_Node;

// stiva
typedef struct Stack
{
    struct S_Node *top;
    int len;
} Stack;

// initializeaza stiva
void S_init(struct Stack **s)
{
    (*s) = (Stack *)malloc(sizeof(Stack));
    (*s)->len = 0;
    (*s)->top = NULL;
}

// extrage un element din stiva
S_Node *pop_stack(Stack *s)
{
    S_Node *p = s->top;
    if (p != NULL && s->len > 0)
    {
        s->top = s->top->next;
        s->len--;
    }
    return p;
}

// elibereaza memoria si sterge stiva
void free_stack(Stack **s)
{
    S_Node *p = pop_stack(*s);
    if (p == NULL)
        free(*s);
    else
    {
        free(p);
        free_stack(s);
    }
}

// adauga un element in stiva
void add_stack(Stack *s, char command[3], node *first, node *last, char info[100], char info1[100])
{
    S_Node *nou = (S_Node *)malloc(sizeof(S_Node));
    strcpy(nou->command, command);
    strcpy(nou->info, info);
    strcpy(nou->info1, info1);
    nou->first = first;
    nou->last = last;
    nou->next = s->top;
    s->top = nou;
    s->len++;
}

// verifica daca stiva este vida
int is_empty(Stack *s)
{
    if (s->len > 0)
        return 0;
    else
        return 1;
}

// functiile aferente listei:

// initializeaza lista
void init(struct TDoubleLinkedList **list)
{
    (*list) = (TDoubleLinkedList *)malloc(sizeof(TDoubleLinkedList));
    (*list)->len = 0;
    (*list)->sentinel = (node *)malloc(sizeof(node));
    (*list)->sentinel->next = (*list)->sentinel;
    (*list)->sentinel->prev = (*list)->sentinel;
}

// calculeaza lungimea listei
int length(TDoubleLinkedList *list)
{
    int k = 0;
    node *p = list->sentinel->next;
    while (p != list->sentinel)
    {
        k++;
        p = p->next;
    }
    return k;
}

// adauga un element in lista, imediat dupa nodul p
// returneaza un pointer la elementul inserat
node *add(TDoubleLinkedList *list, node *p, char data)
{
    node *nou = (node *)malloc(sizeof(node));
    nou->data = data;

    if (length(list) == 0)
    {
        nou->next = list->sentinel;
        nou->prev = list->sentinel;
        list->sentinel->next = nou;
        list->sentinel->prev = nou;
        list->len++;
    }
    else if (p == list->sentinel)
    {
        nou->prev = list->sentinel;
        nou->next = list->sentinel->next;
        list->sentinel->next->prev = nou;
        list->sentinel->next = nou;
        list->len++;
    }
    else if (p == list->sentinel->prev)
    {
        nou->next = list->sentinel;
        nou->prev = list->sentinel->prev;
        list->sentinel->prev->next = nou;
        list->sentinel->prev = nou;
        list->len++;
    }
    else
    {
        nou->next = p->next;
        nou->prev = p;
        p->next->prev = nou;
        p->next = nou;
        list->len++;
    }
    return nou;
}

// returneaza un pointer la nodul de pe linia line, pozitia poz
node *get(TDoubleLinkedList *list, int line, int poz)
{
    if (line == 1 && poz == 0)
        return list->sentinel;
    else
    {
        node *p = list->sentinel->next;
        while (p->line != line && p != list->sentinel)
            p = p->next;
        while (p->poz != poz && p != list->sentinel)
            p = p->next;
        return p;
    }
}

// sterge nodul n si returneaza un pointer la nodul anterior lui n
node *backspace(TDoubleLinkedList *list, node *n)
{
    if (n == list->sentinel)
        return n;
    else if (n == list->sentinel->next)
    {
        list->sentinel->next = list->sentinel->next->next;
        list->sentinel->next->prev = list->sentinel;
        free(n);
        list->len--;
        return list->sentinel;
    }
    else if (n == list->sentinel->prev)
    {
        list->sentinel->prev = list->sentinel->prev->prev;
        list->sentinel->prev->next = list->sentinel;
        free(n);
        list->len--;
        return list->sentinel->prev;
    }
    else
    {
        node *p = n->prev;
        n->next->prev = n->prev;
        n->prev->next = n->next;
        free(n);
        list->len--;
        return p;
    }
}

// sterge nodul de după n si returneaza un pointer la n
node *delete (TDoubleLinkedList *list, node *n)
{
    node *p = n->next;
    p = backspace(list, p);
    return p;
}

// sterge toata linia pe care se afla nodul n
node *delete_line(TDoubleLinkedList *list, node *n)
{
    node *p = get(list, n->line, 0);
    int k = n->line;
    while (p->next->line == k)
        p = delete (list, p);
    p = delete (list, p);
    return p;
}

// indexeaza elementele listei
// atribuie fiecarui element o linie si o pozitie pe linie
// elimina eventualele \n-uri "inutile" din text
void indeX(TDoubleLinkedList *list)
{
    int line = 1, poz = 0;
    list->sentinel->line = line;
    list->sentinel->poz = poz;
    poz = 1;
    node *p = list->sentinel->next;
    while (p->next != list->sentinel)
    {
        if (p->data == '\n' && p->next->data == '\n')
            p = delete (list, p);
        p = p->next;
    }
    p = list->sentinel->next;
    while (p != list->sentinel)
    {
        if (p->data == '\n')
        {
            line++;
            poz = 0;
            p->line = line;
            p->poz = poz;
            poz++;
        }
        else
        {
            p->line = line;
            p->poz = poz;
            poz++;
        }
        p = p->next;
    }
}

// salveaza textul in fisierul de iesire
void save(TDoubleLinkedList *list)
{
    node *p = list->sentinel->next;
    FILE *fptr;
    fptr = fopen("editor.out", "w");
    while (p != list->sentinel)
    {
        fprintf(fptr, "%c", p->data);
        p = p->next;
    }
    fclose(fptr);
}

// goleste lista si elibereaza memoria alocata
void free_list(TDoubleLinkedList **list)
{
    node *p = (*list)->sentinel->next;
    if (p == (*list)->sentinel)
    {
        free((*list)->sentinel);
        free(*list);
    }
    else
    {
        (*list)->sentinel->next = (*list)->sentinel->next->next;
        free(p);
        (*list)->len--;
        free_list(list);
    }
}

// gaseste un cuvant in text, dupa nodul p0
// returneaza un pointer la nodul ce contine ultima litera din cuvant
node *find_string(TDoubleLinkedList *list, char str[20], node *p0)
{
    node *p = p0, *q;
    if (p == list->sentinel)
    {
        p = p->next;
        q = p;
        int ok = 1, i;
        for (i = 0; i < strlen(str); i++)
        {
            if (q->data != str[i])
            {
                ok = 0;
                break;
            }
            q = q->next;
        }
        if (ok == 1)
            return q->prev;
        p = p->next;
    }
    while (p != list->sentinel)
    {
        q = p;
        int ok = 1, i;
        for (i = 0; i < strlen(str); i++)
        {
            if (q->data != str[i])
            {
                ok = 0;
                break;
            }
            q = q->next;
        }
        if (ok == 1)
            return q->prev;
        p = p->next;
    }
    return p;
}

// sterge prima aparitie de dupa nodul p0 a unui cuvant din text
node *delete_word(TDoubleLinkedList *list, char old[20], node *p0)
{
    node *p = find_string(list, old, p0);
    int i;
    if (p != list->sentinel)
    {
        for (i = 0; i < strlen(old); i++)
            p = backspace(list, p);
        return p;
    }
    else
        return p0;
}

// sterge toate aparitiile de dupa nodul p0 ale unui cuvant din text
node *delete_all(TDoubleLinkedList *list, char old[20], node *p0)
{
    node *p = find_string(list, old, p0);
    if (p == list->sentinel)
        return p0;
    else
    {
        p = delete_word(list, old, p0);
        p = delete_all(list, old, p0);
    }
}

// inlocuieste prima aparitie de dupa nodul p0 a unui cuvant cu un altul
node *replace(TDoubleLinkedList *list, char old[20], char new[20], node *p0)
{
    node *p = find_string(list, old, p0);
    int i;
    if (p == list->sentinel)
        return p0;
    else
    {
        p = delete_word(list, old, p0);
        node *q = p;
        for (i = 0; i < strlen(new); i++)
            p = add(list, p, new[i]);
        return q;
    }
}

// inlocuieste toate aparitiile de dupa nodul p0 ale unui cuvant cu un altul
node *replace_all(TDoubleLinkedList *list, char old[20], char new[20], node *p0)
{
    node *p = find_string(list, old, p0);
    if (p == list->sentinel)
        return p0;
    else
    {
        p = replace(list, old, new, p0);
        p = replace_all(list, old, new, p);
    }
}

int main()
{
    TDoubleLinkedList *l;       // lista in care pastrez textul
    Stack *History1, *History2; // stivele pentru operatiile de undo si redo

    // initializez structurile
    init(&l);
    S_init(&History1);
    S_init(&History2);

    // deschid fisierul de intrare
    FILE *fptr;
    fptr = fopen("editor.in", "r");

    char s[10], v[10], ch, info[100], info1[100];
    int line, nr, poz, i, k;
    node *cursor = l->sentinel->next, *first, *last, *r;
    S_Node *n, *p;

    // citesc datele din fisier
    while (1)
    {
        first = NULL;
        last = NULL;
        k = 0;
        for (i = 0; i < 100; i++)
        {
            info[i] = '\0';
            info1[i] = '\0';
        }
        first = cursor;
        fscanf(fptr, "%c", &ch);
        if (ch != '\n')
        {
            cursor = add(l, cursor, ch);
            info[k] = ch;
            k++;
        }

        // citesc caractere pana la intalnirea caracterului ':'
        // le adaug, pe rand, in lista
        while ((ch = fgetc(fptr)) != ':')
        {
            cursor = add(l, cursor, ch);
            info[k] = ch;
            k++;
        }
        indeX(l);
        if (cursor->data == '\n')
        {
            if (cursor != l->sentinel->prev && cursor->line - first->line <= 1)
                cursor = backspace(l, cursor);
        }
        indeX(l);
        last = cursor;

        // adaug comanda de inserare in stiva pentru undo
        add_stack(History1, "in", first, last, info, info1);

        fscanf(fptr, "%c", &ch);
        fscanf(fptr, "%c", &ch);

        // citesc comenzile, pana la intalnirea sirului "::i"
        fscanf(fptr, "%s", s);
        while (strcmp(s, "::i") != 0)
        {
            int ok = 0, call = 0;

            for (i = 0; i < 100; i++)
            {
                info[i] = '\0';
                info1[i] = '\0';
            }
            first = NULL;
            last = NULL;

            // identific comanda si o aplic textului, folosind functiile definite anterior
            // adaug comanda in stiva pentru undo
            if (strcmp(s, "q") == 0)
            {
                // eliberez lista si stivele
                // inchid fisierul de intrare
                // ies din main()
                free_list(&l);
                free_stack(&History1);
                free_stack(&History2);
                fclose(fptr);
                return 0;
            }

            if (strcmp(s, "s") == 0)
            {
                // salvez textul in fisierul de iesire
                save(l);
                // reinitializez stivele pentru undo si redo
                free_stack(&History1);
                free_stack(&History2);
                S_init(&History1);
                S_init(&History2);
            }

            if (strcmp(s, "gl") == 0)
            {
                fscanf(fptr, "%d", &line);
                first = cursor;
                cursor = get(l, line, 0);
                last = cursor;
                add_stack(History1, "gl", first, last, info, info1);
            }

            if (strcmp(s, "dl") == 0)
            {
                call = 1;
                fscanf(fptr, "%s", s);
                if (is_number(s))
                {
                    ok = 1;
                    line = str_to_number(s);
                    cursor = get(l, line, 1);
                    cursor = delete_line(l, cursor);
                }
                else
                    cursor = delete_line(l, cursor);
                indeX(l);
            }

            if (strcmp(s, "b") == 0)
            {
                if (cursor->data == '\n' && cursor == l->sentinel->prev)
                    cursor = cursor->prev;
                cursor = backspace(l, cursor);
                indeX(l);
            }

            if (strcmp(s, "d") == 0)
            {
                call = 1;
                fscanf(fptr, "%s", s);
                first = cursor;
                if (is_number(s))
                {
                    ok = 1;
                    nr = str_to_number(s);
                    for (i = 0; i < nr; i++)
                    {
                        info[i] = cursor->next->data;
                        cursor = delete (l, cursor);
                    }
                }
                else
                {
                    info[1] = cursor->next->data;
                    cursor = delete (l, cursor);
                }
                last = cursor;
                add_stack(History1, "d", first, last, info, info1);
                indeX(l);
            }

            if (strcmp(s, "gc") == 0)
            {
                call = 1;
                fscanf(fptr, "%d", &poz);
                fscanf(fptr, "%s", s);
                first = cursor;
                if (is_number(s) == 1)
                {
                    ok = 1;
                    line = str_to_number(s);
                    cursor = get(l, line, poz);
                }
                else
                    cursor = get(l, cursor->line, poz);
                last = cursor;
                add_stack(History1, "gc", first, last, info, info1);
            }

            if (strcmp(s, "re") == 0)
            {
                fscanf(fptr, "%s", s);
                fscanf(fptr, "%s", v);
                strcpy(info, s);
                strcpy(info1, v);
                first = cursor;
                r = replace(l, s, v, cursor);
                last = r;
                indeX(l);
                add_stack(History1, "re", first, last, info, info1);
            }

            if (strcmp(s, "ra") == 0)
            {
                fscanf(fptr, "%s", s);
                fscanf(fptr, "%s", v);
                r = replace_all(l, s, v, l->sentinel);
                indeX(l);
            }

            if (strcmp(s, "dw") == 0)
            {
                fscanf(fptr, "%s", s);
                strcpy(info, s);
                first = cursor;
                r = delete_word(l, s, cursor);
                last = r;
                indeX(l);
                add_stack(History1, "dw", first, last, info, info1);
            }

            if (strcmp(s, "da") == 0)
            {
                fscanf(fptr, "%s", s);
                r = delete_all(l, s, l->sentinel);
                indeX(l);
            }

            // comanda undo
            if (strcmp(s, "u") == 0)
            {
                // extrag ultima comanda aplicata din stiva pentru undo
                n = pop_stack(History1);

                // o adaug in stiva pentru redo
                add_stack(History2, n->command, n->first, n->last, n->info, n->info1);

                // identific comanda si o anulez prin aplicarea operatiei inverse
                if (n && strcmp(n->command, "in") == 0)
                {
                    cursor = n->last;
                    while (cursor != n->first)
                        cursor = backspace(l, cursor);
                }

                if (n && strcmp(n->command, "gl") == 0)
                    cursor = n->first;

                if (n && strcmp(n->command, "gc") == 0)
                    cursor = n->first;

                if (n && strcmp(n->command, "d") == 0)
                {
                    r = n->last;
                    for (i = 0; i < strlen(n->info); i++)
                        r = add(l, r, n->info[i]);
                    indeX(l);
                }

                if (n && strcmp(n->command, "re") == 0)
                {
                    r = n->last;
                    r = replace(l, n->info1, n->info, r);
                    indeX(l);
                }

                if (n && strcmp(n->command, "dw") == 0)
                {
                    r = n->last;
                    for (i = 0; i < strlen(n->info); i++)
                        r = add(l, r, n->info[i]);
                    indeX(l);
                }

                free(n);
                indeX(l);
            }

            // comanda redo
            if (strcmp(s, "r") == 0)
            {
                // extrag ultima comanda anulata din stiva pentru redo
                p = pop_stack(History2);

                // identific comanda si o reaplic textului
                if (p && strcmp(p->command, "in") == 0)
                {
                    cursor = p->first;
                    for (i = 0; i < strlen(p->info); i++)
                        cursor = add(l, cursor, p->info[i]);
                    if (cursor->data == '\n')
                    {
                        if (cursor != l->sentinel->prev)
                            cursor = backspace(l, cursor);
                    }
                }

                if (p && strcmp(p->command, "gl") == 0)
                    cursor = p->last;

                if (p && strcmp(p->command, "gc") == 0)
                    cursor = p->last;

                if (p && strcmp(p->command, "d") == 0)
                {
                    r = p->first;
                    for (i = 0; i < strlen(p->info); i++)
                        r = delete (l, r);
                    indeX(l);
                }

                if (p && strcmp(p->command, "re") == 0)
                {
                    r = p->last;
                    r = replace(l, p->info, p->info1, r);
                    indeX(l);
                }

                if (p && strcmp(p->command, "dw") == 0)
                {
                    r = p->last;
                    r = delete_word(l, p->info, r);
                    indeX(l);
                }

                free(p);
                indeX(l);
            }

            // citesc urmatoarea comanda
            if ((ok == 1 && call == 1) || call == 0)
                fscanf(fptr, "%s", s);

            /*am folosit variabilele ok si call pentru functiile
             care au numar variabil de parametri*/
        }
        // reiau citirea de text
        fscanf(fptr, "%c", &ch);
    }
    return 0;
}
