package CSCI3170proj;
import java.sql.*;
import java.util.*;

public class Main {
    //testing the connection of database
    private static void connectionTest(){
        try{
            Class.forName("com.mysql.cj.jdbc.Driver");
            Connection con=DriverManager.getConnection(DBInfo.dbAddress, DBInfo.dbUserName, DBInfo.dbPassword);
            con.close();
            System.out.println("Successfully connected to the database!");
        }catch(Exception e){
            System.out.println(e);
        }
        }

        //Main menu
    public static void menu(){
        System.out.println("Welcome to Library Inquiry System!\n");
        System.out.print("-----Main menu-----\nWhat kinds of operations would you like to perform?\n" +
                "1. Operations for Administrator\n" +
                "2. Operations for Library User\n" +
                "3. Operations for Librarian\n" +
                "4. Exit this program\n" +
                "Enter Your Choice: ");
        Scanner input = new Scanner(System.in);
        switch(input.nextInt()){
            case 1: Admin.admin();
            case 2: User.user();
            case 3: Librarian.librarian();
            case 4: exit();
        }
    }
    public static void exit(){
        System.exit(0);
    }
    public static void main(String[] args){
        Main.connectionTest();
        Main.menu();
    }

}
