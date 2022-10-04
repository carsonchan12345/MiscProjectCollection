package CSCI3170proj;

import java.io.File;
import java.sql.*;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Scanner;
import java.util.*;

public class Admin {
    //admin menu
    public static void admin(){
        System.out.print("-----Operations for administrator menu-----\n" +
                "What kind of operation would you like to perform\n" +
                "1. Create All tables\n" +
                "2. Delete all tables\n" +
                "3. Load from datafile\n" +
                "4. Show number of records in each table\n" +
                "5. Return to The main menu\n" +
                "Enter Your Choice: ");
        Scanner input = new Scanner(System.in);
        switch(input.nextInt()){
            case 1: Admin.create();
            case 2: Admin.delete();
            case 3: Admin.load();
            case 4: Admin.showNumberRecords();
            case 5: Main.menu();
        }
    }

    private static void create(){
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            Connection con = DriverManager.getConnection(DBInfo.dbAddress, DBInfo.dbUserName, DBInfo.dbPassword);
            Statement stmt = con.createStatement();
            for (String s:Query.admin_create){
                stmt.executeUpdate(s);
            }
            System.out.println("Processing...Done! Database is initialized");
            stmt.close();
            con.close();
        } catch (Exception e)
        {
            System.out.println(e);
        }
        finally {
           Admin.admin();
        }
    }

    private static void delete(){
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            Connection con = DriverManager.getConnection(DBInfo.dbAddress, DBInfo.dbUserName, DBInfo.dbPassword);
            Statement stmt = con.createStatement();
            for (String s:Query.admin_delete){
                stmt.executeUpdate(s);
            }
            System.out.println("Processing...Done! Database is removed");
            stmt.close();
            con.close();
        } catch (Exception e)
        {
            System.out.println(e);
        }
        finally {
            Admin.admin();
        }
    }

    private static void load(){
        System.out.print("\nType in the Source Data Folder Path: ");
        Scanner input = new Scanner(System.in);
        String path = input.nextLine();
        SimpleDateFormat tmpdate = new SimpleDateFormat("dd/MM/yyyy");
        try{
            Class.forName("com.mysql.cj.jdbc.Driver");
            Connection con = DriverManager.getConnection(DBInfo.dbAddress, DBInfo.dbUserName, DBInfo.dbPassword);
            PreparedStatement pstmt = con.prepareStatement(Query.admin_load[0]);
            Scanner files= new Scanner(new File(path+"/"+"user_category.txt"));
            while (files.hasNextLine()){
                String[] result = files.nextLine().split("\t");
                pstmt.setInt(1,Integer.parseInt(result[0]));
                pstmt.setInt(2,Integer.parseInt(result[1]));
                pstmt.setInt(3,Integer.parseInt(result[2]));
                System.out.println(pstmt);
                pstmt.executeUpdate();
            }
            pstmt = con.prepareStatement(Query.admin_load[1]);
            files = new Scanner(new File(path + "/user.txt"));
            pstmt = con.prepareStatement(Query.admin_load[1]);
            while (files.hasNextLine()){
                String[] result = files.nextLine().split("\t");
                pstmt.setString(1,result[0]);
                pstmt.setString(2,result[1]);
                pstmt.setInt(3,Integer.parseInt(result[2]));
                pstmt.setString(4,result[3]);
                pstmt.setInt(5,Integer.parseInt(result[4]));
                System.out.println(pstmt);
                pstmt.executeUpdate();
            }
            pstmt = con.prepareStatement(Query.admin_load[2]);
            files= new Scanner(new File(path+"/"+"book_category.txt"));
            while (files.hasNextLine()){
                String[] result = files.nextLine().split("\t");
                pstmt.setInt(1,Integer.parseInt(result[0]));
                pstmt.setString(2,result[1]);
                System.out.println(pstmt);
                pstmt.executeUpdate();
            }

            files= new Scanner(new File(path+"/"+"book.txt"));
            while (files.hasNextLine()){
                pstmt = con.prepareStatement(Query.admin_load[3]);
                String[] result = files.nextLine().split("\t");
                pstmt.setString(1,result[0]);
                pstmt.setString(2,result[2]);
                java.sql.Date tmpdate2 = new java.sql.Date(tmpdate.parse(result[4]).getTime());
                pstmt.setDate(3,tmpdate2);
                if ("null".equals(result[5]))
                    pstmt.setNull(4, Types.NULL);
                else
                    pstmt.setDouble(4, Double.parseDouble(result[5]));

                pstmt.setInt(5, Integer.parseInt(result[6]));
                pstmt.setInt(6,Integer.parseInt(result[7]));
                System.out.println(pstmt);
                pstmt.executeUpdate();

                pstmt = con.prepareStatement(Query.admin_load[4]);
                pstmt.setString(1,result[0]);
                for (int i=1; i<=Integer.parseInt(result[1]); i++) {
                    pstmt.setInt(2, i);
                    System.out.println(pstmt);
                    pstmt.executeUpdate();
                }
                pstmt = con.prepareStatement(Query.admin_load[6]);
                String[] tmp=result[3].split(",");
                for (String x:tmp){
                    pstmt.setString(1,x);
                    pstmt.setString(2,result[0]);
                    System.out.println(pstmt);
                    pstmt.executeUpdate();
                }
            }

            pstmt = con.prepareStatement(Query.admin_load[5]);
            files= new Scanner(new File(path+"/"+"check_out.txt"));
            while (files.hasNextLine()){
                String[] result = files.nextLine().split("\t");
                pstmt.setString(1,result[2]);
                pstmt.setString(2,result[0]);
                pstmt.setInt(3,Integer.parseInt(result[1]));
                java.sql.Date tmpdate2 = new java.sql.Date(tmpdate.parse(result[3]).getTime());
                pstmt.setDate(4,tmpdate2);
                if ("null".equals(result[4]))
                    pstmt.setNull(5, Types.NULL);
                else {
                    tmpdate2 = new java.sql.Date(tmpdate.parse(result[4]).getTime());
                    pstmt.setDate(5, tmpdate2);
                }

                System.out.println(pstmt);
                pstmt.executeUpdate();
            }
            System.out.println("Processing...Done. Data is inputted to the database.");
            pstmt.close();
            con.close();
        } catch (Exception e)
        {
            System.out.println(e);
        }
        finally {
            Admin.admin();
        }


    }

    private static void showNumberRecords(){
        System.out.println("Number of records in each table: ");
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            Connection con = DriverManager.getConnection(DBInfo.dbAddress, DBInfo.dbUserName, DBInfo.dbPassword);
            Statement stmt = con.createStatement();
            ResultSet rs;
            int i=0;
            for (String s:Query.admin_record){
                rs=stmt.executeQuery(s);
                rs.next();
                System.out.print(DBInfo.dbtable[i] +": ");
                System.out.println(rs.getInt(1));
                i+=1;
            }
            stmt.close();
            con.close();
        } catch (Exception e)
        {
            System.out.println(e);
        }
        finally {
            Admin.admin();
        }
    }
}
