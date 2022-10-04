package CSCI3170proj;

import java.io.File;
import java.sql.*;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Scanner;
import java.util.*;

public class Librarian {

    public static void librarian(){
        System.out.print("-----Operations for librarian menu-----\n" +
                "What kind of operation would you like to perform\n" +
                "1. Book Borrowing\n" +
                "2. Book Returning\n" +
                "3. List all un-returned book copies which are checked-out within a period\n" +
                "4. Return to The main menu\n" +
                "Enter Your Choice: ");
        Scanner input = new Scanner(System.in);
        switch(input.nextInt()){
            case 1: Librarian.b_borrow();
            case 2: Librarian.b_return();
            case 3: Librarian.list_all();
            case 4: Main.menu();
        }
    }

    private static void list_all() {
        try {
            Scanner input = new Scanner(System.in);
            SimpleDateFormat sdf = new SimpleDateFormat("dd/MM/yyyy");
            System.out.print("Type in the starting date [dd/mm/yyyy]: ");
            String start = input.nextLine();
            java.sql.Date start_date = new java.sql.Date(sdf.parse(start).getTime());
            System.out.print("Type in the ending date [dd/mm/yyyy]: ");
            String end = input.nextLine();
            java.sql.Date end_date = new java.sql.Date(sdf.parse(end).getTime());
            System.out.println("List of UnReturned Book:\n" +
                    "|LibUID|CallNum|CopyNum|Checkout|");
            Connection con = DriverManager.getConnection(DBInfo.dbAddress, DBInfo.dbUserName, DBInfo.dbPassword);
            PreparedStatement stmt = con.prepareStatement(Query.librarian_list_all);
            stmt.setDate(1, start_date);
            stmt.setDate(2,  end_date);
            ResultSet rs1 = stmt.executeQuery();
            while(rs1.next()){
                String libuid = rs1.getString(1);
                String callnum = rs1.getString(2);
                Integer copynum = rs1.getInt(3);
                Date checkout = rs1.getDate(4);

                System.out.println("|"+libuid+"|"+callnum+"|"+copynum+"|"+sdf.format(checkout)+"|");
            }

            rs1.close();
            stmt.close();
            con.close();

        }catch (Exception e){
            System.out.println(e);
        }finally {
            System.out.println("End of Query");
            Librarian.librarian();
        }
    }

    private static void b_return() {
        Scanner input = new Scanner(System.in);
        System.out.print("Enter The User ID: ");
        String user_id = input.nextLine();
        System.out.print("Enter The Call Number: ");
        String callnum = input.nextLine();
        System.out.print("Enter The Copy Number: ");
        Integer copynum = input.nextInt();

        try {
            Connection con = DriverManager.getConnection(DBInfo.dbAddress, DBInfo.dbUserName, DBInfo.dbPassword);
            PreparedStatement stmt = con.prepareStatement(Query.librarian_b_return[0]);
            stmt.setString(1, callnum);
            stmt.setInt(2, copynum);
            stmt.setString(3, user_id);
            ResultSet rs1 = stmt.executeQuery();

            if(rs1.next()){
                stmt = con.prepareStatement(Query.librarian_b_return[1]);
                java.sql.Date date = new java.sql.Date(Calendar.getInstance().getTime().getTime());
                stmt.setDate(1, date);
                stmt.setString(2, callnum);
                stmt.setInt(3, copynum);
                stmt.setString(4, user_id);
                stmt.executeUpdate();

                System.out.print("Enter Your Rating of the book: ");
                Double use_rating = input.nextDouble();
                stmt = con.prepareStatement(Query.librarian_b_return[2]);
                stmt.setString(1, callnum);
                ResultSet rs2 = stmt.executeQuery();
                rs2.next();
                Integer time_borrow = rs2.getInt(1);

                stmt = con.prepareStatement(Query.librarian_b_return[3]);
                stmt.setString(1, callnum);
                ResultSet rs3 = stmt.executeQuery();
                rs3.next();
                Double old_rating = rs3.getDouble(1);

                Double new_rating = (old_rating*time_borrow+use_rating)/(time_borrow+1);
                //System.out.println(old_rating+"  "+time_borrow+"  "+use_rating+"  "+new_rating);
                stmt = con.prepareStatement(Query.librarian_b_return[4]);
                stmt.setDouble(1, new_rating);
                stmt.setString(2, callnum);
                stmt.executeUpdate();

                stmt = con.prepareStatement(Query.librarian_b_return[5]);
                stmt.setInt(1, time_borrow+1);
                stmt.setString(2, callnum);
                stmt.executeUpdate();

                rs2.close();
                rs3.close();
                System.out.println("Book returning Performed successfully.");
            }
            else{
                System.out.println("[Error] An matching borrow record is not found. ");
              //  System.out.println("[Error] An matching borrow record is not found. The book has not been borrowed yet.");
            }
            rs1.close();
            stmt.close();
            con.close();

        }catch (Exception e){
            System.out.println(e);
        }finally {
            Librarian.librarian();
        }
    }

    private static void b_borrow() {
        Scanner input = new Scanner(System.in);
        System.out.print("Enter The User ID: ");
        String user_id = input.nextLine();
        System.out.print("Enter The Call Number: ");
        String callnum = input.nextLine();
        System.out.print("Enter The Copy Number: ");
        Integer copynum = input.nextInt();
        SimpleDateFormat tmpdate = new SimpleDateFormat("dd/MM/yyyy");

        try {
            Connection con = DriverManager.getConnection(DBInfo.dbAddress, DBInfo.dbUserName, DBInfo.dbPassword);
            PreparedStatement stmt = con.prepareStatement(Query.librarian_b_borrow[0]);
            stmt.setString(1, callnum);
            ResultSet rs1 = stmt.executeQuery();
            Integer total_copy = 0;
            rs1.next();
            total_copy = rs1.getInt(1);
            stmt = con.prepareStatement(Query.librarian_b_borrow[1]);
            stmt.setString(1, callnum);
            stmt.setInt(2, copynum);
            ResultSet rs2 = stmt.executeQuery();
            if(!rs2.next() && total_copy>=copynum){
                stmt = con.prepareStatement(Query.librarian_b_borrow[2]);
                stmt.setString(1, user_id);
                stmt.setString(2, callnum);
                stmt.setInt(3, copynum);
                java.sql.Date date = new java.sql.Date(Calendar.getInstance().getTime().getTime());
                stmt.setDate(4,date);
                stmt.setNull(5, Types.NULL);
                stmt.executeUpdate();
                System.out.println("Book borrowing Performed successfully.");
            }else {
                System.out.println("[Error] An matching borrow record is found. The book has been borrowed yet.");
            }

            rs1.close();
            rs2.close();
            stmt.close();
            con.close();

        }catch (Exception e){
            System.out.println(e);
        }finally {
            Librarian.librarian();
        }
    }

}
