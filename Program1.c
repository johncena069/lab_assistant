#include <stdio.h>
#include <stdlib.h>

typedef struct Book {
    int Book_ID, Status;              /* 1 = Available, 0 = Issued */
    char Title[80], Author[80];
    float Price;
} Book;

Book *books = NULL;
int count = 0;

void create(void) {
    int n;
    printf("Number of books to add: ");
    if (scanf("%d", &n) != 1 || n <= 0) return;
    Book *temp = realloc(books, (count + n) * sizeof *books);
    if (!temp) { puts("Memory allocation failed."); return; }
    books = temp;
    for (int i = count; i < count + n; i++) {
        printf("\nBook %d\nID: ", i + 1);
        if (scanf("%d", &books[i].Book_ID) != 1) return;
        printf("Title: ");
        if (scanf(" %s[^\n]", books[i].Title) != 1) return;
        printf("Author: ");
        if (scanf(" %s[^\n]", books[i].Author) != 1) return;
        printf("Price: ");
        if (scanf("%f", &books[i].Price) != 1) return;
        books[i].Status = 1;
    }
    count += n;
    puts("Book records added.");
}

void printBook(int i) {
    printf("%d | %s | %s | %.2f | %s\n", books[i].Book_ID,
           books[i].Title, books[i].Author, books[i].Price,
           books[i].Status ? "Available" : "Issued");
}

int findBook(void) {
    int id;
    printf("Enter Book ID: ");
    if (scanf("%d", &id) != 1) return -1;
    for (int i = 0; i < count; i++)
        if (books[i].Book_ID == id) return i;
    puts("Book not found.");
    return -1;
}

void display(void) {
    if (!count) { puts("No book records."); return; }
    puts("ID | Title | Author | Price | Status");
    for (int i = 0; i < count; i++) printBook(i);
}

void search(void) {
    int i = findBook();
    if (i != -1) printBook(i);
}

void issueBook(void) {
    int i = findBook();
    if (i == -1) return;
    if (!books[i].Status) puts("Book already issued.");
    else { books[i].Status = 0; puts("Book issued successfully."); }
}

void returnBook(void) {
    int i = findBook();
    if (i == -1) return;
    if (books[i].Status) puts("Book is already available.");
    else { books[i].Status = 1; puts("Book returned successfully."); }
}

int main(void) {
    int choice;
    do {
        printf("\n1. Add Book Records\n2. Display All Book Records"
               "\n3. Search Book by Book ID\n4. Issue a Book"
               "\n5. Return a Book\n6. Exit\nChoice: ");
        if (scanf("%d", &choice) != 1) break;
        switch (choice) {
            case 1: create(); break;
            case 2: display(); break;
            case 3: search(); break;
            case 4: issueBook(); break;
            case 5: returnBook(); break;
            case 6: break;
            default: puts("Invalid choice.");
        }
    } while (choice != 6);
    free(books);
    return 0;
}
