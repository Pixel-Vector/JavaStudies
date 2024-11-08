package managerlibrary;
import java.util.*;
public class LibraryManager {
	Admin Library;
	Scanner sc;
	public LibraryManager()
	{
		this.Library=new Admin();
		this.sc=new Scanner(System.in);
	}
	
	void start()
	{
		System.out.println("Library Opened");
		
		do
		{
			System.out.println("Enter your id:");
			int id=sc.nextInt();
			sc.nextLine();
			if(id==0)
			{
				System.out.println("Enter passcode:");
				int passcode=sc.nextInt();
				if(passcode==3401)
				{
					System.out.println("Logged in as admin");
					System.out.println("~~~~~~~~~~~~~~~~~~~~~");
					int choice=0;
					
					do
					{
						System.out.println("\n\n\nYou can:");
	                    System.out.println("0. ADD BOOKS");
	                    System.out.println("1. REMOVE BOOKS");
	                    System.out.println("2. SEE AVAILABLE BOOKS");
	                    System.out.println("3. SEE BORROWERD BOOKS");
	                    System.out.println("4. BORROW BOOK");
	                    System.out.println("5. RETRIEVE BOOKS");
	                    System.out.println("6. ADD USER");
	                    System.out.println("7. REMOVE USER");
	                    System.out.println("8. LOGOUT");
	                    System.out.print("Enter choice: ");
	                    choice = sc.nextInt();
	                    
	                    switch(choice)
	                    {
	                    
	                    case 0: Library.addbook(sc);
	                    delay(1000);
	                    break;
	                    case 1: Library.removebook(sc);
	                    delay(1000);
	                    break;
	                    case 2: Library.seeavailablebooks(sc);
	                    delay(1000);
	                    break;
	                    case 3: Library.seeBorrowedBooks(sc);
	                    delay(1000);
	                    break;
	                    case 4:  System.out.println("Enter the user ID to whom the book will be issued:");
	                    int userId = sc.nextInt();
	                    sc.nextLine(); // Consume newline

	                    // Retrieve the user by ID
	                    User user = Library.getUserById(userId);

	                    if (user != null) {
	                        System.out.println("Enter the book ID to issue:");
	                        int bookId = sc.nextInt();
	                        sc.nextLine(); // Consume newline

	                        // Issue the book to the user by calling borrow
	                        Library.borrow(sc, userId, bookId);
	                    } else {
	                        System.out.println("User not found.");
	                        choice=6;
	                        delay(1000);
	                        break;
	                    }
	                    delay(1000);
	                    break;
	                    case 5: System.out.println("Enter the user ID of the user whose book has to be retrieved:");
	                    int userid=sc.nextInt();
	                    sc.nextLine();
	                    
	                    User user2=Library.getUserById(userid);
	                    if (user2 != null) {
	                        System.out.println("Enter the book ID to retrieve:");
	                        int bookId = sc.nextInt();
	                        sc.nextLine();
	                        Library.retrieve(sc, user2, bookId);
	                    }
	                    else
	                    {
	                    	System.out.println("User Not found;");
	                    	choice=6;
	                    	delay(1000);
	                    	break;
	                    }
	                    delay(1000);
	                    break;
	                    case 6:
	                    	System.out.println("Enter  id and name to be added");
	                    	int userid2=sc.nextInt();
	                    	sc.nextLine();
	                    	System.out.println("Enter name");
	                    	String username=sc.nextLine();
	                    	sc.nextLine();
	                    	boolean checker=Library.checkuser(userid2, username);
	                    	if(checker)
	                    	{
	                    		System.out.println("User exists");
	                    		
	                    	}
	                    	else {
	                    		Library.adduser(userid2, username);
	                    	}
	                    	delay(1000);
	                    	break;
	                    case 7:
	                    	System.out.println("Enter  id to be removed");
	                    	int userid3=sc.nextInt();
	                    	sc.nextLine();
	                    	System.out.println("Enter name");
	                    	String username2=sc.nextLine();
	                    	sc.nextLine();
	                    	boolean checker2=Library.checkuser(userid3, username2);	
	                    	if(checker2)
	                    	{
	                    		Library.removeuser(userid3, username2);
	                    	}
	                    	else
	                    	{
	                    		System.out.println("This user does not exist");
	                    	}
	                    	delay(1000);
	                    	break;
	                    	
	                    	default:
	                    		System.out.println("Wrong choice has been made");
					}
	                    
					}while(choice !=8);
				}
			
			}
//			 else {
//		            // Check if the user exists
//		            User user = Library.getUserById(id); // New method in Admin to retrieve user by ID
//		            
//		            if (user != null) {
//		                System.out.println("Welcome back, " + user.getusername());
//		                
//		                System.out.println("Enter 0 to check issued books");
//		                int bookId = sc.nextInt();
//		                
//		                // Call the borrow method with the existing us
//		            } else {
//		                System.out.println("User not found. Please register first. Login as admin");
//		                
//		            }
//		        }
			
			
		}while(true);
		}
	
	
    private void delay(int milliseconds) {
        try {
            Thread.sleep(milliseconds);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt(); // restore interrupted status
            System.out.println("Unexpected interruption");
        }
    }
	public static void main(String[] args)
	{
		LibraryManager lib=new LibraryManager();
		lib.start();
	}
		
	}
	
