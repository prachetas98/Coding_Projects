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
public class samp{
public static void main (String[] args)
    {
String fullName = "John Erich Daws Black";
int idx = fullName.lastIndexOf(' ');
//if (idx == -1)
    //throw new IllegalArgumentException("Only a single name: " + fullName);
String firstName = fullName.substring(0, idx);
String lastName  = fullName.substring(idx + 1);
System.out.println(firstName);
System.out.println(lastName);
}
}
