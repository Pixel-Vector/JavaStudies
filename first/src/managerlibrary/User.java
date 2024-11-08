package managerlibrary;
import java.util.*;
public class User extends Admin{
	int userid;
	String username;
	boolean isadmin;
	ArrayList<books> books_issued = new ArrayList<>();;
	
	
	public User(int userid, String username, boolean isadmin, ArrayList<books> books_issued)
	{
		this.userid=userid;
		this.username=username;
		this.books_issued=new ArrayList<>();
		this.isadmin=isadmin;
	}
	
	
	public int getuserid()
	{
		return this.userid;
	}
	
	public void setuserid(int userid)
	{
		this.userid=userid;
	}
	
	public String getusername()
	{
		return this.username;
	}
	
	public void setusername(String username)
	{
		this.username=username;
	}
	
	public boolean getisadmin()
	{
		return this.isadmin;
	}
	
	public void setisadmin(boolean isadmin)
	{
		this.isadmin=isadmin;
	}
	
	public ArrayList<books> getissuedbooks()
	{
		return this.books_issued;
	}
	
	public void setbooksissued(ArrayList<books> books_issued)
	{
		this.books_issued=books_issued;
	}
	

//	public void seeavailablebooks(Scanner sc)
//	{
//		System.out.println("Available Books:");
//		if(booksinlib.isEmpty())
//		{
//			System.out.println("Library Empty");
//		}
//		
//		else
//		{
//			for(books book: booksinlib)
//			{
//				System.out.println(book.getname());
//			}
//		}
	
@Override
    public String toString() {
        return "User: " + getusername()+ "ID:"+getuserid();
    }
	
	
	}
	

