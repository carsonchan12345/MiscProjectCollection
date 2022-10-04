package CSCI3170proj;

public final class Query {
    public static String[] admin_create = new String[]{
            "CREATE TABLE user_category(ucid INT, max INT not NULL, period INT not NULL, PRIMARY key(ucid))",
            "CREATE TABLE libuser(libuid CHAR(10), name VARCHAR(25) not NULL, age INT not NULL, address VARCHAR(100) not NULL, ucid INT not NULL, PRIMARY key(libuid))",
            "CREATE TABLE book_category(bcid INT, bcname VARCHAR(30) not NULL,PRIMARY KEY (bcid))",
            "CREATE TABLE book(callnum CHAR(8), title VARCHAR(30) NOT NULL, publish DATE NOT NULL, rating DOUBLE, tborrowed INT not NULL, bcid INT, PRIMARY KEY (callnum))",
            "CREATE TABLE copy(callnum CHAR(8), copynum INT, PRIMARY KEY (callnum,copynum))",
            "CREATE TABLE borrow(libuid CHAR(10), callnum CHAR(8), copynum INT, checkout DATE, return_date DATE, PRIMARY KEY (libuid,callnum,copynum,checkout))",
            "CREATE TABLE authorship(aname VARCHAR(30), callnum CHAR(8), PRIMARY KEY (aname,callnum))"
    };

    public static String[] admin_delete = new String[]{
            "DROP TABLE user_category",
            "DROP TABLE libuser",
            "DROP TABLE book_category",
            "DROP TABLE book",
            "DROP TABLE copy",
            "DROP TABLE borrow",
            "DROP TABLE authorship"
    };

    public static String[] admin_load = new String[]{
            "INSERT INTO user_category VALUES(?,?,?)",
            "INSERT INTO libuser VALUES(?,?,?,?,?)",
            "INSERT INTO book_category VALUES(?,?)",
            "INSERT INTO book VALUES(?,?,?,?,?,?)",
            "INSERT INTO copy VALUES(?,?)",
            "INSERT INTO borrow VALUES(?,?,?,?,?)",
            "INSERT INTO authorship VALUES(?,?)"
    };

    public static String[] admin_record = new String[]{
            "SELECT COUNT(*) FROM user_category",
            "SELECT COUNT(*) FROM libuser",
            "SELECT COUNT(*) FROM book_category",
            "SELECT COUNT(*) FROM book",
            "SELECT COUNT(*) FROM copy",
            "SELECT COUNT(*) FROM borrow",
            "SELECT COUNT(*) FROM authorship"
    };

    public static String[] user_search = new String[]{
            "SELECT callnum,title,rating,bcid FROM book WHERE callnum = ? ",
            "SELECT aname FROM authorship WHERE callnum = ? ",
            "SELECT bcname FROM book_category WHERE bcid = ? ",
            "SELECT COUNT(*) FROM copy WHERE callnum= ?  ",
            "SELECT COUNT(*) FROM borrow WHERE callnum= ? AND return_date IS NULL ",
            "SELECT callnum,title,rating,bcid FROM book WHERE title LIKE ? ORDER BY callnum",
            "SELECT DISTINCT callnum FROM authorship WHERE aname LIKE ? ORDER BY callnum "

    };

    public static String[] user_show = new String[]{
            "SELECT callnum,copynum,checkout,return_date FROM borrow WHERE libuid = ? ",
            "SELECT title,aname FROM book,authorship WHERE book.callnum = ? "

    };

    public static String[] librarian_b_borrow = new String[]{
            "SELECT COUNT(*) FROM copy WHERE callnum= ? ",
            "SELECT * FROM borrow WHERE callnum = ? AND copynum = ? AND return_date IS NULL ",
            "INSERT INTO borrow VALUES(?,?,?,?,?)"
    };

    public static String[] librarian_b_return = new String[]{
            "SELECT * FROM borrow WHERE callnum= ? AND copynum = ? AND libuid = ? AND return_date IS NULL",
            "UPDATE borrow SET return_date = ? WHERE callnum= ? AND copynum = ? AND libuid = ? ",
            "SELECT tborrowed FROM book WHERE callnum = ? ",
            "SELECT rating FROM book WHERE callnum = ? ",
            "UPDATE book SET rating = ? WHERE callnum = ? ",
            "UPDATE book SET tborrowed = ? WHERE callnum = ? "
    };

    public static String librarian_list_all=new String(
            "SELECT libuid,callnum,copynum,checkout FROM borrow WHERE checkout >= ? AND checkout <= ? AND return_date IS NULL ORDER BY checkout DESC"
    );


    public static String user_searchByCallnum=new String("SELECT b.callnum, b.title, b_c.bcname, a.aname, b.rating " +
            "FROM book b,book_category b_c,copy c, authorship a " +
            "WHERE b.bcid=book_category.bcid and ?=book.callnum and a.callnum=b.callnum " +
            "GROUP BY b.callnum ");


    public static String user_searchByTitle=new String("SELECT b.callnum, b.title, b_c.bcname, a.aname, b.rating" +
            "FROM (SELECT * FROM book b WHERE b.title like %?%) b2,book_category b_c,copy c, authorship a" +
            "WHERE b2.bcid=b_c.bcid and a.callnum=b2.callnum" +
            "GROUP BY b2.callnum" +
            "ORDER BY b2.callnum");

    public static String user_searchByAuthor=new String("SELECT b.callnum, b.title, b_c.bcname, a.aname, b.rating" +
            "FROM book b,book_category b_c,copy c, (SELECT * FROM authoriship a WHERE a.aname like %?%) a2" +
            "WHERE book.bcid=book_category.bcid and a2.callnum=b.callnum" +
            "GROUP BY b.callnum" +
            "ORDER BY b.callnum");
//further determine return to yes or no
    public static String user_loan=new String("SELECT b.libuid, b.callnum, b.copynum, b.checkout, b.return_date " +
            "FROM  borrow b " +
            "WHERE  b.libuid=? " +
            "ORDER BY b.checkout DESC");

    //find available copy
    //tmp1=SELECT COUNT(*) FROM copy WHERE callnum=? GROUP BY callnum  (this find total number of copy)
    //tmp2=SELECT COUNT(*) FROM borrow WHERE callnum=? and return is NULL GROUP BY callnum (this find the number of borrowed book)
    //available copy= tmp1 - tmp2

}






