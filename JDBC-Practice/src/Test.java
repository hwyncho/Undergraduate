import java.sql.*;
import java.util.Arrays;
import java.util.Scanner;

public class Test {
    protected static final String URL = "hwyncho.dlinkddns.com:9306";

    protected static final String USER_NAME = "hwyncho";
    protected static final String USER_PW = "abcd1234!";

    protected static final String DB_NAME = "DBHW2";

    private static Connection myConnection = null;

    public static void main(String[] args) throws SQLException {
        if (myDBConnect()) {
            /* Insert Student who have a grade of A */
            Scanner scanner = new Scanner(System.in);

            String table = "Grade_Report";
            String[] column = { "student_number", "section_identifier", "grade" };

            while (true) {
                System.out.print("<<< [student_number], [section_identifier], [grade] : ");

                String input = scanner.nextLine();

                if (input.compareTo("exit") == 0) {
                    scanner.close();
                    break;
                }

                String[] value = input.split(",");

                if (myInsert(table, column, value) != 1) {
                    System.out.println(">>> INSERT ERRROR!");
                }
            }

            /* Select 1 */
            System.out.println(">>> Students who have a grade of A in all their courses :");

            String sql = "SELECT name, major FROM Student WHERE student_number IN"
                    + "(SELECT student_number FROM Grade_Report WHERE grade=\"A\")";

            ResultSet resultSet = mySelect(sql);

            if (resultSet != null) {
                while (resultSet.next()) {
                    String result = String.format("[Name : %s, Major : %s]", resultSet.getString("name"), resultSet.getString("major"));
                    System.out.println(result);
                }
            } else {
                System.out.println(">>> SELECT ERROR!");
            }

            /* Select 2 */
            System.out.println(">>> Students who do not have a grade of A in all their courses :");

            sql = "SELECT name, major FROM Student WHERE student_number NOT IN"
                    + "(SELECT student_number FROM Grade_Report WHERE grade=\"A\")";

            resultSet = mySelect(sql);

            if (resultSet != null) {
                while (resultSet.next()) {
                    String result = String.format("[Name : %s, Major : %s]", resultSet.getString("name"), resultSet.getString("major"));
                    System.out.println(result);
                }
            } else {
                System.out.println(">>> SELECT ERROR!");
            }
        } else {
            System.out.println(">>> DB Connect ERROR!");

            return;
        }
    }

    /***
     * Connect to DB Server
     * if Success
     * @return true
     * else
     * @return false
     */
    public static boolean myDBConnect() {
        try {
            Class.forName("com.mysql.jdbc.Driver");

            myConnection = DriverManager.getConnection(("jdbc:mysql://" + URL + "/").concat(DB_NAME), USER_NAME, USER_PW);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();

            return false;
        } catch (SQLException e) {
            e.printStackTrace();

            return false;
        }

        return true;
    }

    /***
     * INSERT INTO [TABLE] ([COLUMN]) VALUES ([VALUE])
     * @param table Table Name
     * @param column Columns Name
     * @param value Values
     * if Success
     * @return 1
     * else
     * @return 0
     */
    public static int myInsert(String table, String[] column, String[] value) {
        String columns = Arrays.toString(column).replaceAll("\\[|\\]", "");
        String values = Arrays.toString(value).replaceAll("\\[|\\]", "");

        String sql = String.format("INSERT INTO %s (%s) VALUES (%s);", table, columns, values);

        try {
            Statement statement = myConnection.createStatement();

            return statement.executeUpdate(sql);
        } catch (SQLException e) {
            e.printStackTrace();

            return 0;
        }
    }

    /***
     * SELECT
     * @param sql SQL query
     * if Success
     * @return ResultSet
     * else
     * @return null
     */
    public static ResultSet mySelect(String sql) {
        try {
            Statement statement = myConnection.createStatement();
            ResultSet resultSet = statement.executeQuery(sql);

            return resultSet;
        } catch (SQLException e) {
            e.printStackTrace();

            return null;
        }
    }
}
