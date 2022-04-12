#include <bits/stdc++.h>
#include <ctime>
using namespace std;

class User
{
private:
    string password;

public:
    string name;
    string id;
    enum User_Type
    {
        LIBRARIAN,
        STUDENT,
        PROFESSOR
    } user_type;

    User(string name, string id, string password)
    {
        this->name = name;
        this->id = id;
        this->password = password;
    }
    bool match_password(string password)
    {
        return (this->password == password);
    }
    void update_password(string password)
    {
        this->password = password;
    }
};

class Book
{
public:
    string title = "";
    string author = "";
    string ISBN = "";
    string publication = "";
    bool is_available = true;
    string issued_to = "";
    time_t issued_on = time(0);

    Book(string title, string author, string ISBN, string publication, bool is_available = true)
    {
        this->title = title;
        this->author = author;
        this->ISBN = ISBN;
        this->publication = publication;
        this->is_available = is_available;
    }
};

class Professor : public User
{
public:
    static const int FINE_AMOUNT = 5;
    static const int BOOK_LIMIT = INT_MAX;
    static const int MAX_ISSUE_DAYS = 60;
    int pending_fine = 0;
    vector<Book *> book_list;

    Professor(string name, string id, string password) : User(name, id, password)
    {
        this->user_type = PROFESSOR;
    }

    int calculate_fine(string title)
    {
        int fine = 0;
        for (int i = 0; i < book_list.size(); i++)
        {
            if (book_list[i]->title == title)
            {
                time_t now = time(0);
                int days_left = difftime(now, book_list[i]->issued_on) / (60 * 60 * 24);
                days_left = max(days_left - MAX_ISSUE_DAYS, 0);
                fine += days_left * FINE_AMOUNT;
            }
        }
        return fine;
    };

    void clear_fine()
    {
        pending_fine = 0;
    }
};

class Student : public User
{
public:
    static const int FINE_AMOUNT = 2;
    static const int BOOK_LIMIT = 5;
    static const int MAX_ISSUE_DAYS = 30;
    int pending_fine = 0;
    vector<Book *> book_list;

    Student(string name, string id, string password) : User(name, id, password)
    {
        this->user_type = STUDENT;
        this->book_list.clear();
        this->book_list.reserve(BOOK_LIMIT);
    }

    int calculate_fine(string title)
    {
        int fine = 0;
        for (int i = 0; i < book_list.size(); i++)
        {
            if (book_list[i]->title == title)
            {
                time_t now = time(0);
                int days_left = difftime(now, book_list[i]->issued_on) / (60 * 60 * 24);
                days_left = max(days_left - MAX_ISSUE_DAYS, 0);
                fine += days_left * FINE_AMOUNT;
            }
        }
        return fine;
    };

    void clear_fine()
    {
        pending_fine = 0;
    }
};

class Librarian : public User
{
public:
    Librarian(string name, string id, string password) : User(name, id, password)
    {
        this->user_type = LIBRARIAN;
    }
};

class User_Database
{
public:
    vector<Student> student_list;
    vector<Professor> professor_list;
    vector<Librarian> librarian_list;

    void add_user(string name, string id, string password, User::User_Type user_type)
    {
        if (user_type == User::PROFESSOR)
        {
            professor_list.push_back(Professor(name, id, password));
        }
        else if (user_type == User::STUDENT)
        {
            student_list.push_back(Student(name, id, password));
        }
        else if (user_type == User::LIBRARIAN)
        {
            librarian_list.push_back(Librarian(name, id, password));
        }
        else
        {
            cout << "Invalid user type" << endl;
        }
    }

    void update_user(User *user, string name, string id, string password)
    {
        if (name != "")
            user->name = name;
        if (id != "")
            user->id = id;
        if (password != "")
            user->update_password(password);
    }

    void delete_user(User *user)
    {
        for (int i = 0; i < student_list.size(); i++)
        {
            if (student_list[i].id == user->id)
            {
                student_list.erase(student_list.begin() + i);
            }
        }
        for (int i = 0; i < professor_list.size(); i++)
        {
            if (professor_list[i].id == user->id)
            {
                professor_list.erase(professor_list.begin() + i);
            }
        }
        for (int i = 0; i < librarian_list.size(); i++)
        {
            if (librarian_list[i].id == user->id)
            {
                librarian_list.erase(librarian_list.begin() + i);
            }
        }
    }

    User *search(string id)
    {
        for (int i = 0; i < student_list.size(); i++)
        {
            if (student_list[i].id == id || student_list[i].name == id)
            {
                return &student_list[i];
            }
        }
        for (int i = 0; i < professor_list.size(); i++)
        {
            if (professor_list[i].id == id || professor_list[i].name == id)
            {
                return &professor_list[i];
            }
        }
        for (int i = 0; i < librarian_list.size(); i++)
        {
            if (librarian_list[i].id == id || librarian_list[i].name == id)
            {
                return &librarian_list[i];
            }
        }
        return nullptr;
    }

    void print_user_list()
    {
        system("cls");
        cout << "----------------------------------------";
        cout << "---------------------\n\n";
        cout << "\t\tUser List\n"
             << endl;
        int cnt = 1;
        for (int i = 0; i < student_list.size(); i++)
        {
            cout << cnt++ << ".      Name: " << student_list[i].name << endl;
            cout << "\tID: " << student_list[i].id << endl;
            cout << "\tType: Student" << endl;
            cout << endl;
        }
        for (int i = 0; i < professor_list.size(); i++)
        {
            cout << cnt++ << ".      Name: " << professor_list[i].name << endl;
            cout << "\tID: " << professor_list[i].id << endl;
            cout << "\tType: Professor" << endl;
            cout << endl;
        }
        for (int i = 0; i < librarian_list.size(); i++)
        {
            cout << cnt++ + professor_list.size() << ".      Name: " << librarian_list[i].name << endl;
            cout << "\tID: " << librarian_list[i].id << endl;
            cout << "\tType: Librarian" << endl;
            cout << endl;
        }
        cout << "\tPress enter to continue...";
        fflush(stdin);
        getchar();
        return;
    }
};

class Book_Database
{
public:
    vector<Book> book_list;

    void add_book(string title, string author, string ISBN, string publication)
    {
        book_list.push_back(Book(title, author, ISBN, publication));
    }

    void update_book(Book *book, string title, string author, string ISBN, string publication)
    {
        if (title != "")
            book->title = title;
        if (author != "")
            book->author = author;
        if (ISBN != "")
            book->ISBN = ISBN;
        if (publication != "")
            book->publication = publication;
    }

    void delete_book(Book *book)
    {
        for (int i = 0; i < book_list.size(); i++)
        {
            if (book_list[i].title == book->title)
            {
                extern User_Database user_database;
                User *user = user_database.search(book->issued_to);
                if (user != nullptr)
                    return_book(book->title, user);
                book_list.erase(book_list.begin() + i);
                break;
            }
        }
    }

    Book *search(string query)
    {
        for (int i = 0; i < book_list.size(); i++)
        {
            if (book_list[i].title == query ||
                book_list[i].ISBN == query)
            {
                return &book_list[i];
            }
        }
        return nullptr;
    }

    void issue_book(string title, User *user)
    {
        for (int i = 0; i < book_list.size(); i++)
        {
            if (book_list[i].title == title)
            {
                book_list[i].is_available = false;
                book_list[i].issued_to = user->name;
                book_list[i].issued_on = time(0);
                if (user->user_type == User::User_Type::STUDENT)
                {
                    Student *student = (Student *)user;
                    student->book_list.push_back(&book_list[i]);
                }
                else
                {
                    Professor *professor = (Professor *)user;
                    professor->book_list.push_back(&book_list[i]);
                }
                break;
            }
        }
    }

    bool return_book(string title, User *user)
    {
        bool book_returned = false;
        vector<Book *> *book_list;
        int MAX_ISSUE_DAYS;
        int FINE_AMOUNT;
        if (user->user_type == User::User_Type::STUDENT)
        {
            Student *student = (Student *)user;
            book_list = &student->book_list;
            MAX_ISSUE_DAYS = Student::MAX_ISSUE_DAYS;
            FINE_AMOUNT = Student::FINE_AMOUNT;
            student->pending_fine += student->calculate_fine(title);
        }
        else
        {
            Professor *professor = (Professor *)user;
            book_list = &professor->book_list;
            MAX_ISSUE_DAYS = Professor::MAX_ISSUE_DAYS;
            FINE_AMOUNT = Professor::FINE_AMOUNT;
            professor->pending_fine += professor->calculate_fine(title);
        }
        for (int j = 0; j < book_list->size(); j++)
        {
            if ((*book_list)[j]->title == title)
            {
                (*book_list)[j]->is_available = true;
                (*book_list)[j]->issued_to = "";
                book_returned = true;
                book_list->erase(book_list->begin() + j);
                break;
            }
        }
        return book_returned;
    }

    void print_book_list(bool lib = false)
    {
        extern User_Database user_database;
        system("cls");
        cout << "----------------------------------------";
        cout << "---------------------\n\n";
        cout << "\t\tBooks List\n"
             << endl;
        for (int i = 0; i < book_list.size(); i++)
        {
            cout << i + 1 << ".      Title: " << book_list[i].title << endl;
            cout << "\tAuthor: " << book_list[i].author << endl;
            cout << "\tISBN: " << book_list[i].ISBN << endl;
            cout << "\tPublication: " << book_list[i].publication << endl;
            cout << "\tAvailabile: " << (book_list[i].is_available ? "Yes" : "No") << endl;
            if (lib && !book_list[i].is_available)
            {
                cout << "\tIssued to: " << book_list[i].issued_to << endl;
                cout << "\tIssued on: " << ctime(&book_list[i].issued_on);
                time_t due_date;
                if (user_database.search(book_list[i].issued_to)->user_type == User::User_Type::STUDENT)
                    due_date = book_list[i].issued_on + Student::MAX_ISSUE_DAYS * 24 * 60 * 60;
                else
                    due_date = book_list[i].issued_on + Professor::MAX_ISSUE_DAYS * 24 * 60 * 60;
                cout << "\tDue date: " << ctime(&due_date) << endl;
            }
            cout << endl;
        }
        cout << "\tPress enter to continue...";
        fflush(stdin);
        getchar();
        return;
    }

    void print_issued_books(User *user, bool cls = true)
    {
        if (cls)
        {
            system("cls");
            cout << "----------------------------------------";
            cout << "---------------------\n\n";
            cout << "\t\tIssued Books\n"
                 << endl;
        }
        vector<Book *> book_list;
        int MAX_ISSUE_DAYS;
        if (user->user_type == User::User_Type::STUDENT)
        {
            Student *student = (Student *)user;
            book_list = student->book_list;
            MAX_ISSUE_DAYS = Student::MAX_ISSUE_DAYS;
        }
        else
        {
            Professor *professor = (Professor *)user;
            book_list = professor->book_list;
            MAX_ISSUE_DAYS = Professor::MAX_ISSUE_DAYS;
        }
        for (int i = 0; i < book_list.size(); i++)
        {
            cout << i + 1 << ".      Title: " << book_list[i]->title << endl;
            cout << "\tAuthor: " << book_list[i]->author << endl;
            cout << "\tISBN: " << book_list[i]->ISBN << endl;
            cout << "\tPublication: " << book_list[i]->publication << endl;
            cout << "\tIssued on: " << ctime(&book_list[i]->issued_on);
            time_t due_date = book_list[i]->issued_on + MAX_ISSUE_DAYS * 24 * 60 * 60;
            cout << "\tDue date: " << ctime(&due_date);
            cout << endl;
        }
        if (book_list.size() == 0)
            cout << "\n\tNo books have been issued." << endl;
        return;
    }

    void print_available_book_list()
    {
        system("cls");
        cout << "----------------------------------------";
        cout << "---------------------\n\n";
        cout << "\t\tAvailable Books List\n"
             << endl;
        int cnt = 1;
        for (int i = 0; i < book_list.size(); i++)
        {
            if (book_list[i].is_available)
            {
                cout << cnt << ".      Title: " << book_list[i].title << endl;
                cout << "\tAuthor: " << book_list[i].author << endl;
                cout << "\tISBN: " << book_list[i].ISBN << endl;
                cout << "\tPublication: " << book_list[i].publication << '\n'
                     << endl;
                cnt++;
            }
        }
        if (cnt == 1)
        {
            cout << "\tNo books are available." << endl;
        }
        cout << "\tPress enter to continue...";
        fflush(stdin);
        getchar();
        return;
    }
};

// Global Variables
User_Database user_database;
Book_Database book_database;

void populate_database()
{
    // add users
    user_database.add_user("Rishav Bikarwar", "ris27hav", "rishav", User::STUDENT);
    user_database.add_user("Prasanna Patil", "parsanna", "prasanna", User::STUDENT);
    user_database.add_user("Anshuman", "anshuman", "anshuman", User::STUDENT);
    user_database.add_user("Amitabh", "amit20", "amit", User::PROFESSOR);
    user_database.add_user("Somansh Dubey", "spd", "spd", User::PROFESSOR);
    user_database.add_user("SN Pandey", "snp", "snp", User::LIBRARIAN);

    // add books
    book_database.add_book("The Alchemist", "Paulo Coelho", "9780062502179", "HarperCollins Publishers Inc");
    book_database.add_book("Rich Dad Poor Dad", "Robert T. Kiyosaki", "9781612680194", "Plata Publishing");
    book_database.add_book("Atomic Habits", "James Clear", "9781847941831", "Cornerstone Books");
    book_database.add_book("The Power of Habit", "Charles Duhigg", "9780134977510", "Random House");
    book_database.add_book("A Brief History of Time", "Stephen Hawking", "9780857501004", "Transworld Publishers Ltd");
    book_database.add_book("Harry Potter and the Philosopher's Stone", "J.K. Rowling", "9780747532735", "Scholastic Inc");
    book_database.add_book("A Song of Ice and Fire", "George R.R. Martin", "9780553808368", "Bantam Books");

    // issue books
    User *user = user_database.search("ris27hav");
    book_database.issue_book("A Song of Ice and Fire", user);
    struct tm a = {0, 0, 0, 24, 1, 122}; /* February 24, 2022 */
    book_database.search("A Song of Ice and Fire")->issued_on = mktime(&a);
    book_database.issue_book("Harry Potter and the Philosopher's Stone", user);
    struct tm b = {0, 0, 0, 10, 1, 122}; /* February 10, 2022 */
    book_database.search("Harry Potter and the Philosopher's Stone")->issued_on = mktime(&b);

    user = user_database.search("amit20");
    book_database.issue_book("The Alchemist", user);
    struct tm c = {0, 0, 0, 14, 0, 122}; /* January 14, 2022 */
    book_database.search("The Alchemist")->issued_on = mktime(&c);
    book_database.issue_book("Rich Dad Poor Dad", user);
    struct tm d = {0, 0, 0, 15, 2, 122}; /* March 15, 2022 */
    book_database.search("Rich Dad Poor Dad")->issued_on = mktime(&d);
}

User *login()
{
    system("cls");
    string id, password;
    cout << "----------------------------------------";
    cout << "---------------------\n\n";
    cout << "\t\tLOGIN INTO YOUR ACCOUNT!!!\n"
         << endl;
    cout << "\tEnter your id: ";
    cin >> id;
    User *user = user_database.search(id);
    if (!user)
    {
        cout << "\n\tUser not found.\n";
        cout << "\tPress enter to continue...";
        fflush(stdin);
        getchar();
        return nullptr;
    }

    cout << "\tEnter your password: ";
    cin >> password;
    if (!user->match_password(password))
    {
        cout << "\n\tIncorrect password. Please try again.\n";
        cout << "\tPress enter to continue...";
        fflush(stdin);
        getchar();
        return nullptr;
    }

    cout << "\n\tLogin successful.\n";
    cout << "\tWelcome " << user->name << "!\n"
         << endl;
    cout << "\tPress enter to continue...";
    fflush(stdin);
    getchar();
    return user;
}

User *welcome_screen()
{
    User *user = nullptr;
    while (!user)
    {
        system("cls");
        cout << "----------------------------------------";
        cout << "---------------------\n\n";
        cout << "\t\tWELCOME TO THE LIBRARY!!!\n"
             << endl;
        cout << "\t1. Login\n";
        cout << "\t2. Exit\n\n";
        cout << "\tEnter your choice: ";
        string s;
        cin >> s;
        switch (s[0] - '0')
        {
        case 1:
            user = login();
            break;
        case 2:
            cout << "\033[2J\033[1;1H";
            exit(0);
            break;
        default:
            cout << "\tPlease chooose a valid choice!\n\n";
            fflush(stdin);
            getchar();
            break;
        }
    }
    return user;
}

string search_book_menu(bool lib = false)
{
    system("cls");
    cout << "----------------------------------------";
    cout << "---------------------\n\n";
    cout << "\t\tSEARCH BOOKS!!!\n"
         << endl;
    cout << "\tEnter book title or ISBN: ";
    string search_string;
    fflush(stdin);
    getline(cin, search_string);
    Book *book = book_database.search(search_string);
    if (book == nullptr)
    {
        cout << "\n\tNo books found.\n";
        cout << "\tPress enter to continue...";
        fflush(stdin);
        getchar();
        return "";
    }
    cout << "\n\tBook found:\n\n";
    cout << "\tTitle: " << book->title << endl;
    cout << "\tAuthor: " << book->author << endl;
    cout << "\tISBN: " << book->ISBN << endl;
    cout << "\tPublication: " << book->publication << endl;
    cout << "\tAvailabile: " << (book->is_available ? "Yes" : "No") << endl;
    if (lib && !book->is_available)
    {
        cout << "\tIssued to: " << book->issued_to << endl;
        cout << "\tIssued on: " << ctime(&book->issued_on) << endl;
        time_t due_date;
        if (user_database.search(book->issued_to)->user_type == User::STUDENT)
            due_date = book->issued_on + Student::MAX_ISSUE_DAYS * 24 * 60 * 60;
        else
            due_date = book->issued_on + Professor::MAX_ISSUE_DAYS * 24 * 60 * 60;
        cout << "\tDue date: " << ctime(&due_date) << endl;
    }
    cout << endl;
    if (book->is_available && !lib)
    {
        while (true)
        {
            cout << "\t1. Issue this book\n";
            cout << "\t2. Return to previous menu\n\n";
            cout << "\tEnter your choice: ";
            string s;
            cin >> s;
            switch (s[0] - '0')
            {
            case 1:
                return book->title;
            case 2:
                return "";
            default:
                cout << "\tPlease chooose a valid choice!\n\n";
                fflush(stdin);
                getchar();
                break;
            }
        }
    }
    else
    {
        if (!lib)
            cout << "\tThis book is not available.\n";
        cout << "\tPress enter to continue...";
        fflush(stdin);
        getchar();
        return "";
    }
    return "";
}

void issue_book_menu(User *user)
{
    while (true)
    {
        system("cls");
        cout << "----------------------------------------";
        cout << "---------------------\n\n";
        cout << "\t\tISSUE BOOK!!!\n"
             << endl;

        if (user->user_type == User::STUDENT)
        {
            Student *student = (Student *)user;
            if (student->book_list.size() == Student::BOOK_LIMIT)
            {
                cout << "\n\tYou have already issued " << Student::BOOK_LIMIT << " books.\n";
                cout << "\tPress enter to continue...";
                fflush(stdin);
                getchar();
                return;
            }
        }
        else
        {
            Professor *professor = (Professor *)user;
            if (professor->book_list.size() == Professor::BOOK_LIMIT)
            {
                cout << "\n\tYou have already issued " << Professor::BOOK_LIMIT << " books.\n";
                cout << "\tPress enter to continue...";
                fflush(stdin);
                getchar();
                return;
            }
        }

        cout << "\t1. See available books list\n";
        cout << "\t2. Search for a book\n";
        cout << "\t3. Return to previous menu\n\n";
        cout << "\tEnter your choice: ";
        string s;
        cin >> s;
        switch (s[0] - '0')
        {
        case 1:
        {
            book_database.print_available_book_list();
            break;
        }
        case 2:
        {
            string book_name = search_book_menu();
            if (book_name != "")
            {
                book_database.issue_book(book_name, user);
                cout << "\n\tBook issued successfully.\n";
                cout << "\tPress enter to continue...";
                fflush(stdin);
                getchar();
            }
            break;
        }
        case 3:
        {
            return;
        }
        default:
        {
            cout << "\tPlease chooose a valid choice!\n\n";
            fflush(stdin);
            getchar();
            break;
        }
        }
    }
}

void return_book_menu(User *user)
{
    system("cls");
    cout << "----------------------------------------";
    cout << "---------------------\n\n";
    cout << "\t\tRETURN BOOK!!!\n"
         << endl;

    bool books_found = true;
    if (user->user_type == User::STUDENT)
    {
        Student *student = (Student *)user;
        if (student->book_list.size() == 0)
            books_found = false;
    }
    else
    {
        Professor *professor = (Professor *)user;
        if (professor->book_list.size() == 0)
            books_found = false;
    }
    if (!books_found)
    {
        cout << "\n\tYou have no books to return.\n";
        cout << "\tPress enter to continue...";
        fflush(stdin);
        getchar();
        return;
    }

    book_database.print_issued_books(user);
    cout << "\n\tEnter the book title/isbn to return: ";
    string book_name;
    fflush(stdin);
    getline(cin, book_name);
    Book *book = book_database.search(book_name);
    string title;
    if (book == nullptr)
        title = "";
    else
        title = book->title;
    if (book_database.return_book(title, user))
    {
        cout << "\n\tBook returned successfully.\n";
        cout << "\tPress enter to continue...";
        fflush(stdin);
        getchar();
    }
    else
    {
        cout << "\n\tBook not found.\n";
        cout << "\tPress enter to continue...";
        fflush(stdin);
        getchar();
    }
}

void show_std_prof_menu(User::User_Type user_type, User *user)
{
    while (true)
    {
        system("cls");
        cout << "-------------------------";
        cout << "---------------------\n\n";
        cout << "\t\tMAIN MENU\n"
             << endl;
        cout << "\t1. View all books\n";
        cout << "\t2. View issued books\n";
        cout << "\t3. Issue book\n";
        cout << "\t4. Return book\n";
        cout << "\t5. View pending fine\n";
        cout << "\t6. Pay fine\n";
        cout << "\t7. Logout\n\n";
        cout << "\tEnter your choice: ";
        string s;
        cin >> s;
        switch (s[0] - '0')
        {
        case 1:
            book_database.print_book_list();
            break;
        case 2:
            book_database.print_issued_books(user);
            cout << "\tPress enter to continue...";
            fflush(stdin);
            getchar();
            break;
        case 3:
            issue_book_menu(user);
            break;
        case 4:
            return_book_menu(user);
            break;
        case 5:
            if (user_type == User::STUDENT)
            {
                Student *student = (Student *)user;
                cout << "\n\tPending fine: " << student->pending_fine << '\n'
                     << endl;
            }
            else
            {
                Professor *professor = (Professor *)user;
                cout << "\n\tPending fine: " << professor->pending_fine << '\n'
                     << endl;
            }
            cout << "\tPress enter to continue...";
            fflush(stdin);
            getchar();
            break;
        case 6:
            if (user_type == User::STUDENT)
            {
                Student *student = (Student *)user;
                student->clear_fine();
            }
            else
            {
                Professor *professor = (Professor *)user;
                professor->clear_fine();
            }
            cout << "\tFine cleared\n"
                 << endl;
            cout << "\tPress enter to continue...";
            fflush(stdin);
            getchar();
            break;
        case 7:
            return;
        default:
            cout << "\tPlease chooose a valid choice!\n\n";
            fflush(stdin);
            getchar();
            break;
        }
    }
}

void add_book_menu()
{
    system("cls");
    cout << "-------------------------";
    cout << "---------------------\n\n";
    cout << "\t\tADD BOOK\n"
         << endl;
    string title, author, isbn, publication;
    cout << "\tEnter the book title: ";
    fflush(stdin);
    getline(cin, title);
    cout << "\tEnter the book author: ";
    fflush(stdin);
    getline(cin, author);
    cout << "\tEnter the book ISBN: ";
    fflush(stdin);
    getline(cin, isbn);
    cout << "\tEnter the book publication: ";
    fflush(stdin);
    getline(cin, publication);
    book_database.add_book(title, author, isbn, publication);
    cout << "\n\tBook added successfully.\n";
    cout << "\tPress enter to continue...";
    fflush(stdin);
    getchar();
}

void update_book_menu()
{
    system("cls");
    cout << "-------------------------";
    cout << "---------------------\n\n";
    cout << "\t\tUPDATE BOOK\n"
         << endl;
    string title, author, isbn, publication;
    cout << "\tEnter the book title/ISBN to find the book to update: ";
    fflush(stdin);
    getline(cin, title);
    Book *book = book_database.search(title);
    if (book == nullptr)
    {
        cout << "\n\tBook not found.\n";
        cout << "\tPress enter to continue...";
        fflush(stdin);
        getchar();
        return;
    }
    cout << "\tTitle: " << book->title << endl;
    cout << "\tAuthor: " << book->author << endl;
    cout << "\tISBN: " << book->ISBN << endl;
    cout << "\tPublication: " << book->publication << endl;
    cout << "\n\tEnter the new book details to change. Leave a field empty to retain current information.\n\n";
    cout << "\tEnter the new book title: ";
    fflush(stdin);
    getline(cin, title);
    cout << "\tEnter the new book author: ";
    fflush(stdin);
    getline(cin, author);
    cout << "\tEnter the new book ISBN: ";
    fflush(stdin);
    getline(cin, isbn);
    cout << "\tEnter the new book publication: ";
    fflush(stdin);
    getline(cin, publication);
    book_database.update_book(book, title, author, isbn, publication);
    cout << "\n\tBook updated successfully.\n";
    cout << "\tPress enter to continue...";
    fflush(stdin);
    getchar();
}

void delete_book_menu()
{
    system("cls");
    cout << "-------------------------";
    cout << "---------------------\n\n";
    cout << "\t\tDELETE BOOK\n"
         << endl;
    string title, author, isbn, publication;
    cout << "\tEnter the book title/ISBN to find the book to remove: ";
    fflush(stdin);
    getline(cin, title);
    Book *book = book_database.search(title);
    if (book == nullptr)
    {
        cout << "\n\tBook not found.\n";
        cout << "\tPress enter to continue...";
        fflush(stdin);
        getchar();
        return;
    }
    cout << "\tTitle: " << book->title << endl;
    cout << "\tAuthor: " << book->author << endl;
    cout << "\tISBN: " << book->ISBN << endl;
    cout << "\tPublication: " << book->publication << endl;
    cout << "\n\tAre you sure you want to remove this book? (y/n): ";
    string s;
    cin >> s;
    if (s[0] == 'y' || s[0] == 'Y')
    {
        book_database.delete_book(book);
        cout << "\n\tBook removed successfully.\n";
    }
    else if (s[0] == 'n' || s[0] == 'N')
    {
        cout << "\n\tBook not removed.\n";
    }
    else
    {
        cout << "\n\tInvalid input. Book not removed.\n";
    }
    cout << "\tPress enter to continue...";
    fflush(stdin);
    getchar();
}

void modify_book_menu()
{
    while (true)
    {
        system("cls");
        cout << "-------------------------";
        cout << "---------------------\n\n";
        cout << "\t\tMODIFY BOOK\n"
             << endl;
        cout << "\t1. Add book\n";
        cout << "\t2. Update book\n";
        cout << "\t3. Delete book\n";
        cout << "\t4. Return to previous menu\n\n";
        cout << "\tEnter your choice: ";
        string s;
        cin >> s;
        switch (s[0] - '0')
        {
        case 1:
            add_book_menu();
            break;
        case 2:
            update_book_menu();
            break;
        case 3:
            delete_book_menu();
            break;
        case 4:
            return;
        default:
            cout << "\tPlease choose a valid choice!\n\n";
            fflush(stdin);
            getchar();
            break;
        }
    }
}

void add_user_menu()
{
    system("cls");
    cout << "-------------------------";
    cout << "---------------------\n\n";
    cout << "\t\tADD USER\n"
         << endl;
    string user_type, name, id, password;
    cout << "\tEnter the user type (student/professor/librarian): ";
    fflush(stdin);
    getline(cin, user_type);
    cout << "\tEnter the user name: ";
    fflush(stdin);
    getline(cin, name);
    cout << "\tEnter the user id: ";
    fflush(stdin);
    getline(cin, id);
    cout << "\tEnter the password: ";
    fflush(stdin);
    getline(cin, password);
    if (user_type == "student")
        user_database.add_user(name, id, password, User::STUDENT);
    else if (user_type == "professor")
        user_database.add_user(name, id, password, User::PROFESSOR);
    else if (user_type == "librarian")
        user_database.add_user(name, id, password, User::LIBRARIAN);
    else
    {
        cout << "\tInvalid user type.\n";
        cout << "\tPress enter to continue...";
        fflush(stdin);
        getchar();
        return;
    }
    cout << "\n\tUser added successfully.\n";
    cout << "\tPress enter to continue...";
    fflush(stdin);
    getchar();
}

void update_user_menu()
{
    system("cls");
    cout << "-------------------------";
    cout << "---------------------\n\n";
    cout << "\t\tUPDATE USER\n"
         << endl;
    string user_type, name, id, password;
    cout << "\tEnter the user id/name to find the user to update: ";
    fflush(stdin);
    getline(cin, id);
    User *user = user_database.search(id);
    if (user == nullptr)
    {
        cout << "\n\tUser not found.\n";
        cout << "\tPress enter to continue...";
        fflush(stdin);
        getchar();
        return;
    }
    cout << "\tUser found.\n";
    cout << "\tUser type: ";
    if (user->user_type == User::STUDENT)
        cout << "student\n";
    else if (user->user_type == User::PROFESSOR)
        cout << "professor\n";
    else
        cout << "librarian\n";
    cout << "\tUser name: " << user->name << endl;
    cout << "\tUser id: " << user->id << endl;

    cout << "\n\tEnter the new user details to change. Leave a field empty to retain current information.\n\n";
    cout << "\tEnter the new user name: ";
    fflush(stdin);
    getline(cin, name);
    cout << "\tEnter the new user id: ";
    fflush(stdin);
    getline(cin, id);
    cout << "\tEnter the new password: ";
    fflush(stdin);
    getline(cin, password);
    user_database.update_user(user, name, id, password);
    cout << "\n\tUser updated successfully.\n";
    cout << "\tPress enter to continue...";
    fflush(stdin);
    getchar();
}

void delete_user_menu()
{
    system("cls");
    cout << "-------------------------";
    cout << "---------------------\n\n";
    cout << "\t\tDELETE USER\n"
         << endl;
    string id;
    cout << "\tEnter the user id/name to find the user to remove: ";
    fflush(stdin);
    getline(cin, id);
    User *user = user_database.search(id);
    if (user == nullptr)
    {
        cout << "\n\tUser not found.\n";
        cout << "\tPress enter to continue...";
        fflush(stdin);
        getchar();
        return;
    }
    cout << "\tUser found.\n";
    cout << "\tUser type: " << user->user_type << endl;
    cout << "\tUser name: " << user->name << endl;
    cout << "\tUser id: " << user->id << endl;
    cout << "\n\tAre you sure you want to remove this user? (y/n): ";
    string s;
    cin >> s;
    if (s[0] == 'y' || s[0] == 'Y')
    {
        user_database.delete_user(user);
        cout << "\n\tUser removed successfully.\n";
    }
    else if (s[0] == 'n' || s[0] == 'N')
    {
        cout << "\n\tUser not removed.\n";
    }
    else
    {
        cout << "\n\tInvalid input. User not removed.\n";
    }
    cout << "\tPress enter to continue...";
    fflush(stdin);
    getchar();
}

void modify_user_menu()
{
    while (true)
    {
        system("cls");
        cout << "-------------------------";
        cout << "---------------------\n\n";
        cout << "\t\tMODIFY USER\n"
             << endl;
        cout << "\t1. Add user\n";
        cout << "\t2. Update user\n";
        cout << "\t3. Delete user\n";
        cout << "\t4. Return to previous menu\n\n";
        cout << "\tEnter your choice: ";
        string s;
        cin >> s;
        switch (s[0] - '0')
        {
        case 1:
            add_user_menu();
            break;
        case 2:
            update_user_menu();
            break;
        case 3:
            delete_user_menu();
            break;
        case 4:
            return;
        default:
            cout << "\tPlease choose a valid choice!\n\n";
            fflush(stdin);
            getchar();
            break;
        }
    }
}

void search_user_menu()
{
    system("cls");
    cout << "-------------------------";
    cout << "---------------------\n\n";
    cout << "\t\tSEARCH USER\n"
         << endl;
    string id;
    cout << "\tEnter the user id/name to find the user: ";
    fflush(stdin);
    getline(cin, id);
    User *user = user_database.search(id);
    if (user == nullptr)
    {
        cout << "\n\tUser not found.\n";
        cout << "\tPress enter to continue...";
        fflush(stdin);
        getchar();
        return;
    }
    cout << "\tUser found.\n\n";
    if (user->user_type == User::STUDENT)
        cout << "\tUser type: student\n";
    else if (user->user_type == User::PROFESSOR)
        cout << "\tUser type: professor\n";
    else
        cout << "\tUser type: librarian\n";
    cout << "\tUser name: " << user->name << endl;
    cout << "\tUser id: " << user->id << endl;
    if (user->user_type != User::LIBRARIAN)
    {
        cout << "\n\tBooks issued by this user:\n\n";
        book_database.print_issued_books(user, false);
    }
    cout << "\n\tPress enter to continue...";
    fflush(stdin);
    getchar();
}

void show_librarian_menu()
{
    while (true)
    {
        system("cls");
        cout << "-------------------------";
        cout << "---------------------\n\n";
        cout << "\t\tMAIN MENU\n"
             << endl;
        cout << "\t1. Add/Update/Delete Books\n";
        cout << "\t2. Add/Update/Delete Users\n";
        cout << "\t3. View all books\n";
        cout << "\t4. View all users\n";
        cout << "\t5. Find User that issued the Book\n";
        cout << "\t6. Find books issued by the User\n";
        cout << "\t7. Logout\n\n";
        cout << "\tEnter your choice: ";
        string s;
        cin >> s;
        switch (s[0] - '0')
        {
        case 1:
            modify_book_menu();
            break;
        case 2:
            modify_user_menu();
            break;
        case 3:
            book_database.print_book_list(true);
            break;
        case 4:
            user_database.print_user_list();
            break;
        case 5:
            search_book_menu(true);
            break;
        case 6:
            search_user_menu();
            break;
        case 7:
            return;
        default:
            cout << "\tPlease chooose a valid choice!\n\n";
            fflush(stdin);
            getchar();
            break;
        }
    }
}

int main()
{
    populate_database();
    while (true)
    {
        User *user = welcome_screen();
        if (user->user_type == User::User_Type::LIBRARIAN)
        {
            show_librarian_menu();
        }
        else
        {
            show_std_prof_menu(user->user_type, user);
        }
    }
    return 0;
}