/////
// Name: Prachetas Deshpande
// Lab 5
// Purpose: This program simulates a GUI program. This gives the library name and shelf a
// book is located. The book is determined either by the author name or the ISBN or the book name
/////

import javax.swing.*;
import java.sql.*;
import java.io.File;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import java.util.*;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.text.ParseException;
import javax.swing.table.DefaultTableModel;
import javax.swing.JTable;
import java.awt.GridBagLayout;
public class Lab5a_ex
{
  // Ths program is going to connect to the database takes MemberIDs and ISBNs,
  // Author names and book names and then display the library name and shelf number
    public static void main (String[] args)
    {
	String value;
	String member_id="";
	String ISBN="";
	String name="";
	String Auth_name="";
	String query = "";
	String fname = "";
	String lname = "";
	String DOB = "";
	char Gender;
	int id = 0;

	// Get the value


	// Display results

	Connection con = null;

    try {
      Statement stmt;
      ResultSet rs;
      // Register the JDBC driver for MySQL.
      Class.forName("com.mysql.jdbc.Driver");

      // Define URL of database server for
      // database named 'user' on the faure.
      String url =
            "jdbc:mysql://faure/chetas?serverTimezone=UTC";

      // Get a connection to the database for a
      // user named 'user' with the password
      // 123456789.
      con = DriverManager.getConnection(
                        url,"chetas", "833369314");

      // Display URL and connection information
      System.out.println("URL: " + url);
      System.out.println("Connection: " + con);

      // Get a Statement object
      stmt = con.createStatement();
      while(true){
        value = JOptionPane.showInputDialog ("Enter a MemberID:");
	try{
        rs = stmt.executeQuery("SELECT MemberID from Member where MemberID = " + value);
        while (rs.next()) {
	  //System.out.println("In while");
          System.out.println (rs.getString("MemberID"));
	  member_id=rs.getString("MemberID");
	  id = Integer.parseInt(member_id);                // Get the memberID
      	}

	  int val = Integer.parseInt(value);
	if(id == val){
		JTextField xField = new JTextField(5);            // Create the design of the panel
   	JTextField yField = new JTextField(5);
		JTextField zField = new JTextField(5);
		JPanel myPanel = new JPanel();
   	myPanel.add(new JLabel("Enter ISBN:"));        // Give ISBN
   	myPanel.add(xField);
   	myPanel.add(Box.createHorizontalStrut(15)); // a spacer
   	myPanel.add(new JLabel("Enter Name:"));        // Give the name of the book
   	myPanel.add(yField);
		myPanel.add(Box.createHorizontalStrut(15));
		myPanel.add(new JLabel("Enter Author:"));       // Give the name of the Author
		myPanel.add(zField);
		int result = JOptionPane.showConfirmDialog(null, myPanel,
        "Please Enter either ISBN or Book name or Author name", JOptionPane.OK_CANCEL_OPTION);
		if (result == JOptionPane.OK_OPTION) {
			ISBN = xField.getText();                       // If the user clicks ok
			name = yField.getText();
			Auth_name = zField.getText();
			ISBN = ISBN.trim();
			name = name.trim();
			Auth_name = Auth_name.trim();
			DefaultTableModel model = new DefaultTableModel(new String[]{"lib_name","shelf"}, 0);
			JTable table=new JTable();
			if(ISBN.isEmpty()==false){
			  get_lib_shelf(ISBN,con);                   // This function will return the library name and shelf
			}
			else if(name.isEmpty()==false){            // If user gives the naem of the book
			get_ISBN_name(name,con);
			}
			else if(Auth_name.isEmpty()==false){         // If the user entered the author name
				int idx = Auth_name.lastIndexOf(' ');
				String firstName = Auth_name.substring(0, idx);
				String lastName  = Auth_name.substring(idx + 1);
				get_ISBN_Auth(firstName,lastName,con);
			}
   		}
		else {
      JOptionPane.showMessageDialog(null,"No data entered");
			//break;
    }
	}
	else{
		JPanel panel = new JPanel();
		panel.add(new JLabel("Do you want to add the member with member_id: " + val));
		int res = JOptionPane.showConfirmDialog(null, panel,
        "ADD a new Member", JOptionPane.OK_CANCEL_OPTION);
		if(res == JOptionPane.OK_OPTION){
			add_member(con,val);                         // Adds a member if it doen't exist
		}
    else{
      JOptionPane.showMessageDialog(null,"Member ID not added");
      break;
    }
	}
      }catch(Exception e){
        System.out.print(e);
        System.out.println(
                  "error in the try code");
      }//end catch
    }
      con.close();
    }catch( Exception e ) {
      e.printStackTrace();

    }//end catch


	return;
	}

// This function returns the library name and shelf given the ISBN
// If ISBN is not found or there are no copies available it will output
// to the message box accordingly
	public static void get_lib_shelf(String ISBN, Connection con){
		Statement stmt;
		ResultSet rs;
    boolean available = false;
		try{

			stmt = con.createStatement();
			DefaultTableModel model = new DefaultTableModel(new String[]{"lib_name","shelf"}, 0);
			JTable table=new JTable();
      String query1 = "select unchecked_copies from location where ISBN = '" + ISBN + "';";
      rs = stmt.executeQuery(query1);
      if(rs.next() == false){
        JOptionPane.showMessageDialog(null,"No books in stock");
        return;
      }
      else {
      do {
        //System.out.println("In rs.next() loop");
        int e = rs.getInt("unchecked_copies");
        if(e!=0)
          available = true;
        //model.addRow(new Object[]{e});
      }while(rs.next());
    }
      if(available == true){            // if there are available copies
			String query = "select lib_name,shelf from location where unchecked_copies!=0 and ISBN= '" + ISBN + "';";
				rs = stmt.executeQuery(query);
				if(rs.next() == false){
					JOptionPane.showMessageDialog(null,"Books not in stock");
					return;
				}
				else {
				do {
					String d = rs.getString("lib_name");
    					int e = rs.getInt("shelf");
					model.addRow(new Object[]{d, e});
				}while(rs.next());
				}
				table.setModel(model);
				JPanel panel = new JPanel();
				panel.add(new JScrollPane(table));
				JOptionPane.showMessageDialog(null,panel);

  }
  else{
    JOptionPane.showMessageDialog(null,"All copies currently checked out");
  }
}
		catch(Exception e){
			System.out.print(e);
        System.out.println(
                  "Error detected in determining lib_name and shelf");
		}
}

// This function gives the list of books by the name and call the get_lib_shelf
//function to return the library name and shelf
	public static void get_ISBN_name(String name, Connection con){
		Statement stmt;
		ResultSet rs;
		try{

			stmt = con.createStatement();
			DefaultTableModel model = new DefaultTableModel(new String[]{"ISBN","Title"}, 0);
			JTable table=new JTable();
			table.setRowSelectionAllowed(true);
			  String query="select ISBN,Title from Book where Title LIKE '%" + name + "%';";
				rs = stmt.executeQuery(query);
        if(rs.next()== false){
          JOptionPane.showMessageDialog(null,"No books by that title exist in our database");
        }
        else{
          do{
					   String d = rs.getString("ISBN");
    			   String e = rs.getString("Title");
					   model.addRow(new Object[]{d, e});
				}while(rs.next());
				table.setModel(model);
				JPanel panel = new JPanel();
				panel.add(new JScrollPane(table));
				JOptionPane.showMessageDialog(null,panel);
				int row = table.getSelectedRow();
				String ISBN = model.getValueAt(row,0).toString();
				get_lib_shelf(ISBN,con);
      }
		}
		catch(Exception e){
			System.out.print(e);
        System.out.println(
                  "Error in derermining lib_name and shelf");
		}
	}

  // This function gives the list of books by the Author name and calls the get_lib_shelf
  // function to return the library name and shelf
	public static void get_ISBN_Auth(String first, String last, Connection con){
		Statement stmt;
		ResultSet rs;
		try{

			stmt = con.createStatement();
			DefaultTableModel model = new DefaultTableModel(new String[]{"ISBN","Title", "First_name","Last_name"}, 0);
			JTable table=new JTable();
			table.setRowSelectionAllowed(true);
			  String query="select b.ISBN,b.Title,a.First_name,a.Last_name from Book b JOIN written w ON b.ISBN = w.ISBN JOIN Author a ON a.AuthorID=w.AuthorID where a.Last_name='" + last + "'or a.First_name= '" + first + "';";
				rs = stmt.executeQuery(query);
        if(rs.next()== false){
          JOptionPane.showMessageDialog(null,"This author does not exist in the database");
        }
        else{
          do{
				//while(rs.next()){
					//System.out.println("In rs.next() loop");
					     String d = rs.getString("ISBN");
    					 String e = rs.getString("Title");
					     String f = rs.getString("First_name");
					     String g = rs.getString("Last_name");
					     model.addRow(new Object[]{d, e, f, g});
				}while(rs.next());
				table.setModel(model);
				JPanel panel = new JPanel();
				panel.add(new JScrollPane(table));
				JOptionPane.showMessageDialog(null,panel);
				int row = table.getSelectedRow();
				String ISBN = model.getValueAt(row,0).toString();
				get_lib_shelf(ISBN,con);
      }
		}
		catch(Exception e){
			System.out.print(e);
        System.out.println(
                  "Error in derermining lib_name and shelf");
		}
	}

// This function adds a member if it does not exist
	public static void add_member(Connection con, int member_id){
		String fname ="";
		String lname = "";
		String DOB = "";
		char Gender;
   	JTextField yField = new JTextField(5);
		JTextField zField = new JTextField(5);
		JTextField aField = new JTextField(5);
		JRadioButton optionMale = new JRadioButton("Male");
		JRadioButton optionFemale = new JRadioButton("Female");
		ButtonGroup group = new ButtonGroup();
		group.add(optionMale);
		group.add(optionFemale);
		optionMale.setSelected(true);
		JPanel myPanel = new JPanel();
   	myPanel.add(new JLabel("Enter First Name:"));
   	myPanel.add(yField);
		myPanel.add(Box.createVerticalStrut(5));
		myPanel.add(new JLabel("Enter Last Name:"));
		myPanel.add(zField);
		myPanel.add(Box.createVerticalStrut(5));
		myPanel.add(new JLabel("Enter Date of Birth in yyyy-mm-dd format:"));
		myPanel.add(aField);
		myPanel.add(Box.createVerticalStrut(5));
		myPanel.add(new JLabel("Enter Gender:"));
		myPanel.add(optionMale);
		myPanel.add(optionFemale);
		int result = JOptionPane.showConfirmDialog(null, myPanel,
        "Please Enter a new record of Member", JOptionPane.OK_CANCEL_OPTION);
		if (result == JOptionPane.OK_OPTION) {
			fname = yField.getText();
			lname = zField.getText();
			DOB = aField.getText();
			fname = fname.trim();
			lname = lname.trim();
			DOB = DOB.trim();
			boolean is_male = optionMale.isSelected();
			if(is_male == true)
				Gender = 'M';
			else
				Gender = 'F';
			String q = "insert into Member (MemberID,First_name,Last_name,DOB,Gender) values ('" + member_id + "', '" + fname + "', '" + lname + "', '" + DOB + "', '" + Gender + "');";

			try{
				Statement stmt = con.createStatement();
				stmt.executeUpdate(q);
			}catch(Exception e){
				System.out.print(e);
			}
	    	}
	}
}
