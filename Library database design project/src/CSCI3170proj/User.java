package CSCI3170proj;

import java.io.File;
import java.sql.*;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Scanner;
import java.util.*;

public class User {
    public static void user(){
        System.out.print("-----Operations for Library menu-----\n" +
                "What kind of operation would you like to perform\n" +
                "1. Search for Books\n" +
                "2. Show loan record of a user\n" +
                "3. Return to the main menu\n" +
                "Enter Your Choice: ");
        Scanner input = new Scanner(System.in);
        switch(input.nextInt()){
            case 1: User.search();
            case 2: User.show_loan();
            case 3: Main.menu();
        }
    }

    private static void show_loan() {
        System.out.print("Enter the userID: ");
        Scanner input = new Scanner(System.in);

        String userid = input.nextLine();

        try {
            System.out.println("Loan Record: \n" +
                    "|CallNum|CopyNum|Title|Author|Check-out|Returned?|");            Class.forName("com.mysql.cj.jdbc.Driver");
            Connection con = DriverManager.getConnection(DBInfo.dbAddress, DBInfo.dbUserName, DBInfo.dbPassword);
            PreparedStatement stmt = con.prepareStatement(Query.user_show[0]);
            stmt.setString(1,userid);
            ResultSet rs0 = stmt.executeQuery();
            while(rs0.next()){
                SimpleDateFormat sdf = new SimpleDateFormat("YYYY-MM-dd");
                String callnum = rs0.getString(1);
                Integer copynum = rs0.getInt(2);
                String checkout = sdf.format(rs0.getDate(3));
                String returned = rs0.getString(4)!=null ? "yes": "no";

                stmt = con.prepareStatement(Query.user_show[1]);
                stmt.setString(1,callnum);
                ResultSet rs1 = stmt.executeQuery();
                rs1.next();
                String title = rs1.getString(1);
                String aname = rs1.getString(2);
                System.out.println("|"+callnum+"|"+copynum+"|"+title+"|"+aname+"|"+checkout+"|"+returned+"|");

                rs1.close();
            }
            rs0.close();
            stmt.close();
            con.close();
        } catch (Exception e)
        {
            System.out.println(e);
        }
        finally {
            System.out.println("End of Query");
            User.user();
        }
    }

    private static void search() {
        System.out.print("Choose the Search criterion:\n" +
                "1. call number\n" +
                "2. title\n" +
                "3. author\n" +
                "Choose the search criterion: ");
        Scanner input = new Scanner(System.in);
        switch(input.nextInt()){
            case 1: User.searchByCallnum();
            case 2: User.searchByTitle();
            case 3: User.searchByAuthor();
        }
    }

    private static void searchByAuthor() {
        System.out.print("Type in the Search Keyword: ");
        Scanner input = new Scanner(System.in);
        String keyword = input.nextLine();
        try {
            System.out.println("|Call Num|Title|Book Category|Author|Rating|Available No. of Copy|");
            Class.forName("com.mysql.cj.jdbc.Driver");
            Connection con = DriverManager.getConnection(DBInfo.dbAddress, DBInfo.dbUserName, DBInfo.dbPassword);

            PreparedStatement stmt = con.prepareStatement(Query.user_search[6]);
            stmt.setString(1,'%'+keyword+'%');
            ResultSet rs0 = stmt.executeQuery();
            while (rs0.next()) {
                String each_callnum = rs0.getString(1);
                stmt = con.prepareStatement(Query.user_search[0]);
                stmt.setString(1, each_callnum);
                ResultSet rs1 = stmt.executeQuery();
                while (rs1.next()) {
                    String callnum = rs1.getString(1);
                    String title = rs1.getString(2);
                    Double rating = rs1.getDouble(3);
                    Integer bcid = rs1.getInt(4);

                    stmt = con.prepareStatement(Query.user_search[1]);
                    stmt.setString(1, callnum);
                    ResultSet rs2 = stmt.executeQuery();
                    rs2.next();
                    String author = rs2.getString(1);

                    stmt = con.prepareStatement(Query.user_search[2]);
                    stmt.setInt(1, bcid);
                    ResultSet rs3 = stmt.executeQuery();
                    rs3.next();
                    String bcname = rs3.getString(1);

                    stmt = con.prepareStatement(Query.user_search[3]);
                    stmt.setString(1, callnum);
                    ResultSet rs4 = stmt.executeQuery();
                    rs4.next();

                    stmt = con.prepareStatement(Query.user_search[4]);
                    stmt.setString(1, callnum);
                    ResultSet rs5 = stmt.executeQuery();
                    rs5.next();
                    Integer no_copy = rs4.getInt(1) - rs5.getInt(1);

                    System.out.println("|" + callnum + "|" + title + "|" + bcname + "|" + author + "|" + rating + "|" + no_copy + "|");

                    rs2.close();
                    rs3.close();
                    rs4.close();
                    rs5.close();
                }
                rs1.close();
            }
            stmt.close();
            con.close();
        } catch (Exception e)
        {
            System.out.println(e);
        }
        finally {
            System.out.println("End of Query");
            User.user();
        }
    }

    private static void searchByTitle() {
        System.out.print("Type in the Search Keyword: ");
        Scanner input = new Scanner(System.in);
        String keyword = input.nextLine();
        try {
            System.out.println("|Call Num|Title|Book Category|Author|Rating|Available No. of Copy|");
            Class.forName("com.mysql.cj.jdbc.Driver");
            Connection con = DriverManager.getConnection(DBInfo.dbAddress, DBInfo.dbUserName, DBInfo.dbPassword);
            PreparedStatement stmt = con.prepareStatement(Query.user_search[5]);
            stmt.setString(1,'%'+keyword+'%');
            ResultSet rs1 = stmt.executeQuery();
            while(rs1.next()){
                String callnum = rs1.getString(1);
                String title = rs1.getString(2);
                Double rating = rs1.getDouble(3);
                Integer bcid = rs1.getInt(4);

                stmt = con.prepareStatement(Query.user_search[1]);
                stmt.setString(1,callnum);
                ResultSet rs2 = stmt.executeQuery();
                rs2.next();
                String author = rs2.getString(1);

                stmt = con.prepareStatement(Query.user_search[2]);
                stmt.setInt(1,bcid);
                ResultSet rs3 = stmt.executeQuery();
                rs3.next();
                String bcname = rs3.getString(1);

                stmt = con.prepareStatement(Query.user_search[3]);
                stmt.setString(1,callnum);
                ResultSet rs4 = stmt.executeQuery();
                rs4.next();

                stmt = con.prepareStatement(Query.user_search[4]);
                stmt.setString(1,callnum);
                ResultSet rs5= stmt.executeQuery();
                rs5.next();
                Integer no_copy = rs4.getInt(1) - rs5.getInt(1);

                System.out.println("|"+callnum+"|"+title+"|"+bcname+"|"+author+"|"+rating+"|"+no_copy+"|");

                rs2.close();
                rs3.close();
                rs4.close();
                rs5.close();
            }
            rs1.close();
            stmt.close();
            con.close();
        } catch (Exception e)
        {
            System.out.println(e);
        }
        finally {
            System.out.println("End of Query");
            User.user();
        }

    }

    private static void searchByCallnum() {
        System.out.print("Type in the Search Keyword: ");
        Scanner input = new Scanner(System.in);
        String keyword = input.nextLine();
        try {
            System.out.println("|Call Num|Title|Book Category|Author|Rating|Available No. of Copy|");
            Class.forName("com.mysql.cj.jdbc.Driver");
            Connection con = DriverManager.getConnection(DBInfo.dbAddress, DBInfo.dbUserName, DBInfo.dbPassword);
            PreparedStatement stmt = con.prepareStatement(Query.user_search[0]);
            stmt.setString(1,keyword);
            ResultSet rs1 = stmt.executeQuery();
            while(rs1.next()){
                String callnum = rs1.getString(1);
                String title = rs1.getString(2);
                Double rating = rs1.getDouble(3);
                Integer bcid = rs1.getInt(4);

                stmt = con.prepareStatement(Query.user_search[1]);
                stmt.setString(1,callnum);
                ResultSet rs2 = stmt.executeQuery();
                rs2.next();
                String author = rs2.getString(1);

                stmt = con.prepareStatement(Query.user_search[2]);
                stmt.setInt(1,bcid);
                ResultSet rs3 = stmt.executeQuery();
                rs3.next();
                String bcname = rs3.getString(1);

                stmt = con.prepareStatement(Query.user_search[3]);
                stmt.setString(1,callnum);
                ResultSet rs4 = stmt.executeQuery();
                rs4.next();

                stmt = con.prepareStatement(Query.user_search[4]);
                stmt.setString(1,callnum);
                ResultSet rs5= stmt.executeQuery();
                rs5.next();
                Integer no_copy = rs4.getInt(1) - rs5.getInt(1);

                System.out.println("|"+callnum+"|"+title+"|"+bcname+"|"+author+"|"+rating+"|"+no_copy+"|");

                rs2.close();
                rs3.close();
                rs4.close();
                rs5.close();
            }
            rs1.close();
            stmt.close();
            con.close();
        } catch (Exception e)
        {
            System.out.println(e);
        }
        finally {
            System.out.println("End of Query");
            User.user();
        }

    }


}
