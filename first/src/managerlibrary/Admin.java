package managerlibrary;
import java.util.*;
public class Admin {
	ArrayList<books> booksinlib;
	ArrayList<User> usersoflib;
	
	public Admin()
	{
		booksinlib=new ArrayList<>();
		usersoflib=new ArrayList<>();
		System.out.println("Signed in as Administrator");
	}
	
	public void addbook(Scanner sc)
	{
		System.out.println("Enter book id: \n"+"---------");
		int id=sc.nextInt();
		sc.nextLine();
		System.out.println("Enter book name: \n");
		String bkname=sc.nextLine();
		
		User issued_by=null;
		boolean isavailable=true;
		books newbook=new books(id, bkname, issued_by, isavailable);
		this.booksinlib.add(newbook);
		System.out.println("New book has been added");
		}
	
	public void removebook(Scanner sc)
	{
		System.out.println("Enter book id to be removed:");
		int bkid=sc.nextInt();
		for(books book : booksinlib)
		{
			if(book.getid()==bkid)
			{
				booksinlib.remove(book);
			}
		}
	}
	
	public void borrow(Scanner sc, int userId, int bookId) {
	    User borrower = null;

	    // Find the user in usersoflib by userId
	    for (User user : usersoflib) {
	        if (user.getuserid() == userId) {  // Assuming getUserId() is implemented in User
	            borrower = user;
	            break;
	        }
	    }

	    // If user is not found, display an error and return
	    if (borrower == null) {
	        System.out.println("User with ID " + userId + " not found in the library system.");
	        return;
	    }

	    // Search for the book and issue it to the user
	    for (books book : booksinlib) {
	        if (book.getid() == bookId) {
	        	
	            if (book.availability()) {
	            	   book.setavailability(false);
	                   book.isissuedby(borrower); // Set the borrower found in usersoflib
	                   System.out.println("Book borrowed by User ID " + userId + " (" + borrower.getusername() + ")");
	                   
	                   // Add the book to the user's issued books list
	                   ArrayList<books> updatedIssuedBooks = borrower.getissuedbooks();
	                   updatedIssuedBooks.add(book);
	                   borrower.setbooksissued(updatedIssuedBooks);
	                return;
	            } else {
	                System.out.println("Book is currently unavailable.");
	                return;
	            }
	        }
	    }
	    System.out.println("Book ID not found in library.");
	}
	
	public void retrieve(Scanner sc,User user, int bookId )
	{
		
		for(books book: booksinlib)
		{
			if (book.getid() == bookId) {
	            // Mark the book as available
	            book.setavailability(true);
	            book.retrievebook();
	            System.out.println("Book returned to the library.");

	            // Remove the book from the user's issuedbooks list
	            if (user.getissuedbooks().contains(book)) {
	                user.getissuedbooks().remove(book);
	                book.setavailability(true);
	                book.isissuedby(null);
	                System.out.println("Book removed from user's issued books list.");
	            } else {
	                System.out.println("This book was not issued to the user.");
	            }
	            return; // Exit after the book is returned
	        }
		}
		System.out.println("Book ID not found in library.");
	}
	
	public void seeavailablebooks(Scanner sc)
	{
		System.out.println("Available Books:");
		if(booksinlib.isEmpty())
		{
			System.out.println("Library Empty");
		}
		
		else
		{
			for(books book: booksinlib)
			{
				System.out.println(book.getid()+"-------"+book.getname());
				
			}
		}
	}
	
	public void seeBorrowedBooks(Scanner sc) {
        System.out.println("\n\n------------------");
        System.out.println("Borrowed Books");
        for (books book : booksinlib) {
            if (!book.availability()) {
                System.out.println("\n" + book.getname());
                
                // Find the user who borrowed the book in usersoflib
                User borrower = null;
                for (User user : usersoflib) {
                    if (user.equals(book.issuedby())) { // Assuming issuedby() returns a User or comparable identifier
                        borrower = user;
                        break;
                    }
                }
                
                // If borrower is found, print the username; otherwise, show a default message
                if (borrower != null) {
                    System.out.println("Borrowed by : " + borrower.getusername());
                } else {
                    System.out.println("Borrower not found in library records.");
                }
            }
        }
	}
	
	public void adduser(int userid, String username)
	{
		for(User user : usersoflib)
		{
			 if (user.getuserid()==userid && user.getusername().equals(username)) {
	                System.out.println("User already exists.");
	                return;
			 }
		}
		
		User newUser = new User(userid, username, false, null);
		usersoflib.add(newUser);
        System.out.println("New user added.");
	}
	
	public boolean checkuser(int userid, String username)
	{
		for(User user : usersoflib)
		{
			 if (user.getuserid()==userid && user.getusername().equals(username)) 
			 {
	                System.out.println("User already exists.");
	                return true;
			 }
			 
		}
		return false;
	}
	
	public User getUserById(int userid) {
	    for (User user : usersoflib) {
	        if (user.getuserid() == userid) {
	            return user;
	        }
	    }
	    return null; // Return null if the user is not found
	}
	
	public void removeuser(int userid, String username)
	{
		for(User user : usersoflib)
		{
			 if (user.getuserid()==userid && user.getusername().equals(username)) {
	                usersoflib.remove(user);
	                System.out.println("User remove successfully");
	                return;
			 }
		}
	}
	
	

}
