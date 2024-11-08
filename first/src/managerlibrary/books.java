package managerlibrary;


public class books {
	int id;
	String book_name;
	User issued_by;
	boolean isavailable;
	
	public books(int id, String name, User issued_by, boolean isavailable)
	{
		this.id=id;
		this.book_name=name;
		this.issued_by=issued_by;
		this.isavailable=isavailable;
	}
	
	public int getid()
	{
		return this.id;
	}
	public void setid(int id)
	{
		this.id=id;
	}
	public String getname()
	{
		return this.book_name;
	}
	public void setname(String name)
	{
		this.book_name=name;
	}
	public boolean availability()
	{
		return this.isavailable;
	}
	public User issuedby()
	{
		return issued_by;
	}
	public void isissuedby(User issued_by)
	{
		this.issued_by=issued_by;
	}
	public void retrievebook()
	{
		this.issued_by=null;
	}
	public void setavailability(boolean isavailable)
	{
		this.isavailable= isavailable;
	}
	
	@Override
	public String toString()
	{
		return "{" +
	            " id='" + getid() + "'" +
	            ", name='" + getname() + "'" +
	            ", issued_by='" + issuedby() + "'" +
	            ", isAvailable='" + availability() + "'" +
	            "}";
	}

}
