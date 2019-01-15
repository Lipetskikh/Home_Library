#ifndef INITDB__H
#define INITDB__H

#include <QtSql>
#include <QVector>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTime>
#include <QtCore>
#include <QMainWindow>
#include <QtWidgets>
#include <QToolBar>
#include <QLineEdit>
#include <QFileDialog>

//QVector<QVector<QString>> books;
QVector<int> ratings;
QVector<QString> authors;
QVector<QString> genres;
int b = 0;
int a = 0;
int g = 0;

void addBook(QSqlQuery &q, const QString &title, int year, const QString &authorId,
             const QString &genreId, int rating)
{
    q.addBindValue(title);
    q.addBindValue(year);
    q.addBindValue(authorId);
    q.addBindValue(genreId);
    q.addBindValue(rating);
    q.exec();
}

void addGenre(QSqlQuery &q, const QString &name) // QVariant
{
    q.addBindValue(name);
    q.exec();
    //return q.lastInsertId();
}

void addAuthor(QSqlQuery &q, const QString &name, const QDate &birthdate) // QVariant
{
    q.addBindValue(name);
    q.addBindValue(birthdate);
    q.exec();
    //return q.lastInsertId();
}

void loadAuthors(QSqlQuery &q)
{
    // Отображение диалогового окна и получение названия файла, выбранного пользователем
    QString fileName = QFileDialog::getOpenFileName(0, "Open File", "", "Text Files (*.txt)");

    if (fileName != "")
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(0, "Error", "Could not open file");
            return;
        }
        // Загрузка книги из текстового файла
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString name;
            int year;
            int month;
            int day;
            name = in.readLine();
            year = in.readLine().toInt();
            month = in.readLine().toInt();
            day = in.readLine().toInt();
            QDate birthdate(year, month, day);
            authors.push_back(name);
            addAuthor(q, name, birthdate);
            a++;
        }
        file.close();
    }
}

void loadGenres(QSqlQuery &q)
{
    // Отображение диалогового окна и получение названия файла, выбранного пользователем
    QString fileName = QFileDialog::getOpenFileName(0, "Open File", "", "Text Files (*.txt)");

    if (fileName != "")
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(0, "Error", "Could not open file");
            return;
        }
        // Загрузка книги из текстового файла
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString genre;
            genre = in.readLine();
            genres.push_back(genre);
            addGenre(q, genre);
            g++;
        }
        file.close();
    }
}

void loadBooks(QSqlQuery &q)
{
    // Отображение диалогового окна и получение названия файла, выбранного пользователем
    QString fileName = QFileDialog::getOpenFileName(0, "Open File", "", "Text Files (*.txt)");

    if (fileName != "")
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(0, "Error", "Could not open file");
            return;
        }
        // Загрузка книги из текстового файла
        QTextStream in(&file);
        QVector<QString> titles;
        QVector<int> years;
        QVector<QString> auth;
        QVector<QString> gen;
        //QVector<QString> bb;
        //QVector<int> ratings;
        while (!in.atEnd())
        {
            QString title;
            int year;
            QString author;
            QString genre;
            int rating;
            title = in.readLine();
            year = in.readLine().toInt();
            author = in.readLine();
            genre = in.readLine();
            rating = in.readLine().toInt();

            titles.push_back(title);
            years.push_back(year);
            auth.push_back(author);
            gen.push_back(genre);
            ratings.push_back(rating);
            b++;
        }
        file.close();

        for (int i = 0; i < b; i++)
        {
            for (int j = 0; j < a; j++)
            {
                for (int k = 0; k < g; k++)
                {
                    if ((authors.at(j) == auth.at(i)) && (genres.at(k) == gen.at(i)))
                    {
                         addBook(q, titles.at(i), years.at(i), authors.at(j), genres.at(k), ratings.at(i));
                    }
                }
            }
        }
    }
}

QSqlError initDb()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");

    if (!db.open())
        return db.lastError();

    QStringList tables = db.tables();
    if (tables.contains("books", Qt::CaseInsensitive)
        && tables.contains("authors", Qt::CaseInsensitive))
        return QSqlError();

    QSqlQuery q;
    if (!q.exec(QLatin1String("create table books(id integer primary key, title varchar, year integer, author varchar, genre varchar, rating integer)")))
        return q.lastError();
    if (!q.exec(QLatin1String("create table authors(id integer primary key, name varchar, birthdate date)")))
        return q.lastError();
    if (!q.exec(QLatin1String("create table genres(id integer primary key, name varchar)")))
        return q.lastError();

    if (!q.prepare(QLatin1String("insert into authors(name, birthdate) values(?, ?)")))
        return q.lastError();
    loadAuthors(q);
    //QVariant asimovId = addAuthor(q, QLatin1String("Isaac Asimov") , QDate(1920, 2, 1));
    //QVariant greeneId = addAuthor(q, QLatin1String("Graham Greene"), QDate(1904, 10, 2));
    //QVariant pratchettId = addAuthor(q, QLatin1String("Terry Pratchett"), QDate(1948, 4, 28));


    if (!q.prepare(QLatin1String("insert into genres(name) values(?)")))
        return q.lastError();
    loadGenres(q);
    //QVariant sfiction = addGenre(q, QLatin1String("Science Fiction"));
    //QVariant fiction = addGenre(q, QLatin1String("Fiction"));
    //QVariant fantasy = addGenre(q, QLatin1String("Fantasy"));

    if (!q.prepare(QLatin1String("insert into books(title, year, author, genre, rating) values(?, ?, ?, ?, ?)")))
        return q.lastError();
    loadBooks(q);
    /*
    addBook(q, QLatin1String("Foundation"), 1951, asimovId, sfiction, 3);
    addBook(q, QLatin1String("Foundation and Empire"), 1952, asimovId, sfiction, 4);
    addBook(q, QLatin1String("Second Foundation"), 1953, asimovId, sfiction, 3);
    addBook(q, QLatin1String("Foundation's Edge"), 1982, asimovId, sfiction, 3);
    addBook(q, QLatin1String("Foundation and Earth"), 1986, asimovId, sfiction, 4);
    addBook(q, QLatin1String("Prelude to Foundation"), 1988, asimovId, sfiction, 3);
    addBook(q, QLatin1String("Forward the Foundation"), 1993, asimovId, sfiction, 3);
    addBook(q, QLatin1String("The Power and the Glory"), 1940, greeneId, fiction, 4);
    addBook(q, QLatin1String("The Third Man"), 1950, greeneId, fiction, 5);
    addBook(q, QLatin1String("Our Man in Havana"), 1958, greeneId, fiction, 4);
    addBook(q, QLatin1String("Guards! Guards!"), 1989, pratchettId, fantasy, 3);
    addBook(q, QLatin1String("Night Watch"), 2002, pratchettId, fantasy, 3);
    addBook(q, QLatin1String("Going Postal"), 2004, pratchettId, fantasy, 3);
    */

    return QSqlError();
}

#endif // INITDB__H
